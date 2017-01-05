#include "output.h"
#include "global.h"

uint8 speed_percentage_to_voltage(uint8 speed_percentage) {
    uint8 speed_voltage;

    speed_voltage = ((uint16)(speed_percentage - 1)) * (5 - 1) / (MAX_SPEED_PERCENTAGE - 1) + 1;

    return speed_voltage;
}

static inline uint8 speed_voltage_to_pwm(uint8 speed_voltage) {
    uint8 speed_pwm;

    ///TODO:
    speed_pwm = speed_voltage;

    return speed_pwm;
}

static inline uint8 speed_percentage_to_pwm(uint8 speed_percentage) {
    return speed_voltage_to_pwm(speed_percentage_to_voltage(speed_percentage));
}

void output_flush_beer(void) {
    if (global.output.beer) {
        OUTPUT_BEER = 1;
    } else {
        OUTPUT_BEER = 0;
    }
}

void output_flush_speed(void) {
    CCAP0L = speed_voltage_to_pwm(global.output.speed_voltage);
	CCAP0H = CCAP0L;
}

void output_flush_start_stop_dir(void) {
    if (global.output.dir == CONFIG_GROUP_DIR_FORWARD) {
        OUTPUT_DIR = 0;
    } else if (global.output.dir == CONFIG_GROUP_DIR_REVERSE) {
        OUTPUT_DIR = 1;
    } else {
        OUTPUT_DIR = 0;
    }

    if (global.output.stop) {
        OUTPUT_START = 0;
        OUTPUT_STOP = 1;
    } else if (global.output.start) {
        OUTPUT_STOP = 0;
        OUTPUT_START = 1;
    } else {
        OUTPUT_START = 0;
        OUTPUT_STOP = 1;
    }
}

void output_flush(void) {
    output_flush_beer();
    output_flush_speed();
    output_flush_start_stop_dir();
}

void output_init(void) {
    /* IO Config */
	P1M1 = 0;
	P1M0 = 0x07;

    /* PWM Config */
	CCON = 0;
	CMOD = 0x08;
	CCAPM0 = 0x42;
	CL = 0;
	CH = 0;
	CCAP0H = 0;
    CCAP0L = 0;

    /* Others Init */
    global.output.start = 0;
    global.output.stop = 1;
    global.output.dir = CONFIG_GROUP_DIR_FORWARD;
    global.output.speed_voltage = 0;
    global.output.beer = 0;
    output_flush();
    OUTPUT_SC_BACK = 0;

    /* Start PWM */
	//CR = 1;
}

