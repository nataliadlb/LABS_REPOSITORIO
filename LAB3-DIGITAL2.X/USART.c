#include "USART.h"
#include <stdio.h>
#include <stdlib.h>

void USART_Init(void){ 
    // config general
    RCSTAbits.SPEN = 1; 
    TXSTAbits.SYNC = 0; //Asynchronous
    //trasmission
    TXSTAbits.TXEN = 1; // enable
    TXSTAbits.TX9 = 0; // 8-bit transmission
    TXSTAbits.BRGH = 1; //High speed
    TXSTAbits.TRMT = 0; //TSR full
    //reception
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    RCREG = 0;
}

void USART_Init_BaudRate(void){
    SPBRGH = 0;
    SPBRG = 12; //Para tener un Baud rate de 9600 x = ((FOSC/Baud rate)/64) - 1
}

void USART_INTERRUPT(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    PIE1bits.TXIE = 1;
    //PIE1bits.RCIE = 1;
}

//void Trasmission_1(char val_1_mapeado){
//    TXREG = val_1_mapeado;
//    while (TRMT == 1){
//        return;
//    }
//}
//void Trasmission_2(char val_2_mapeado);

