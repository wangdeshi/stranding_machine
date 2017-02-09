#include "type.h"
#include "util.h"

void delay(uint8 x) {
    uint8 i;

    i = 15;

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

uint32 xdata _util_bound_x;
uint32 xdata _util_bound_min;
uint32 xdata _util_bound_max;
uint32 xdata _util_bound_add_x;
uint32 xdata _util_bound_add_y;
uint32 xdata _util_bound_add_min;
uint32 xdata _util_bound_add_max;
uint32 xdata _util_bound_sub_x;
uint32 xdata _util_bound_sub_y;
uint32 xdata _util_bound_sub_min;
uint32 xdata _util_bound_sub_max;

uint32 bound_function(void) {
    if (_util_bound_x < _util_bound_min) {
        _util_bound_x = _util_bound_min;
    }

    if (_util_bound_x > _util_bound_max) {
        _util_bound_x = _util_bound_max;
    }

    return _util_bound_x;
}

uint32 bound_add_function(void) {
    if (_util_bound_add_x >= _util_bound_add_max) {
        return _util_bound_add_max;
    }

    _util_bound_add_x += _util_bound_add_y;

    return bound(_util_bound_add_x, _util_bound_add_min, _util_bound_add_max);
}

uint32 bound_sub_function(void) {
    if (_util_bound_sub_x <= _util_bound_sub_min) {
        return _util_bound_sub_min;
    }

    if (_util_bound_sub_x > _util_bound_sub_y) {
        _util_bound_sub_x -= _util_bound_sub_y;
    } else {
        _util_bound_sub_x = 0;
    }

    return bound(_util_bound_sub_x, _util_bound_sub_min, _util_bound_sub_max);
}

uint32 util_pow(uint8 x, uint8 y) {
    uint32 z;

    z = 1;
    while (y--) {
        z *= x;
    }

    return z;
}

