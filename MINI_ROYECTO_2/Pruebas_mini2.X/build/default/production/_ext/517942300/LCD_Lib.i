# 1 "../../../../../../../Downloads/LCD_Lib.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files (x86)/Microchip/MPLABX/v5.40/packs/Microchip/PIC16Fxxx_DFP/1.2.33/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "../../../../../../../Downloads/LCD_Lib.c" 2








#pragma warning disable 520


# 1 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 1 3
# 13 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef signed char int8_t;






typedef signed int int16_t;







typedef __int24 int24_t;







typedef signed long int int32_t;
# 52 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef unsigned char uint8_t;





typedef unsigned int uint16_t;






typedef __uint24 uint24_t;






typedef unsigned long int uint32_t;
# 88 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef signed char int_least8_t;







typedef signed int int_least16_t;
# 109 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef __int24 int_least24_t;
# 118 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef signed long int int_least32_t;
# 136 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef unsigned char uint_least8_t;






typedef unsigned int uint_least16_t;
# 154 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef __uint24 uint_least24_t;







typedef unsigned long int uint_least32_t;
# 181 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef signed char int_fast8_t;






typedef signed int int_fast16_t;
# 200 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef __int24 int_fast24_t;







typedef signed long int int_fast32_t;
# 224 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef unsigned char uint_fast8_t;





typedef unsigned int uint_fast16_t;
# 240 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef __uint24 uint_fast24_t;






typedef unsigned long int uint_fast32_t;
# 268 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef int32_t intmax_t;
# 282 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c90\\stdint.h" 3
typedef uint32_t uintmax_t;






typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 11 "../../../../../../../Downloads/LCD_Lib.c" 2
# 34 "../../../../../../../Downloads/LCD_Lib.c"
__bit RS;

void LCD_Write_Nibble(uint8_t n);
void LCD_Cmd(uint8_t Command);
void LCD_Goto(uint8_t col, uint8_t row);
void LCD_PutC(char LCD_Char);
void LCD_Print(char* LCD_Str);
void LCD_Begin();

void LCD_Write_Nibble(uint8_t n)
{
  LCD_RS = RS;
  LCD_D4 = n & 0x01;
  LCD_D5 = (n >> 1) & 0x01;
  LCD_D6 = (n >> 2) & 0x01;
  LCD_D7 = (n >> 3) & 0x01;


  LCD_EN = 0;
  __delay_us(1);
  LCD_EN = 1;
  __delay_us(1);
  LCD_EN = 0;
  __delay_us(100);
}

void LCD_Cmd(uint8_t Command)
{
  RS = 0;
  LCD_Write_Nibble(Command >> 4);
  LCD_Write_Nibble(Command);
  if((Command == 0x01) || (Command == 0x02))
    __delay_ms(2);
}

void LCD_Goto(uint8_t col, uint8_t row)
{
  switch(row)
  {
    case 2:
      LCD_Cmd(0xC0 + col - 1);
      break;
    case 3:
      LCD_Cmd(0x94 + col - 1);
      break;
    case 4:
      LCD_Cmd(0xD4 + col - 1);
    break;
    default:
      LCD_Cmd(0x80 + col - 1);
  }

}

void LCD_PutC(char LCD_Char)
{
  RS = 1;
  LCD_Write_Nibble(LCD_Char >> 4);
  LCD_Write_Nibble(LCD_Char );
}

void LCD_Print(char* LCD_Str)
{
  uint8_t i = 0;
  RS = 1;
  while(LCD_Str[i] != '\0')
  {
    LCD_Write_Nibble(LCD_Str[i] >> 4);
    LCD_Write_Nibble(LCD_Str[i++] );
  }
}

void LCD_Begin()
{
  RS = 0;

  LCD_RS = 0;
  LCD_EN = 0;
  LCD_D4 = 0;
  LCD_D5 = 0;
  LCD_D6 = 0;
  LCD_D7 = 0;
  LCD_RS_DIR = 0;
  LCD_EN_DIR = 0;
  LCD_D4_DIR = 0;
  LCD_D5_DIR = 0;
  LCD_D6_DIR = 0;
  LCD_D7_DIR = 0;

  __delay_ms(40);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(0x02);
  __delay_ms(5);
  LCD_Cmd(0x20 | (2 << 2));
  __delay_ms(50);
  LCD_Cmd(0x0C);
  __delay_ms(50);
  LCD_Cmd(0x01);
  __delay_ms(50);
  LCD_Cmd(0x04 | 0x02);
  __delay_ms(50);
}
