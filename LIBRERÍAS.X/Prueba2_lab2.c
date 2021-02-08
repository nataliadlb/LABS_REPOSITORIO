/*
 * File:   Prueba2_lab2.c
 * Author: Nataliadlb
 *
 * Created on 6 de febrero de 2021, 11:20 AM
 */

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



#include <xc.h>
#include <stdint.h>
#include "Oscilador.h"
#include "Config_ADC.h"
#include "Display.h"

#define _XTAL_FREQ 4000000 

void setup(void); 
void Config_INTERRUPT(void);
void CONVERSION_ADC(void); 
void TOGGLE_1(void); 
void DisplayADC(void); 
void Revision(void);

uint8_t contador = 0; //Variable de incremento para contador 
uint8_t debouncing1 = 0; 
uint8_t debouncing2 = 0; 
uint8_t cont_timer = 0; 
uint8_t toggle = 0; 
int val_1; 
int ADC_NIBBLE1; // unsigned int ADC_NIBBLE1;
int ADC_NIBBLE2; //unsigned int ADC_NIBBLE2;
int ADC_VALOR;
int ADC_SWAP;

void __interrupt() ISR(void) {

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

    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        __delay_ms(2); //Inicio de conversion ADC
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO != 0) { //Mientras el bit go sea distinto de cero entonces:
            ADC_VALOR = ADC(ADRESL, ADRESH);
            Revision();
            DisplayADC();
        }
    }

    if (INTCONbits.TMR0IF == 1) { //Configuracion del timer 0 es base al ejemplo hecho en clase.
        INTCONbits.TMR0IF  = 0;
        TMR0 = 6;
        Revision();
        cont_timer++; //cont_timer++
//        Revision();
    }
}

void main(void) {
    setup(); //Configuracion de puertos de entrada y salida
    Config_INTERRUPT(); //Configuracion de la interrupcion del puerto B

    
    while (1) {
        if (cont_timer >= 1) { //Configuracion del timer 0 es base al ejemplo hecho en clase.
            cont_timer = 0;
            TOGGLE_1();
        }
        CONVERSION_ADC(); //Separacion de Nibbles
//        Revision();
    }

    return;
}

void TOGGLE_1(void) {
    if (toggle == 1) {
        toggle = 0;
    } else if (toggle == 0) {
        toggle = 1;

    }
}

void DisplayADC(void) {
    PORTE = 0;
    if (toggle == 0) {
        PORTEbits.RE1 = 1;
        PORTD = display(ADC_NIBBLE2);
        
        //__delay_ms(1);
        //PORTEbits.RE2 = 0;
    } else if (toggle == 1) {
        PORTEbits.RE2 = 1;
        PORTD = display(ADC_NIBBLE1);
        
        //__delay_ms(1);
        //PORTEbits.RE1 = 0;
    }
}

void CONVERSION_ADC(void) { //CONFIGURACION DE SEPARACION DE NIBBLES en base a este codigo https://www.geeksforgeeks.org/swap-two-nibbles-byte/ 
    ADC_SWAP = SWAP_ADC(ADC_VALOR); // swap de los nibbles
    ADC_NIBBLE1 = NIBBLE1_ADC(ADC_VALOR); // AND con 0b00001111 para dejar 
    ADC_NIBBLE2 = NIBBLE2_ADC(ADC_SWAP); 

}

void Revision(void){
    if (ADC_VALOR >= contador) {
            PORTEbits.RE0 = 1;

        }
    else if (ADC_VALOR < contador){
            PORTEbits.RE0 = 0;
        }
}

void setup(void) { //Configuraci?n de puertos de entrada y salida
    initOsc(0b00000110);
    ANSEL = 0b00000001; //Salida
    ANSELH = 0; //Se utiliza ?nicamente el canal AN0
    TRISA = 0b00000001; //Salida.
    PORTA = 0; 
    TRISB = 0b00000011; // Entrada
    PORTB = 0;
    PORTC = 0;
    TRISC = 0; //Salida
    TRISD = 0; //Salida
    PORTD = 0;
    TRISE = 0;
    PORTE = 0;
}

void Config_INTERRUPT(void) {
    TMR0 = 6;
    OPTION_REG = 0b10001000;
    INTCON = 0b10101001;
    IOCB = 0b00000011;
    PIE1bits.ADIE = 1; // enables ADC interrupt
    PIR1bits.ADIF = 1;
    ADCON1 = 0b00000000;
    ADCON0 = 0b01000001;
    
}
