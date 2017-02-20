#include "global.h"
#include "util.h"

struct GLOBAL xdata global;

static void flag_init(void) {
    global.flag.f10ms = 0;
    global.flag.f50ms = 0;
    global.flag.flashes = 0;
}

static void init(void) {
    flag_init();
    key_init();
    input_init();
    output_init();
    beer_init();
    display_init();
    display_power_on_logo();
    if (display_system_config_check() == 0) {
        delay_longtime(20);
    }
    //display_power_on_warning();
    //delay_longtime(20);
    get_config();
    EA = 0;
    strand_init();
    EA = 1;
}

int main(void) {
    init();
    
    while (1) {
        if (global.flag.f10ms) {
            global.flag.f10ms = 0;
            strand_process();
            input_clear();
        }
        beer_process();
        if (global.flag.f50ms) {
            global.flag.f50ms = 0;
            display_process();
            key_clear();
        }
    }

    return 0;
}

