/*
 * Pseudocodigo -- Laboratorio # 3
 * Author: Natalia de León Bercián
 * carné: 18193
 * Digital 2
 *
 * Created on 7 de febrero de 2021
 */

//****************************************************************************//
//IMPORTAR LIBRERIAS                                                          //
//****************************************************************************//
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "Oscilador.h"
#include "LCD.h"
#include "Config_ADC.h"


//****************************************************************************//
//CONFIGURACION BITS                                                          //
//****************************************************************************//
// CONFIG1
#pragma config FOSC = XT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

#define RS PORTEbits.RE0
#define RW PORTEbits.RE1
#define EN PORTEbits.RE2
#define D0 PORTDbits.RD0
#define D1 PORTDbits.RD1
#define D2 PORTDbits.RD2
#define D3 PORTDbits.RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//
int ADC_VALOR_1;
int ADC_VALOR_2;

//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void); 
void Config_INTERRUPT(void);

//****************************************************************************//
//INTERRUPCIONES                                                              //
//****************************************************************************//
void __interrupt() ISR(void) {
    
    // ---- Interrupción del ADC ----
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        ADC_Config (0);
        __delay_ms(2); //Inicio de conversion ADC
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
            ADC_VALOR_1 = ADC(ADRESL, ADRESH);
            
        } 
        
        ADC_Config (1);
        __delay_ms(2); //Inicio de conversion ADC
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
            ADC_VALOR_2 = ADC(ADRESL, ADRESH);
        }
        PORTB = ADC_VALOR_2;
    }
    
}
    

//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//
void main(void) {
    setup(); //Configuracion de puertos de entrada y salida
    Config_INTERRUPT(); //Configuracion de la interrupcion del puerto B

    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
//        PORTB = ADC_VALOR_1;
    }
    return;
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//

//********************* CONFIGURACION PRINCIPAL ******************************//

void setup(void) { //Configuración de puertos de entrada y salida
    initOsc(0b00000111); //8MHz
    ANSEL = 0b00000011; //RA0 y RA1 como analogico
    ANSELH = 0; 
    TRISA = 0b00000011; //potenciometros, como entrada
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;
    TRISB = 0;
    TRISC = 0; 
    TRISD = 0; 
    PORTD = 0;
    TRISE = 0;
    PORTE = 0;
}

//**************** **** CONFIGURACION INTERRUPCIONES *************************//

void Config_INTERRUPT(void) {
    INTCON = 0b11000000;
    PIE1bits.ADIE = 1; // enables ADC interrupt
    PIR1bits.ADIF = 1;
//    ADCON1 = 0b00000000;
//    ADCON0bits.ADCS1 = 0;
//    ADCON0bits.ADCS0 = 1;
//    ADCON0bits.ADON = 1;   
}
