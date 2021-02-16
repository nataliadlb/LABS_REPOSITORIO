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

//****************************************************************************//
//CONFIGURACION BITS                                                          //
//****************************************************************************//
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
int temp;
int temp_val;

//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
//void Config_INTERRUPT(void); 
void semaforo(void);
//****************************************************************************//
//INTERRUPCIONES                                                    //
//****************************************************************************//

void __interrupt() ISR(void) {}
    

//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    setup();
    //Config_INTERRUPT() ;
    
    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
        ADCON0bits.GO = 1; //Inicio de conversion ADC
        while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
            //mv_temp_val = ADRESH;
            //temp_val = ADRESH;
            mv_temp_val = ((ADRESH * 150) / 255);
        }
        //mv_temp_val = ((temp_val * 500) / 255); //mv
        //mv_temp_val = ((temp_val * 150) / 255); //v per guardando dos decimales
//        temp = (mv_temp_val)/10; //grados
            
            //para tener 2 decimales y guradarlo como int
            //temp = (mv_temp_val)/1000;
            semaforo();
        

        
    }

}

//****************************************************************************//
//CONFIGURACION  (puertos, bits...)                                           //
//****************************************************************************//

//----- puertos -----//
void setup(void) {
    initOsc(0b00000111);
    ANSEL = 0b00000001; //RA0 como analogico
    ANSELH = 0; 
    TRISA = 0b00000001; //potenciometro, como entrada
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
    ADCON1bits.VCFG0 = 1;
    ADCON0 = 0b01000001;
}

//----- interrupciones -----//

//void Config_INTERRUPT(void) {
//    INTCONbits.GIE = 1;
//    INTCONbits.PEIE = 1;
//    PIE1bits.ADIE = 1; // enables ADC interrupt
//    PIR1bits.ADIF = 1;
//    
//    
//}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//

void semaforo(void){
    if (mv_temp_val <= 25){
        RE0 = 0; 
        RE1 = 0;
        RE2 = 1;
    }
    else if (25 < mv_temp_val  && mv_temp_val <= 36){
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