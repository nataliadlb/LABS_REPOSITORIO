/*
 * Título: Esclavo sensor de temperatura
 * Autor: Natalia de Leon Bercian
 * Carne: 18193
 * Seccion: 20
 * 
 * 
 * Created on 15 de febrero de 2021
 */

//****************************************************************************//
//IMPORTAR LIBRERIAS                                                          //
//****************************************************************************//
#include <xc.h>
#include <stdint.h>
#include "Oscilador.h"
#include "SPI.h"

//****************************************************************************//
//CONFIGURACION BITS                                                          //
//****************************************************************************//
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT       // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//****************************************************************************//
//DEFINE                                                                      //
//****************************************************************************//
#define _XTAL_FREQ 8000000

//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//
int mv_temp_val;
//int temp;
uint8_t temp_val;

//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
void Config_INTERRUPT(void); 
void semaforo(void);
//****************************************************************************//
//INTERRUPCIONES                                                    //
//****************************************************************************//

void __interrupt() ISR(void) {
    
    //---- interrupcion SPI ----//
    if(SSPIF == 1){
        spiWrite(temp_val);
        SSPIF = 0;
    }  
}
    
//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    setup();
    
    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
        //Tomado de mi laboratorio # 3
        __delay_ms(2); 
        ADCON0bits.GO = 1; //Inicio de conversion ADC
        while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
            temp_val = ADRESH; 
            mv_temp_val = ((ADRESH * 150) / 255); //mapearlo de 0-1.5V
        } 
        semaforo();
    }

}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//

void semaforo(void){//Encender led segun rango de temperatura
    
    if (mv_temp_val <= 25){
        RE0 = 0; 
        RE1 = 0;
        RE2 = 1;
    }
    else if (mv_temp_val > 25 && mv_temp_val <= 36){ 
        RE0 = 0; 
        RE1 = 1;
        RE2 = 0;
    }
    else if (mv_temp_val > 36){
        RE0 = 1; 
        RE1 = 0;
        RE2 = 0;
    }
}

//****************************************************************************//
//CONFIGURACION  (puertos, bits...)                                           //
//****************************************************************************//

//----- puertos -----//
void setup(void) {
    initOsc(0b00000111);
    nRBPU = 0;
    
    ANSEL = 0b00000001; //RA0 como analogico
    ANSELH = 0; 
    
    TRISA = 0b00000001; //sensor, como entrada
    TRISB = 0; 
    TRISC = 0;
    TRISD = 0;
    TRISE = 0; // semaforo como output
    
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    //  ADCON1 = 0;
    ADCON0 = 0b01000001;
    ADCON1bits.VCFG0 = 1;
    //ADCON1 = 0x07;
    
    Config_INTERRUPT() ;
    TRISAbits.TRISA5 = 1; // slave select 
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

//----- interrupciones -----//
void Config_INTERRUPT(void){
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
}