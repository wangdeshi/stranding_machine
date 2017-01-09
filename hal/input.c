#include "input.h"
#include "global.h"

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
    input_clear();
    global.input.pulse = 0;
}
