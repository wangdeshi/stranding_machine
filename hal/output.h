#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "type.h"

struct OUTPUT {
    uint8 start;
    uint8 stop;
    uint8 dir;
    uint8 speed_pwm;
    uint8 beer;
};

#define MAX_SPEED_PERCENTAGE    100
#define IDLE_SPEED_PWM          0xff

uint8 speed_percentage_to_pwm(uint8 speed_percentage);
uint8 speed_voltage_to_pwm(uint8 speed_voltage);

#define output_flush_beer do {  \
    if (global.output.beer) {   \
        OUTPUT_BEER = 1;        \
    } else {                    \
        OUTPUT_BEER = 0;        \
    }                           \
} while (0)

#define output_flush_speed do {             \
	CR = 0;                                 \
	CL = 0;                                 \
	CH = 0;                                 \
    CCAP0L = global.output.speed_pwm;       \
	CCAP0H = global.output.speed_pwm;       \
	CR = 1;                                 \
} while (0)

#define output_flush_start_stop_dir do {                                \
    if (global.output.stop) {                                           \
        OUTPUT_START_CLOCKWISE = 0;                                     \
        OUTPUT_START_COUNTER_CLOCKWISE = 0;                             \
        OUTPUT_SC_BACK = 0;                                             \
        OUTPUT_STOP = 1;                                                \
    } else if (global.output.start) {                                   \
        if (global.output.dir == CONFIG_GROUP_DIR_FORWARD) {            \
            OUTPUT_START_CLOCKWISE = 1;                                 \
            OUTPUT_START_COUNTER_CLOCKWISE = 0;                         \
        } else if (global.output.dir == CONFIG_GROUP_DIR_REVERSE) {     \
            OUTPUT_START_CLOCKWISE = 0;                                 \
            OUTPUT_START_COUNTER_CLOCKWISE = 1;                         \
        } else {                                                        \
            OUTPUT_START_CLOCKWISE = 0;                                 \
            OUTPUT_START_COUNTER_CLOCKWISE = 1;                         \
        }                                                               \
        OUTPUT_SC_BACK = 1;                                             \
        OUTPUT_STOP = 0;                                                \
    } else {                                                            \
        OUTPUT_START_CLOCKWISE = 0;                                     \
        OUTPUT_START_COUNTER_CLOCKWISE = 0;                             \
        OUTPUT_SC_BACK = 0;                                             \
        OUTPUT_STOP = 0;                                                \
    }                                                                   \
} while (0)

#define output_flush do {           \
    output_flush_beer;              \
    output_flush_speed;             \
    output_flush_start_stop_dir;    \
} while (0)

void output_init(void);

#endif 
