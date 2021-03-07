/* 
 * File: LCD.h  
 *Se adaptó para 8 bits
 * 
 * Comentarios 
 * Se utilizó y se adaptaron las librerías de Ligo George 
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/ 
 */

#include <xc.h>
#include "LCD.h"

//------ Función de inicio ------// 
void Lcd_Init(void){
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x30);
    __delay_ms(5);
  Lcd_Cmd(0x30);
    __delay_ms(11);
  Lcd_Cmd(0x30);
  Lcd_Cmd(0x38); //numero de lineas
  Lcd_Cmd(0x0C); //apagado
  Lcd_Cmd(0x01);
  Lcd_Cmd(0x06);
}

//------ Función para escoger el puerto ------//
void Lcd_Port(char a){
    PORTD = a;
}

void Lcd_Cmd(char a){
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;         // => E = 0
}

//------ Función para limpiar el visualizador ------//
void Lcd_Clear(void){
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

//------ Función para posicionar el cursor ------//
void Lcd_Set_Cursor(char a, char b){
	char temp;
	if(a == 1){
        temp = 0x80 + b - 1;
        Lcd_Cmd(temp);
	}
	else if(a == 2){
		temp = 0xC0 + b - 1;
		Lcd_Cmd(temp);
	}
}

//------ Función para mandar los caracteres al puerto ------//
void Lcd_Write_Char(char a){
   RS = 1;             // => RS = 1
   Lcd_Port(a);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
}

//------ Función para mandar strings al puerto ------//
void Lcd_Write_String(char *a){
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

//------ Función para mover a la derecha ------//
void Lcd_Shift_Right(void){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

//------ Función para mover a la izquierda ------//
void Lcd_Shift_Left(void){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}

