/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */



#ifndef USART_H
#define	USART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//******************************************************************************
//Configuracioes USART
//******************************************************************************
void USART_Init(void); // Config Trasmision y recepcion USART
void USART_Init_BaudRate(void); // Config para Baud rate de 9600
void Trasmission_1(char val_1_mapeado);// funcion para constantemente mandar los valores ADC
void Trasmission_2(char val_2_mapeado);
void USART_INTERRUPT(void);


#endif	/* USART_H */