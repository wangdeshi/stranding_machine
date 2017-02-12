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

uint32 xdata _util_get_digit_bits_digit;

uint8 get_digit_bits_function(void) {
    if (_util_get_digit_bits_digit > 999999) 
        return 7;

    if (_util_get_digit_bits_digit > 99999) 
        return 6;

    if (_util_get_digit_bits_digit > 9999) 
        return 5;

    if (_util_get_digit_bits_digit > 999) 
        return 4;

    if (_util_get_digit_bits_digit > 99) 
        return 3;

    if (_util_get_digit_bits_digit > 9) 
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

uint32 util_pow_function(uint8 y) {
    if (y == 0) {
        return 1;
    } else if (y == 1) {
        return 10;
    } else if (y == 2) {
        return 100;
    } else if (y == 3) {
        return 1000;
    } else if (y == 4) {
        return 10000;
    } else if (y == 5) {
        return 100000;
    } else if (y == 6) {
        return 1000000;
    } else if (y == 7) {
        return 10000000;
    } else {
        return 100000000;
    }
}

#if 0
uint32 util_pow(uint8 x, uint8 y) {
    uint32 z;

    z = 1;
    while (y--) {
        z *= x;
    }

    return z;
}
#endif 
