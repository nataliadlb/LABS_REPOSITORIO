/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DISPLAY_H
#define	DISPLAY_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>

//******************************************************************************
//initOsc
// Número para mostrar en el display en hexadecimal
// En el parámetro NUM se coloca el valor en decimal que se quiere que aparezca 
// en el display y se carga los leds del display para que aparezca
//******************************************************************************
void display(uint8_t NUM);

#endif	/* DISPLAY_H */

