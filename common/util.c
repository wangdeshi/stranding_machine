#include "type.h"

void delay(uint8 x) {
    uint8 i;

    i = 85;

    while (x--) {
        while (i--);
    }
}

void delay_longtime(uint8 x) {
    while(x--)
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

uint8 bound(uint8 x, uint8 min, uint8 max) {
    if (x < min) {
        x = min;
    }

    if (x > max) {
        x = max;
    }

    return x;
}

uint8 bound_add(uint8 x, uint8 y, uint8 min, uint8 max) {
    if (x >= max) {
        return max;
    }

    x += y;

    return bound(x, min, max);
}

uint8 bound_sub(uint8 x, uint8 y, uint8 min, uint8 max) {
    if (x <= min) {
        return min;
    }

    if (x > y) {
        x -= y;
    } else {
        x = 0;
    }

    return bound(x, min, max);
}

uint32 util_pow(uint8 x, uint8 y) {
    uint32 z;

    z = 1;
    while (y--) {
        z *= x;
    }

    return z;
}

