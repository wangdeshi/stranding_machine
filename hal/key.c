#include "key.h"
#include "global.h"

static uint8 xdata key_menu_last, key_menu;
static uint8 xdata key_left_last, key_left;
static uint8 xdata key_right_last, key_right;
static uint8 xdata key_up_last, key_up;
static uint8 xdata key_down_last, key_down;
static uint8 xdata key_add_last, key_add;
static uint8 xdata key_sub_last, key_sub;
static uint8 xdata key_zero_last, key_zero;
static uint8 xdata key_enter_last, key_enter;

static inline uint8 key_check(uint8 key_state, uint8 * key, uint8 * key_last) {
    uint8 ret;

    *key = key_state;
    if (((*key_last) == 0) && ((*key) == 1)) {
        ret = 1;
    } else {
        ret = 0;
    }
    *key_last = *key;

    return ret;
}

void time0_process(void) interrupt 1 {
    static xdata uint8 count1 = 0, count2 = 3;
    static xdata uint16 count3 = 0;
    uint8 flag1;

    TH0 = (65536 - 1000) / 256;
    TL0 = (65536 - 1000) % 256;	

    if (global.strand.brake_state == STRAND_BRAKE_STATE_TIMER_START) {
        count3++;
        if (count3 >= 700) {
            count3 = 0;
            global.strand.brake_state = STRAND_BRAKE_STATE_TIMER_TIMEOUT;
        }
    }

    count2++;
    if (count2 >= 50) {
        count2 = 0;
        global.flag.f50ms = 1;
    }

    flag1 = 0;
    count1++; 
    if (count1 >= 10) {
        count1 = 0;
        flag1 = 1;
        global.flag.f10ms = 1;
    }

    if (flag1) {
        flag1 = 0;

        K3 = 0;
        if (key_check((K0 ? 0 : 1), &key_sub, &key_sub_last)) {
            global.key.key_sub = 1;        
        }
        if (key_check((K1 ? 0 : 1), &key_up, &key_up_last)) {
            global.key.key_up = 1;        
        }
        if (key_check((K2 ? 0 : 1), &key_add, &key_add_last)) {
            global.key.key_add = 1;        
        }
        K3 = 1;

        K4 = 0;
        if (key_check((K0 ? 0 : 1), &key_right, &key_right_last)) {
            global.key.key_right = 1;        
        }
        if (key_check((K1 ? 0 : 1), &key_menu, &key_menu_last)) {
            global.key.key_menu = 1;        
        }
        if (key_check((K2 ? 0 : 1), &key_left, &key_left_last)) {
            global.key.key_left = 1;        
        }
        K4 = 1;

        K5 = 0;
        if (key_check((K0 ? 0 : 1), &key_enter, &key_enter_last)) {
            global.key.key_enter = 1;        
        }
        if (key_check((K1 ? 0 : 1), &key_down, &key_down_last)) {
            global.key.key_down = 1;        
        }
        if (key_check((K2 ? 0 : 1), &key_zero, &key_zero_last)) {
            global.key.key_zero = 1;        
        }
        K5 = 1;
    }
}

void key_clear(void) {
    global.key.key_menu = 0;
    global.key.key_left = 0;
    global.key.key_right = 0;
    global.key.key_up = 0;
    global.key.key_down = 0;
    global.key.key_add = 0;
    global.key.key_sub = 0;
    global.key.key_zero = 0;
    global.key.key_enter = 0;
}

void key_init(void) {
    /* Timer0 Config 1ms */
	ET0 = 1;
	TMOD = 0x01;
	TH0 = (65536 - 1000) / 256;
	TL0 = (65536 - 1000) % 256;
	TR0 = 1;

    /* Others Init */
    key_menu_last = key_menu = 0;
    key_left_last = key_left = 0;
    key_right_last = key_right = 0;
    key_up_last = key_up = 0;
    key_down_last = key_down = 0;
    key_add_last = key_add = 0;
    key_sub_last = key_sub = 0;
    key_zero_last = key_zero = 0;
    key_enter_last = key_enter = 0;

    global.key.key_menu = 0;
    global.key.key_left = 0;
    global.key.key_right = 0;
    global.key.key_up = 0;
    global.key.key_down = 0;
    global.key.key_add = 0;
    global.key.key_sub = 0;
    global.key.key_zero = 0;
    global.key.key_enter = 0;
}
