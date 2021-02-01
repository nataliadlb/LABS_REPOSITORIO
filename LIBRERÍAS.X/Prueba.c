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



//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//
uint8_t contador;
uint8_t Valor_hex;

//****************************************************************************//
//INTERRUPCIONES                                                                //
//****************************************************************************//
void __interrupt() ISR(void){
    
    if (INTCONbits.RBIF == 1){
        if (PORTBbits.RB0 == 1){
            while (PORTBbits.RB0 == 1){
                contador = contador;
            }
            contador++;
        }
        if (PORTBbits.RB1 == 1){
            while (PORTBbits.RB1 == 1){
            contador = contador;
            }
            contador--;
        }
        INTCONbits.RBIF = 0;
    }
    
}

//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
void ContadorLEDS(void);

//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    contador = 0;
    setup();
    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
        ContadorLEDS();
//        Display(4);
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
    INTCONbits.GIE = 1; // enable de todas las interrupciones
    INTCONbits.RBIE = 1; //enable interrupcion on change
    INTCONbits.RBIF = 0; // 0 la bandera de la interrupcion on change
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    INTCONbits.PEIE = 1; // enables  all unmasked peripheral interrupts
    PIE1bits.ADIE = 1; // enables ADC interrupt
    PIR1bits.ADIF = 0; // clear A/D Converter Interrupt Flag bit
    ADCON0 = 0b01000001;
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//
void ContadorLEDS(void){
    PORTC = contador;
}