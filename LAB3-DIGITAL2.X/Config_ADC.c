#include <proc/pic16f887.h>

#include "Config_ADC.h"

int VAL;
int VAL_SWAP;
int VAL_NIBBLE1;
int VAL_NIBBLE2;

// FUNCION PARA OBTENER EN UNA VARIABLE EL VALOR DE LA CONVERSION ADC
int ADC(uint8_t ADRESL_, uint8_t ADRESH_){
    VAL = (ADRESL << 8) | ADRESH;
    return VAL;
}
// FUNCION PARA HACER SWAP EL VALOR DE LA CONVERSION
int SWAP_ADC(uint8_t VAL_ADC){
    VAL_SWAP = (((VAL_ADC & 0x0F)<<4) | ((VAL_ADC & 0xF0)>>4));
    return VAL_SWAP;
}

int NIBBLE1_ADC(uint8_t VAL_ADC){
    VAL_NIBBLE1 = (VAL_ADC & 15);
    return VAL_NIBBLE1;
}

int NIBBLE2_ADC(uint8_t VAL_SWAP){
    VAL_NIBBLE2 = (VAL_SWAP & 15);
    return VAL_NIBBLE2;
}

void ADC_channel(uint8_t AN_num){
    
    switch(AN_num){
        case 0: //AN0 (RA0)
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
        case 1: //AN1 (RA1)
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
        case 2: //AN2 (RA2)
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
        case 3: //AN3 (RA3)
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
        case 4: //AN4 (RA5)
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
        case 5: //AN5 (RE0)
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
        case 6: //AN6 (RE1)
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
        case 7: //AN7 (RE2)
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
        case 8: //AN8 (RB2)
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
        case 9: //AN9 (RB3)
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
        case 10: //AN10 (RB1)
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
        case 11: //AN11 (RB4)
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
        case 12: //AN12 (RB0)
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
        case 13: //AN13 (RB5)
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
        case 14: //CVref
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
        case 15: //Fixed Ref.
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
        default:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break; 
    }
}    