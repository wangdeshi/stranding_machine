#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "type.h"
#include "eeprom.h"
#include "key.h"
#include "input.h"
#include "output.h"
#include "display.h"
#include "strand.h"

struct GLOBAL_FLAG {
    uint8 f10ms;
    uint8 f50ms;
    uint8 flashes;
};

struct GLOBAL {
    struct CONFIG cfg;
    struct KEY key;
    struct INPUT input;
    struct OUTPUT output;
    struct DISPLAY display;
    struct STRAND strand;
    struct GLOBAL_FLAG flag;
};

extern struct GLOBAL xdata global;

#endif
