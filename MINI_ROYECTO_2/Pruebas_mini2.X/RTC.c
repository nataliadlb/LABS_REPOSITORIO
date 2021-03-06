
#include <xc.h>
#include <stdint.h>
#include "RTC.h"
#include "LCD.h"

#define button1      RB0   // button B1 is connected to RB0 pin
#define button2      RB1   // button B2 is connected to RB1 pin

uint8_t  i, second, minute, hour, m_day, month, year;

uint8_t bcd_to_decimal(uint8_t number) {
  return((number >> 4) * 10 + (number & 0x0F));
}
 
// convert decimal to BCD function
uint8_t decimal_to_bcd(uint8_t number) {
  return(((number / 10) << 4) + (number % 10));
}
 
// display time and date function
void RTC_display(){
  static char Time[] = "TIME: 00:00:00";
  static char Date[] = "DATE: 00/00/2000";
 
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
    Lcd_Set_Cursor(1,2);
    Lcd_Write_String(Date);
//  LCD_Goto(1, 1);    // go to column 1, row 1
//  LCD_Print(Time);   // print time
//  LCD_Goto(1, 2);    // go to column 1, row 2
//  LCD_Print(Date);   // print date
}

__bit debounce(){
  uint8_t count = 0;
  for(uint8_t i = 0; i < 5; i++) {
    if (button1 == 0)
      count++;
    __delay_ms(10);
  }
  if(count > 2)  return 1;
  else           return 0;
}

void blink(void)
{
  uint8_t j = 0;
  while(j < 100 && button1 && button2) {
    j++;
    __delay_ms(5);
  }
}
 
// Edit time and date function
uint8_t edit(uint8_t x, uint8_t y, uint8_t parameter)
{
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
 
      LCD_Goto(x, y);
      LCD_PutC(parameter / 10 + '0');
      LCD_PutC(parameter % 10 + '0');
      __delay_ms(200);
 
    }
 
    LCD_Goto(x, y);
    LCD_Print("  ");  // print 2 spaces
    blink();
 
    LCD_Goto(x, y);
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