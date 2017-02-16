#include "key.h"
#include "global.h"

volatile static uint8 xdata key_menu_last, key_menu;
volatile static uint8 xdata key_left_last, key_left;
volatile static uint8 xdata key_right_last, key_right;
volatile static uint8 xdata key_up_last, key_up;
volatile static uint8 xdata key_down_last, key_down;
volatile static uint8 xdata key_add_last, key_add;
volatile static uint8 xdata key_sub_last, key_sub;
volatile static uint8 xdata key_zero_last, key_zero;
volatile static uint8 xdata key_enter_last, key_enter;

volatile static uint8 xdata input_reset_first, input_reset_second;
volatile static uint8 xdata input_reset_last, input_reset;
volatile static uint8 xdata input_start_last, input_start;
volatile static uint8 xdata input_stop_last, input_stop;

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

void time0_process(void) interrupt (1) __using (1) {
    static xdata uint8 count1 = 1, count2 = 2;
    static xdata uint16 count3 = 3, count4 = 4, count5 = 5, count6 = 6, count7 = 7;
    static xdata uint32 group_last_turns = 0;

    TH0 = (uint8)((65536 - 1000) >> 8);
    TL0 = (uint8)((65536 - 1000));	

    count5++;
    if (count5 >= 300) {
        count5 = 0;
        if (global.strand.group_current_turns > group_last_turns) {
            global.strand.group_current_speed = 
                (global.strand.group_current_turns - group_last_turns) * 200;
        } else {
            global.strand.group_current_speed = 0;
        }
        group_last_turns = global.strand.group_current_turns;
    }

    count4++;
    if (count4 >= 500) {
        count4 = 0;
        global.flag.flashes = !global.flag.flashes;
    }

    if (global.strand.state == STRAND_STATE_BRAKING) {
        count3++;
        if (count3 >= 700) {
            count3 = 0;
            global.strand.state = STRAND_STATE_BRAKE_DONE;
        }
    }

    count2++;
    if (count2 >= 50) {
        count2 = 0;
        global.flag.f50ms = 1;
    }

    count1++; 
    if (count1 >= 10) {
        count1 = 0;
        global.flag.f10ms = 1;

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

    count6++;
    if (count6 >= 30) {
        count6 = 0;
        if (key_check((INPUT_RESET ? 0 : 1), &input_reset, &input_reset_last)) {
            if ((input_reset_first == 0) && (input_reset_second == 0)) {
                input_reset_first = 1;
                count7 = 0;
            } else if ((input_reset_first == 1) && (input_reset_second == 0)) {
                input_reset_second = 1;
                global.input.reset_double_click = 1;        
                input_reset_first = 0;
                input_reset_second = 0;
            }
        }
        if (key_check((INPUT_START ? 0 : 1), &input_start, &input_start_last)) {
            global.input.start = 1;        
        }
        if (key_check((INPUT_STOP ? 0 : 1), &input_stop, &input_stop_last)) {
            global.input.stop = 1;        
        }
    }

    count7++;
    if (count7 >= 1000) {
        if (input_reset_first) {
            input_reset_first = 0;
            input_reset_second = 0;
            global.input.reset = 1;        
        }
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
    TH0 = (uint8)((65536 - 1000) >> 8);
    TL0 = (uint8)((65536 - 1000));	
	TR0 = 1;

    /* Others Init */
    input_reset_first = input_reset_second = 0;
    input_reset_last = input_reset = 0;
    input_start_last = input_start = 0;
    input_stop_last = input_stop = 0;

    key_menu_last = key_menu = 1;
    key_left_last = key_left = 1;
    key_right_last = key_right = 1;
    key_up_last = key_up = 1;
    key_down_last = key_down = 1;
    key_add_last = key_add = 1;
    key_sub_last = key_sub = 1;
    key_zero_last = key_zero = 1;
    key_enter_last = key_enter = 1;

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

