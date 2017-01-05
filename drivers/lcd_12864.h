#ifndef __LCD_12864_H__
#define __LDC_12864_H__

#include "type.h"

void lcd12864_init(void);

void lcd12864_ddram_write(uint8 addr, uint8 * date, uint8 n);

void lcd12864_ddram_clear(void);

void lcd12864_cgram_write(uint8 addr, uint8 * date, uint8 n);

void lcd12864_gdram_write(uint8 * date);

void lcd12864_gdram_clear(void);

#endif
