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

#define motor_run_low_speed(group_id) do {                  \
    global.output.speed_pwm = global.strand.low_speed_pwm;  \
    output_flush_speed;                                     \
    global.strand.group_current_low_speed_turns = 0;        \
    global.output.start = 1;                                \
    global.output.stop = 0;                                 \
    global.output.dir = global.cfg.groups.group[group_id].dir;  \
    output_flush_start_stop_dir;                            \
} while (0)

#define motor_run(group_id) do {                                \
    global.output.speed_pwm = global.strand.high_speed_pwm;     \
    output_flush_speed;                                         \
    global.output.start = 1;                                    \
    global.output.stop = 0;                                     \
    global.output.dir = global.cfg.groups.group[group_id].dir;  \
    output_flush_start_stop_dir;                                \
} while (0)

#define motor_braking_start do {                \
    global.output.speed_pwm = IDLE_SPEED_PWM;   \
    output_flush_speed;                         \
    global.output.start = 0;                    \
    global.output.stop = 1;                     \
    output_flush_start_stop_dir;                \
} while (0)

#define motor_braking_stop do {                 \
    global.output.speed_pwm = IDLE_SPEED_PWM;   \
    output_flush_speed;                         \
    global.output.start = 0;                    \
    global.output.stop = 0;                     \
    output_flush_start_stop_dir;                \
} while (0)

#endif
