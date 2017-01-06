#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "type.h"
#include "eeprom.h"
#include "key.h"
#include "input.h"
#include "output.h"
#include "display.h"
#include "strand.h"

#define SYS_CFG_MAX_PULSE               10
#define SYS_CFG_MIN_PULSE               10
#define SYS_CFG_MAX_AHEAD               50
#define SYS_CFG_MIN_AHEAD               0
#define SYS_CFG_MAX_SPEED_VOLTAGE       3
#define SYS_CFG_MIN_SPEED_VOLTAGE       1

#define USER_CFG_MAX_GROUPS             30
#define USER_CFG_MIN_GROUPS             1
#define USER_CFG_MAX_ARRIVAL            99999
#define USER_CFG_MIN_ARRIVAL            0
#define USER_CFG_MAX_SPEED_PERCENTAGE   99 
#define USER_CFG_MIN_SPEED_PERCENTAGE   1
#define USER_CFG_MAX_AHEAD              99
#define USER_CFG_MIN_AHEAD              0

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
