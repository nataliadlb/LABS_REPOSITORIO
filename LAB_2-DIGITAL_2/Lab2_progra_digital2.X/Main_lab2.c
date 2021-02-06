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
#include <stdint.h>
#include "Display.h"
#include "Oscilador.h"
#include "Config_ADC.h"

//****************************************************************************//
//CONFIGURACION BITS                                                          //
//****************************************************************************//
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
#define _XTAL_FREQ 4000000 //8000000


//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//
uint8_t contador = 0;
unsigned int val_1;
unsigned int ADC_VALOR;
unsigned int ADC_SWAP;
unsigned int ADC_NIBBLE1;
unsigned int ADC_NIBBLE2;
uint8_t cont_timer = 0;
uint8_t toggle = 0;
uint8_t debouncing1 = 0;
uint8_t debouncing2 = 0;

//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
void Config_INTERRUPT(void);
void DisplayADC(void);
void TOGGLE(void);
void CONVERSION_ADC(void);
void Revision(void);

//****************************************************************************//
//INTERRUPCIONES                                                                //
//****************************************************************************//
void __interrupt() ISR(void){
    
    if (INTCONbits.RBIF == 1){ // Interrupcion on change
            
            if (PORTBbits.RB0 == 1){ //debouncing
                debouncing1 = 1;
                contador = contador;
            }
            if (PORTBbits.RB1 == 1){ //debouncing
                debouncing2 = 1;
                contador = contador;
            }
            if(PORTBbits.RB0 == 0 && debouncing1 == 1){
                contador++;
                PORTC = contador;
                debouncing1 = 0;
            }
            if(PORTBbits.RB1 == 0 && debouncing2 == 1){
                contador--;
                PORTC = contador;
                debouncing2 = 0;
            }
            INTCONbits.RBIF = 0; //limpiar bandera
        }
        
    if (PIR1bits.ADIF == 1){
        PIR1bits.ADIF = 0;
        __delay_ms(2);
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO != 0){
            //val_1 = ADRESH;
            ADC_VALOR = ADC(ADRESL, ADRESH); // de la libreria se obtiene la conver.
            CONVERSION_ADC();
            //PORTD = val_1;
            //CONVERSION_ADC();
            DisplayADC();
        }
        
        
        }

    if (INTCONbits.TMR0IF == 1){
        
        cont_timer++;
        INTCONbits.TMR0IF = 0;
        if (cont_timer >= 1){
            cont_timer =0;
            TOGGLE();
        }
        Revision();
        TMR0 = 1; 
        
       }
}


//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    setup();
    Config_INTERRUPT();
//    contador = 0;

    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
//        if (cont_timer >= 1){
//            cont_timer =0;
//            TOGGLE();
// 
//        }
        //CONVERSION_ADC();
        
//        if (ADC_VALOR > contador) {
//            PORTEbits.RE0 = 1;
//
//        }
//        else if (ADC_VALOR < contador) {
//            PORTEbits.RE0 = 0;
//        }
    }
    return;

}

//****************************************************************************//
//CONFIGURACION  (puertos, bits...)                                           //
//****************************************************************************//

void setup(void) {
    initOsc(0b00000111); //4MHz
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
   
}
//****************************************************************************//
//INTERRUPCIONES                                                              //
//****************************************************************************//
void Config_INTERRUPT(void){
    TMR0 = 1;
    INTCON = 0b10101001;//0b11101000; //config de interrupciones 0b10101001;
    OPTION_REG = 0b10000001; //0b10000000; //config TMR0
    
    //CONFIG PUERTOS INTERRUPCION ON CHANGE
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;

    // CONFIG ADC
    PIE1bits.ADIE = 1; // enables ADC interrupt
    PIR1bits.ADIF = 1;//0; // clear A/D Converter Interrupt Flag bit
    ADCON1 = 0b00000000;
    ADCON0 = 0b01000001; 
    //ADCON0bits.GO = 1;
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//

void DisplayADC(void) {
    PORTE = 0;
    if (toggle == 0) {
        //display(ADC_NIBBLE1);
        PORTEbits.RE1 = 1;
        PORTD = display(ADC_NIBBLE1);
        
        //PORTEbits.RE2  = 0;
    } else if (toggle == 1) {
        //display(ADC_NIBBLE2);
        PORTEbits.RE2 = 1;
        PORTD = display(ADC_NIBBLE2);
        
        //PORTEbits.RE1 = 0;
    }
}   

void TOGGLE(void){
    if (toggle == 1) {
        toggle = 0;
    } 
    else if (toggle == 0) {
        toggle = 1;
    }   
}

void CONVERSION_ADC(void){
    ADC_SWAP = SWAP_ADC(ADC_VALOR); // swap de los nibbles
    ADC_NIBBLE1 = NIBBLE1_ADC(ADC_VALOR); // AND con 0b00001111 para dejar 
    ADC_NIBBLE2 = NIBBLE2_ADC(ADC_SWAP); 
}

void Revision(void){
    if (ADC_VALOR >= contador) {
        PORTEbits.RE0 = 1;

        }
    else  {
        PORTEbits.RE0 = 0;
        }
}