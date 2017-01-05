#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "type.h"

struct OUTPUT {
    uint8 start;
    uint8 stop;
    uint8 dir;
    uint8 speed_voltage;
    uint8 beer;
};

#define MAX_SPEED_PERCENTAGE    100

uint8 speed_percentage_to_voltage(uint8 speed_percentage);
void output_flush_beer(void);
void output_flush_speed(void);
void output_flush_start_stop_dir(void);
void output_flush(void);
void output_init(void);

#endif 
