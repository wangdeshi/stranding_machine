#ifndef __LCD_H__
#define __LCD_H__

#include "type.h"

#define LCD_SHOW_DIGIT_ALIGN_LEFT       1
#define LCD_SHOW_DIGIT_ALIGN_RIGHT      2
#define LCD_SHOW_DIGIT_ALIGN_CENTER     3

extern uint8 xdata _lcd_addr;
extern uint8 xdata _lcd_n;
extern uint8 xdata _lcd_align;
extern uint8 xdata _lcd_show_digit_bits;
extern uint8 xdata _lcd_point;
extern uint8 xdata _lcd_clear_enable;
extern uint8 xdata _lcd_clear_bit;
extern uint32 xdata _lcd_digit;

void lcd_show_digit_function(void);

void lcd_show_cgram(uint8 addr, uint8 index, uint8 n);

#define lcd_show_digit(addr, n, digit, point, align, show_digit_bits, clear_enable, clear_bit) do {        \
    _lcd_addr = (addr);                                                                             \
    _lcd_n = (n);                                                                                   \
    _lcd_digit = (digit);                                                                           \
    _lcd_point = (point);                                                                           \
    _lcd_align = (align);                                                                           \
    _lcd_show_digit_bits = (show_digit_bits);                                                       \
    _lcd_clear_enable = (clear_enable);                                                             \
    _lcd_clear_bit = (clear_bit);                                                                   \
    lcd_show_digit_function();                                                                      \
} while (0)

#define lcd_show_digit_normal(addr, n, digit) lcd_show_digit((addr), (n), (digit), 0, LCD_SHOW_DIGIT_ALIGN_RIGHT, 0, 0, 0)

#define lcd_show_digit_xbits(addr, n, digit, x) lcd_show_digit((addr), (n), (digit), 0, LCD_SHOW_DIGIT_ALIGN_RIGHT, (x), 0, 0)

#define lcd_show_digit_xbits_center(addr, n, digit, x) lcd_show_digit((addr), (n), (digit), 0, LCD_SHOW_DIGIT_ALIGN_CENTER, (x), 0, 0)

#define lcd_show_digit_xbits_point(addr, n, digit, point, x) lcd_show_digit((addr), (n), (digit), (point), LCD_SHOW_DIGIT_ALIGN_RIGHT, (x), 0, 0)

#define lcd_show_digit_clear_bit(addr, n, digit, x, clear_bit) lcd_show_digit((addr), (n), (digit), 0, LCD_SHOW_DIGIT_ALIGN_RIGHT, (x), 1, (clear_bit))

#define lcd_show_digit_clear_bit_center(addr, n, digit, x, clear_bit) lcd_show_digit((addr), (n), (digit), 0, LCD_SHOW_DIGIT_ALIGN_CENTER, (x), 1, (clear_bit))

#define lcd_show_digit_clear_bit_point(addr, n, digit, point, x, clear_bit) lcd_show_digit((addr), (n), (digit), (point), LCD_SHOW_DIGIT_ALIGN_RIGHT, (x), 1, (clear_bit))

#define lcd_show_ascii(addr, ascii, n) lcd12864_ddram_write((addr), (ascii), (n))

#define lcd_show_picture(picture) lcd12864_gdram_write((picture))

#define lcd_clear do {          \
    lcd12864_ddram_clear();     \
    lcd12864_gdram_clear();     \
} while (0)

#define lcd_init lcd12864_init()

#endif
