#include "lcd_12864.h"
#include "lcd.h"
#include "display.h"
#include "font.h"
#include "global.h"
#include "util.h"

static uint8 xdata last_page_id = 255;

static void show_ascii_zheng_fan(uint8 dir) {
    if (dir == CONFIG_GROUP_DIR_FORWARD) {
        lcd12864_cgram_write(0x00, cgram_zheng, 32);
        lcd_show_cgram(0x8f, 0, 2);
    } else {
        lcd12864_cgram_write(0x00, cgram_fan, 32);
        lcd_show_cgram(0x8f, 0, 2);
    }
}

static void show_flashes_ascii_zheng_fan(uint8 dir) {
    if (global.flag.flashes) {
        lcd_show_ascii(0x8f, ascii_space, 2);
    } else {
        show_ascii_zheng_fan(dir);
    }
}

static void show_flashes_digit(uint8 addr, uint8 n, uint32 digit, uint8 x, uint8 clear_bit) {
    if (global.flag.flashes) {
        //lcd_show_digit_clear_bit(addr, n, digit, x, clear_bit);
        lcd_show_digit(addr, n, digit, LCD_SHOW_DIGIT_ALIGN_RIGHT, x, 1, clear_bit);
    } else {
        lcd_show_digit_xbits(addr, n, digit, x);
        //lcd_show_digit(addr, n, digit, LCD_SHOW_DIGIT_ALIGN_RIGHT, x, 0, 0);
    }
}

static void process_page_working(void) {
    if (last_page_id != global.display.page_id) {
        last_page_id = global.display.page_id;
        lcd12864_ddram_clear();
        lcd_show_picture(pixel_working);
    }

    /* refresh raw1 */
    lcd_show_digit_normal(0x84, 2, global.cfg.groups.num);
    if ((global.strand.state == STRAND_STATE_STANDBY) ||
            (global.strand.state == STRAND_STATE_PAUSE) ||
            (global.strand.state == STRAND_STATE_FINISH)) {
        lcd12864_cgram_write(0x00, cgram_dai, 32);
        lcd12864_cgram_write(0x10, cgram_ji, 32);
        lcd_show_cgram(0x86, 0, 2);
        lcd_show_cgram(0x87, 2, 2);
    } else {
        lcd12864_cgram_write(0x00, cgram_yun, 32);
        lcd12864_cgram_write(0x10, cgram_zhuan, 32);
        lcd_show_cgram(0x86, 0, 2);
        lcd_show_cgram(0x87, 2, 2);
    }

    /* refresh raw2 */
    lcd_show_digit_normal(0x92, 6, global.strand.group_current_speed);

    /* refresh raw3 */
    lcd_show_digit_normal(0x8a, 6, global.strand.output);

    /* refresh raw4 */
    lcd_show_digit_normal(0x9a, 6, global.strand.group_current_turns);
    lcd_show_digit(0x9f, 2, global.strand.group_id, LCD_SHOW_DIGIT_ALIGN_LEFT, 0, 0, 0);

    /* key process */
    if (global.strand.state == STRAND_STATE_STANDBY) {
        if (global.key.key_menu) {
            global.key.key_menu = 0;
            global.display.page_id = DISPLAT_PAGE_ID_USER_CONFIG;
        }
    }
}

static void process_page_user_config(void) {
    static uint8 xdata group_id = 0;
    static uint8 xdata item = 0;
    static uint8 xdata offset = 0;
    uint8 digit_bits;

    if (last_page_id != global.display.page_id) {
        last_page_id = global.display.page_id;
        group_id = 0;
        item = 0;
        offset = 0;
        lcd12864_ddram_clear();
        lcd_show_picture(pixel_user_config);
    }

    /* refresh */
    switch (item) {
        case 0:
            show_flashes_digit(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS), offset);
            lcd_show_digit_xbits(0x86, 2, group_id, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits(0x9d, 2, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 1:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            show_flashes_digit(0x86, 2, group_id, get_digit_bits(USER_CFG_MAX_GROUPS), offset);
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits(0x9d, 2, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 2:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x86, 2, group_id, get_digit_bits(USER_CFG_MAX_GROUPS));
            show_flashes_digit(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL), offset);
            //show_flashes_digit(0x93, 6, 2000, get_digit_bits(USER_CFG_MAX_ARRIVAL), offset);
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            //lcd_show_digit_xbits(0x8b, 2, 19, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits(0x9d, 2, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 3:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x86, 2, group_id, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            show_flashes_digit(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE), offset);
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits(0x9d, 2, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 4:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x86, 2, group_id, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_flashes_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits(0x9d, 2, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 5:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x86, 2, group_id, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            show_flashes_digit(0x9d, 2, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD), offset);
            break;
        default:
            break;
    }

    /* key process */
    switch (item) {
        case 0:
            if (global.key.key_add) {
                global.cfg.groups.num = bound_add(global.cfg.groups.num, util_pow(10, offset), USER_CFG_MIN_GROUPS, USER_CFG_MAX_GROUPS);
                fill_default_group_config(global.cfg.groups.num);
            }
            if (global.key.key_sub) {
                global.cfg.groups.num = bound_sub(global.cfg.groups.num, util_pow(10, offset), USER_CFG_MIN_GROUPS, USER_CFG_MAX_GROUPS);
                fill_default_group_config(global.cfg.groups.num);
            }
            digit_bits = get_digit_bits(USER_CFG_MAX_GROUPS);
            break;
        case 1:
            if (global.key.key_add) {
                group_id = bound_add(group_id, util_pow(10, offset), 0, global.cfg.groups.num - 1);
            }
            if (global.key.key_sub) {
                group_id = bound_sub(group_id, util_pow(10, offset), 0, global.cfg.groups.num - 1);
            }
            digit_bits = get_digit_bits(USER_CFG_MAX_GROUPS);
            break;
        case 2:
            if (global.key.key_add) {
                global.cfg.groups.group[group_id].arrival = bound_add(
                        global.cfg.groups.group[group_id].arrival, 
                        util_pow(10, offset), 
                        USER_CFG_MIN_ARRIVAL, 
                        USER_CFG_MAX_ARRIVAL);
            }
            if (global.key.key_sub) {
                global.cfg.groups.group[group_id].arrival = bound_sub(
                        global.cfg.groups.group[group_id].arrival, 
                        util_pow(10, offset), 
                        USER_CFG_MIN_ARRIVAL, 
                        USER_CFG_MAX_ARRIVAL);
            }
            digit_bits = get_digit_bits(USER_CFG_MAX_ARRIVAL);
            break;
        case 3:
            if (global.key.key_add) {
                global.cfg.groups.group[group_id].speed_percentage = bound_add(
                        global.cfg.groups.group[group_id].speed_percentage, 
                        util_pow(10, offset), 
                        USER_CFG_MIN_SPEED_PERCENTAGE, 
                        USER_CFG_MAX_SPEED_PERCENTAGE);
            }
            if (global.key.key_sub) {
                global.cfg.groups.group[group_id].speed_percentage = bound_sub(
                        global.cfg.groups.group[group_id].speed_percentage, 
                        util_pow(10, offset), 
                        USER_CFG_MIN_SPEED_PERCENTAGE, 
                        USER_CFG_MAX_SPEED_PERCENTAGE);
            }
            digit_bits = get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE);
            break;
        case 4:
            if (global.key.key_add) {
                global.cfg.groups.group[group_id].dir = CONFIG_GROUP_DIR_FORWARD;
            }
            if (global.key.key_sub) {
                global.cfg.groups.group[group_id].dir = CONFIG_GROUP_DIR_REVERSE;
            }
            digit_bits = 0;
            break;
        case 5:
            if (global.key.key_add) {
                global.cfg.groups.group[group_id].ahead = bound_add(
                        global.cfg.groups.group[group_id].ahead, 
                        util_pow(10, offset), 
                        USER_CFG_MIN_AHEAD, 
                        USER_CFG_MAX_AHEAD);
            }
            if (global.key.key_sub) {
                global.cfg.groups.group[group_id].ahead = bound_sub(
                        global.cfg.groups.group[group_id].ahead, 
                        util_pow(10, offset), 
                        USER_CFG_MIN_AHEAD, 
                        USER_CFG_MAX_AHEAD);
            }
            digit_bits = get_digit_bits(USER_CFG_MAX_AHEAD);
            break;
        default:
            digit_bits = 0;
            break;
    }
    
    offset = bound(offset, 0, digit_bits - 1);

    if (digit_bits) {
        if (global.key.key_left) {
            offset = bound_add(offset, 1, 0, digit_bits - 1);
        } 
        if (global.key.key_right) {
            offset = bound_sub(offset, 1, 0, digit_bits - 1);
        }
    }
    if (global.key.key_up) {
        offset = 0;
        item = bound_sub(item, 1, 0, 5);
    } 
    if (global.key.key_down) {
        offset = 0;
        item = bound_add(item, 1, 0, 5);
    }
    if (global.key.key_enter) {
        global.display.page_id = DISPLAT_PAGE_ID_WORKING;
        set_all_group_config();
    }
}

static void process_page_system_config(void) {
    static uint8 xdata item = 0;
    static uint8 xdata offset = 0;
    uint8 digit_bits;

    if (last_page_id != global.display.page_id) {
        last_page_id = global.display.page_id;
        item = 0;
        offset = 0;
        lcd12864_ddram_clear();
        lcd_show_picture(pixel_system_config);
    }

    /* refresh */
    switch (item) {
        case 0:
            show_flashes_digit(0x94, 2, global.cfg.system.pulse, get_digit_bits(SYS_CFG_MAX_PULSE), offset);
            lcd_show_digit_xbits(0x8c, 2, global.cfg.system.ahead, get_digit_bits(SYS_CFG_MAX_AHEAD));
            lcd_show_digit_xbits(0x9c, 2, global.cfg.system.speed_voltage, get_digit_bits(SYS_CFG_MAX_SPEED_VOLTAGE));
            break;
        case 1:
            lcd_show_digit_xbits(0x94, 2, global.cfg.system.pulse, get_digit_bits(SYS_CFG_MAX_PULSE));
            show_flashes_digit(0x8c, 2, global.cfg.system.ahead, get_digit_bits(SYS_CFG_MAX_AHEAD), offset);
            lcd_show_digit_xbits(0x9c, 2, global.cfg.system.speed_voltage, get_digit_bits(SYS_CFG_MAX_SPEED_VOLTAGE));
            break;
        case 2:
            lcd_show_digit_xbits(0x94, 2, global.cfg.system.pulse, get_digit_bits(SYS_CFG_MAX_PULSE));
            lcd_show_digit_xbits(0x8c, 2, global.cfg.system.ahead, get_digit_bits(SYS_CFG_MAX_AHEAD));
            show_flashes_digit(0x9c, 2, global.cfg.system.speed_voltage, get_digit_bits(SYS_CFG_MAX_SPEED_VOLTAGE), offset);
            break;
        default:
            break;
    }

    /* key process */
    switch (item) {
        case 0:
            if (global.key.key_add) {
                global.cfg.system.pulse = bound_add(global.cfg.system.pulse, util_pow(10, offset), SYS_CFG_MIN_PULSE, SYS_CFG_MAX_PULSE);
            }
            if (global.key.key_sub) {
                global.cfg.system.pulse = bound_sub(global.cfg.system.pulse, util_pow(10, offset), SYS_CFG_MIN_PULSE, SYS_CFG_MAX_PULSE);
            }
            digit_bits = get_digit_bits(SYS_CFG_MAX_PULSE);
            break;
        case 1:
            if (global.key.key_add) {
                global.cfg.system.ahead = bound_add(global.cfg.system.ahead, util_pow(10, offset), SYS_CFG_MIN_AHEAD, SYS_CFG_MAX_AHEAD);
            }
            if (global.key.key_sub) {
                global.cfg.system.ahead = bound_sub(global.cfg.system.ahead, util_pow(10, offset), SYS_CFG_MIN_AHEAD, SYS_CFG_MAX_AHEAD);
            }
            digit_bits = get_digit_bits(SYS_CFG_MAX_AHEAD);
            break;
        case 2:
            if (global.key.key_add) {
                global.cfg.system.speed_voltage = bound_add(global.cfg.system.speed_voltage, util_pow(10, offset), SYS_CFG_MIN_SPEED_VOLTAGE, SYS_CFG_MAX_SPEED_VOLTAGE);
            }
            if (global.key.key_sub) {
                global.cfg.system.speed_voltage = bound_sub(global.cfg.system.speed_voltage, util_pow(10, offset), SYS_CFG_MIN_SPEED_VOLTAGE, SYS_CFG_MAX_SPEED_VOLTAGE);
            }
            digit_bits = get_digit_bits(SYS_CFG_MAX_SPEED_VOLTAGE);
            break;
        default:
            digit_bits = 0;
            break;
    }

    offset = bound(offset, 0, digit_bits - 1);

    if (digit_bits) {
        if (global.key.key_left) {
            offset = bound_add(offset, 1, 0, digit_bits - 1);
        } 
        if (global.key.key_right) {
            offset = bound_sub(offset, 1, 0, digit_bits - 1);
        }
    }
    if (global.key.key_up) {
        offset = 0;
        item = bound_sub(item, 1, 0, 2);
    } 
    if (global.key.key_down) {
        offset = 0;
        item = bound_add(item, 1, 0, 2);
    }
    if (global.key.key_enter) {
        global.display.page_id = DISPLAT_PAGE_ID_WORKING;
        set_system_config();
    }
}

static void process_page_null(void) {
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
            process_page_working();
            break;
        case DISPLAT_PAGE_ID_USER_CONFIG:
            process_page_user_config();
            break;
        case DISPLAT_PAGE_ID_SYSTEM_CONFIG:
            process_page_system_config();
            break;
        default:
            process_page_null();
            break;
    }
}

void display_power_on_logo(void) {
    lcd_show_picture(pixel_power_on_logo);
}

void display_power_on_warning(void) {
    lcd_show_picture(pixel_power_on_warning);
}

uint8 display_system_config_check(void) {
    uint8 c;

    c = 50;
    K4 = 0;
    while (c--) {
        delay(255);
        if (K1) {
            K4 = 1;
            return 0;
        }
    }
    K4 = 1;
    global.display.page_id = DISPLAT_PAGE_ID_SYSTEM_CONFIG;
    return 1;
}

void display_init(void) {
    lcd_init();
    global.display.page_id = DISPLAT_PAGE_ID_WORKING;
}

