#include "strand.h"
#include "global.h"

static inline void motor_run_low_speed(uint8 group_id) {
    global.output.speed_voltage = global.cfg.system.speed_voltage;
    output_flush_speed();
    global.strand.group_turns_low_speed = 0;

    global.output.start = 1;
    global.output.stop = 0;
    global.output.dir = global.cfg.groups.group[group_id].dir;
    output_flush_start_stop_dir();
}

static inline void motor_run(uint8 group_id) {
    global.output.speed_voltage = speed_percentage_to_voltage(global.cfg.groups.group[group_id].speed_percentage);
    output_flush_speed();

    global.output.start = 1;
    global.output.stop = 0;
    global.output.dir = global.cfg.groups.group[group_id].dir;
    output_flush_start_stop_dir();
}

static inline void motor_braking_start(void) {
    global.output.start = 0;
    global.output.stop = 1;
    output_flush_start_stop_dir();
    global.strand.brake_state = STRAND_BRAKE_STATE_TIMER_START;
}

static inline void motor_braking_stop(void) {
    global.output.start = 0;
    global.output.stop = 0;
    output_flush_start_stop_dir();
    global.strand.brake_state = STRAND_BRAKE_STATE_TIMER_STOP;
}

/* pause process */
void int0_process(void) interrupt 0 {
    
}

/* pulse process */
void int1_process(void) interrupt 2 {
    static uint8 xdata pulse = 0;
    //uint32 high_speed_turns;
    uint8 group_id;

    if (global.cfg.groups.num == 0) {
        return;
    }

    pulse++;
    if (pulse >= global.cfg.system.pulse) {
        pulse = 0;
        global.strand.group_turns++;
        global.strand.group_turns_low_speed++;
    }

    if (global.display.page_id != DISPLAT_PAGE_ID_WORKING) {
        return;
    }

    group_id = global.strand.group_id;

    switch (global.strand.state) {
        case STRAND_STATE_STANDBY:
            break;
        case STRAND_STATE_RUN: {
            if (global.strand.group_turns >= global.strand.high_speed_turns) {
                global.strand.state = STRAND_STATE_RUN_LOW_SPEED;
                motor_run_low_speed(group_id);
            } 
            break;
        }
        case STRAND_STATE_RUN_LOW_SPEED: {
            if ((global.strand.group_turns >= 
                        global.cfg.groups.group[group_id].arrival) ||
                    (global.strand.group_turns_low_speed >= 
                     global.strand.low_speed_turns)) {
                global.strand.state = STRAND_STATE_BRAKING;
                motor_braking_start();
            }                                         
            break;
        }
        case STRAND_STATE_BRAKING:
            break;
        case STRAND_STATE_PAUSE:
            break;
        case STRAND_STATE_FINISH:
            break;
        default:
            break;
    }
}

void strand_process(void) {
    uint8 group_id;

    if (global.display.page_id != DISPLAT_PAGE_ID_WORKING) {
        return;
    }

    if (global.cfg.groups.num == 0) {
        return;
    }

    group_id = global.strand.group_id;

    ///TODO:reset
    switch (global.strand.state) {
        case STRAND_STATE_FINISH: 
            if (global.input.start) {
                global.strand.group_id++;
                if (global.strand.group_id >= global.cfg.groups.num) {
                    global.strand.group_id = 0;
                    global.strand.output++;
                }
                group_id = global.strand.group_id;
            }
        case STRAND_STATE_STANDBY: 
        case STRAND_STATE_PAUSE: {
            if (global.input.start) {
                if (global.strand.group_turns >= global.strand.high_speed_turns) {
                    global.strand.state = STRAND_STATE_RUN;
                    motor_run(group_id);
                } else {
                    global.strand.state = STRAND_STATE_RUN_LOW_SPEED;
                    motor_run_low_speed(group_id);
                }
            }
            break;
        }
        case STRAND_STATE_RUN: {
            if (global.input.start || global.input.stop) {
                global.strand.state = STRAND_STATE_RUN_LOW_SPEED;
                motor_run_low_speed(group_id);
            }
            break;
        }
        case STRAND_STATE_RUN_LOW_SPEED: {
            break;
        }
        case STRAND_STATE_BRAKING: {
            if (global.strand.brake_state == STRAND_BRAKE_STATE_TIMER_TIMEOUT) {
                motor_braking_stop();           
                if (global.strand.group_turns >= global.cfg.groups.group[group_id].arrival) {
                    global.strand.state = STRAND_STATE_FINISH;
                } else {
                    global.strand.state = STRAND_STATE_PAUSE;
                }
            }
            break;
        }
        default:
            global.strand.state = STRAND_STATE_STANDBY;
            break;
    }

    global.input.start = 0;
    global.input.stop = 0;
    global.input.reset = 0;
}

void strand_group_init(uint8 group_id) {
    if (group_id >= global.cfg.groups.num) {
        return;
    }

    global.strand.state = STRAND_STATE_STANDBY;
    global.strand.group_id = group_id;
    global.strand.group_speed = 0;
    global.strand.group_turns = 0;
    global.strand.brake_state = STRAND_BRAKE_STATE_TIMER_STOP;
    global.strand.group_turns_low_speed = 0;
    global.strand.low_speed_turns = global.cfg.system.ahead + 
        global.cfg.groups.group[group_id].ahead; 
    global.strand.high_speed_turns = (global.cfg.groups.group[group_id].arrival > 
            (global.cfg.groups.group[group_id].ahead + 
             global.cfg.system.ahead)) ? 
        (global.cfg.groups.group[group_id].arrival - 
         global.cfg.groups.group[group_id].ahead - 
         global.cfg.system.ahead) : 0;
}

void strand_init(void) {
    global.strand.output = 0;
    
    if (global.cfg.groups.num) {
        strand_group_init(0);
    }
}
