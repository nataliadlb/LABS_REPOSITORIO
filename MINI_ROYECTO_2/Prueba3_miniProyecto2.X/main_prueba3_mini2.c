/*
 * C�digo -- Laboratorio # 3
 * Author: Natalia de Le�n Berci�n
 * carn�: 18193
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

#define button1      RB0   // button B1 is connected to RB0 pin
#define button2      RB1   // button B2 is connected to RB1 pin

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

static char Time[] = "TIME: 20:37:00";
static char Date[] = "DATE: 06/03/2021";

//******************************************************************************
//Prototipos de funciones
//******************************************************************************

void setup(void); 
void Show_val_LCD(void);
void RTC_display(void);
uint8_t decimal_to_bcd(uint8_t number);
uint8_t bcd_to_decimal(uint8_t number);
__bit debounce ();
uint8_t edit(uint8_t x, uint8_t y, uint8_t parameter);


//******************************************************************************
//Interrupciones
//****************************************************************************** 

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

//******************************************************************************
//Ciclo Principal
//******************************************************************************

void main(void) {
    setup();
    TRISD = 0x00;
    Lcd_Init();
    Lcd_Clear();

    
    while (1) {

//        if(!button1)     // if button B1 is pressed
//        if(debounce()){ // call debounce function (make sure if B1 is pressed)
//          i = 0;
//          hour   = edit(7,  1, hour);
//          minute = edit(10, 1, minute);
//          m_day  = edit(7,  2, m_day);
//          month  = edit(10, 2, month);
//          year   = edit(15, 2, year);
//
//          while(debounce());  // call debounce function (wait for button B1 to be released)
//
//          // convert decimal to BCD
//          minute = decimal_to_bcd(minute);
//          hour   = decimal_to_bcd(hour);
//          m_day  = decimal_to_bcd(m_day);
//          month  = decimal_to_bcd(month);
//          year   = decimal_to_bcd(year);
//          // end conversion
//
//          // Write data to DS3231 RTC
//          I2C_Master_Start();         // start I2C
//          I2C_Master_Write(0xD0);     // RTC chip address
//          I2C_Master_Write(0);        // send register address
//          I2C_Master_Write(0);        // reset seconds and start oscillator
//          I2C_Master_Write(minute);   // write minute value to RTC chip
//          I2C_Master_Write(hour);     // write hour value to RTC chip
//          I2C_Master_Write(1);        // write day value (not used)
//          I2C_Master_Write(m_day);    // write date value to RTC chip
//          I2C_Master_Write(month);    // write month value to RTC chip
//          I2C_Master_Write(year);     // write year value to RTC chip
//          I2C_Master_Stop();          // stop I2C
//
//      __delay_ms(200);
// 
//    }
        
          // convert decimal to BCD
          //minute = decimal_to_bcd(minute);
          //hour   = decimal_to_bcd(hour);
//          m_day  = decimal_to_bcd(m_day);
//          month  = decimal_to_bcd(month);
//          year   = decimal_to_bcd(year);
          // end conversion

          // Write data to DS3231 RTC
          I2C_Master_Start();         // start I2C
          I2C_Master_Write(0xD0);     // RTC chip address
          I2C_Master_Write(0);        // send register address
          I2C_Master_Write(0);        // reset seconds and start oscillator
          I2C_Master_Write(minute);   // write minute value to RTC chip
          I2C_Master_Write(hour);     // write hour value to RTC chip
          I2C_Master_Write(1);        // write day value (not used)
          I2C_Master_Write(6);    // write date value to RTC chip
          I2C_Master_Write(3);    // write month value to RTC chip
          I2C_Master_Write(27);     // write year value to RTC chip
          I2C_Master_Stop();          // stop I2C

      __delay_ms(200);
        
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

        RTC_display();
        
        //__delay_ms(100);
        

    }
}

//******************************************************************************
//Funciones
//******************************************************************************


uint8_t bcd_to_decimal(uint8_t number) {
  return((number >> 4) * 10 + (number & 0x0F));
}
 
// convert decimal to BCD function
uint8_t decimal_to_bcd(uint8_t number) {
  return(((number / 10) << 4) + (number % 10));
}
 
void RTC_display(void){
    // convert data from BCD format to decimal format
    second = bcd_to_decimal(second);
    minute = bcd_to_decimal(minute);
    hour   = bcd_to_decimal(hour);
    m_day  = bcd_to_decimal(m_day);
    month  = bcd_to_decimal(month);
    year   = bcd_to_decimal(year);
    // end conversion

    // update time
    Time[6]  = hour   / 10 + '0';
    Time[7]  = hour   % 10 + '0';
    Time[9]  = minute / 10 + '0';
    Time[10] = minute % 10 + '0';
    Time[12] = second / 10 + '0';
    Time[13] = second % 10 + '0';
    // update date
    Date[6]  = m_day  / 10 + '0';
    Date[7]  = m_day  % 10 + '0';
    Date[9]  = month  / 10 + '0';
    Date[10] = month  % 10 + '0';
    Date[14] = year   / 10 + '0';
    Date[15] = year   % 10 + '0';
    
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(Time);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(Date);
}

__bit debounce (){
  uint8_t count = 0;
  for(uint8_t i = 0; i < 5; i++) {
    if (button1 == 0)
      count++;
    __delay_ms(10);
  }
  if(count > 2)  return 1;
  else           return 0;
}

// make editing parameter blinks function
void blink()
{
  uint8_t j = 0;
  while(j < 100 && button1 && button2) {
    j++;
    __delay_ms(5);
  }
}
 
// Edit time and date function
uint8_t edit(uint8_t x, uint8_t y, uint8_t parameter){
  while(debounce());  // call debounce function (wait for B1 to be released)
 
  while(1) {
 
    while(!button2)    // if button B2 is pressed
    {
      parameter++;
      if(i == 0 && parameter > 23)   // if hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 1 && parameter > 59)   // if minutes > 59 ==> minutes = 0
        parameter = 0;
      if(i == 2 && parameter > 31)   // if date > 31 ==> date = 1
        parameter = 1;
      if(i == 3 && parameter > 12)   // if month > 12 ==> month = 1
        parameter = 1;
      if(i == 4 && parameter > 99)   // if year > 99 ==> year = 0
        parameter = 0;
 
      Lcd_Set_Cursor(x, y);
      LCD_PutC(parameter / 10 + '0');
      LCD_PutC(parameter % 10 + '0');
      __delay_ms(200);
 
    }
 
    Lcd_Set_Cursor(x, y);
    Lcd_Write_String("  ");  // print 2 spaces
    blink();
 
    Lcd_Set_Cursor(x, y);
    LCD_PutC(parameter / 10 + '0');
    LCD_PutC(parameter % 10 + '0');
    blink();
 
    if(!button1)     // if button B1 is pressed
    if(debounce())   // call debounce function (make sure if B1 is pressed)
    {
      i++;   // increment 'i' for the next parameter
      return parameter;     // return parameter value and exit
    }
 
  }
 
}

// ------ configuraciones ----- //
void setup(void) {
    //initOsc(7); //8MHz
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
    //USART_Init_BaudRate();
    //USART_Init();
    //USART_INTERRUPT();

}