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

uint8 xdata _digit_bits[8];
uint32 xdata _digit_get_bit_digit;

void digit_get_bit_function(void) {
    uint8 i;

    for (i = 0; i < 8; i++) {
        _digit_bits[i] = 0;
    }

    while (_digit_get_bit_digit >= 100000000) {
        _digit_get_bit_digit -= 100000000;
    }
    
    while (_digit_get_bit_digit >= 10000000) {
        _digit_get_bit_digit -= 10000000;
        _digit_bits[7]++;
    }
    
    while (_digit_get_bit_digit >= 1000000) {
        _digit_get_bit_digit -= 1000000;
        _digit_bits[6]++;
    }
    
    while (_digit_get_bit_digit >= 100000) {
        _digit_get_bit_digit -= 100000;
        _digit_bits[5]++;
    }
    
    while (_digit_get_bit_digit >= 10000) {
        _digit_get_bit_digit -= 10000;
        _digit_bits[4]++;
    }
    
    while (_digit_get_bit_digit >= 1000) {
        _digit_get_bit_digit -= 1000;
        _digit_bits[3]++;
    }
    
    while (_digit_get_bit_digit >= 100) {
        _digit_get_bit_digit -= 100;
        _digit_bits[2]++;
    }
    
    while (_digit_get_bit_digit >= 10) {
        _digit_get_bit_digit -= 10;
        _digit_bits[1]++;
    }
    
    while (_digit_get_bit_digit >= 1) {
        _digit_get_bit_digit -= 1;
        _digit_bits[0]++;
    }
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

uint32 _util_inc_x;
uint32 _util_inc_min;
uint32 _util_inc_max;
uint8 _util_inc_offset;

uint32 inc_without_carry_function(void) {
    uint8 tmp;

    digit_get_bit(_util_inc_x);

    if (_util_inc_offset > 7) {
        _util_inc_offset = 7;
    }
    
    tmp = 0;
    if (_digit_bits[_util_inc_offset] < 9) {
        _util_inc_x += util_pow(10, _util_inc_offset);
        if (_util_inc_x > _util_inc_max) {
            _util_inc_x -= util_pow(10, _util_inc_offset);
            tmp = _digit_bits[_util_inc_offset];
        }
    } else {
        tmp = _digit_bits[_util_inc_offset];
    }

    while (tmp--) {
        _util_inc_x -= util_pow(10, _util_inc_offset);
        if (_util_inc_x < _util_inc_min) {
            _util_inc_x += util_pow(10, _util_inc_offset);
            break; 
        } 
    }

    return _util_inc_x;
}

uint32 _util_dec_x;
uint32 _util_dec_min;
uint32 _util_dec_max;
uint8 _util_dec_offset;

uint32 dec_without_borrow_function(void) {
    uint8 tmp;

    digit_get_bit(_util_dec_x);

    if (_util_dec_offset > 7) {
        _util_dec_offset = 7;
    }
    
    tmp = 0;
    if (_digit_bits[_util_dec_offset] > 0) {
        _util_dec_x -= util_pow(10, _util_dec_offset);
        if (_util_dec_x < _util_dec_min) {
            _util_dec_x += util_pow(10, _util_dec_offset);
            tmp = 9 - _digit_bits[_util_dec_offset];
        }
    } else {
        tmp = 9;
    }

    while (tmp--) {
        _util_dec_x += util_pow(10, _util_dec_offset);
        if (_util_dec_x > _util_dec_max) {
            _util_dec_x -= util_pow(10, _util_dec_offset);
            break;
        } 
    }

    return _util_dec_x;
}

