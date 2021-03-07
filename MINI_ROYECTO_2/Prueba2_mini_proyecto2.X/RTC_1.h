/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */



#ifndef RTC_H
#define	RTC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>


uint8_t bcd_to_decimal(uint8_t number);
uint8_t decimal_to_bcd(uint8_t number);
//void RTC_display(void);
//__bit debounce();
//void blink(void);
//uint8_t edit(uint8_t x, uint8_t y, uint8_t parameter);


#endif	/* RTC_H */