/*
 * File:  Main_Lab2 _Digital2.c
 * Autor: Natalia de Leon Bercian
 * Carne: 18193
 * Seccion: 20
 * 
 * Titulo: 
 * 
 * Created on 29 de enero de 2021
 */

//****************************************************************************//
//IMPORTAR LIBRERIAS                                                          //
//****************************************************************************//
#include <xc.h>
#include "Display.h"
#include "Oscilador.h"
#include "Config_ADC.h"

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
uint8_t contador;
int ADC_VALOR;
uint8_t ADC_SWAP;
uint8_t ADC_NIBBLE1;
uint8_t ADC_NIBBLE2;
uint8_t cont_multiplex;

//****************************************************************************//
//INTERRUPCIONES                                                                //
//****************************************************************************//
void __interrupt() ISR(void){
    
    if (INTCONbits.RBIF == 1){ // Interrupcion on change
        if (PORTBbits.RB0 == 1){
            while (PORTBbits.RB0 == 1){ //debouncing
                contador = contador;
            }
            contador++;
        }
        if (PORTBbits.RB1 == 1){
            while (PORTBbits.RB1 == 1){ //debouncing
            contador = contador;
            }
            contador--;
        }
        INTCONbits.RBIF = 0; //limpiar bandera
    } 
   
    if (PIR1bits.ADIF == 1){
        ADC_VALOR = ADC(ADRESL, ADRESH); // de la libreria se obtiene la conver.
        ADC_SWAP = SWAP_ADC(ADC_VALOR); // swap de los nibbles
        ADC_NIBBLE1 = NIBBLE1_ADC(ADC_VALOR); // AND con 0b00001111 para dejar 
        ADC_NIBBLE2 = NIBBLE2_ADC(ADC_SWAP);  // solo el nibble que necesito
        PIR1bits.ADIF = 0;
        __delay_ms(10);     //tiempo para iniciar otra conversion
        ADCON0bits.GO_nDONE = 1;
    }
    
    if (INTCONbits.TMR0IF == 1){
        cont_multiplex++;
        TMR0 = 61;  
        INTCONbits.TMR0IF = 0;
//        
       }
}

//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
void InitTimer0(void);
void ContadorLEDS(void);
void DisplayADC1(void);
void DisplayADC2(void);

//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    setup();
    contador = 0;
    cont_multiplex = 0;
    PORTEbits.RE1 = 1;
    
    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
        ContadorLEDS(); //funcion en que se manda el valor de contador a leds
        if (cont_multiplex == 20){
            PORTD++;
            cont_multiplex = 0;
        }
        //DisplayADC1(); 
//        if (ADC_VALOR >= contador){ // control si el valor de la conversion es 
//            PORTEbits.RE0 = 1;      // mayor que el contador
//        }
//        else {
//            PORTEbits.RE0 = 0;
//        }
    }

}

//****************************************************************************//
//CONFIGURACION  (puertos, bits...)                                           //
//****************************************************************************//

void setup(void) {
    initOsc(0b00000110);
    ANSEL = 0b00000001; // RA0 analogico para POT
    ANSELH = 0;
    TRISA = 0b00000001; // RA0 como input
    PORTA = 0;
    TRISB = 0b00000011;
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    TRISE = 0;
    PORTE = 0;
    INTCONbits.GIE = 1;
    INTCONbits.T0IE = 1;
    OPTION_REGbits.nRBPU =1;
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 1;
    TMR0 = 61;
    InitTimer0();
    
    //INTERUPCION ON CHANGE
    INTCONbits.RBIE = 1; //enable interrupcion on change
    INTCONbits.RBIF = 0; // 0 la bandera de la interrupcion on change
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    // ADC CONFIGURACIONES
    INTCONbits.PEIE = 1; // enables  all unmasked peripheral interrupts
    PIE1bits.ADIE = 1; // enables ADC interrupt
    PIR1bits.ADIF = 0; // clear A/D Converter Interrupt Flag bit
    ADCON0 = 0b11000001;
    ADCON0bits.GO_nDONE = 1;
    
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//
void ContadorLEDS(void){
    PORTC = contador;
}

//TMR0 CONFIGURACIONES
void InitTimer0(void){
  OPTION_REG	 = 0x87;
  TMR0		 = 61;
  INTCON	 = 0xA0; 
}

void DisplayADC1(void){
    //PORTEbits.RE1 = 1;
    PORTD = display(ADC_NIBBLE1);
}

void DisplayADC2(void){
    //PORTEbits.RE1 = 1;
    PORTD = display(ADC_NIBBLE2);
}