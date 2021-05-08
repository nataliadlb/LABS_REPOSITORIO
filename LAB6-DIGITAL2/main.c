//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

//*****************************************************************************
// VARIABLES
//*****************************************************************************
uint8_t i;
uint8_t semaforo = 0;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************
int
main(void)
{
    volatile uint32_t ui32Loop;

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); //RED LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); //BLUE LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); //GREEN LED

    //
    // Loop forever.
    //
    while(1){
        if(semaforo == 0){
            // Turn on the RED 1LED.
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
            for(ui32Loop = 0; ui32Loop < 900000; ui32Loop++){ // Delay for a bit.
            }
            // Turn off the RED LED.
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
            for(ui32Loop = 0; ui32Loop < 900000; ui32Loop++){
            }


            // Turn on the BLUE LED.
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            for(ui32Loop = 0; ui32Loop < 900000; ui32Loop++){ // Delay for a bit.
            }

            // Turn off the BLUE LED.
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
            for(ui32Loop = 0; ui32Loop < 900000; ui32Loop++){
            }

            for (i = 0; i < 3; i++){
                // Turn on the GREEN LED.
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
                for(ui32Loop = 0; ui32Loop < 400000; ui32Loop++){ // Delay for a bit
                 }

                // Turn off the LED.
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
                for(ui32Loop = 0; ui32Loop < 400000; ui32Loop++){
                }
            }

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
            for(ui32Loop = 0; ui32Loop < 900000; ui32Loop++){
            }
            semaforo = 1;

        }

    }
}
