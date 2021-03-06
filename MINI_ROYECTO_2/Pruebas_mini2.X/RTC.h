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
void RTC_display(void);

#endif	/* RTC_H */
