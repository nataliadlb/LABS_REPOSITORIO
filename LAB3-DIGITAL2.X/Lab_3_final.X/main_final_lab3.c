/*
 * File:   main.c
 * Author: katha
 *
 * Created on 7 de febrero de 2021, 02:03 PM
 */

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

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//Variables
//******************************************************************************
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

//uint8_t ADC_res1;
//uint8_t ADC_res2;
//uint8_t cont = 0;
//char data1[20];
//char data2[16];
//char USART_LEER;
//int VAL1;
//int VAL2;
float S1_val = 0.0;
float S2_val = 0.0;
char data_total[20];
uint8_t cont;
char data_recive;


//******************************************************************************
//Interrupciones
//****************************************************************************** 

void __interrupt() ISR(void) {
data_recive = Read_USART();
        //cont++;
        if (data_recive == '+'){
            cont++;
            PORTB = cont;
        }
        else if (data_recive == '-'){
            cont--;
        }
        data_recive = 0;
        }
}


//******************************************************************************
//Prototipos de funciones
//******************************************************************************
//void setup(void);
//void ADC_VALOR(void);
//void ADC_CH0(void);
//void ADC_CH1(void);
//void COM1(void);
//void COM2(void);
void setup(void); 
void Config_INTERRUPT(void);
void ADC_INTERRUPT(void);
void ADC_channel1(void);
void ADC_channel2(void);
void ADC_to_string(void);
void Show_val_LCD(void);
//******************************************************************************
//Ciclo Principal
//******************************************************************************

void main(void) {
    setup();
    TRISD = 0x00;
    Lcd_Init();

    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("S1:");
    Lcd_Set_Cursor(1, 7);
    Lcd_Write_String("S2:");
    Lcd_Set_Cursor(1, 14);
    Lcd_Write_String("S3:");
    
    while (1) {
        AADC_channel1();
        __delay_ms(1);
        ADC_channel2();
        
        Write_USART_String("S1   S2   S3 \n"); //enviar los datos del pic a la compu
        sprintf(data_total, "%2.1fV  %2.1fV    %d", S2_val, S1_val, cont); //convertir los valores de voltaje y el contador a un string para que los lea bien la compu
        Write_USART_String(data_total); //enviar el string con los valores a la pc
        Write_USART(13);//13 y 10 la secuencia es para dar un salto de linea 
        Write_USART(10);
        
        //sprintf(data2, "%dV    %dV    %d", VAL1, VAL2, cont);
        Lcd_Clear();
        Lcd_Set_Cursor(1,2); //nombres S1, S2 y S3
        Lcd_Write_String("S1:   S2:   S3:");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(data_total);



    }
    return;
}

//******************************************************************************
//Funciones
//******************************************************************************

void setup(void) {
    ANSEL = 0b00000011; //IO CONFIG
    ANSELH = 0;
    TRISA = 0b00000011;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    //ADC 
    ADCON1 = 0b00000000; //Justificado a la izquierda
    PIE1bits.ADIE = 0;
    PIR1bits.ADIF = 0;
    OPTION_REG = 0b00000000;
    INTCON = 0b00000000;

    //INTERRUPCION
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    INTCONbits.GIE = 1;

    // USART CONFIG
    SPBRG = 12; //BAUD RATE DE 9600 A 8MHz
    TXSTAbits.CSRC = 0;
    TXSTAbits.TX9 = 0; //8 bits
    TXSTAbits.TXEN = 1; //ENABLE
    TXSTAbits.SYNC = 0; // ASYNC
    TXSTAbits.BRGH = 0; //LOW SPEED
    TXSTAbits.TRMT = 0;

    RCSTAbits.SPEN = 1; // ENABLE
    RCSTAbits.RX9 = 0; //8 bits
    RCSTAbits.CREN = 1;



}

void ADC_CH0(void) {
    ADC_ch(0); //channel 0
    //Cinfiguracion bits ADCON0
    ADCON0bits.ADCS0 = 1; //Clock ADC conversion
    ADCON0bits.ADCS1 = 0; //Fosc
    ADCON0bits.ADON = 1; //Se habilita el ADC
    __delay_us(40); //Para conversion
    ADCON0bits.GO = 1; //Inicia la conversión
    while (ADCON0bits.GO != 0) { //Waiting for conversion to complete
        ADC_res1 = ADC_val(ADRESL, ADRESH);
        VAL1 = ((ADC_res1 * 50) / 255);

    }
}

void ADC_CH1(void) {
    ADC_ch(1); //channel 1
    ADCON0bits.ADCS0 = 1; //Clock ADC conversion
    ADCON0bits.ADCS1 = 0; //Fosc
    ADCON0bits.ADON = 1; //Se habilita el ADC
    __delay_us(40); //Para conversion
    ADCON0bits.GO = 1; //Inicia la conversión
    while (ADCON0bits.GO != 0) { //Waiting for conversion to complete
        ADC_res2 = ADC_val(ADRESL, ADRESH);
        VAL2 = ((ADC_res2 * 50) / 255);

    }
}




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
#include <pic16f887.h>
#include "Oscilador.h"
#include "LCD.h"
#include "Config_ADC.h"
#include "USART.h"


//****************************************************************************//
//CONFIGURACION BITS                                                          //
//****************************************************************************//
// CONFIG1
#pragma config FOSC = XT // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

float S1_val = 0.0;
float S2_val = 0.0;
char data_total[20];
uint8_t cont;
char data_recive;
//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void); 
void Config_INTERRUPT(void);
void ADC_INTERRUPT(void);
void ADC_channel1(void);
void ADC_channel2(void);
void ADC_to_string(void);
void Show_val_LCD(void);
//****************************************************************************//
//INTERRUPCIONES                                                              //
//****************************************************************************//
void __interrupt() ISR(void){
    if (PIR1bits.RCIF == 1){
        data_recive = Read_USART();
        //cont++;
        if (data_recive == '+'){
            cont++;
            PORTB = cont;
        }
        else if (data_recive == '-'){
            cont--;
        }
        data_recive = 0;
        }
    //return;
} 

//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//
void main(void) {
    setup(); //Configuracion de puertos de entrada y salida
    Lcd_Init();
    Lcd_Clear();
    cont = 0;
    TRISD = 0x00;
    
    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
        ADC_channel1();
        __delay_ms(1);
        ADC_channel2();
        
        Write_USART_String("S1    S2   Cont \n");
        sprintf(data_total, "%2.1fV  %2.1fV    %d", S2_val, S1_val, cont);
        //ADC_to_string();
        Write_USART_String(data_total);//enviar el string con los valores a la pc
        Write_USART(13);//13 y 10 la secuencia es para dar un salto de linea 
        Write_USART(10);
        
        Show_val_LCD();
        __delay_ms(500);
}
}
//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//
void ADC_channel1(void){
    ADC_Config (0);
    __delay_ms(1); //Inicio de conversion ADC
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
        S1_val = ((ADRESH * 5.0) / 255);
    }
}

void ADC_channel2(void){
    ADC_Config (1);
    __delay_ms(1); //Inicio de conversion ADC
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO != 0) { //Mientras no se haya termindo una convers.
        S2_val = ((ADRESH * 5.0) / 255);
    }
}

void ADC_to_string(void){
    sprintf(data_total, "%2.1fV  %2.1fV    %d", S2_val, S1_val, cont);
}

void Show_val_LCD(void){
    //Valores de S1 y S2
    Lcd_Clear();
    Lcd_Set_Cursor(1,2); //nombres S1, S2 y S3
    Lcd_Write_String("S1:   S2:   S3:");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(data_total);
}

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
    USART_Init_BaudRate();
    USART_Init();
    USART_INTERRUPT();
}


