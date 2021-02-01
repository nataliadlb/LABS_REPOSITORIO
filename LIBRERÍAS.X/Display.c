#include "Display.h"

uint8_t Valor_hex;

void display(uint8_t NUM){
    
    switch(NUM){
        case 0:
            Valor_hex = 0b11111100;
            break;
        case 1:
            Valor_hex = 0b01100000;
            break;
        case 2:
            Valor_hex = 0b11011010;
            break;
        case 3:
            Valor_hex = 0b11110010;
            break; 
        case 4:
            Valor_hex = 0b01100110;
            break;
        case 5:
            Valor_hex = 0b10110110;
            break;
        case 6:
            Valor_hex = 0b10111110;
            break;
        case 7:
            Valor_hex = 0b11100000;
            break;
        case 8:
            Valor_hex = 0b11111110;
            break;
        case 9:
            Valor_hex = 0b11110110;
            break;
        case 10: // A
            Valor_hex = 0b11101110;
            break;
        case 11: // B
            Valor_hex = 0b00111110;
            break;
        case 12: // C
            Valor_hex = 0b10011100;
            break;  
        case 13: // D
            Valor_hex = 0b01111010 ;
            break;
        case 14: // E
            Valor_hex = 0b10011110;
            break;
        case 15 : // F
            Valor_hex = 0b10001110;
            break;
        default:
            Valor_hex = 0b11111100;
            break;
    }
}