#ifndef __EEPROM_24C02_H__
#define __EEPROM_24C02_H__

#include "type.h"

void eeprom_write(uint8 addr, uint8 date);

void eeprom_write_uint32(uint8 addr, uint32 date);

uint8 eeprom_read(uint8 addr);

uint32 eeprom_read_uint32(uint8 addr);

#endif
