#ifndef __STRAND_H__
#define __STRAND_H__

#include "type.h"

#define STRAND_STATE_STANDBY            1
#define STRAND_STATE_RUN                2
#define STRAND_STATE_RUN_LOW_SPEED      3
#define STRAND_STATE_BRAKING            4
#define STRAND_STATE_BRAKE_DONE         5
#define STRAND_STATE_PAUSE              6
#define STRAND_STATE_FINISH             7

struct STRAND {
    uint8 state;
    uint8 group_id;
    uint32 group_current_speed;
    uint32 group_current_turns;
    uint32 group_current_low_speed_turns;
    uint32 group_expected_low_speed_turns;
    uint32 group_expected_high_speed_turns;
    uint32 output;
    uint8 high_speed_pwm;
    uint8 low_speed_pwm;
};

void strand_group_init(uint8 group_id);

#ifdef __SDCC__
extern void int1_process(void) interrupt 2 using 2;
#endif

void strand_process(void);

void strand_init(void);

#endif
