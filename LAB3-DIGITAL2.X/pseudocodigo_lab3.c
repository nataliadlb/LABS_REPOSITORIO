/*
 * Laboratorio # 2
 * Author: Natalia de León Bercián
 * carné: 18193
 * Digital 2
 *
 * Created on 29 de enero de 2021
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
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

#define _XTAL_FREQ 4000000 

//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//


//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void); 
void Config_INTERRUPT(void);
//****************************************************************************//
//INTERRUPCIONES                                                              //
//****************************************************************************//
void __interrupt() ISR(void) {
    


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
    }
    return;
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//

//********************* CONFIGURACION PRINCIPAL ******************************//

void setup(void) { //Configuración de puertos de entrada y salida
    initOsc(0b00000110);
    ANSEL = 0b00000001; //RA0 como analogico
    ANSELH = 0; 
    TRISA = 0b00000001; //potenciometro, como entrada
    PORTA = 0; 
    TRISB = 0b00000011; // pussh, como entradas
    PORTB = 0;
    PORTC = 0;
    TRISC = 0; 
    TRISD = 0; 
    PORTD = 0;
    TRISE = 0;
    PORTE = 0;
}

//**************** **** CONFIGURACION INTERRUPCIONES *************************//

void Config_INTERRUPT(void) {
    TMR0 = 6; // Valor que se le agrega al TMR0 para que ocurra cada 1000ns
    OPTION_REG = 0b10001000;
    INTCON = 0b10101001;
    IOCB = 0b00000011;
    PIE1bits.ADIE = 1; // enables ADC interrupt
    PIR1bits.ADIF = 1;
    ADCON1 = 0b00000000;
    ADCON0 = 0b01000001;
    
}