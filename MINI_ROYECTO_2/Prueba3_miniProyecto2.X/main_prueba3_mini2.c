/*
 * Código -- Laboratorio # 3
 * Author: Natalia de León Bercián
 * carné: 18193
 * Digital 2
 *
 * Created on 1 de marzo de 2021
 * 
 * Las funciones relacionadas al sensor RTC fueron tomadas y luego modificadas 
 * del sitio: https://simple-circuit.com/mplab-xc8-ds1307-ds3231-pic-mcu/ 
 * Autor: Simple Projects
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
#include "USART.h"
#include "Oscilador.h"
#include "I2C.h"
//#include "RTC.h"

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
uint8_t  i, second, minute, hour, m_day, month, year;
char data_total[20];
uint8_t cont;
char data_recive;
char cont_send[8];


static char Time[] = "TIME: 00:00:00";
static char Date[] = "DATE: 00/00/00";

//******************************************************************************
//Prototipos de funciones
//******************************************************************************

void setup(void); 
void Write_to_RTC(void);
void RTC_display(void);
uint8_t decimal_to_bcd(uint8_t number); //decimal to Binary coded decimal
uint8_t bcd_to_decimal(uint8_t number); //Binary coded decimal to decimal

//******************************************************************************
//Interrupciones
//****************************************************************************** 

void __interrupt() ISR(void) {
    if(PIR1bits.RCIF == 1){
        data_recive = RCREG; //Recibe los datos que manda la terminal
        if (data_recive == '1'){ //auementa
            PORTAbits.RA6 = 1;
        }
        else if (data_recive == '2'){ //decrementa
            PORTAbits.RA6 = 0;
        }
        else if (data_recive == '3'){ //decrementa
            PORTAbits.RA7 = 1;
        }
        else if (data_recive == '4'){ //decrementa
            PORTAbits.RA7 = 0;
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
    Write_to_RTC(); //escribir valores iniciales de fecha y hora al RTC
    //Write_USART_String("HOLA PINCHE");
   
    while (1) {
        
        //Código obtenido de Simple Projects, pero modificado con la libreía I2C
        I2C_Master_Start();           // start I2C
        I2C_Master_Write(0xD0);       // RTC chip address
        I2C_Master_Write(0);          // send register address
        I2C_Master_RepeatedStart();   // restart I2C
        I2C_Master_Write(0xD1);       // initialize data read
        second = I2C_Master_Read(1);  // read seconds from register 0
        minute = I2C_Master_Read(1);  // read minutes from register 1
        hour   = I2C_Master_Read(1);  // read hour from register 2
        I2C_Master_Read(1);           // read day from register 3 (not used)
        m_day  = I2C_Master_Read(1);  // read date from register 4
        month  = I2C_Master_Read(1);  // read month from register 5
        year   = I2C_Master_Read(0);  // read year from register 6
        I2C_Master_Stop();            // stop I2C

        RTC_display(); //ir constantemente convirtiendo, aumentando y actualizando
        
        // ---- comunicación serial ---- //
        for(i = 0; i < 13; i++){
            Write_USART_String(Time[i]);
        }
        //Write_USART_String(Time); //enviar el string con los valores de hora
        //Write_USART_String("  ");
        //Write_USART_String(Date); //enviar el string con los valores de fecha
        Write_USART(13);//13 y 10 la secuencia es para dar un salto de linea 
        Write_USART(10);
        
//        sprintf(cont_send, "%d", cont);
//        Lcd_Set_Cursor(1,1);
//        Lcd_Write_String(cont_send);
//        
//        Write_USART_String(cont_send);
//        //xWrite_USART_String(cont);
//        Write_USART(13);//13 y 10 la secuencia es para dar un salto de linea 
//        Write_USART(10);
//        cont++;
        //__delay_ms(500);
        __delay_ms(100);
    }
}

//******************************************************************************
//Funciones
//******************************************************************************

// Convertir Binary coded decimal to decimal
uint8_t bcd_to_decimal(uint8_t number) {
  return((number >> 4) * 10 + (number & 0x0F));
}
 
// Convertir decimal to Binary coded decimal
uint8_t decimal_to_bcd(uint8_t number) {
  return(((number / 10) << 4) + (number % 10));
}
 
void RTC_display(void){
    // Convertir datos BCD a decimal
    second = bcd_to_decimal(second);
    minute = bcd_to_decimal(minute);
    hour   = bcd_to_decimal(hour);
    m_day  = bcd_to_decimal(m_day);
    month  = bcd_to_decimal(month);
    year   = bcd_to_decimal(year);
    // end conversion

    // uActualizar tiempo
    Time[6]  = hour   / 10 + '0';
    Time[7]  = hour   % 10 + '0';
    Time[9]  = minute / 10 + '0';
    Time[10] = minute % 10 + '0';
    Time[12] = second / 10 + '0';
    Time[13] = second % 10 + '0';
    
    // Actualizar fecha
    Date[6]  = m_day  / 10 + '0';
    Date[7]  = m_day  % 10 + '0';
    Date[9]  = month  / 10 + '0';
    Date[10] = month  % 10 + '0';
    Date[12] = year   / 10 + '0';
    Date[13] = year   % 10 + '0';
    
    //Mostrar en LCD (PRUEBA)
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(Time);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(Date);
}

//Escribir valores iniciales al RTC, obtenido de Simple projects
void Write_to_RTC(void){ 
    I2C_Master_Start();         // start I2C
    I2C_Master_Write(0xD0);     // RTC chip address
    I2C_Master_Write(0);        // send register address
    I2C_Master_Write(0);        // reset seconds and start oscillator
    I2C_Master_Write(48);        // write minute value to RTC chip //y media
    I2C_Master_Write(6);       // write hour value to RTC chip
    I2C_Master_Write(1);        // write day value (not used)
    I2C_Master_Write(8);        // write date value to RTC chip
    I2C_Master_Write(3);        // write month value to RTC chip
    I2C_Master_Write(27);       // write year value to RTC chip
    I2C_Master_Stop();          // stop I2C
}

// ------ configuraciones ----- //
void setup(void) {
    //OSCCON = 0x07;
    ANSEL = 0; //RA0 y RA1 como analogico
    ANSELH = 0; 
    TRISA = 0; //potenciometros, como entrada
    TRISB = 0b00000011;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    TRISD = 0; 
    TRISE = 0;
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    I2C_Master_Init(100000);
    USART_Init_BaudRate();
    USART_Init();
    USART_INTERRUPT();

}