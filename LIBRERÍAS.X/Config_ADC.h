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
// Número para mostrar en el display en hexadecimal
// En el parámetro NUM se coloca el valor en decimal que se quiere que aparezca 
// en el display y se carga los leds del display para que aparezca
//******************************************************************************
int ADC(uint8_t ADRESL_, uint8_t ADRESH_);
int SWAP_ADC(uint8_t VAL_ADC);
int NIBBLE1_ADC(uint8_t VAL_ADC);
int NIBBLE2_ADC(uint8_t VAL_SWAP);

#endif	/* CONFIG_ADC_H */


