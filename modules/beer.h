#ifndef __BEER_H__
#define __BEER_H__

#include "type.h"

#define BEER_BI_MAX_NUM     10

struct BEER {
    uint8 bi[BEER_BI_MAX_NUM];
    uint8 w_p;
    uint8 r_p;
};

void beer_bi(void);

void beer_bibi(void);

void beer_process(void);

void beer_init(void);

#endif
