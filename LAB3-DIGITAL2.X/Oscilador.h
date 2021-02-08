/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef OSCILADOR_H
#define	OSCILADOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//******************************************************************************
//initOsc
// Inicialización de Oscilador interno
// Tiene parámetro la opción del IRCF (pag. 62). Para configurar una frecuencia
// Ejemplo de como se va a llamar a la funcion: initOsc(7)
//******************************************************************************
void initOsc(uint8_t IRCF);


#endif	/* OSCILADOR_H */

