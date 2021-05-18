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
#define led_e GPIO_PIN_5 // PORT C
#define led_f GPIO_PIN_4 // PORT C
#define led_g GPIO_PIN_3 // PORT B
#define led_dp GPIO_PIN_6 // PORT B


//*****************************************************************************
//
// VARIABLES
//
//*****************************************************************************
uint32_t ui32Period; //periodo para TMR0
uint32_t ui32Status; //Status de UART
char letra;
uint8_t ban_ocupado = 0;
uint8_t debouncing = 0;
uint8_t Aumento = 1;

//UART
uint8_t Parqueo1 = 0;
uint8_t Parqueo2 = 0;
uint8_t Parqueo3 = 0;
uint8_t Parqueo4 = 0;


//*****************************************************************************
//
// PROTOTIPOS DE FUNCIONES
//
//*****************************************************************************
void InitUART(void);
void UARTIntHandler(void);

void DISPLAY(uint8_t num_display);
void delayMs(uint32_t ui32Ms);
void LEDS_G_R(void);
void Mostrar_display(void);
void MandarVal(void);


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
    //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_4);

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
    // Se inicializa la comunicación UART
    InitUART();


//    // Se manda mensajes por UART
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
//
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
        LEDS_G_R();
        Mostrar_display();
        MandarVal();
    }
}

//*****************************************************************************
//
// FUNCIONES
//
//*****************************************************************************

//**************************************************************************************************************
// Handler de la interrupcion del UART - Recordar modificar el archivo tm4c123ght6pm_startup_css.c
//**************************************************************************************************************
//void UARTIntHandler(void){
//        // Get the interrupt status
//        ui32Status = UARTIntStatus(UART2_BASE, true);
//
//        // Clear the asserted interrupts.
//        UARTIntClear(UART2_BASE, ui32Status);
//
//        // Loop while there are characters in the receive FIFO.
//        while(UARTCharsAvail(UART2_BASE)){
//            letra = UARTCharGet(UART2_BASE);
//            UARTCharPutNonBlocking(UART2_BASE,letra); //manda a un puerto especifico
//        }
//}

//**************************************************************************************************************
// FUNCION PARA ENCENDER LEDS DEPENDIENDO SI ESTA VACIO U OCUPADO
//**************************************************************************************************************
void LEDS_G_R(void){
    if((GPIOPinRead(GPIO_PORTB_BASE, PUSH_1))){
        while(GPIOPinRead(GPIO_PORTB_BASE, PUSH_1)){}//debouncing push 1
        if (Aumento == 1){
            GPIOPinWrite(GPIO_PORTB_BASE, led_green1 , 0);//se apaga led verde
            GPIOPinWrite(GPIO_PORTB_BASE, led_red1 , led_red1);//se enciende led rojo
            ban_ocupado++;
            debouncing = 1;
            Parqueo1 = 1;
        }
        else if (Aumento == 0){
            GPIOPinWrite(GPIO_PORTB_BASE, led_green1 , led_green1);//se enciende led verde
            GPIOPinWrite(GPIO_PORTB_BASE, led_red1 , 0);//se apaga led rojo
            ban_ocupado--;
            debouncing = 1;
            Parqueo1 = 0;
        }

    }
    else if((GPIOPinRead(GPIO_PORTD_BASE, PUSH_2))){
        while(GPIOPinRead(GPIO_PORTD_BASE, PUSH_2)){} //debouncing push2
        if (Aumento == 1){
            GPIOPinWrite(GPIO_PORTD_BASE, led_green2 , 0); //OFF green
            GPIOPinWrite(GPIO_PORTD_BASE, led_red2 , led_red2); //ON red
            ban_ocupado++;
            debouncing = 1;
            Parqueo2 = 1;
        }
        else if (Aumento == 0){
            GPIOPinWrite(GPIO_PORTD_BASE, led_green2 , led_green2); //OFF green
            GPIOPinWrite(GPIO_PORTD_BASE, led_red2 , 0); //ON red
            ban_ocupado--;
            debouncing = 1;
            Parqueo2 = 0;
        }


    }
    else if((GPIOPinRead(GPIO_PORTE_BASE, PUSH_3))){
        while(GPIOPinRead(GPIO_PORTE_BASE, PUSH_3)){}//debouncing push 3

        if (Aumento == 1){
            GPIOPinWrite(GPIO_PORTE_BASE, led_green3 , 0);
            GPIOPinWrite(GPIO_PORTB_BASE, led_red3 , led_red3);
            ban_ocupado++;
            debouncing = 1;
            Parqueo3 = 1;
        }
        else if (Aumento == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, led_green3 , led_green3);
            GPIOPinWrite(GPIO_PORTB_BASE, led_red3 , 0);
            ban_ocupado--;
            debouncing = 1;
            Parqueo3 = 0;
        }


    }
    else if((GPIOPinRead(GPIO_PORTE_BASE, PUSH_4))){
        while(GPIOPinRead(GPIO_PORTE_BASE, PUSH_4)){} //debouncing push 4
        if (Aumento == 1){
            GPIOPinWrite(GPIO_PORTD_BASE, led_green4 , 0);
            GPIOPinWrite(GPIO_PORTE_BASE, led_red4 , led_red4);
            ban_ocupado++;
            debouncing = 1;
            Parqueo4 = 1;
        }
        else if (Aumento == 0){
            GPIOPinWrite(GPIO_PORTD_BASE, led_green4 , led_green4);
            GPIOPinWrite(GPIO_PORTE_BASE, led_red4 , 0);
            ban_ocupado--;
            debouncing = 1;
            Parqueo4 = 0;
        }


    }

}

//**************************************************************************************************************
// FUNCION PARA ENCENDER LEDS DEPENDIENDO SI ESTA VACIO U OCUPADO
//**************************************************************************************************************
void Mostrar_display(void){

    if (Aumento == 1 && debouncing == 1 && (GPIOPinRead(GPIO_PORTE_BASE, PUSH_4) == 0) && (GPIOPinRead(GPIO_PORTE_BASE, PUSH_3) == 0) && (GPIOPinRead(GPIO_PORTD_BASE, PUSH_2) == 0) && (GPIOPinRead(GPIO_PORTB_BASE, PUSH_1) == 0)){
        if (ban_ocupado == 0){
            DISPLAY(5); //Se apagan todos
            DISPLAY(4);
            Aumento = 1;
            debouncing = 0;
        }
        else if (ban_ocupado == 1){
            DISPLAY(5);
            DISPLAY(3);
            Aumento = 1;
            debouncing = 0;
        }
        else if (ban_ocupado == 2){
            DISPLAY(5);
            DISPLAY(2);
            Aumento = 1;
            debouncing = 0;
        }
        else if (ban_ocupado == 3){
            DISPLAY(5);
            DISPLAY(1);
            Aumento = 1;
            debouncing = 0;
        }
        else if (ban_ocupado == 4){
            DISPLAY(5);
            DISPLAY(0);
            Aumento = 0;
            debouncing = 0;
        }
    }
     if (Aumento == 0 && debouncing == 1 && (GPIOPinRead(GPIO_PORTE_BASE, PUSH_4) == 0) && (GPIOPinRead(GPIO_PORTE_BASE, PUSH_3) == 0) && (GPIOPinRead(GPIO_PORTD_BASE, PUSH_2) == 0) && (GPIOPinRead(GPIO_PORTB_BASE, PUSH_1) == 0)){
         if (ban_ocupado == 3){
             DISPLAY(5); //Se apagan todos
             DISPLAY(1);
             Aumento = 0;
             debouncing = 0;
         }
         else if (ban_ocupado == 2){
             DISPLAY(5);
             DISPLAY(2);
             Aumento = 0;
             debouncing = 0;
         }
         else if (ban_ocupado == 1){
             DISPLAY(5);
             DISPLAY(3);
             Aumento = 0;
             debouncing = 0;
         }
         else if (ban_ocupado == 0){
             DISPLAY(5);
             DISPLAY(4);
             //ban_ocupado = 0;
             Aumento = 1;
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
            GPIOPinWrite(GPIO_PORTC_BASE, led_e , led_e);
            GPIOPinWrite(GPIO_PORTC_BASE, led_f , led_f);
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
            GPIOPinWrite(GPIO_PORTC_BASE, led_e , led_e);
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
            GPIOPinWrite(GPIO_PORTC_BASE, led_f , led_f);
            GPIOPinWrite(GPIO_PORTB_BASE, led_g , led_g);
            GPIOPinWrite(GPIO_PORTB_BASE, led_dp , led_dp);
            break;
        case 5:
            GPIOPinWrite(GPIO_PORTB_BASE, led_a, 0);
            GPIOPinWrite(GPIO_PORTA_BASE, led_b , 0);
            GPIOPinWrite(GPIO_PORTC_BASE, led_c , 0);
            GPIOPinWrite(GPIO_PORTC_BASE, led_d , 0);
            GPIOPinWrite(GPIO_PORTC_BASE, led_e , 0);
            GPIOPinWrite(GPIO_PORTC_BASE, led_f , 0);
            GPIOPinWrite(GPIO_PORTB_BASE, led_g , 0);
            GPIOPinWrite(GPIO_PORTB_BASE, led_dp , 0);
            break;
    }
}
//**************************************************************************************************************
// Inicialización de UART - Tomado del ejemplo que subio Pablo Mazariegos
//**************************************************************************************************************
void InitUART(void){
//
//    /*Enable the peripheral UART Module 0*/
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
//
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2)){
//    }
//
//    /*Enable the GPIO Port d*/
//    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//
//    GPIOPinConfigure(GPIO_PD6_U2RX);
//    GPIOPinConfigure(GPIO_PD7_U2TX);
//
//    // Se habilitan las interrupciones Globales
//    IntMasterEnable();
//
//    /* Make the UART pins be peripheral controlled. */
//    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
//
//    //UARTDisable(UART2_BASE);
//    /* Sets the configuration of a UART. */
//    UARTConfigSetExpClk(
//            UART2_BASE, SysCtlClockGet(), 115200,
//            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//
//    IntEnable (INT_UART2);
//
//    UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);
//    UARTEnable (UART2_BASE);


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

//**************************************************************************************************************
// FUNCION DELAY
//**************************************************************************************************************
void delayMs(uint32_t ui32Ms) {  //Funcion obtenida de https://gist.github.com/ctring/7f12d812fb594eecc493
    SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}

//*****************************************************************************
//
// Send a string to the UART.  This function sends a string of characters to a
// particular UART module.
//
//*****************************************************************************
void MandarVal(void){
    //    // Se manda mensajes por UART
    if (Parqueo1 == 0){ //parqueo 1 vacio = 0
        UARTCharPut(UART0_BASE, '0');
        UARTCharPut(UART0_BASE, 10);
        UARTCharPut(UART0_BASE, 13);
    }
    if (Parqueo1 == 1){ //parqueo 1 OCUPADO = 1
            UARTCharPut(UART0_BASE, '1');
            UARTCharPut(UART0_BASE, 10);
            UARTCharPut(UART0_BASE, 13);
    }
    //
    if (Parqueo2 == 0){ //parqueo 2 VACIO = 3
            UARTCharPut(UART0_BASE, '3');
            UARTCharPut(UART0_BASE, 10);
            UARTCharPut(UART0_BASE, 13);
    }
    if (Parqueo2 == 1){ //parqueo 2 OCUPADO = 4
            UARTCharPut(UART0_BASE, '4');
            UARTCharPut(UART0_BASE, 10);
            UARTCharPut(UART0_BASE, 13);
    }
    //
    if (Parqueo3 == 0){ //parqueo 2 VACIO = 5
            UARTCharPut(UART0_BASE, '5');
            UARTCharPut(UART0_BASE, 10);
            UARTCharPut(UART0_BASE, 13);
    }
    if (Parqueo3 == 1){ //parqueo 2 OCUPADO = 6
            UARTCharPut(UART0_BASE, '6');
            UARTCharPut(UART0_BASE, 10);
            UARTCharPut(UART0_BASE, 13);
    }
    //
    if (Parqueo4 == 0){ //parqueo 2 VACIO = 7
            UARTCharPut(UART0_BASE, '7');
            UARTCharPut(UART0_BASE, 10);
            UARTCharPut(UART0_BASE, 13);
    }
    if (Parqueo4 == 1){ //parqueo 2 OCUPADO = 8
            UARTCharPut(UART0_BASE, '8');
            UARTCharPut(UART0_BASE, 10);
            UARTCharPut(UART0_BASE, 13);
    }
}
