#include "key.h"
#include "global.h"
#include "util.h"

static uint8 xdata key_menu_last, key_menu;
static uint8 xdata key_left_last, key_left;
static uint8 xdata key_right_last, key_right;
static uint8 xdata key_up_last, key_up;
static uint8 xdata key_down_last, key_down;
static uint8 xdata key_add_last, key_add;
static uint8 xdata key_sub_last, key_sub;
static uint8 xdata key_zero_last, key_zero;
static uint8 xdata key_enter_last, key_enter;

static uint8 xdata key_zero_first, key_zero_second;
static uint16 xdata key_zero_count = 9;

static uint8 xdata input_reset_first, input_reset_second;
static uint16 xdata input_reset_count = 7;
static uint8 xdata input_reset_last, input_reset;
static uint8 xdata input_start_last, input_start;
static uint8 xdata input_stop_last, input_stop;

#define key_check(key_state, key, key_last) (                                                  \
    (key) = (key_state),                                                                            \
    ((((key_last) == 0) && ((key) == 1)) ? (((key_last) = (key)), 1) : (((key_last) = (key)), 0))   \
)

#define input_check(key_state, key, key_last) (                                                  \
    (key) = (key_state),                                                                            \
    ((((key_last) == 0) && ((key) == 1)) ? (((key_last) = (key)), 1) : (((key_last) = (key)), 0))   \
)


void time0_process(void) interrupt 1 using 1 {
    static xdata uint16 count1 = 1, count2 = 2;
    static xdata uint16 count3 = 3, count4 = 4, count5 = 5;
    static xdata uint32 group_last_turns = 0;
    static xdata uint32 last_turns = 0;
    static xdata uint32 last_low_speed_turns = 0;
    static xdata uint16 turns_idle_count = 0, low_speed_turns_idle_count = 0;

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

    switch (global.strand.state) {
        case STRAND_STATE_RUN: 
            if (global.strand.group_current_turns == last_turns) {
                turns_idle_count++;
                if (turns_idle_count > 1000) {
                    turns_idle_count = 0;
                    last_turns = 0;
                    motor_braking_stop;           
                    global.strand.state = STRAND_STATE_FINISH;
                }
            } else {
                turns_idle_count = 0;
                last_turns = global.strand.group_current_turns;
            }
            break;
        case STRAND_STATE_RUN_LOW_SPEED: 
            if (global.strand.group_current_low_speed_turns == last_low_speed_turns) {
                low_speed_turns_idle_count++;
                if (low_speed_turns_idle_count > 1000) {
                    low_speed_turns_idle_count = 0;
                    last_low_speed_turns = 0;
                    motor_braking_stop;           
                    global.strand.state = STRAND_STATE_FINISH;
                }
            } else {
                low_speed_turns_idle_count = 0;
                last_low_speed_turns = global.strand.group_current_low_speed_turns;
            }
            break;
        case STRAND_STATE_BRAKING:
            count3++;
            if (count3 <= 100) {
                OUTPUT_BEER = 1;
            } else if (count3 <= 200) {
                OUTPUT_BEER = 0;
            } else if (count3 < 300) {
                if ((global.strand.group_current_turns >= 
                            global.cfg.groups.group[global.strand.group_id].arrival) &&
                        ((global.strand.group_id + 1) >= global.cfg.groups.num)) {
                    OUTPUT_BEER = 1;
                } else {
                    OUTPUT_BEER = 0;
                }
            } else {
                OUTPUT_BEER = 0;
            }

            if (count3 >= 400) {
                count3 = 0;
                global.strand.state = STRAND_STATE_BRAKE_DONE;
            }
            break;
        case STRAND_STATE_STANDBY: 
        case STRAND_STATE_BRAKE_DONE:
        case STRAND_STATE_PAUSE:
        case STRAND_STATE_FINISH: 
        default:
            break;
    }

    count2++;
    if (count2 >= 50) {
        count2 = 0;
        global.flag.f50ms = 1;
    }

    count1++; 
    if (count1 >= 6) {
        count1 = 0;
        global.flag.f10ms = 1;
    }

    input_reset_count++;
    if (input_reset_count >= 1000) {
        if (input_reset_first) {
            input_reset_first = 0;
            input_reset_second = 0;
            global.input.reset = 1;        
        }
    }

    key_zero_count++; 
    if (key_zero_count >= 1000) {
        if (key_zero_first) {
            key_zero_first = 0;
            key_zero_second = 0;
            global.key.key_zero = 1;
        }
    }
}

void key_process(void) {
    K3 = 0;
    if (key_check((K0 ? 0 : 1), key_sub, key_sub_last)) {
        global.key.key_sub = 1;        
    }
    if (key_check((K1 ? 0 : 1), key_up, key_up_last)) {
        global.key.key_up = 1;        
    }
    if (key_check((K2 ? 0 : 1), key_add, key_add_last)) {
        global.key.key_add = 1;        
    }
    K3 = 1;

    K4 = 0;
    if (key_check((K0 ? 0 : 1), key_right, key_right_last)) {
        global.key.key_right = 1;        
    }
    if (key_check((K1 ? 0 : 1), key_menu, key_menu_last)) {
        global.key.key_menu = 1;        
    }
    if (key_check((K2 ? 0 : 1), key_left, key_left_last)) {
        global.key.key_left = 1;        
    }
    K4 = 1;

    K5 = 0;
    if (key_check((K0 ? 0 : 1), key_enter, key_enter_last)) {
        global.key.key_enter = 1;        
    }
    if (key_check((K1 ? 0 : 1), key_down, key_down_last)) {
        global.key.key_down = 1;        
    }
    if (key_check((K2 ? 0 : 1), key_zero, key_zero_last)) {
        if ((key_zero_first == 0) && (key_zero_second == 0)) {
            key_zero_first = 1;
            key_zero_count = 0;
        } else if ((key_zero_first == 1) && (key_zero_second == 0)) {
            key_zero_second = 1;
            global.key.key_zero_double_click = 1;
            key_zero_first = 0;
            key_zero_second = 0;
        }
    }
    K5 = 1;

    if (input_check((INPUT_RESET ? 0 : 1), input_reset, input_reset_last)) {
        if ((input_reset_first == 0) && (input_reset_second == 0)) {
            input_reset_first = 1;
            input_reset_count = 0;
        } else if ((input_reset_first == 1) && (input_reset_second == 0)) {
            input_reset_second = 1;
            global.input.reset_double_click = 1;        
            input_reset_first = 0;
            input_reset_second = 0;
        }
    }
    if (input_check((INPUT_START ? 0 : 1), input_start, input_start_last)) {
        global.input.start = 1;        
    }
    if (input_check((INPUT_STOP ? 0 : 1), input_stop, input_stop_last)) {
        global.input.stop = 1;        
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
    global.key.key_zero_double_click = 0;
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

    key_zero_first = key_zero_second = 0;
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
    global.key.key_zero_double_click = 0;
    global.key.key_enter = 0;
}

