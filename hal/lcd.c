#include "lcd_12864.h"
#include "lcd.h"
#include "util.h"

static uint8 xdata lcd_data[16];

static inline uint8 ascii(uint8 x) { return (0x30 + x); }

static uint8 lcd_digit_get_bit(uint32 digit, uint8 x) {
    uint8 i;

    for (i = 0; i < x; i++) {
        digit /= 10;
    }

    return (digit % 10);
}

void lcd_show_digit(uint8 addr, uint8 n, uint32 digit, uint8 align, uint8 show_digit_bits, uint8 clear_enable, uint8 clear_bit) {
    uint8 digit_bits;
    uint8 show_bits;
    uint8 start;
    uint8 i;

    if (n > 16) {
        n = 16;
    }
    if (n & 0x01) {
        n &= 0xfe;
    }

    if (show_digit_bits) {
        show_bits = (show_digit_bits < n) ? show_digit_bits : n;
    } else {
        digit_bits = get_digit_bits(digit);
        show_bits = (digit_bits < n) ? digit_bits : n;
    }

    for (i = 0; i < n; i++) {
        lcd_data[i] = 0x20;   
    }

    if (align == LCD_SHOW_DIGIT_ALIGN_LEFT) {
        start = 0; 
    } else if (align == LCD_SHOW_DIGIT_ALIGN_RIGHT) {
        start = n - show_bits;
    } else if (align == LCD_SHOW_DIGIT_ALIGN_CENTER) {
        start = (n - show_bits) >> 1;
    } else {
        start = n - show_bits;
    }

    for (i = 0; i < show_bits; i++) {
        lcd_data[start + i] = ascii(lcd_digit_get_bit(digit, show_bits - 1 - i));
    }

    if (clear_enable && (clear_bit < show_bits)) {
        lcd_data[start + show_bits - 1 - clear_bit] = 0x20;
    }

    lcd12864_ddram_write(addr, lcd_data, n);
}

void lcd_show_digit_normal(uint8 addr, uint8 n, uint32 digit) {
    lcd_show_digit(addr, n, digit, LCD_SHOW_DIGIT_ALIGN_RIGHT, 0, 0, 0);
}

void lcd_show_digit_xbits(uint8 addr, uint8 n, uint32 digit, uint8 x) {
    lcd_show_digit(addr, n, digit, LCD_SHOW_DIGIT_ALIGN_RIGHT, x, 0, 0);
}

void lcd_show_digit_clear_bit(uint8 addr, uint8 n, uint32 digit, uint8 x, uint8 clear_bit) {
    lcd_show_digit(addr, n, digit, LCD_SHOW_DIGIT_ALIGN_RIGHT, x, 1, clear_bit);
}

void lcd_show_ascii(uint8 addr, uint8 * ascii, uint8 n) {
    lcd12864_ddram_write(addr, ascii, n);
}

void lcd_show_cgram(uint8 addr, uint8 index, uint8 n) {
    uint8 cgram_addr[2];

    cgram_addr[0] = 0x00;
    cgram_addr[1] = index;
    lcd_show_ascii(addr, cgram_addr, n);
}

void lcd_show_picture(uint8 * picture) {
    lcd12864_gdram_write(picture);
}

void lcd_clear(void) {
    lcd12864_ddram_clear();
    lcd12864_gdram_clear();
}

void lcd_init(void) {
    lcd12864_init();
}

