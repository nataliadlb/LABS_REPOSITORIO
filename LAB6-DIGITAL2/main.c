//*****************************************************************************
// Laboratorio # 6
// Natalia de León Bericán
// carne: 18193
// Digital 2
// Sección 20
// Mayo 2021
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/systick.h"

#define LED_ROJO GPIO_PIN_1
#define LED_VERDE GPIO_PIN_3
#define LED_AMARILLO GPIO_PIN_1|GPIO_PIN_3

//*****************************************************************************
//
// VARIABLES
//
//*****************************************************************************
uint8_t i;
uint8_t semaforo = 0;
uint32_t value = 0;

//*****************************************************************************
//
// PROTOTIPOS DE FUNCIONES
//
//*****************************************************************************
void delayMs(uint32_t ui32Ms);


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
// CONFIGURACION
//
//*****************************************************************************
int
main(void){
    //Config de reloj
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
    }

    // Enable the GPIO pins. OUTPUTS/INPUTS
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_ROJO); //RED LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); //BLUE LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); //GREEN LED
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4); //PUSH COMO ENTRADA

    //*****************************************************************************
    // Loop forever.
    //*****************************************************************************

    while(1){
        if ( !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) ){
            semaforo = 1;
            if(semaforo == 1 & GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){
                // Turn on the RED 1LED.
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED_VERDE);
                delayMs(1000);

                // Turn off the RED LED.
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
                delayMs(1000);

                for (i = 0; i < 3; i++){
                    // Turn on the GREEN LED.
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED_VERDE);
                    delayMs(500);

                    // Turn off the LED.
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
                    delayMs(500);
                }

                // Turn on YELLOW LED
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED_AMARILLO);
                delayMs(1000);

                // Turn off YELLOW
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
                delayMs(1000);

                // Turn on the RED 1LED.
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED_ROJO);
                delayMs(1000);

                semaforo = 0;
            }
        }

        //value= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);
        //if( (value & GPIO_PIN_4) == 0)
        //    semaforo = 1;


        //if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0){
        //    semaforo = 1;
        //}



    }
}


//*****************************************************************************
//
// FUNCIONES
//
//*****************************************************************************
void delayMs(uint32_t ui32Ms) {  //Funcion obtenida de https://gist.github.com/ctring/7f12d812fb594eecc493
    SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}
