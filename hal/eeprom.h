#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "type.h"

#define MAX_GROUP_NUM       30

struct CONFIG_SYSTEM {
    uint8 speed_voltage;
    uint8 ahead;
    uint8 pulse;
};

#define CONFIG_GROUP_DIR_FORWARD    1
#define CONFIG_GROUP_DIR_REVERSE    2
struct CONFIG_GROUP {
    uint8 isconfig;
    uint8 dir;
    uint8 speed_percentage;
    uint8 ahead;
    uint32 arrival;
};

struct CONFIG_GROUPS {
    uint8 num;
    struct CONFIG_GROUP group[MAX_GROUP_NUM];
};

struct CONFIG {
    struct CONFIG_SYSTEM system;
    struct CONFIG_GROUPS groups;
};

#define CONFIG_SIZE_MAGIC           0x04
#define CONFIG_SIZE_SYSTEM          0x10
#define CONFIG_SIZE_GROUPS_NUM      0x01
#define CONFIG_SIZE_ONE_GROUP       0x07

#define CONFIG_ADDRESS_MAGIC        0
#define CONFIG_ADDRESS_SYSTEM       (CONFIG_ADDRESS_MAGIC + CONFIG_SIZE_MAGIC)
#define CONFIG_ADDRESS_GROUPS_NUM   (CONFIG_ADDRESS_SYSTEM + CONFIG_SIZE_SYSTEM)
#define CONFIG_ADDRESS_GROUP_BEGIN  (CONFIG_ADDRESS_GROUPS_NUM + CONFIG_SIZE_GROUPS_NUM)

void set_system_config(void);

void set_all_group_config(void);

void get_config(void);

#endif
