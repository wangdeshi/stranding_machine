#ifndef __UTIL_H__
#define __UTIL_H__

#include "type.h"

extern uint32 xdata _util_get_digit_bits_digit;
extern uint8 xdata _digit_bits[8];
extern uint32 xdata _digit_get_bit_digit;
extern uint32 xdata _util_bound_x;
extern uint32 xdata _util_bound_min;
extern uint32 xdata _util_bound_max;
extern uint32 xdata _util_bound_add_x;
extern uint32 xdata _util_bound_add_y;
extern uint32 xdata _util_bound_add_min;
extern uint32 xdata _util_bound_add_max;
extern uint32 xdata _util_bound_sub_x;
extern uint32 xdata _util_bound_sub_y;
extern uint32 xdata _util_bound_sub_min;
extern uint32 xdata _util_bound_sub_max;

extern uint32 _util_inc_x;
extern uint32 _util_inc_min;
extern uint32 _util_inc_max;
extern uint8 _util_inc_offset;
extern uint32 _util_dec_x;
extern uint32 _util_dec_min;
extern uint32 _util_dec_max;
extern uint8 _util_dec_offset;

void delay(uint8 x);

void delay_longtime(uint8 x);

uint32 util_pow_function(uint8 y);

#define util_pow(x, y)      util_pow_function((y))

uint8 get_digit_bits_function(void);

#define get_digit_bits(digit) (             \
    _util_get_digit_bits_digit = (digit),   \
    get_digit_bits_function()               \
)

#define digit_get_bit(digit) (       \
    _digit_get_bit_digit = (digit),     \
    digit_get_bit_function()         \
)

uint32 bound_function(void);

uint32 bound_add_function(void);

uint32 bound_sub_function(void);

#define bound(x, min, max) (                    \
    _util_bound_x = (x),                        \
    _util_bound_min = (min),                    \
    _util_bound_max = (max),                    \
    bound_function()                            \
)

#define bound_add(x, y, min, max) (             \
    _util_bound_add_x = (x),                    \
    _util_bound_add_y = (y),                    \
    _util_bound_add_min = (min),                \
    _util_bound_add_max = (max),                \
    bound_add_function()                        \
)

#define bound_sub(x, y, min, max) (             \
    _util_bound_sub_x = (x),                    \
    _util_bound_sub_y = (y),                    \
    _util_bound_sub_min = (min),                \
    _util_bound_sub_max = (max),                \
    bound_sub_function()                        \
)

uint32 inc_without_carry_function(void);

uint32 dec_without_borrow_function(void);

#define inc_without_carry(x, offset, min, max) (          \
    _util_inc_x = (x),                    \
    _util_inc_min = (min),                    \
    _util_inc_max = (max),                    \
    _util_inc_offset = (offset),                    \
    inc_without_carry_function()                        \
)

#define dec_without_borrow(x, offset, min, max) (          \
    _util_dec_x = (x),                    \
    _util_dec_min = (min),                    \
    _util_dec_max = (max),                    \
    _util_dec_offset = (offset),                    \
    dec_without_borrow_function()                        \
)

#endif
