#ifndef __KEY_H__
#define __KEY_H__

#include "type.h"

struct KEY {
    uint8 key_menu;
    uint8 key_left;
    uint8 key_right;
    uint8 key_up;
    uint8 key_down;
    uint8 key_add;
    uint8 key_sub;
    uint8 key_zero;
    uint8 key_enter;
};

void key_clear(void);

void key_init(void);

#endif
