/*
 * Título: Maestro mini proyecto
 * Autor: Natalia de Leon Bercian
 * Carne: 18193
 * Seccion: 20
 * 
 * 
 * Created on 15 de febrero de 2021
 */


//****************************************************************************//
//IMPORTAR LIBRERIAS                                                          //
//****************************************************************************//
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "LCD.h"
#include "Oscilador.h"
#include "USART.h"
#include "SPI.h"

//****************************************************************************//
//CONFIGURACION BITS                                                          //
//****************************************************************************//
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
char data_total[20];
char data[8];
uint8_t hola_esclavo;
uint8_t cont;
//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
void ADC_to_string(void);
void Show_val_LCD(void);

//****************************************************************************//
//INTERRUPCIONES                                                    //
//****************************************************************************//

void __interrupt() ISR(void) {
}

//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    setup();
    PORTB = 0;
    cont = 0;
    //TRISD = 0x00;
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,2); //nombres S1, S2 y S3
    Lcd_Write_String("S1:   S2:   S3:");


    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while (1) {
        RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(hola_esclavo);
       cont = spiRead();
       
       __delay_ms(1);
       RC2 = 1;       //Slave Deselect 
       
       __delay_ms(100);

        //Write_USART_String("S1   S2   S3 \n"); 
       PORTB = cont;
       ADC_to_string();
         
//        Write_USART_String(data_total); //enviar el string con los valores a la pc
//        Write_USART(13);//13 y 10 la secuencia es para dar un salto de linea 
//        Write_USART(10);
        
        Show_val_LCD();
        //__delay_ms(500);
    }
}

//****************************************************************************//
//CONFIGURACION                                            //
//****************************************************************************//

//----- puertos -----//
void setup(void) {
    initOsc(7); //8MHz
    ANSEL = 0; 
    ANSELH = 0; 
    
    TRISA = 0;
    TRISB = 0;
    //TRISCbits.TRISC6 = 0;
    //TRISCbits.TRISC7 = 1; // RX
    
    TRISD = 0; 
    TRISE = 0;
    
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    //----- SPI -----//
    //PORTCbits.RC2 = 1;
    TRISC2 = 0;
    PORTCbits.RC2 = 1;
    //PORTCbits.RC0 = 1;
    
    //----- USART -----//
    //USART_Init_BaudRate();
    //USART_Init();
    //USART_INTERRUPT();
    
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}




//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//

void ADC_to_string(void){
   
    sprintf(data, "%.3i", cont);
    //sprintf(data, "%d", cont);
}

void Show_val_LCD(void){
    //---- Valores de S1 y S2 ----//
    //Lcd_Clear();
    Lcd_Set_Cursor(2,2);
    Lcd_Write_String(data);
//    Lcd_Write_Char(data[0]);
//    Lcd_Write_Char(data[1]);
//    Lcd_Write_Char(data[2]);
//    Lcd_Write_Char(data[3]);
    
    //    Lcd_Write_String(data_total);
    
//    lcd_write_char(data[0]);
//    lcd_write_char('.');
//    lcd_write_char(str_pot_a[2]);
//    lcd_write_char(str_pot_a[3]);
//    lcd_write_char(str_pot_a[4]);
//    lcd_write_char(' ');
}