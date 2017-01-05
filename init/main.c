#include "global.h"
#include "util.h"
#include "lcd_12864.h"

struct GLOBAL xdata global;

void init(void) {
    get_config();
    key_init();
    input_init();
    output_init();
    display_init();
    strand_init();
    display_power_on_logo();
    delay_longtime(20);
    //display_power_on_warning();
    //delay_longtime(20);
    //lcd12864_gdram_clear();
    EA = 1;
}

int main(void) {
    init();
    
    while (1) {
        if (global.flag.f10ms) {
            global.flag.f10ms = 0;
            input_check();
            //strand_process();
            input_clear();
        }

        if (global.flag.f50ms) {
            global.flag.f50ms = 0;
            display_process();
            key_clear();
        }
    }

    return 0;
}
