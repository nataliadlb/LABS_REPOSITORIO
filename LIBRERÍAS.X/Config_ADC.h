/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONFIG_ADC_H
#define	CONFIG_ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//******************************************************************************
//initOsc
// N�mero para mostrar en el display en hexadecimal
// En el par�metro NUM se coloca el valor en decimal que se quiere que aparezca 
// en el display y se carga los leds del display para que aparezca
//******************************************************************************
uint8_t ADC(uint8_t ADRESL_, uint8_t ADRESH_);

#endif	/* CONFIG_ADC_H */


