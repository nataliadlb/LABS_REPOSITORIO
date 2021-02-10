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
#pragma config FOSC = HS// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown Out Reset Selection bits (BOR disabled)
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
uint8_t a;
//float S1_val;
//float S2_val;
uint8_t S3_cont;
unsigned int x;

//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void); 
void Config_INTERRUPT(void);
//float bin_to_float(uint8_t ADC_VAL); //funcion para convertir el valor de la 
                                     //conversion ADC en decimales
void USART_Init_transmission(void); // Config Trasmision //char y const long int
void USART_Init_reception(void);// Config recepcion de datos
void Trasmission(void);// funcion para constantemente mandar los valores ADC
void Receive(void); //funcion para constantemente recibir datos de la compu
//****************************************************************************//
//INTERRUPCIONES                                                              //
//****************************************************************************//
//void __interrupt() ISR(void) {
//    
//    // ---- Interrupción del ADC ----
//    if (PIR1bits.ADIF) {
//        PIR1bits.ADIF = 0;
//        ADC_Config (0);
//        __delay_ms(2); //Inicio de conversion ADC
//        ADCON0bits.GO = 1;
//        while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
//            ADC_VALOR_1 = ADC(ADRESL, ADRESH);
//            
//        } 
//        
//        ADC_Config (1);
//        __delay_ms(2); //Inicio de conversion ADC
//        ADCON0bits.GO = 1;
//        while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
//            ADC_VALOR_2 = ADC(ADRESL, ADRESH);
//        }
//        PORTB = ADC_VALOR_2;
//    }
//    
//}

//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//
void main(void) {
    setup(); //Configuracion de puertos de entrada y salida
    //Config_INTERRUPT(); //Configuracion de la interrupcion del puerto B
    Lcd_Init();
//    USART_Init_transmission();
//    USART_Init_reception();
    
    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
        //S1_val = bin_to_float(ADC_VALOR_1);
        //xS2_val = bin_to_float(ADC_VALOR_2);
        //nombres S1, S2 y S3
        Lcd_Clear();
        Lcd_Set_Cursor(1,2);
        Lcd_Write_String("S1:");
        Lcd_Set_Cursor(1,8);
        Lcd_Write_String("S2:");
        Lcd_Set_Cursor(1,13);
        Lcd_Write_String("S3:");
        //Valores de S1 y S2
//        Lcd_Set_Cursor(2,1);
//        Lcd_Write_Char(S1_val);
//        Lcd_Set_Cursor(2,7);
//        Lcd_Write_Char(S2_val);
//        Lcd_Set_Cursor(2,13);
//        Lcd_Write_Char(S3_cont);
//        __delay_ms(2000);

    }
    return ;
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//
//float bin_to_float(uint8_t ADC_VAL){
////convertir cada valor de la conversion ADC (cada POT) y parasrlo a decimal para
//    //poder desplegarlo en la LCD
//}
//
//void Trasmission(void){
//    //trasmitir los valores de la conversion ADC a la computadora
//}
//
//void Receive(void){
    //Recibir el valor del contador, cada vez que se presione + o -
//}

//********************* CONFIGURACION PRINCIPAL ******************************//

void setup(void) { //Configuración de puertos de entrada y salida
    initOsc(0b00000111); //8MHz
    ANSEL = 0b00000011; //RA0 y RA1 como analogico
    ANSELH = 0; 
    TRISA = 0b00000011; //potenciometros, como entrada
    TRISB = 0;
    TRISC = 0; 
    TRISD = 0; 
    TRISE = 0;
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    
}

//**************** **** CONFIGURACION INTERRUPCIONES *************************//

void Config_INTERRUPT(void) {
    INTCON = 0b11000000;
    PIE1bits.ADIE = 1; // enables ADC interrupt
    PIR1bits.ADIF = 1; 
}

//********************* CONFIGURACION COM SERIAL *****************************//
//void USART_Init_transmission(void){
//    BRGH = 1;
//    TXEN = 1;
//    SYNC = 0;
//    SPEN = 1;
//}
//
//void USART_Init_reception(void){
//    SPEN =1;
//    CREN =1;
//    SREN = 1;
//    
//}
