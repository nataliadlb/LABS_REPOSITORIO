/*
 * File            : I2C.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordo�ez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 * 
 * Para este mini proyecto, esta librer�a fue tomada del repositorio de Pablo 
 * Mazariegos: https://github.com/pdmazariegos-uvg/ie3027.git 
 * 
 */
#include "I2C.h"
#include <stdint.h>
//*****************************************************************************
// Funci�n para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000; //I2C MASTER MODE, clk = Fosc/(4*(SSPADD+1)) | 
                         //CKP realese control: holds clock low | 
                         //SSPEN: enable SDA & SCL | // SSPOV : No overflow | 
                         //WCOL: No collision
    
    SSPCON2 = 0;         //SEN: clock streching disabled | RSEN: repeated start cond
                         //PEN (SCK release control): Stop condition Idle | 
                         //RCEN: Receive idle | 
                         //ACKEN: (master receive mode): acknowledge sequence idle
                         //ACKDT (master receive mode): yes acknowledge
                         //ACKSTAT (master trasmit mode): acknowledge was receive from slave
                         //GCEN: General call adress disabled
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1; //SCl
    TRISCbits.TRISC4 = 1; //SDA
}

//*****************************************************************************
// Funci�n de espera: mientras se est� iniciada una comunicaci�n,
// est� habilitado una recepci�n, est� habilitado una parada
// est� habilitado un reinicio de la comunicaci�n, est� iniciada
// una comunicaci�n o se este transmitiendo, el IC2 PIC se esperar�
// antes de realizar alg�n trabajo
//*****************************************************************************
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

//*****************************************************************************
// Funci�n de inicio de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_Start()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.SEN = 1;    //inicia la comunicaci�n i2c
}

//*****************************************************************************
// Funci�n de reinicio de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RSEN = 1;   //reinicia la comunicaci�n i2c
}

//*****************************************************************************
// Funci�n de parada de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_Stop()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.PEN = 1;    //detener la comunicaci�n i2c
}

//*****************************************************************************
//Funci�n de transmisi�n de datos del maestro al esclavo
//esta funci�n devolver� un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPBUF = d;             
}

//*****************************************************************************
//Funci�n de recepci�n de datos enviados por el esclavo al maestro
//esta funci�n es para leer los datos que est�n en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato le�do
}

//*****************************************************************************
// Funci�n para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110 I2C slave mode, enable clock, enabl SDA & SCL, no overflow, no collision
    SSPSTAT = 0x80;     // 0b10000000 //slew rate control enable for standar speed mode
    SSPCON2 = 0x01;     // 0b00000001 //clk streching enabled
    TRISC3 = 1;         //SCL
    TRISC4 = 1;         //SDA
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}


//*****************************************************************************
uint8_t bcd_to_decimal(uint8_t number) {
  return((number >> 4) * 10 + (number & 0x0F));
}
 
// convert decimal to BCD function
uint8_t decimal_to_bcd(uint8_t number) {
  return(((number / 10) << 4) + (number % 10));
}
 
// display time and date function
void RTC_display()
{
  static char Time[] = "TIME: 00:00:00";
  static char Date[] = "DATE: 00/00/2000";
 
  // convert data from BCD format to decimal format
  second = bcd_to_decimal(second);
  minute = bcd_to_decimal(minute);
  hour   = bcd_to_decimal(hour);
  m_day  = bcd_to_decimal(m_day);
  month  = bcd_to_decimal(month);
  year   = bcd_to_decimal(year);
  // end conversion
 
  // update time
  Time[6]  = hour   / 10 + '0';
  Time[7]  = hour   % 10 + '0';
  Time[9]  = minute / 10 + '0';
  Time[10] = minute % 10 + '0';
  Time[12] = second / 10 + '0';
  Time[13] = second % 10 + '0';
  // update date
  Date[6]  = m_day  / 10 + '0';
  Date[7]  = m_day  % 10 + '0';
  Date[9]  = month  / 10 + '0';
  Date[10] = month  % 10 + '0';
  Date[14] = year   / 10 + '0';
  Date[15] = year   % 10 + '0';
 
  LCD_Goto(1, 1);    // go to column 1, row 1
  LCD_Print(Time);   // print time
  LCD_Goto(1, 2);    // go to column 1, row 2
  LCD_Print(Date);   // print date
}