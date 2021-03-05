/*
 * T�tulo: Mini proyecto 2
 * Autor: Natalia de Leon Bercian
 * Carne: 18193
 * Seccion: 20
 * 
 * 
 * Created on 1 de marzo de 2021
 */

//****************************************************************************//
//IMPORTAR LIBRERIAS                                                          //
//****************************************************************************//
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "USART.h"

//****************************************************************************//
//CONFIGURACION BITS                                                          //
//****************************************************************************//
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//****************************************************************************//
//DEFINE                                                                      //
//****************************************************************************//
#define _XTAL_FREQ 8000000

// --- SENSOR --- //
#define SMPLRT_DIV 0x19
#define PWR_MGMT_1 0x6B
#define CONFIG 0x1A
#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG 0x1B
#define INT_ENABLE 0x38
#define ACCEL_XOUT_H 0x3B

//****************************************************************************//
//VARIABLES                                                                   //
//****************************************************************************//
char data_recive;
uint8_t cont;
char prueba;
//****************************************************************************//
//PROTOTIPOS DE FUNCIONES                                                     //
//****************************************************************************//
void setup(void);
void MPU6050_Init(void);
//****************************************************************************//
//INTERRUPCIONES                                                    //
//****************************************************************************//

void __interrupt() ISR(void) {
    if(PIR1bits.RCIF == 1){
        data_recive = RCREG; //Recibe los datos que manda la terminal
        if (data_recive == 'P11'){ //auementa
            cont++;
            PORTB = cont;
        }
        else if (data_recive == 'P10'){ //decrementa
            cont--;
            PORTB = cont;
        }
        else if (data_recive == 'P21'){ //decrementa
            cont--;
            PORTB = cont;
        }
        else if (data_recive == 'P20'){ //decrementa
            cont--;
            PORTB = cont;
        }
        data_recive = 0;
        }
}
//****************************************************************************//
//PROGRAMACION PRINCIPAL                                                      //
//****************************************************************************//

void main(void) {
    setup();
    prueba = "Hola";
    //MPU6050_Init();

    //************************************************************************//
    //LOOP PRINCIPAL                                                          //
    //************************************************************************//
    while(1){
        // ----* SENSOR *---- //
//        I2C_Master_Start();
//        I2C_Master_Write(0x50);
//        I2C_Master_Write(PORTB);
//        I2C_Master_Stop();
//        __delay_ms(200);
//       
//        I2C_Master_Start();
//        I2C_Master_Write(0x51);
//        PORTD = I2C_Master_Read(0);
//        I2C_Master_Stop();
//        __delay_ms(200);
//        PORTB++;  
        
        // ----* USART *---- //
        Write_USART_String(prueba); 
        Write_USART(13);//13 y 10 la secuencia es para dar un salto de linea 
        Write_USART(10);
    }
    //return;
}

//****************************************************************************//
//CONFIGURACION  (puertos, bits...)                                           //
//****************************************************************************//

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    USART_Init_BaudRate();
    USART_Init();
    USART_INTERRUPT();
    I2C_Master_Init(100000);        // Inicializar Comuncaci�n I2C
}

//****************************************************************************//
//FUNCIONES                                                                   //
//****************************************************************************//

void MPU6050_Init(void){

      // Setting The Sample (Data) Rate
      I2C_Master_Start(); //I2C_Master_Start(0xD0)
      I2C_Master_Write(SMPLRT_DIV);
      I2C_Master_Write(0x07);
      I2C_Master_Stop();
      // Setting The Clock Source
      I2C_Master_Start();
      I2C_Master_Write(PWR_MGMT_1);
      I2C_Master_Write(0x01);
      I2C_Master_Stop();
      // Configure The DLPF
      I2C_Master_Start();
      I2C_Master_Write(CONFIG);
      I2C_Master_Write(0x00);
      I2C_Master_Stop();
      // Configure The ACCEL (FSR= +-2g)
      I2C_Master_Start();
      I2C_Master_Write(ACCEL_CONFIG);
      I2C_Master_Write(0x00);
      I2C_Master_Stop();
      // Configure The GYRO (FSR= +-2000d/s)
      I2C_Master_Start();
      I2C_Master_Write(GYRO_CONFIG);
      I2C_Master_Write(0x18);
      I2C_Master_Stop();
      // Enable Data Ready Interrupts
      I2C_Master_Start();
      I2C_Master_Write(INT_ENABLE);
      I2C_Master_Write(0x01);
      I2C_Master_Stop();
}

void MPU_Start_Loc()

{
	I2C_Master_Wait();	/* I2C start with device write address */
	I2C_Master_Write(ACCEL_XOUT_H);/* Write start location address to read */ 
	I2C_Master_RepeatedStart();/* I2C start with device read address */
}