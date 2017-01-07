#ifndef __UTIL_H__
#define __UTIL_H__

#include "type.h"

void delay(uint8 x);

void delay_longtime(uint8 x);

uint8 get_digit_bits(uint32 digit);

uint32 bound(uint32 x, uint32 min, uint32 max);

uint32 bound_add(uint32 x, uint32 y, uint32 min, uint32 max);

uint32 bound_sub(uint32 x, uint32 y, uint32 min, uint32 max);

uint32 util_pow(uint8 x, uint8 y);

#endif
