//*****************************************************************************
/**
 Proyecto # 4
 Natalia de Le�n Beric�n
 carne: 18193
 Digital 2
 Secci�n 20
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
#include "driverlib/pin_map.h"
#include <string.h>
#include "grlib/grlib.h"

#include "driverlib/fpu.h"
#include "inc/hw_uart.h"
#include "driverlib/rom_map.h"

#define XTAL 16000000
#define led_green1 GPIO_PIN_5 // PORT B
#define led_green2 GPIO_PIN_0 // PORT D
#define led_green3 GPIO_PIN_4 // PORT E
#define led_green4 GPIO_PIN_3 // PORT D

#define led_red1 GPIO_PIN_1 // PORT B
#define led_red2 GPIO_PIN_2 // PORT D
#define led_red3 GPIO_PIN_4 // PORT B
#define led_red4 GPIO_PIN_2 // PORT E

#define PUSH_1 GPIO_PIN_0 // PORT B
#define PUSH_2 GPIO_PIN_1 // PORT D
#define PUSH_3 GPIO_PIN_5 // PORT E
#define PUSH_4 GPIO_PIN_1 // PORT E

#define led_a GPIO_PIN_7 // PORT B
#define led_b GPIO_PIN_4 // PORT A
#define led_c GPIO_PIN_7 // PORT C
#define led_d GPIO_PIN_6 // PORT C
#define led_e GPIO_PIN_3 // PORT A
#define led_f GPIO_PIN_6 // PORT D
#define led_g GPIO_PIN_3 // PORT B
#define led_dp GPIO_PIN_6 // PORT B


//*****************************************************************************
//
// VARIABLES
//
//*****************************************************************************

uint8_t ban_ocupado = 0; //valor de parqueos ocupados
uint8_t debouncing = 0; //debouncing de todos los botones
uint8_t Aumento1 = 1; //bandera para saber si ya se presiono push1
uint8_t Aumento2 = 1; //bandera para saber si ya se presiono push2
uint8_t Aumento3 = 1; //bandera para saber si ya se presiono push3
uint8_t Aumento4 = 1; //bandera para saber si ya se presiono push4

//*****************************************************************************
//
// PROTOTIPOS DE FUNCIONES
//
//*****************************************************************************
void InitUART(void);
void DISPLAY(uint8_t num_display);
void LEDS_G_R(void);
void Mostrar_display(void);

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
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Verificar que se habilitaron los puertos
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){
     }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){
     }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)){
     }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){
     }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){
     }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
     }

    // Configurar OUTPUTS/INPUTS
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_4);
    //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0); //PUSH COMO ENTRADA
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_5);

    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD); //weak pull DOWN
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD); //weak pull DOWN
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD); //weak pull DOWN
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD); //weak pull DOWN

    /**
        CONFIGURACION UART
    */
    // Se inicializa la comunicaci�n UART
    InitUART();

    //TODOS LOS PARQUEOS DISPONIBLES
    GPIOPinWrite(GPIO_PORTB_BASE, led_green1 , led_green1);
    GPIOPinWrite(GPIO_PORTD_BASE, led_green2 , led_green2);
    GPIOPinWrite(GPIO_PORTE_BASE, led_green3 , led_green3);
    GPIOPinWrite(GPIO_PORTD_BASE, led_green4 , led_green4);
    DISPLAY(4);
    ban_ocupado = 0;

    //*****************************************************************************
    // Loop forever.
    //*****************************************************************************

    while(1){
        LEDS_G_R(); // En donde lee si se presionan los push
        Mostrar_display(); //Muestra en el display el numero de parqueos libres
    }
}

//*****************************************************************************
//
// FUNCIONES
//
//*****************************************************************************

//**************************************************************************************************************
// FUNCION PARA ENCENDER LEDS DEPENDIENDO SI ESTA VACIO U OCUPADO
//**************************************************************************************************************
void LEDS_G_R(void){
    if((GPIOPinRead(GPIO_PORTB_BASE, PUSH_1))){
        while(GPIOPinRead(GPIO_PORTB_BASE, PUSH_1)){}              //debouncing push 1
        if (Aumento1 == 1){
            GPIOPinWrite(GPIO_PORTB_BASE, led_green1 , 0);         //se apaga led verde
            GPIOPinWrite(GPIO_PORTB_BASE, led_red1 , led_red1);    //se enciende led rojo
            ban_ocupado++;                                         //aumenta valor de parqueos ocupados
            debouncing = 1;                                        //bandera de debouncing
            Aumento1 = 0;                                          //control de que si ya se apacho una vez el boton
            UARTCharPut(UART1_BASE, '0');                          //se manda el caracter por UART
        }
        else if (Aumento1 == 0){
            GPIOPinWrite(GPIO_PORTB_BASE, led_green1 , led_green1); //se enciende led verde
            GPIOPinWrite(GPIO_PORTB_BASE, led_red1 , 0);            //se apaga led rojo
            ban_ocupado--;                                          //se disminuye variable de parqueos ocupados
            debouncing = 1;                                         //bandera debouncing
            Aumento1 = 1;                                           //bandera que significa que se presiono por segunda vez el boton
            UARTCharPut(UART1_BASE, '1');                           //se manda valor a UART
        }

    }
    else if((GPIOPinRead(GPIO_PORTD_BASE, PUSH_2))){
        while(GPIOPinRead(GPIO_PORTD_BASE, PUSH_2)){}               //debouncing push2
        if (Aumento2 == 1){
            GPIOPinWrite(GPIO_PORTD_BASE, led_green2 , 0);          //OFF green
            GPIOPinWrite(GPIO_PORTD_BASE, led_red2 , led_red2);     //ON red
            ban_ocupado++;
            debouncing = 1;
            Aumento2 = 0;
            UARTCharPut(UART1_BASE, '2');                           //se manda valor a UART
        }
        else if (Aumento2 == 0){
            GPIOPinWrite(GPIO_PORTD_BASE, led_green2 , led_green2); //ON green
            GPIOPinWrite(GPIO_PORTD_BASE, led_red2 , 0);            //OFF red
            ban_ocupado--;
            debouncing = 1;
            Aumento2 = 1;
            UARTCharPut(UART1_BASE, '3');                           //se manda valor a UART
        }

    }
    else if((GPIOPinRead(GPIO_PORTE_BASE, PUSH_3))){
        while(GPIOPinRead(GPIO_PORTE_BASE, PUSH_3)){}               //debouncing push 3

        if (Aumento3 == 1){
            GPIOPinWrite(GPIO_PORTE_BASE, led_green3 , 0);
            GPIOPinWrite(GPIO_PORTB_BASE, led_red3 , led_red3);
            ban_ocupado++;
            debouncing = 1;
            Aumento3 = 0;
            UARTCharPut(UART1_BASE, '4');                          //se manda valor a UART

        }
        else if (Aumento3 == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, led_green3 , led_green3);
            GPIOPinWrite(GPIO_PORTB_BASE, led_red3 , 0);
            ban_ocupado--;
            debouncing = 1;
            Aumento3 = 1;
            UARTCharPut(UART1_BASE, '5');                           //se manda valor a UART
        }


    }
    else if((GPIOPinRead(GPIO_PORTE_BASE, PUSH_4))){
        while(GPIOPinRead(GPIO_PORTE_BASE, PUSH_4)){}               //debouncing push 4
        if (Aumento4 == 1){
            GPIOPinWrite(GPIO_PORTD_BASE, led_green4 , 0);
            GPIOPinWrite(GPIO_PORTE_BASE, led_red4 , led_red4);
            ban_ocupado++;
            debouncing = 1;
            Aumento4 = 0;
            UARTCharPut(UART1_BASE, '6');                           //se manda valor a UART
        }
        else if (Aumento4 == 0){
            GPIOPinWrite(GPIO_PORTD_BASE, led_green4 , led_green4);
            GPIOPinWrite(GPIO_PORTE_BASE, led_red4 , 0);
            ban_ocupado--;
            debouncing = 1;
            Aumento4 = 1;
            UARTCharPut(UART1_BASE, '7');                           //se manda valor a UART
        }
    }

}

//**************************************************************************************************************
// FUNCION PARA ENCENDER LEDS DEPENDIENDO SI ESTA VACIO U OCUPADO
//**************************************************************************************************************
void Mostrar_display(void){

    if (debouncing == 1 && (GPIOPinRead(GPIO_PORTE_BASE, PUSH_4) == 0) && (GPIOPinRead(GPIO_PORTE_BASE, PUSH_3) == 0) && (GPIOPinRead(GPIO_PORTD_BASE, PUSH_2) == 0) && (GPIOPinRead(GPIO_PORTB_BASE, PUSH_1) == 0)){
        if (ban_ocupado == 0){
            DISPLAY(5); //Se apagan todos
            DISPLAY(4);
            debouncing = 0;
        }
        else if (ban_ocupado == 1){ // Si hay un parqueo ocupado
            DISPLAY(5);
            DISPLAY(3);
            debouncing = 0;
        }
        else if (ban_ocupado == 2){ // Si hay dos parqueo ocupado
            DISPLAY(5);
            DISPLAY(2);
            debouncing = 0;
        }
        else if (ban_ocupado == 3){ // Si hay tres parqueo ocupado
            DISPLAY(5);
            DISPLAY(1);
            debouncing = 0;
        }
        else if (ban_ocupado == 4){ // Si hay cuatro parqueo ocupado
            DISPLAY(5);
            DISPLAY(0);
            debouncing = 0;
        }
    }
}
//**************************************************************************************************************
// FUNCION DE NUMEROS PARA DISPLAY
//**************************************************************************************************************
void DISPLAY(uint8_t num_display){
    switch(num_display){
        case 0:
            GPIOPinWrite(GPIO_PORTB_BASE, led_a , led_a);
            GPIOPinWrite(GPIO_PORTA_BASE, led_b , led_b);
            GPIOPinWrite(GPIO_PORTC_BASE, led_c , led_c);
            GPIOPinWrite(GPIO_PORTC_BASE, led_d , led_d);
            GPIOPinWrite(GPIO_PORTA_BASE, led_e , led_e);
            GPIOPinWrite(GPIO_PORTD_BASE, led_f , led_f);
            GPIOPinWrite(GPIO_PORTB_BASE, led_dp , led_dp);
            break;
        case 1:
            GPIOPinWrite(GPIO_PORTA_BASE, led_b , led_b);
            GPIOPinWrite(GPIO_PORTC_BASE, led_c , led_c);
            GPIOPinWrite(GPIO_PORTB_BASE, led_dp , led_dp);
            break;
        case 2:
            GPIOPinWrite(GPIO_PORTB_BASE, led_a , led_a);
            GPIOPinWrite(GPIO_PORTA_BASE, led_b , led_b);
            GPIOPinWrite(GPIO_PORTC_BASE, led_d , led_d);
            GPIOPinWrite(GPIO_PORTA_BASE, led_e , led_e);
            GPIOPinWrite(GPIO_PORTB_BASE, led_g , led_g);
            GPIOPinWrite(GPIO_PORTB_BASE, led_dp , led_dp);
            break;
        case 3:
            GPIOPinWrite(GPIO_PORTB_BASE, led_a , led_a);
            GPIOPinWrite(GPIO_PORTA_BASE, led_b , led_b);
            GPIOPinWrite(GPIO_PORTC_BASE, led_c , led_c);
            GPIOPinWrite(GPIO_PORTC_BASE, led_d , led_d);
            GPIOPinWrite(GPIO_PORTB_BASE, led_g , led_g);
            GPIOPinWrite(GPIO_PORTB_BASE, led_dp , led_dp);
            break;
        case 4:
            GPIOPinWrite(GPIO_PORTA_BASE, led_b , led_b);
            GPIOPinWrite(GPIO_PORTC_BASE, led_c , led_c);
            GPIOPinWrite(GPIO_PORTD_BASE, led_f , led_f);
            GPIOPinWrite(GPIO_PORTB_BASE, led_g , led_g);
            GPIOPinWrite(GPIO_PORTB_BASE, led_dp , led_dp);
            break;
        case 5:
            GPIOPinWrite(GPIO_PORTB_BASE, led_a, 0);
            GPIOPinWrite(GPIO_PORTA_BASE, led_b , 0);
            GPIOPinWrite(GPIO_PORTC_BASE, led_c , 0);
            GPIOPinWrite(GPIO_PORTC_BASE, led_d , 0);
            GPIOPinWrite(GPIO_PORTA_BASE, led_e , 0);
            GPIOPinWrite(GPIO_PORTD_BASE, led_f , 0);
            GPIOPinWrite(GPIO_PORTB_BASE, led_g , 0);
            GPIOPinWrite(GPIO_PORTB_BASE, led_dp , 0);
            break;
    }
}
//**************************************************************************************************************
// Inicializaci�n de UART - Tomado del ejemplo que subio Pablo Mazariegos
//**************************************************************************************************************
void InitUART(void){

    /*Enable the peripheral UART Module 1*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1)){
    }

    /*Enable the GPIO Port d*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);

    // Se habilitan las interrupciones Globales
    IntMasterEnable();

    /* Make the UART pins be peripheral controlled. */
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    UARTDisable(UART1_BASE);
    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(
            UART1_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    IntEnable (INT_UART1);

    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
    UARTEnable (UART1_BASE);
}


