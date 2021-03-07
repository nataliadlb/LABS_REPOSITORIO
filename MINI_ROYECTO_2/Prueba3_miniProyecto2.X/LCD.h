/* 
 * File: librería LCD  
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS PORTEbits.RE0
#endif

#ifndef RW
#define RW PORTEbits.RE1
#endif

#ifndef EN
#define EN PORTEbits.RE2
#endif

#ifndef D0
#define D0 PORTDbits.RD0
#endif

#ifndef D1
#define D1 PORTDbits.RD1
#endif

#ifndef D2
#define D2 PORTDbits.RD2
#endif

#ifndef D3
#define D3 PORTDbits.RD3
#endif

#ifndef D4
#define D4 PORTDbits.RD4
#endif

#ifndef D5
#define D5 PORTDbits.RD5
#endif

#ifndef D6
#define D6 PORTDbits.RD6
#endif

#ifndef D7
#define D7 PORTDbits.RD7
#endif

// This is a guard condition so that contents of this file are not included
// more than once. 

#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//******************************************************************************
//initOsc
// Número para mostrar en el display en hexadecimal
// En el parámetro NUM se coloca el valor en decimal que se quiere que aparezca 
// en el display y se carga los leds del display para que aparezca
//******************************************************************************
void Lcd_Init(void);
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();
void Lcd_Clear(void);



#endif	/* LCD_H */


