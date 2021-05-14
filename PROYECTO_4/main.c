//*****************************************************************************
/**
 Proyecto # 4
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
#include "driverlib/debug.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include <string.h>
#include "grlib/grlib.h"
#include "driverlib/pin_map.h"

//#define XTAL 16000000

//*****************************************************************************
//
// VARIABLES
//
//*****************************************************************************
uint32_t ui32Period; //periodo para TMR0
uint32_t ui32Status; //Status de UART

uint8_t Ban_rojo = 0;
uint8_t Ban_verde = 0;
uint8_t Ban_azul = 0;
char letra;

//*****************************************************************************
//
// PROTOTIPOS DE FUNCIONES
//
//*****************************************************************************
void Timer0IntHandler(void);
void InitUART(void);
void UARTIntHandler(void);


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

    // Config reloj para habilitar puertos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Verificar que se habilitó puerto F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
     }
    // Verificar que se habilitó puerto F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){
     }

    // Configurar OUTPUTS/INPUTS
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4); //PUSH COMO ENTRADA
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD); //weak pull up
    /**
        CONFIGURACION UART
    */
    // Se inicializa la comunicación UART
    InitUART();

    /**
        CONFIGURACION TIMER
    */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){
    }

    // Configuración del Timer 0 como temporizador períodico
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Se calcula el período para el temporizador (1 seg)
    ui32Period = (SysCtlClockGet()) / 2;

    // Establecer el periodo del temporizador
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

    TimerEnable (TIMER0_BASE, TIMER_A|TIMER_B);

    // Se establece que exista la interrupción por Timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //Establecemos la ISR
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);

    // Se habilita la interrupción por el TIMER0A
    IntEnable(INT_TIMER0A);

    // Se habilita el Timer
    TimerEnable(TIMER0_BASE, TIMER_A);


    // Se manda mensajes por UART
    UARTCharPut(UART0_BASE, 'C');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 'm');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 'z');
    UARTCharPut(UART0_BASE, 'a');
    UARTCharPut(UART0_BASE, 'm');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 's');
    UARTCharPut(UART0_BASE, 10);
    UARTCharPut(UART0_BASE, 13);


    //*****************************************************************************
    // Loop forever.
    //*****************************************************************************

    while(1){
        if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_4))){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1 , GPIO_PIN_1);
        }

    }
}

//*****************************************************************************
//
// FUNCIONES
//
//*****************************************************************************

//**************************************************************************************************************
// Handler de la interrupcion del TIMER 0 - Recordar modificar el archivo tm4c123ght6pm_startup_css.c
//**************************************************************************************************************
void Timer0IntHandler(void){
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);


}

//**************************************************************************************************************
// Handler de la interrupcion del UART - Recordar modificar el archivo tm4c123ght6pm_startup_css.c
//**************************************************************************************************************
void UARTIntHandler(void){
        // Get the interrupt status
        ui32Status = UARTIntStatus(UART0_BASE, true);

        // Clear the asserted interrupts.
        UARTIntClear(UART0_BASE, ui32Status);

        // Loop while there are characters in the receive FIFO.
        while(UARTCharsAvail(UART0_BASE)){
            letra = UARTCharGet(UART0_BASE);
            UARTCharPutNonBlocking(UART0_BASE,letra); //manda a un puerto especifico

        }
}


//**************************************************************************************************************
// Inicialización de UART - Tomado del ejemplo que subio Pablo Mazariegos
//**************************************************************************************************************
void InitUART(void){

    /*Enable the peripheral UART Module 0*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){
    }

    /*Enable the GPIO Port A*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    // Se habilitan las interrupciones Globales
    IntMasterEnable();

    /* Make the UART pins be peripheral controlled. */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTDisable(UART0_BASE);
    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(
            UART0_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    IntEnable (INT_UART0);

    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTEnable (UART0_BASE);

}


