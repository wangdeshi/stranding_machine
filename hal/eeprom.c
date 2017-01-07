#include "eeprom_24c02.h"
#include "eeprom.h"
#include "global.h"

static uint32 config_magic = 0x55aa1122;

static inline uint8 group_config_address(uint8 group_id) { 
    return (group_id * CONFIG_SIZE_ONE_GROUP + CONFIG_ADDRESS_GROUP_BEGIN); 
}

static void set_magic_config(void) {
    eeprom_write_uint32(CONFIG_ADDRESS_MAGIC, config_magic);
}

static uint32 get_magic_config(void) {
    uint32 magic;

    magic = eeprom_read_uint32(CONFIG_ADDRESS_MAGIC);

    return magic;
}

static void get_system_config(void) {
    struct CONFIG_SYSTEM * cfg;
    uint8 addr;

    cfg = &global.cfg.system;
    addr = CONFIG_ADDRESS_SYSTEM;

    cfg->speed_voltage = eeprom_read(addr++);
    cfg->ahead = eeprom_read(addr++);
    cfg->pulse = eeprom_read(addr++);
}

static void get_group_config(uint8 group_id) {
    struct CONFIG_GROUP * cfg;
    uint8 addr;
    uint8 isconfig_and_dir;

    cfg = &global.cfg.groups.group[group_id];
    addr = group_config_address(group_id);

    isconfig_and_dir = eeprom_read(addr++);
    if (isconfig_and_dir & 0x80) {
        cfg->isconfig = 1;
    } else {
        cfg->isconfig = 0;
    }

    cfg->dir = isconfig_and_dir & 0x7f;

    cfg->speed_percentage = eeprom_read(addr++);
    cfg->ahead = eeprom_read(addr++);
    cfg->arrival = eeprom_read_uint32(addr);
}

static void get_all_group_config(void) {
    uint8 group_id;

    fill_default_group_config(0);
    global.cfg.groups.num = eeprom_read(CONFIG_ADDRESS_GROUPS_NUM);
    for (group_id = 0; group_id < global.cfg.groups.num; group_id++) {
        get_group_config(group_id);
    }
}

void get_config(void) {
    uint32 magic;
    uint8 group_id;

    magic = get_magic_config();
    if (magic != config_magic) {
        /* default system config */
        fill_default_system_config();

        /* default group config */
        fill_default_group_config(1);

        /* write to eeprom */
        set_magic_config();
        set_system_config();
        set_all_group_config();
    }
    get_system_config();
    get_all_group_config();
}

void set_system_config(void) {
    struct CONFIG_SYSTEM * cfg;
    uint8 addr;

    cfg = &global.cfg.system;
    addr = CONFIG_ADDRESS_SYSTEM;

    eeprom_write(addr++, cfg->speed_voltage);
    eeprom_write(addr++, cfg->ahead);
    eeprom_write(addr++, cfg->pulse);
}

static void set_group_config(uint8 group_id) {
    struct CONFIG_GROUP * cfg;
    uint8 addr;
    uint8 isconfig_and_dir;

    cfg = &global.cfg.groups.group[group_id];
    addr = group_config_address(group_id);

    isconfig_and_dir = cfg->dir;
    if (cfg->isconfig) {
        isconfig_and_dir |= 0x80;
    } else {
        isconfig_and_dir &= 0x7f;
    }

    eeprom_write(addr++, isconfig_and_dir);
    eeprom_write(addr++, cfg->speed_percentage);
    eeprom_write(addr++, cfg->ahead);
    eeprom_write_uint32(addr, cfg->arrival);
}

void set_all_group_config(void) {
    uint8 group_id;

    eeprom_write(CONFIG_ADDRESS_GROUPS_NUM, global.cfg.groups.num);
    for (group_id = 0; group_id < global.cfg.groups.num; group_id++) {
        set_group_config(group_id);
    }
}

void fill_default_system_config(void) {
    global.cfg.system.speed_voltage = 2;
    global.cfg.system.ahead = 30;
    global.cfg.system.pulse = 1;
}

void fill_default_group_config(uint8 group_nums) {
    uint8 group_id;

    global.cfg.groups.num = group_nums;

    for (group_id = 0; group_id < group_nums; group_id++) {
        global.cfg.groups.group[group_id].isconfig = 1;
        global.cfg.groups.group[group_id].dir = CONFIG_GROUP_DIR_FORWARD;
        global.cfg.groups.group[group_id].speed_percentage = 1;
        global.cfg.groups.group[group_id].ahead = 0;
        global.cfg.groups.group[group_id].arrival = 0;
    }

    for (group_id = group_nums; group_id < MAX_GROUP_NUM; group_id++) {
        global.cfg.groups.group[group_id].isconfig = 0;
        global.cfg.groups.group[group_id].dir = 0;
        global.cfg.groups.group[group_id].speed_percentage = 0;
        global.cfg.groups.group[group_id].ahead = 0;
        global.cfg.groups.group[group_id].arrival = 0;
    }
}

