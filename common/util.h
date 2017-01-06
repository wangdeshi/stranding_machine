#ifndef __UTIL_H__
#define __UTIL_H__

#include "type.h"

void delay(uint8 x);

void delay_longtime(uint8 x);

uint8 get_digit_bits(uint32 digit);

uint8 bound(uint8 x, uint8 min, uint8 max);

uint8 bound_add(uint8 x, uint8 y, uint8 min, uint8 max);

uint8 bound_sub(uint8 x, uint8 y, uint8 min, uint8 max);

uint32 util_pow(uint8 x, uint8 y);

#endif
