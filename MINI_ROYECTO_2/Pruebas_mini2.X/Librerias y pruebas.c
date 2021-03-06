/*
 * https://simple-circuit.com/mplab-xc8-ds1307-ds3231-pic-mcu/
 * 
 * Real time clock using PIC16F887 MCU and RTC chip (DS1307, DS3231 or DS3232)
 * Time and date are displayed on 1602 LCD screen.
 * C Code for MPLAB XC8 compiler.
 * Internal oscillator used @ 8MHz.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
*/ 
 
// set configuration words
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
 
// button definitions
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
//End LCD module connections
 
 #define _XTAL_FREQ 8000000

#include <xc.h>
#include <stdint.h>        // include stdint header
#include "LCD.c"       // include LCD driver source file
#include "I2C.h"
#include "RTC.h"
 
// variables declaration
uint8_t  i, second, minute, hour, m_day, month, year;
 
/********************** I2C functions **************************/
 
/********************** end I2C functions **********************/
 
// a small function for button1 (B1) debounce
 
/********************** RTC chip functions *********************/

// convert decimal to BCD function

 
// display time and date function

// make editing parameter blinks function

/********************** end RTC chip functions *****************/
 
/*************************** main function *********************/
void main(void)
{
  OSCCON = 0X70;        // set internal oscillator to 8MHz
  ANSELH = 0;           // configure all PORTB pins as digital
  // enable RB0 and RB1 internal pull ups
  nRBPU  = 0;      // clear RBPU bit (OPTION_REG.7)
  WPUB   = 0x03;   // WPUB register = 0b00000011
 
  __delay_ms(1000);
 
  I2C_Master_Init(100000);   // initialize I2C bus with clock frequency of 100kHz
 
  Lcd_Init();        // initialize LCD module
 
  while(1) {
 
//    if(!button1)     // if button B1 is pressed
//    if(debounce())   // call debounce function (make sure if B1 is pressed)
//    {
//      i = 0;
//      hour   = edit(7,  1, hour);
//      minute = edit(10, 1, minute);
//      m_day  = edit(7,  2, m_day);
//      month  = edit(10, 2, month);
//      year   = edit(15, 2, year);
// 
//      while(debounce());  // call debounce function (wait for button B1 to be released)
// 
//      // convert decimal to BCD
//      minute = decimal_to_bcd(minute);
//      hour   = decimal_to_bcd(hour);
//      m_day  = decimal_to_bcd(m_day);
//      month  = decimal_to_bcd(month);
//      year   = decimal_to_bcd(year);
//      // end conversion
// 
//      // Write data to DS3231 RTC
//      I2C_Start();         // start I2C
//      I2C_Write(0xD0);     // RTC chip address
//      I2C_Write(0);        // send register address
//      I2C_Write(0);        // reset seconds and start oscillator
//      I2C_Write(minute);   // write minute value to RTC chip
//      I2C_Write(hour);     // write hour value to RTC chip
//      I2C_Write(1);        // write day value (not used)
//      I2C_Write(m_day);    // write date value to RTC chip
//      I2C_Write(month);    // write month value to RTC chip
//      I2C_Write(year);     // write year value to RTC chip
//      I2C_Stop();          // stop I2C
// 
//      __delay_ms(200);
// 
//    }
 
    // read current time and date from the RTC chip
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
    
    __delay_ms(50);   // wait 50 ms
 
  }
 
}
 
/*************************** end main function ********************************/