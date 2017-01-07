#include "input.h"
#include "global.h"

///FIXME: Eliminate Dithering
void input_check(void) {
    global.input.reset = !INPUT_RESET;
    global.input.start = !INPUT_START;
    global.input.stop = !INPUT_STOP;
}

void input_clear(void) {
    global.input.reset = 0;
    global.input.start = 0;
    global.input.stop = 0;
}

void input_init(void) {
    /* Interrupt0 Config */
	//EX0=1;
	//IT0=1;

    /* Interrupt1 Config */
	//EX1=1;
	//IT1=1;

    /* Others Init */
    global.input.reset = 0;
    global.input.start = 0;
    global.input.stop = 0;
    global.input.pulse = 0;
}
