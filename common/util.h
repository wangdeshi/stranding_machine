#ifndef __UTIL_H__
#define __UTIL_H__

#include "type.h"

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

void delay(uint8 x);

void delay_longtime(uint8 x);

uint8 get_digit_bits(uint32 digit);

uint32 bound_function(void);

uint32 bound_add_function(void);

uint32 bound_sub_function(void);

uint32 util_pow(uint8 x, uint8 y);

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

#endif
