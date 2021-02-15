/*
 * Título:  Esclavo contador
 * Autor: Natalia de Leon Bercian
 * Carne: 18193
 * Seccion: 20
 * 
 * Titulo: 
 * 
 * Created on 15 de febrero de 2021
 */

//****************************************************************************//
//IMPORTAR LIBRERIAS                                                          //
//****************************************************************************//

#include <stdint.h>
#include "Oscilador.h"
#include <xc.h>

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
#define _XTAL_FREQ 8000000


//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//
uint8_t contador = 0; //Variable de incremento para contador 
uint8_t debouncing1 = 0; //Variable que controla debouncing de un push
uint8_t debouncing2 = 0;

//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
void Config_INTERRUPT(void);

//****************************************************************************//
//INTERRUPCIONES                                                    //
//****************************************************************************//

void __interrupt() ISR(void) {
    // tomado de mi laboratorio # 2, el código de la interrupción 
    if (INTCONbits.RBIF == 1){ // Interrupcion on change
            
            if (PORTBbits.RB0 == 1){ //debouncing
                debouncing1 = 1;
                contador = contador;
            }
            if (PORTBbits.RB1 == 1){ //debouncing
                debouncing2 = 1;
                contador = contador;
            }
            if(PORTBbits.RB0 == 0 && debouncing1 == 1){//hasta revisar bandera...
                contador++;                 // de deboucing y que el boton no...
                PORTC = contador;           //este presionado, se aumenta o... 
                debouncing1 = 0;            //decrementa.
            }
            if(PORTBbits.RB1 == 0 && debouncing2 == 1){
                contador--;
                PORTC = contador;
                debouncing2 = 0;
            }
            INTCONbits.RBIF = 0; //limpiar bandera
        }
}
//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    setup();
    Config_INTERRUPT();

    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
    }

}

//****************************************************************************//
//CONFIGURACION                                                               //
//****************************************************************************//

//----- puertos -----//
void setup(void) { //Configuración de puertos de entrada y salida
    initOsc(0b00000111);
    ANSEL = 0; 
    ANSELH = 0; 
    TRISA = 0; 
    TRISB = 0b00000011; // push, como entradas
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}

//----- interrupciones -----//
void Config_INTERRUPT(void) {
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCB = 0b00000011;  
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//


