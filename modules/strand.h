#ifndef __STRAND_H__
#define __STRAND_H__

#include "type.h"

#define STRAND_STATE_STANDBY            1
#define STRAND_STATE_RUN                2
#define STRAND_STATE_RUN_LOW_SPEED      3
#define STRAND_STATE_BRAKING            4
#define STRAND_STATE_PAUSE              5
#define STRAND_STATE_FINISH             6

#define STRAND_BRAKE_STATE_TIMER_STOP         1
#define STRAND_BRAKE_STATE_TIMER_START        2
#define STRAND_BRAKE_STATE_TIMER_TIMEOUT      3

struct STRAND {
    uint8 state;
    uint8 group_id;
    uint32 group_speed;
    uint32 group_turns;
    uint32 output;
    uint8 brake_state;
    uint32 group_turns_low_speed;
    uint32 high_speed_turns;
    uint32 low_speed_turns;
};

extern void int1_process(void) interrupt (2);

void strand_process(void);

void strand_init(void);

#endif
