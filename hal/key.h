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
    uint8 key_zero_double_click;
    uint8 key_enter;
};

extern void key_process(void);

extern void key_clear(void);

extern void key_init(void);

#ifdef __SDCC__
extern void time0_process(void) interrupt 1 using 1;
#endif

#endif
