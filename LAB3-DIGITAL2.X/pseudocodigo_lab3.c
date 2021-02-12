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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pic16f887.h>
#include "Oscilador.h"
#include "LCD.h"
#include "Config_ADC.h"
#include "USART.h"


//****************************************************************************//
//CONFIGURACION BITS                                                          //
//****************************************************************************//
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

//float S1_val;
//float S2_val;
uint8_t S1_val;
uint8_t S2_val;
uint8_t S3_cont;
char* data1[8];
char* data2[8];
uint8_t eusart_toggle  = 0;
uint8_t ADC_toggle  = 0;
uint8_t cont_usart;
uint8_t cont;
uint8_t data_recive;
//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void); 
void Config_INTERRUPT(void);
void ADC_INTERRUPT(void);
void Trasmission(void);// funcion para constantemente mandar los valores ADC
//void Receive(void); //funcion para constantemente recibir datos de la compu
void titulos_LCD(void);
void ADC_channel1(void);
void ADC_channel2(void);
void ADC_to_string(void);
void Show_val_LCD(void);
//****************************************************************************//
//INTERRUPCIONES                                                              //
//****************************************************************************//
//void __interrupt() ISR(void){
//    if (PIR1bits.RCIF == 0){
//            data_recive = RCREG;
//        }
//    return;
//}
//    if (PIE1bits.TXIE && PIR1bits.TXIF){ //PIE1bits.TXIE && 
//        if (eusart_toggle)
//        {
//            PORTB = cont_usart;
//            TXREG = data2[cont_usart];
//        }
//        else
//        {
//            TXREG = data1[cont_usart];
//        }
//        cont_usart++;
//
//        if (cont_usart == 10)
//        {
//            eusart_toggle = !eusart_toggle;
//            cont_usart = 0;
//        }
//    }
//}
//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//
void main(void) {
    setup(); //Configuracion de puertos de entrada y salida
    USART_Init();
    USART_Init_BaudRate();
   // USART_INTERRUPT();
    Lcd_Init();
    titulos_LCD();
    
    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
        ADC_channel1();
        __delay_ms(1);
        ADC_channel2();
       
        ADC_to_string();
        Show_val_LCD();
        Trasmission();
        
        PORTB = cont;
        if (data_recive == '+'){
            cont++;
        }
        if(data_recive == '-'){
            cont--;
        }
       data_recive = 0;
    }
}
//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//


void titulos_LCD(void){
        Lcd_Set_Cursor(1,2); //nombres S1, S2 y S3
        Lcd_Write_String("S1:   S2:  S3:");
}

void ADC_channel1(void){
    ADC_Config (0);
    __delay_ms(1); //Inicio de conversion ADC
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
        S1_val = ADRESH;
        //S1_val = ((ADRESH * 5.0) / 255);
    }
}

void ADC_channel2(void){
    ADC_Config (1);
    __delay_ms(1); //Inicio de conversion ADC
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
        S2_val = ADRESH;
        //S2_val = ((ADRESH * 5.0) / 255);
    }
}

void ADC_to_string(void){
    sprintf(data2, "%.3iV", S1_val<<1); //poner las conversiones en numeros, 
    sprintf(data1, "%.3iV", S2_val<<1); // segun el voltaje
//    sprintf(data2, "%1.2f", S1_val);
//    sprintf(data1, "%1.2f", S2_val);
}

void Show_val_LCD(void){
    //Valores de S1 y S2
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(data2[0]);
        Lcd_Write_Char('.');
        Lcd_Write_Char(data2[1]);
        Lcd_Write_Char(data2[2]);
        Lcd_Write_Char(data2[3]);
        Lcd_Write_Char(' ');
        
        __delay_ms(1);
        
        Lcd_Set_Cursor(2,7);
        Lcd_Write_Char(data1[0]);
        Lcd_Write_Char('.');
        Lcd_Write_Char(data1[1]);
        Lcd_Write_Char(data1[2]);
        Lcd_Write_Char(data1[3]);
        Lcd_Write_Char(' ');
        
//        Lcd_Write_Char(S3_cont);
//        __delay_ms(2000);
}
void Trasmission(void){
    if (PIR1bits.TXIF ){ //PIE1bits.TXIE && 
            if (eusart_toggle){
                TXREG = data2;
                eusart_toggle = 0;
            }
            else{
                TXREG = data1;
                eusart_toggle = 1;
            }
            cont_usart++;

            if (cont_usart == 4){
                //eusart_toggle = !eusart_toggle;
                cont_usart = 0;
            }

        }
    //trasmitir los valores de la conversion ADC a la computadora
}
//
//void Receive(void){
    //Recibir el valor del contador, cada vez que se presione + o -
//}

//********************* CONFIGURACION PRINCIPAL ******************************//

void setup(void) { //Configuración de puertos de entrada y salida
    initOsc(0b00000110); //8MHz
    ANSEL = 0b00000011; //RA0 y RA1 como analogico
    ANSELH = 0; 
    TRISA = 0b00000011; //potenciometros, como entrada
    TRISB = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    TRISD = 0; 
    TRISE = 0;
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}

//void ADC_INTERRUPT(void){
//    INTCONbits.GIE = 1;
//    INTCONbits.PEIE = 1;
//    PIE1bits.ADIE = 1; // enables ADC interrupt
//    PIR1bits.ADIF = 1; 
//}

//********************* CONFIGURACION COM SERIAL *****************************//


