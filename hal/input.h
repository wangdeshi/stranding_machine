#ifndef __INPUT_H__
#define __INPUT_H__

#include "type.h"

struct INPUT {
    uint8 reset_double_click;
    uint8 reset;
    uint8 start;
    /* interrupt0 */
    uint8 stop;
    /* interrupt1 */
    uint8 pulse;
};

void input_clear(void);

void input_init(void);

#endif
