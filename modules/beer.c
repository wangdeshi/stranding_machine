#include "beer.h"
#include "global.h"

void beer_bi(void) {

}

void beer_bibi(void) {

}

void beer_process(void) {
    static uint8 count;
    static uint8 bi_count;


    if (global.beer.bi[global.beer.r_p]) {
        if (global.flag.beer_flag) {
            count++;
        }
        switch (count) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                break;
        }
    }
}

void beer_init(void) {
    uint8 i;

    for (i = 0; i < BEER_BI_MAX_NUM; i++) {
        global.beer.bi[i] = 0;
    }
    global.beer.w_p = 0;
    global.beer.r_p = 0;
}

