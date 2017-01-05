#include "type.h"

void delay_us1(uint32 us) {
    while (us--) {
//        _nop_();
    }
}

void delay(uint8 c) {
    uint8 i;

    i = 85;

    while (c--) {
        while (i--);
    }
}

void delay_longtime(uint8 c) {
    while(c--)
        delay(255);
}

uint8 get_digit_bits(uint32 digit) {
    if (digit > 999999) 
        return 7;

    if (digit > 99999) 
        return 6;

    if (digit > 9999) 
        return 5;

    if (digit > 999) 
        return 4;

    if (digit > 99) 
        return 3;

    if (digit > 9) 
        return 2;

    return 1;
}

