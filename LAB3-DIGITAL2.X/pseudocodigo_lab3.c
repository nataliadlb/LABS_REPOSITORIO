/*
 * Código -- Laboratorio # 3
 * Author: Natalia de León Bercián
 * carné: 18193
 * Digital 2
 *
 * Created on 7 de febrero de 2021
 */


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

//******************************************************************************
//Librerias
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "LCD.h"
#include "Config_ADC.h"
#include "USART.h"
#include "Oscilador.h"

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//Define
//******************************************************************************
#define _XTAL_FREQ 8000000



//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//

float S1_val = 0.0;
float S2_val = 0.0;
char data_total[20];
uint8_t cont;
char data_recive;

//******************************************************************************
//Prototipos de funciones
//******************************************************************************

void setup(void); 
void ADC_channel1(void);
void ADC_channel2(void);
void ADC_to_string(void);
void Show_val_LCD(void);


//******************************************************************************
//Interrupciones
//****************************************************************************** 

void __interrupt() ISR(void) {
    if(PIR1bits.RCIF == 1){
        data_recive = RCREG; //Recibe los datos que manda la terminal
        if (data_recive == '+'){ //auementa
            cont++;
            PORTB = cont;
        }
        else if (data_recive == '-'){ //decrementa
            cont--;
            PORTB = cont;
        }
        data_recive = 0;
        }
}

//******************************************************************************
//Ciclo Principal
//******************************************************************************

void main(void) {
    setup();
    TRISD = 0x00;
    Lcd_Init();
    Lcd_Clear();

    
    while (1) {
        ADC_channel1(); //conversion ADC de un pot
        __delay_ms(1);
        ADC_channel2(); //conversion ADC del otro pot
         
        Write_USART_String("S1   S2   S3 \n"); //enviar los datos del pic a la compu
        ADC_to_string();
        //sprintf(data_total, "%1.2fV  %1.1fV    %d", S2_val, S1_val, cont); //convertir los valores de voltaje y el contador a un string para que los lea bien la compu
        Write_USART_String(data_total); //enviar el string con los valores a la pc
        Write_USART(13);//13 y 10 la secuencia es para dar un salto de linea 
        Write_USART(10);
        
        Show_val_LCD();
        __delay_ms(500);

    }
}

//******************************************************************************
//Funciones
//******************************************************************************
void ADC_to_string(void){
    sprintf(data_total, "%1.2fV %1.2fV  %d", S2_val, S1_val, cont);
}

void Show_val_LCD(void){
    //Valores de S1 y S2
    Lcd_Clear();
    Lcd_Set_Cursor(1,2); //nombres S1, S2 y S3
    Lcd_Write_String("S1:   S2:   S3:");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(data_total);
}

void ADC_channel1(void) {
    ADC_Config (0); //channel 0
    __delay_us(40); 
    ADCON0bits.GO = 1; //Inicia la conversión
    while (ADCON0bits.GO != 0) { //Mientras no se termine la conversion
        S1_val = ((ADRESH * 5.0) / 255);

    }
}

void ADC_channel2(void) {
    ADC_Config (1); //channel 1
    __delay_us(40); //Para conversion
    ADCON0bits.GO = 1; //Inicia la conversión ADC
    while (ADCON0bits.GO != 0) { //Mientras no se termine la conversion
        S2_val = ((ADRESH * 5.0) / 255);

    }
}

void setup(void) {
    initOsc(7); //8MHz
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
    USART_Init_BaudRate();
    USART_Init();
    USART_INTERRUPT();

}