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



#endif	/* USART_H */