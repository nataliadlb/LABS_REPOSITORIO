/* 
 * File: LCD.h  
 *Se adapt� para 8 bits
 * 
 * Comentarios 
 * Se utiliz� y se adaptaron las librer�as de Ligo George 
 * de la p�gina www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/ 
 */

#include "LCD.h"

//------ Funci�n de inicio ------// 
void Lcd_Init(){
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x030);
    __delay_ms(5);
  Lcd_Cmd(0x030);
    __delay_us(160);
  Lcd_Cmd(0x030);
    __delay_us(160);
    // o busy flag...?
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x030); // CREO
  Lcd_Cmd(0x008);
  Lcd_Cmd(0x001);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x006);
}

//------ Funci�n para escoger el puerto ------//
void Lcd_Port(char a){
    PORTD = a;
//	if(a & 1)
//		D0 = 1;
//	else
//		D0 = 0;
//
//	if(a & 2)
//		D1 = 1;
//	else
//		D1 = 0;
//
//	if(a & 4)
//		D2 = 1;
//	else
//		D2 = 0;
//
//	if(a & 8)
//		D3 = 1;
//	else
//		D3 = 0;
//    
//    if(a & 16)
//		D4 = 1;
//	else
//		D4 = 0;
//    
//    if(a & 32)
//		D5 = 1;
//	else
//		D5 = 0;
//    
//     if(a & 64)
//		D6 = 1;
//	else
//		D6 = 0;
//    
//     if(a & 128)
//		D7 = 1;
//	else
//		D7 = 0;
}

void Lcd_Cmd(char a){
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;         // => E = 0
}

//------ Funci�n para limpiar el visualizador ------//
void Lcd_Clear(void){
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

//------ Funci�n para posicionar el cursor ------//
void Lcd_Set_Cursor(char a, char b){
	char temp,z,y;
	if(a == 1){
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2){
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

//------ Funci�n para mandar los caracteres al puerto ------//
void Lcd_Write_Char(char a){
   Lcd_Port(a);
   //maybeeeee....
   
//   char temp,y;
//   temp = a & 0x0F;
//   y = a & 0xF0;
//   RS = 1;             // => RS = 1
//   Lcd_Port(y>>4);             //Data transfer
//   EN = 1;
//   __delay_us(40);
//   EN = 0;
//   Lcd_Port(temp);
//   EN = 1;
//   __delay_us(40);
//   EN = 0;
}

//------ Funci�n para mandar strings al puerto ------//
void Lcd_Write_String(char *a){
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

//------ Funci�n para mover a la derecha ------//
void Lcd_Shift_Right(){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

//------ Funci�n para mover a la izquierda ------//
void Lcd_Shift_Left(){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}
