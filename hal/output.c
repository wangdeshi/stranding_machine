#include "output.h"
#include "global.h"

static inline uint8 speed_duty_ratio_to_pwm(uint8 duty_ratio) {
    uint8 speed_pwm;
    uint16 tmp;
    uint8 quotient;

    if (duty_ratio > 100) {
        duty_ratio = 100;
    }

    tmp = duty_ratio;
    tmp <<= 8;

    quotient = 0;
    while (tmp >= 100) {
        tmp -= 100;
        quotient++;
    }

    speed_pwm = 255 - quotient;

    return speed_pwm;
}

uint8 speed_percentage_to_pwm(uint8 speed_percentage) {
    return speed_duty_ratio_to_pwm(speed_percentage);
}

uint8 speed_voltage_to_pwm(uint8 speed_voltage) {
    uint8 duty_ratio;

    ///FIXME:
    //f([0V, 5V]) -> pwm[0%, 100%], speed_voltage unit 100mV
    if (speed_voltage > 50) {
        speed_voltage = 50;
    }

    duty_ratio = (speed_voltage << 1);

    return speed_duty_ratio_to_pwm(duty_ratio);
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
    global.output.stop = 0;
    global.output.dir = CONFIG_GROUP_DIR_FORWARD;
    global.output.speed_pwm = 0xff;
    global.output.beer = 0;
    output_flush;
    OUTPUT_SC_BACK = 0;

    /* Start PWM */
	CR = 1;
}

