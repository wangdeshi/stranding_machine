#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "type.h"

#define DISPLAT_PAGE_ID_POWER_ON        0
#define DISPLAT_PAGE_ID_WORKING         1
#define DISPLAT_PAGE_ID_USER_CONFIG     2
#define DISPLAT_PAGE_ID_SYSTEM_CONFIG   3
#define DISPLAT_PAGE_ID_INVALID         255

struct DISPLAY {
    uint8 page_id;
    uint8 last_page_id;
};

void display_process(void);
void display_power_on_logo(void);
void display_power_on_warning(void);
uint8 display_system_config_check(void);
void display_init(void);

#endif
