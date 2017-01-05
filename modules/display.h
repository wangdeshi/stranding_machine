#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "type.h"

struct DISPLAY_PAGE {
    uint8 private1;
};

#define DISPLAT_PAGE_ID_POWER_ON        0
#define DISPLAT_PAGE_ID_WORKING         1
#define DISPLAT_PAGE_ID_USER_CONFIG     2
#define DISPLAT_PAGE_ID_SYSTEM_CONFIG   3

struct DISPLAY {
    uint8 page_id;
    struct DISPLAY_PAGE page;
};

void display_process(void);
void display_power_on_logo(void);
void display_power_on_warning(void);
void display_form(void);
void display_init(void);

#endif
