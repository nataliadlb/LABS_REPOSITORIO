#include "Oscilador.h"

void initOsc(uint8_t IRCF){
    
    switch(IRCF){
        case 0: //31 kHz
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 0;
            break;
         case 1: //125 kHz
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 1;
            break; 
        case 2: //250 kHz
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;
            break;
        case 3: //500 kHz
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 1;
            break;
        case 4: //1 MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 0;
            break;
        case 5: //2 MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 1;
            break;
        case 6: //4 MHz (default)
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;
            break;
        case 7: //8 MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 1;
            break;
        default://4 MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;
            break;
    }
    OSCCONbits.SCS = 1; //System Clock Select: Internal Oscillator
}

