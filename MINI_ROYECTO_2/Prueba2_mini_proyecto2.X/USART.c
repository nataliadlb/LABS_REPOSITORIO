#include "USART.h"
#include <stdio.h>
#include <stdint.h>
#include <xc.h>

#define _XTAL_FREQ 8000000

void USART_Init(void){ 
    // config general
    RCSTAbits.SPEN = 1; 
    TXSTAbits.SYNC = 0; //Asynchronous
    //trasmission
    TXSTAbits.TXEN = 1; // enable
    //reception
    RCSTAbits.CREN = 1;
    RCREG = 0;
}

void USART_Init_BaudRate(void){
    SPBRGH = 0;
    SPBRG = 12; //Para tener un Baud rate de 9600,115200 x = ((FOSC/Baud rate)/64) - 1
}

void USART_INTERRUPT(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    //PIE1bits.TXIE = 1;

}

//Extraido de https://electrosome.com/uart-pic-microcontroller-mplab-xc8/
void Write_USART(uint8_t a){
    while(!TRMT);
    TXREG=a;
}
void Write_USART_String(char *a){
    uint8_t i;
    for(i=0;a[i]!='\0';i++){
        Write_USART(a[i]);
    }
}
uint8_t Read_USART(){
  while(!RCIF);
  return RCREG;
}


