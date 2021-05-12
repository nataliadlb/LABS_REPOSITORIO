//*****************************************************************************
/**
 Laboratorio # 6
 Natalia de León Bericán
 carne: 18193
 Digital 2
 Sección 20
 Mayo 2021
*/
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

#define LED_ROJO GPIO_PIN_1
#define LED_VERDE GPIO_PIN_3
#define LED_AZUL GPIO_PIN_2

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
int main(void){
    //Config de reloj a 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Config reloj para habilitar puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Verificar que se habilitó puerto F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
     }
    // Configurar OUTPUTS/INPUTS
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_ROJO);   //RED LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); //BLUE LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); //GREEN LED


}
