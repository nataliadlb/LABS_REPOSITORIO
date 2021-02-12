// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config MCLRE = OFF      
#pragma config CP = OFF         
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF      
#pragma config LVP = OFF        

// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF        

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//******************************************************************************
// Includes
//******************************************************************************

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "Config_ADC.h"
#include "USART.h"

//******************************************************************************
// Defines
//******************************************************************************

#define _XTAL_FREQ 4000000

#define tmr_preload 6;

//******************************************************************************
// Variables
//******************************************************************************

uint8_t adc_data1 = 0;
uint8_t adc_data2 = 0;
uint8_t uart_data = 0;
uint8_t uart_cont = 0;
uint8_t str_pos   = 0;


// IMPORTANT
// for som reason if both string have the same length
// the las char of str_pot_b overwrites the first char of str_pot_a
char* str_pot_a[6];
char* str_pot_b[5];

bool    adc_flag     = false;
bool    eusart_flag  = false;

//******************************************************************************
// function declarations
//******************************************************************************

void setup(void);
void adc_logic(void);
void display(void);

//******************************************************************************
// Main
//******************************************************************************

void main(void)
{
    setup();
    while(1) 
    {
        adc_logic();

        sprintf(str_pot_a, "A%.3iV", adc_data1<<1);
        sprintf(str_pot_b, "B%.3iV", adc_data2<<1);

        display();

        PORTB = uart_data;
        if (uart_data == '+')
        {
            uart_cont++;
        }

        if (uart_data == '-')
        {
            uart_cont--;
        }
        uart_data = 0;

    }
}

void __interrupt() isr(void)
{

    if (PIE1bits.TXIE && PIR1bits.TXIF)
    {
        if (eusart_flag)
        {
            TXREG = str_pot_a[str_pos];
        }
        else
        {
            TXREG = str_pot_b[str_pos];
        }
        str_pos++;

        if (str_pos == 5)
        {
            eusart_flag = !eusart_flag;
            str_pos = 0;
        }
    }

    if (PIR1bits.RCIF)
    {
        uart_data = RCREG;
    }

    if (PIR1bits.ADIF)
    {
        PIR1bits.ADIF = 0;

        if (adc_flag)
        {
            adc_data1 = ADRESH;
        }
        else
        {
            adc_data2 = ADRESH;
        }

        adc_flag = !adc_flag;
    }
}

void display(void)
{
    lcd_move_cursor(0,0);
    lcd_write_string("  S1:  S2:  S3: ");
    lcd_move_cursor(1,0);

    char* str[3];

    sprintf(str, "%.3i", uart_cont);

    lcd_write_char(str_pot_a[1]);
    lcd_write_char('.');
    lcd_write_char(str_pot_a[2]);
    lcd_write_char(str_pot_a[3]);
    lcd_write_char(str_pot_a[4]);
    lcd_write_char(' ');

    lcd_write_char(str_pot_b[1]);
    lcd_write_char('.');
    lcd_write_char(str_pot_b[2]);
    lcd_write_char(str_pot_b[3]);
    lcd_write_char(str_pot_b[4]);
    lcd_write_char(' ');

    lcd_write_char(str[0]);
    lcd_write_char(str[1]);
    lcd_write_char(str[2]);
    lcd_write_char(str[3]);
}

void adc_logic(void)
{
    if (ADCON0bits.GO)
    {
        return;
    }

    if (adc_flag)
    {
        adc_select_channel(0);
    }
    else
    {
        adc_select_channel(1);
    }

    adc_wait();
    adc_start();
}

//******************************************************************************
// Configuration
//******************************************************************************
void setup(void)
{
    ANSEL  = 0x03;
    ANSELH = 0x00;
    
    TRISA = 0x03;
    TRISB = 0x00;
    TRISC = 0x80;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    // interrupts
    IOCB = 0x00;
    INTCONbits.RBIE = 0;
    INTCONbits.T0IE = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    OSCCON = 0b01100001;

    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;

    TMR0 = tmr_preload;

    INTCONbits.T0IF = 0;

    adc_config();
    adc_isr_enable();

    lcd_init();
    lcd_cmd(0x0c); // turn off cursor

    eusart_init_tx();
    eusart_enable_tx_isr();

    eusart_init_rx();
    eusart_enable_rx_isr();

    return;
}
