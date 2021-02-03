#include "Display.h"

uint8_t Valor_hex;

uint8_t display(uint8_t ADC_VALOR){
    
    switch(ADC_VALOR){
        case 0:
            Valor_hex = 0b00111111;
            break;
        case 1:
            Valor_hex = 0b000000110;
            break;
        case 2:
            Valor_hex = 0b11011011;
            break;
        case 3:
            Valor_hex = 0b01001111;
            break; 
        case 4:
            Valor_hex = 0b01100110;
            break;
        case 5:
            Valor_hex = 0b01101101;
            break;
        case 6:
            Valor_hex = 0b01111101;
            break;
        case 7:
            Valor_hex = 0b00000111;
            break;
        case 8:
            Valor_hex = 0b01111111;
            break;
        case 9:
            Valor_hex = 0b01101111;
            break;
        case 10: // A
            Valor_hex = 0b01110111;
            break;
        case 11: // B
            Valor_hex = 0b01111100;
            break;
        case 12: // C
            Valor_hex = 0b00111001;
            break;  
        case 13: // D
            Valor_hex = 0b01011110;
            break;
        case 14: // E
            Valor_hex = 0b01111001;
            break;
        case 15 : // F
            Valor_hex = 0b01110001;
            break;
        default:
            Valor_hex = 0b00111111;
            break;
    }
   return Valor_hex;
}