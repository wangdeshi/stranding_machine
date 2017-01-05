#include "lcd.h"
#include "display.h"
#include "font.h"
#include "global.h"
#include "util.h"

static uint8 xdata last_page_id = 255;

static void display_show_ascii_zheng_fan(uint8 dir) {
    if (dir == CONFIG_GROUP_DIR_FORWARD) {
        lcd_show_ascii(0x8e, gb2312_zheng, 2);
    } else {
        lcd_show_ascii(0x8e, gb2312_fan, 2);
    }
}

static void display_show_flashes_ascii_zheng_fan(uint8 dir) {
    if (global.flag.flashes) {
        lcd_show_ascii(0x8e, ascii_space, 2);
    } else {
        display_show_ascii_zheng_fan(dir);
    }
}

static void display_show_flashes_digit(uint8 addr, uint8 n, uint32 digit, uint8 clear_bit) {
    if (global.flag.flashes) {
        lcd_show_digit_clear_bit(addr, n, digit, clear_bit);
    } else {
        lcd_show_digit_normal(addr, n, digit);
    }
}

static void display_process_page_working(void) {
    if (global.display.page_id != last_page_id) {
        lcd_clear();
        //lcd_show_picture(pixel_working);
    }

    /* refresh raw1 */
    lcd_show_digit_normal(0x84, 2, global.cfg.groups.num);
    if (global.strand.state == STRAND_STATE_STANDBY) {
        lcd_show_ascii(0x86, gb2312_daiji, 4);
    } else {
        lcd_show_ascii(0x86, gb2312_yunzhuan, 4);
    }

    /* refresh raw2 */
    lcd_show_digit_normal(0x92, 8, global.strand.group_speed);

    /* refresh raw3 */
    lcd_show_digit_normal(0x8a, 8, global.strand.output);

    /* refresh raw4 */
    lcd_show_digit_normal(0x9a, 8, global.strand.group_turns);

    /* key process */
    ///TODO
    if (global.strand.state == STRAND_STATE_STANDBY) {
        if (global.key.key_menu) {
            global.key.key_menu = 0;
            global.display.page_id = DISPLAT_PAGE_ID_USER_CONFIG;
        }
    } else {
    
    }
}

static void display_process_page_user_config(void) {
    static uint8 xdata group_id = 0;
    static uint8 xdata item = 0;
    static uint8 xdata offset = 0;
    uint8 digit_bits;

    if (global.display.page_id != last_page_id) {
        group_id = 0;
        item = 0;
        offset = 0;
        lcd_clear();
        //lcd_show_picture(pixel_user_config);
    }

    /* refresh */
    switch (item) {
        case 0:
            display_show_flashes_digit(0x84, 2, global.cfg.groups.num, offset);
            lcd_show_digit_normal(0x87, 2, group_id);
            lcd_show_digit_normal(0x92, 10, global.cfg.groups.group[group_id].arrival);
            lcd_show_digit_normal(0x8a, 2, global.cfg.groups.group[group_id].speed_percentage);
            display_show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_normal(0x9d, 2, global.cfg.groups.group[group_id].ahead);
            break;
        case 1:
            lcd_show_digit_normal(0x84, 2, global.cfg.groups.num);
            display_show_flashes_digit(0x87, 2, group_id, offset);
            lcd_show_digit_normal(0x92, 10, global.cfg.groups.group[group_id].arrival);
            lcd_show_digit_normal(0x8a, 2, global.cfg.groups.group[group_id].speed_percentage);
            display_show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_normal(0x9d, 2, global.cfg.groups.group[group_id].ahead);
            break;
        case 2:
            lcd_show_digit_normal(0x84, 2, global.cfg.groups.num);
            lcd_show_digit_normal(0x87, 2, group_id);
            display_show_flashes_digit(0x92, 10, global.cfg.groups.group[group_id].arrival, offset);
            lcd_show_digit_normal(0x8a, 2, global.cfg.groups.group[group_id].speed_percentage);
            display_show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_normal(0x9d, 2, global.cfg.groups.group[group_id].ahead);
            break;
        case 3:
            lcd_show_digit_normal(0x84, 2, global.cfg.groups.num);
            lcd_show_digit_normal(0x87, 2, group_id);
            lcd_show_digit_normal(0x92, 10, global.cfg.groups.group[group_id].arrival);
            display_show_flashes_digit(0x8a, 2, global.cfg.groups.group[group_id].speed_percentage, offset);
            display_show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_normal(0x9d, 2, global.cfg.groups.group[group_id].ahead);
            break;
        case 4:
            lcd_show_digit_normal(0x84, 2, global.cfg.groups.num);
            lcd_show_digit_normal(0x87, 2, group_id);
            lcd_show_digit_normal(0x92, 10, global.cfg.groups.group[group_id].arrival);
            lcd_show_digit_normal(0x8a, 2, global.cfg.groups.group[group_id].speed_percentage);
            display_show_flashes_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_normal(0x9d, 2, global.cfg.groups.group[group_id].ahead);
            break;
        case 5:
            lcd_show_digit_normal(0x84, 2, global.cfg.groups.num);
            lcd_show_digit_normal(0x87, 2, group_id);
            lcd_show_digit_normal(0x92, 10, global.cfg.groups.group[group_id].arrival);
            lcd_show_digit_normal(0x8a, 2, global.cfg.groups.group[group_id].speed_percentage);
            display_show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            display_show_flashes_digit(0x9d, 2, global.cfg.groups.group[group_id].ahead, offset);
            break;
        default:
            break;
    }

    /* key process */
    ///TODO
    switch (item) {
        case 0:
            digit_bits = get_digit_bits(global.cfg.groups.num);
            if (global.key.key_add) {
            
            }
            if (global.key.key_sub) {
            
            }
            break;
        case 1:
            digit_bits = get_digit_bits(group_id);
            break;
        case 2:
            digit_bits = get_digit_bits(global.cfg.groups.group[group_id].arrival);
            break;
        case 3:
            digit_bits = get_digit_bits(global.cfg.groups.group[group_id].speed_percentage);
            break;
        case 4:
            digit_bits = 0;
            break;
        case 5:
            digit_bits = get_digit_bits(global.cfg.groups.group[group_id].ahead);
            break;
        default:
            digit_bits = 0;
            break;
    }
    
    if (digit_bits) {
        if (global.key.key_left) {
            offset++;
            if (offset >= digit_bits) {
                offset = digit_bits - 1;
            }
        } 
        if (global.key.key_right) {
            if (offset > 0) {
                offset--;
            }
        }
    }
    if (global.key.key_up) {
        offset = 0;
        if (item > 0) {
            item--;
        }
    } 
    if (global.key.key_down) {
        offset = 0;
        item++;
        if (item > 5) {
            item = 5;
        }
    }
    if (global.key.key_enter) {
        global.display.page_id = DISPLAT_PAGE_ID_WORKING;
        ///TODO:save
    }
}

static void display_process_page_system_config(void) {
    if (global.display.page_id != last_page_id) {
        lcd_clear();
        //lcd_show_picture(pixel_system_config);
    }

    /* refresh raw2 */
    lcd_show_digit_normal(0x93, 2, global.cfg.system.pulse);

    /* refresh raw3 */
    lcd_show_digit_normal(0x8b, 2, global.cfg.system.ahead);

    /* refresh raw4 */
    lcd_show_digit_normal(0x9b, 2, global.cfg.system.speed_voltage);

    /* key process */
    ///TODO
}

static void display_process_page_null(void) {
    if (global.display.page_id != last_page_id) {
        lcd_clear();

        /* raw1 */
        lcd_show_ascii(0x80, gb2312_cuowuye, 6);
    }

    /* refresh raw1 */
    lcd_show_digit_normal(0x83, 2, global.display.page_id);
}

void display_process(void) {
    switch (global.display.page_id) {
        case DISPLAT_PAGE_ID_WORKING:
            display_process_page_working();
            break;
        case DISPLAT_PAGE_ID_USER_CONFIG:
            display_process_page_user_config();
            break;
        case DISPLAT_PAGE_ID_SYSTEM_CONFIG:
            display_process_page_system_config();
            break;
        default:
            display_process_page_null();
            break;
    }

    last_page_id = global.display.page_id;
}

void display_power_on_logo(void) {
    lcd_show_picture(pixel_power_on_logo);
}

void display_power_on_warning(void) {
    lcd_show_picture(pixel_power_on_warning);
}

void display_init(void) {
    lcd_init();
    global.display.page_id = DISPLAT_PAGE_ID_WORKING;
}

