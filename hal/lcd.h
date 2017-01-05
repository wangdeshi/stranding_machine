#ifndef __LCD_H__
#define __LCD_H__

#include "type.h"

#define LCD_SHOW_DIGIT_ALIGN_LEFT       1
#define LCD_SHOW_DIGIT_ALIGN_RIGHT      2
#define LCD_SHOW_DIGIT_ALIGN_CENTER     3


void lcd_show_digit(uint8 addr, uint8 n, uint32 digit, uint8 align, uint8 show_digit_bits, uint8 clear_enable, uint8 clear_bit);

void lcd_show_digit_normal(uint8 addr, uint8 n, uint32 digit);

void lcd_show_digit_clear_bit(uint8 addr, uint8 n, uint32 digit, uint8 clear_bit);

void lcd_show_ascii(uint8 addr, uint8 * ascii, uint8 n);

void lcd_show_picture(uint8 * picture);

void lcd_clear(void);

void lcd_init(void);

#endif
