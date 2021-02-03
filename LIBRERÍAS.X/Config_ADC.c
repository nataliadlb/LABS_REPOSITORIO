#include "Config_ADC.h"

uint8_t VAL;

uint8_t ADC(uint8_t ADRESL_, uint8_t ADRESH_){
    VAL = (ADRESL << 8) | ADRESH;
    return VAL;
}
