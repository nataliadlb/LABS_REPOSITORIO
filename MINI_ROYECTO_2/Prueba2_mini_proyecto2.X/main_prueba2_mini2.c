/*
 * Título: Mini proyecto 2
 * Autor: Natalia de Leon Bercian
 * Carne: 18193
 * Seccion: 20
 * 
 * 
 * Created on 1 de marzo de 2021
 */

//****************************************************************************//
//IMPORTAR LIBRERIAS                                                          //
//****************************************************************************//
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "USART.h"
#include "LCD.h"
#include "RTC.h"
#include "Oscilador"

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
#define button1      RB0   // button B1 is connected to RB0 pin
#define button2      RB1   // button B2 is connected to RB1 pin
//LCD module connections
#define LCD_RS       RD0
#define LCD_EN       RD1
#define LCD_D4       RD2
#define LCD_D5       RD3
#define LCD_D6       RD4
#define LCD_D7       RD5
#define LCD_RS_DIR   TRISD0
#define LCD_EN_DIR   TRISD1
#define LCD_D4_DIR   TRISD2
#define LCD_D5_DIR   TRISD3
#define LCD_D6_DIR   TRISD4
#define LCD_D7_DIR   TRISD5
//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//
char data_recive;
uint8_t cont;
char prueba;
int holiwis;
uint8_t  i, second, minute, hour, m_day, month, year;
//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
//****************************************************************************//
//INTERRUPCIONES                                                    //
//****************************************************************************//

void __interrupt() ISR(void) {
    if(PIR1bits.RCIF == 1){
        data_recive = RCREG; //Recibe los datos que manda la terminal
        if (data_recive == 'P11'){ //auementa
            PORTAbits.RA6 = 1;
        }
        else if (data_recive == 'P10'){ //decrementa
            PORTAbits.RA6 = 0;
        }
        else if (data_recive == 'P21'){ //decrementa
            PORTAbits.RA7 = 1;
        }
        else if (data_recive == 'P20'){ //decrementa
            PORTAbits.RA7 = 0;
        }
        data_recive = 0;
        }
}
//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    setup();
    
    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while(1){
        // ----* SENSOR *---- //
//        I2C_Master_Start();
//        I2C_Master_Write(0x50);
//        I2C_Master_Write(PORTB);
//        I2C_Master_Stop();
//        __delay_ms(200);
//       
//        I2C_Master_Start();
//        I2C_Master_Write(0x51);
//        PORTD = I2C_Master_Read(0);
//        I2C_Master_Stop();
//        __delay_ms(200);
//        PORTB++;  
        I2C_Master_Start();           // start I2C
        I2C_Master_Write(0xD0);       // RTC chip address
        I2C_Master_Write(0);          // send register address
        I2C_Master_RepeatedStart();  // restart I2C
        I2C_Master_Write(0xD1);       // initialize data read
        second = I2C_Master_Read(1);  // read seconds from register 0
        minute = I2C_Master_Read(1);  // read minutes from register 1
        hour   = I2C_Master_Read(1);  // read hour from register 2
        I2C_Master_Read(1);           // read day from register 3 (not used)
        m_day  = I2C_Master_Read(1);  // read date from register 4
        month  = I2C_Master_Read(1);  // read month from register 5
        year   = I2C_Master_Read(0);  // read year from register 6
        I2C_Master_Stop();            // stop I2C

        RTC_display();    // print time & date
    
    //  LCD_Goto(1, 1);    // go to column 1, row 1
    //  LCD_Print(Time);   // print time
    //  LCD_Goto(1, 2);    // go to column 1, row 2
    //  LCD_Print(Date);   // print date
    
//    __delay_ms(50);   // wait 50 ms
//        // ----* USART *---- //
//        PORTAbits.RA7 = 1;
//        __delay_ms(1000);
//        PORTAbits.RA6 = 1;
//        __delay_ms(1000);
//        PORTAbits.RA7 = 0;
//        __delay_ms(1000);
//        PORTAbits.RA6 = 0;
//        __delay_ms(1000);
//        Write_USART(holiwis); 
//        Write_USART(13);//13 y 10 la secuencia es para dar un salto de linea 
//        Write_USART(10);
//        PORTD = holiwis;
//        holiwis++;
    }
    //return;
}

//****************************************************************************//
//CONFIGURACION  (puertos, bits...)                                           //
//****************************************************************************//

void setup(void) {
    OSCCON = 0X70;
    nRBPU  = 0;      // clear RBPU bit (OPTION_REG.7)
    WPUB   = 0x03;
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
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
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    Lcd_Init();
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//

