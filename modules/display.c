#include "lcd_12864.h"
#include "lcd.h"
#include "display.h"
#include "font.h"
#include "global.h"
#include "util.h"

#define show_ascii_zheng_fan(dir) do {                  \
    if ((dir) == CONFIG_GROUP_DIR_FORWARD) {            \
        lcd12864_cgram_write(0x00, cgram_zheng, 32);    \
        lcd_show_cgram(0x8f, 0, 2);                     \
    } else {                                            \
        lcd12864_cgram_write(0x00, cgram_fan, 32);      \
        lcd_show_cgram(0x8f, 0, 2);                     \
    }                                                   \
} while (0)

#define show_flashes_ascii_zheng_fan(dir) do {          \
    if (global.flag.flashes) {                          \
        lcd_show_ascii(0x8f, ascii_space, 2);           \
    } else {                                            \
        show_ascii_zheng_fan((dir));                    \
    }                                                   \
} while (0)

#define show_flashes_digit(addr, n, digit, x, clear_bit) do {                           \
    if (global.flag.flashes) {                                                          \
        lcd_show_digit_clear_bit((addr), (n), (digit), (x), (clear_bit));               \
    } else {                                                                            \
        lcd_show_digit_xbits((addr), (n), (digit), (x));                                \
    }                                                                                   \
} while (0)

#define show_flashes_digit_center(addr, n, digit, x, clear_bit) do {                           \
    if (global.flag.flashes) {                                                          \
        lcd_show_digit_clear_bit_center((addr), (n), (digit), (x), (clear_bit));               \
    } else {                                                                            \
        lcd_show_digit_xbits_center((addr), (n), (digit), (x));                                \
    }                                                                                   \
} while (0)

#define show_flashes_digit_point(addr, n, digit, point, x, clear_bit) do {              \
    if (global.flag.flashes) {                                                          \
        lcd_show_digit_clear_bit_point((addr), (n), (digit), (point), (x), (clear_bit));               \
    } else {                                                                            \
        lcd_show_digit_xbits_point((addr), (n), (digit), (point), (x));                                \
    }                                                                                   \
} while (0)

static void process_page_working(void) {
    if (global.display.last_page_id != global.display.page_id) {
        global.display.last_page_id = global.display.page_id;
        lcd12864_ddram_clear();
        lcd_show_picture(pixel_working);
    }

    /* refresh raw1 */
    lcd_show_digit_xbits_center(0x83, 4, global.cfg.groups.num, 0);
    lcd_show_digit_xbits_center(0x86, 4, global.strand.group_id + 1, 0);

    /* refresh raw2 */
    lcd_show_digit_normal(0x92, 6, global.strand.group_current_speed);

    /* refresh raw3 */
    lcd_show_digit_normal(0x8a, 6, global.strand.output);

    /* refresh raw4 */
    lcd_show_digit_normal(0x9a, 6, global.strand.group_current_turns);
    if ((global.strand.state == STRAND_STATE_STANDBY) ||
            (global.strand.state == STRAND_STATE_PAUSE) ||
            (global.strand.state == STRAND_STATE_FINISH)) {
        lcd12864_cgram_write(0x00, cgram_dai, 32);
        lcd12864_cgram_write(0x10, cgram_ji, 32);
        lcd_show_cgram(0x9e, 0, 2);
        lcd_show_cgram(0x9f, 2, 2);
    } else {
        lcd12864_cgram_write(0x00, cgram_yun, 32);
        lcd12864_cgram_write(0x10, cgram_zhuan, 32);
        lcd_show_cgram(0x9e, 0, 2);
        lcd_show_cgram(0x9f, 2, 2);
    }

    /* key process */
    switch (global.strand.state) {
        case STRAND_STATE_STANDBY:
        case STRAND_STATE_PAUSE:
        case STRAND_STATE_FINISH:
            if (global.key.key_menu) {
                global.key.key_menu = 0;
                global.display.page_id = DISPLAT_PAGE_ID_USER_CONFIG;
            }
            if (global.key.key_sub) {
                if (global.strand.output) {
                    global.strand.output--;
                    set_strand_output();
                }
            }
            if (global.key.key_down) {
                global.strand.output = 0;
                set_strand_output();
            }
            if (global.key.key_zero) {
                strand_group_init(global.strand.group_id);
            }
            if (global.key.key_zero_double_click) {
                global.strand.group_id = 0;
                strand_group_init(global.strand.group_id);
            }
            break;
        case STRAND_STATE_RUN:
            break;
        case STRAND_STATE_RUN_LOW_SPEED:
            break;
        case STRAND_STATE_BRAKING:
            break;
        case STRAND_STATE_BRAKE_DONE:
            break;
        default:
            break;
    }
}

static void process_page_user_config(void) {
    static uint8 xdata group_id = 0;
    static uint8 xdata item = 0;
    static uint8 xdata offset = 0;
    uint8 digit_bits;

    if (global.display.last_page_id != global.display.page_id) {
        global.display.last_page_id = global.display.page_id;
        group_id = 0;
        item = 0;
        offset = get_digit_bits(USER_CFG_MAX_GROUPS) - 1;
        lcd12864_ddram_clear();
        lcd_show_picture(pixel_user_config);
    }

    /* refresh */
    switch (item) {
        case 0:
            show_flashes_digit(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS), offset);
            lcd_show_digit_xbits(0x86, 2, group_id + 1, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits_center(0x9d, 4, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 1:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            show_flashes_digit(0x86, 2, group_id + 1, get_digit_bits(USER_CFG_MAX_GROUPS), offset);
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits_center(0x9d, 4, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 2:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x86, 2, group_id + 1, get_digit_bits(USER_CFG_MAX_GROUPS));
            show_flashes_digit(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL), offset);
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits_center(0x9d, 4, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 3:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x86, 2, group_id + 1, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            show_flashes_digit(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE), offset);
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits_center(0x9d, 4, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 4:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x86, 2, group_id + 1, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_flashes_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            lcd_show_digit_xbits_center(0x9d, 4, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD));
            break;
        case 5:
            lcd_show_digit_xbits(0x83, 2, global.cfg.groups.num, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x86, 2, group_id + 1, get_digit_bits(USER_CFG_MAX_GROUPS));
            lcd_show_digit_xbits(0x93, 6, global.cfg.groups.group[group_id].arrival, get_digit_bits(USER_CFG_MAX_ARRIVAL));
            lcd_show_digit_xbits(0x8b, 2, global.cfg.groups.group[group_id].speed_percentage, get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE));
            show_ascii_zheng_fan(global.cfg.groups.group[group_id].dir);
            show_flashes_digit_center(0x9d, 4, global.cfg.groups.group[group_id].ahead, get_digit_bits(USER_CFG_MAX_AHEAD), offset);
            break;
        default:
            break;
    }

    /* key process */
    switch (item) {
        case 0:
            if (global.key.key_add) {
                global.cfg.groups.num = inc_without_carry(global.cfg.groups.num, offset, USER_CFG_MIN_GROUPS, USER_CFG_MAX_GROUPS);
                change_group_nums(global.cfg.groups.num);
            }
            if (global.key.key_sub) {
                global.cfg.groups.num = dec_without_borrow(global.cfg.groups.num, offset, USER_CFG_MIN_GROUPS, USER_CFG_MAX_GROUPS);
                change_group_nums(global.cfg.groups.num);
            }
            if (group_id >= global.cfg.groups.num) {
                group_id = global.cfg.groups.num - 1;
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
                global.cfg.groups.group[group_id].arrival = inc_without_carry(
                        global.cfg.groups.group[group_id].arrival, 
                        offset, 
                        USER_CFG_MIN_ARRIVAL, 
                        USER_CFG_MAX_ARRIVAL);
            }
            if (global.key.key_sub) {
                global.cfg.groups.group[group_id].arrival = dec_without_borrow(
                        global.cfg.groups.group[group_id].arrival, 
                        offset, 
                        USER_CFG_MIN_ARRIVAL, 
                        USER_CFG_MAX_ARRIVAL);
            }
            digit_bits = get_digit_bits(USER_CFG_MAX_ARRIVAL);
            break;
        case 3:
            if (global.key.key_add) {
                global.cfg.groups.group[group_id].speed_percentage = inc_without_carry(
                        global.cfg.groups.group[group_id].speed_percentage, 
                        offset, 
                        USER_CFG_MIN_SPEED_PERCENTAGE, 
                        USER_CFG_MAX_SPEED_PERCENTAGE);
            }
            if (global.key.key_sub) {
                global.cfg.groups.group[group_id].speed_percentage = dec_without_borrow(
                        global.cfg.groups.group[group_id].speed_percentage, 
                        offset, 
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
                global.cfg.groups.group[group_id].ahead = inc_without_carry(
                        global.cfg.groups.group[group_id].ahead, 
                        offset, 
                        USER_CFG_MIN_AHEAD, 
                        USER_CFG_MAX_AHEAD);
            }
            if (global.key.key_sub) {
                global.cfg.groups.group[group_id].ahead = dec_without_borrow(
                        global.cfg.groups.group[group_id].ahead, 
                        offset, 
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
        item = bound_sub(item, 1, 0, 5);
        switch (item) {
            case 0:
                offset = get_digit_bits(USER_CFG_MAX_GROUPS) - 1;
                break;
            case 1:
                offset = get_digit_bits(USER_CFG_MAX_GROUPS) - 1;
                break;
            case 2:
                offset = get_digit_bits(USER_CFG_MAX_ARRIVAL) - 1;
                break;
            case 3:
                offset = get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE) - 1;
                break;
            case 4:
                offset = 0;
                break;
            case 5:
                offset = get_digit_bits(USER_CFG_MAX_AHEAD) - 1;
                break;
            default:
                offset = 0;
                break;
        }
    } 
    if (global.key.key_down) {
        item = bound_add(item, 1, 0, 5);
        switch (item) {
            case 0:
                offset = get_digit_bits(USER_CFG_MAX_GROUPS) - 1;
                break;
            case 1:
                offset = get_digit_bits(USER_CFG_MAX_GROUPS) - 1;
                break;
            case 2:
                offset = get_digit_bits(USER_CFG_MAX_ARRIVAL) - 1;
                break;
            case 3:
                offset = get_digit_bits(USER_CFG_MAX_SPEED_PERCENTAGE) - 1;
                break;
            case 4:
                offset = 0;
                break;
            case 5:
                offset = get_digit_bits(USER_CFG_MAX_AHEAD) - 1;
                break;
            default:
                offset = 0;
                break;
        }
    }
    if (global.key.key_enter) {
        global.display.page_id = DISPLAT_PAGE_ID_WORKING;
        global.strand.group_id = 0;
        strand_group_init(global.strand.group_id);
        set_all_group_config();
    }
}

static void process_page_system_config(void) {
    static uint8 xdata item = 0;
    static uint8 xdata offset = 0;
    uint8 digit_bits;

    if (global.display.last_page_id != global.display.page_id) {
        global.display.last_page_id = global.display.page_id;
        item = 0;
        offset = get_digit_bits(SYS_CFG_MAX_PULSE) - 1;
        lcd12864_ddram_clear();
        lcd_show_picture(pixel_system_config);
    }

    /* refresh */
    switch (item) {
        case 0:
            show_flashes_digit(0x93, 4, global.cfg.system.pulse, get_digit_bits(SYS_CFG_MAX_PULSE), offset);
            lcd_show_digit_xbits(0x8c, 2, global.cfg.system.ahead, get_digit_bits(SYS_CFG_MAX_AHEAD));
            lcd_show_digit_xbits(0x9b, 4, global.cfg.system.speed_percentage, get_digit_bits(SYS_CFG_MAX_SPEED_PERCENTAGE));
            break;
        case 1:
            lcd_show_digit_xbits(0x93, 4, global.cfg.system.pulse, get_digit_bits(SYS_CFG_MAX_PULSE));
            show_flashes_digit(0x8c, 2, global.cfg.system.ahead, get_digit_bits(SYS_CFG_MAX_AHEAD), offset);
            lcd_show_digit_xbits(0x9b, 4, global.cfg.system.speed_percentage, get_digit_bits(SYS_CFG_MAX_SPEED_PERCENTAGE));
            break;
        case 2:
            lcd_show_digit_xbits(0x93, 4, global.cfg.system.pulse, get_digit_bits(SYS_CFG_MAX_PULSE));
            lcd_show_digit_xbits(0x8c, 2, global.cfg.system.ahead, get_digit_bits(SYS_CFG_MAX_AHEAD));
            show_flashes_digit(0x9b, 4, global.cfg.system.speed_percentage, get_digit_bits(SYS_CFG_MAX_SPEED_PERCENTAGE), offset);
            break;
        default:
            break;
    }

    /* key process */
    switch (item) {
        case 0:
            if (global.key.key_add) {
                global.cfg.system.pulse = inc_without_carry(global.cfg.system.pulse, offset, SYS_CFG_MIN_PULSE, SYS_CFG_MAX_PULSE);
            }
            if (global.key.key_sub) {
                global.cfg.system.pulse = dec_without_borrow(global.cfg.system.pulse, offset, SYS_CFG_MIN_PULSE, SYS_CFG_MAX_PULSE);
            }
            digit_bits = get_digit_bits(SYS_CFG_MAX_PULSE);
            break;
        case 1:
            if (global.key.key_add) {
                global.cfg.system.ahead = inc_without_carry(global.cfg.system.ahead, offset, SYS_CFG_MIN_AHEAD, SYS_CFG_MAX_AHEAD);
            }
            if (global.key.key_sub) {
                global.cfg.system.ahead = dec_without_borrow(global.cfg.system.ahead, offset, SYS_CFG_MIN_AHEAD, SYS_CFG_MAX_AHEAD);
            }
            digit_bits = get_digit_bits(SYS_CFG_MAX_AHEAD);
            break;
        case 2:
            if (global.key.key_add) {
                global.cfg.system.speed_percentage = inc_without_carry(global.cfg.system.speed_percentage, offset, SYS_CFG_MIN_SPEED_PERCENTAGE, SYS_CFG_MAX_SPEED_PERCENTAGE);
            }
            if (global.key.key_sub) {
                global.cfg.system.speed_percentage = dec_without_borrow(global.cfg.system.speed_percentage, offset, SYS_CFG_MIN_SPEED_PERCENTAGE, SYS_CFG_MAX_SPEED_PERCENTAGE);
            }
            digit_bits = get_digit_bits(SYS_CFG_MAX_SPEED_PERCENTAGE);
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
        item = bound_sub(item, 1, 0, 2);
        switch (item) {
            case 0:
                offset = get_digit_bits(SYS_CFG_MAX_PULSE) - 1;
                break;
            case 1:
                offset = get_digit_bits(SYS_CFG_MAX_AHEAD) - 1;
                break;
            case 2:
                offset = get_digit_bits(SYS_CFG_MAX_SPEED_PERCENTAGE) - 1;
                break;
            default:
                offset = 0;
                break;
        }
    } 
    if (global.key.key_down) {
        item = bound_add(item, 1, 0, 2);
        switch (item) {
            case 0:
                offset = get_digit_bits(SYS_CFG_MAX_PULSE) - 1;
                break;
            case 1:
                offset = get_digit_bits(SYS_CFG_MAX_AHEAD) - 1;
                break;
            case 2:
                offset = get_digit_bits(SYS_CFG_MAX_SPEED_PERCENTAGE) - 1;
                break;
            default:
                offset = 0;
                break;
        }
    }
    if (global.key.key_enter) {
        global.display.page_id = DISPLAT_PAGE_ID_WORKING;
        set_system_config();
    }
}

static void process_page_null(void) {
    if (global.display.page_id != global.display.last_page_id) {
        lcd_clear;

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
    lcd_init;
    global.display.page_id = DISPLAT_PAGE_ID_WORKING;
    global.display.last_page_id = DISPLAT_PAGE_ID_INVALID;
}

