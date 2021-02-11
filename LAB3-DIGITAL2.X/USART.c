#include "USART.h"

void USART_Init(void){ 
    // config general
    RCSTAbits.SPEN = 1; 
    TXSTAbits.SYNC = 0; //Asynchronous
    //trasmission
    TXSTAbits.TXEN = 1; // enable
    TXSTAbits.TX9 = 0; // 8-bit transmission
    TXSTAbits.BRGH = 0; //Low speed
    TXSTAbits.TRMT = 0; //TSR full
    //reception
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
}

void USART_Init_BaudRate(void){
    SPBRGH = 12; //Para tener un Baud rate de 9600 x = ((FOSC/Baud rate)/64) - 1
}

