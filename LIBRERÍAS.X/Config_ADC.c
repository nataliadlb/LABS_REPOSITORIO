#include "Config_ADC.h"

uint8_t VAL;
uint8_t VAL_SWAP;
uint8_t VAL_NIBBLE1;
uint8_t VAL_NIBBLE2;

// FUNCION PARA OBTENER EN UNA VARIABLE EL VALOR DE LA CONVERSION ADC
uint8_t ADC(uint8_t ADRESL_, uint8_t ADRESH_){
    VAL = (ADRESL << 8) | ADRESH;
    return VAL;
}
// FUNCION PARA HACER SWAP EL VALOR DE LA CONVERSION
uint8_t SWAP_ADC(uint8_t VAL_ADC){
    VAL_SWAP = (((VAL_ADC & 0x0F)<<4) | ((VAL_ADC & 0xF0)>>4));
    return VAL_SWAP;
}

uint8_t NIBBLE1_ADC(uint8_t VAL_ADC){
    VAL_NIBBLE1 = VAL_ADC & 15;
    return VAL_NIBBLE1;
}

uint8_t NIBBLE2_ADC(uint8_t VAL_SWAP){
    VAL_NIBBLE2 = VAL_SWAP & 15;
    return VAL_NIBBLE2;
}