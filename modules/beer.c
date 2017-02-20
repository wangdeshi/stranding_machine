#include "beer.h"
#include "global.h"

static uint8 bi_count;

#define do_beer_sound do {      \
    global.output.beer = 1;     \
    output_flush_beer();        \
} while (0)

#define do_beer_stillness do {  \
    global.output.beer = 0;     \
    output_flush_beer();        \
} while (0)

static void beer_stillness(void) {
    bi_count = 0;
}

void beer_bi(void) {
    bi_count = 1;
}

void beer_bibi(void) {
    bi_count = 2;
}

void beer_process(void) {
    static uint8 count = 0;

    if (global.flag.beer_flag) {
        global.flag.beer_flag = 0;
        switch (count) {
            case 0:
                if (bi_count) {
                    do_beer_sound;        
                } else {
                    do_beer_stillness;        
                }
                break;
            case 1:
                do_beer_stillness;        
                break;
            case 2:
                if (bi_count > 1) {
                    do_beer_sound;        
                } else {
                    do_beer_stillness;        
                }
                break;
            case 3:
                do_beer_stillness;        
                break;
            default:
                do_beer_stillness;        
                break;
        }
        count++;
        if (count >= 4) {
            count = 0;
            beer_stillness();
        }
    }
}

void beer_init(void) {
    beer_stillness();
    do_beer_stillness;        
}

