#include "lcd_12864.h"
#include "lcd.h"
#include "util.h"

uint8 xdata _lcd_addr;
uint8 xdata _lcd_n;
uint8 xdata _lcd_align;
uint8 xdata _lcd_show_digit_bits;
uint8 xdata _lcd_point;
uint8 xdata _lcd_clear_enable;
uint8 xdata _lcd_clear_bit;
uint32 xdata _lcd_digit;

#define get_ascii(x) (0x30 + (x))

static uint8 xdata lcd_data[16];

void lcd_show_digit_function(void) {
    static uint8 xdata digit_bits;
    static uint8 xdata show_bits, show_n;
    static uint8 xdata start, end;
    uint8 i;

    if (_lcd_n > 16) {
        _lcd_n = 16;
    }
    if (_lcd_n & 0x01) {
        _lcd_n &= 0xfe;
    }

    if (_lcd_show_digit_bits) {
        show_bits = (_lcd_show_digit_bits < _lcd_n) ? _lcd_show_digit_bits : _lcd_n;
    } else {
        digit_bits = get_digit_bits(_lcd_digit);
        show_bits = (digit_bits < _lcd_n) ? digit_bits : _lcd_n;
    }
    if (_lcd_point) {
        if (show_bits >= _lcd_n) {
            show_bits -= 1;
        }
        show_n = show_bits + 1;
    } else {
        show_n = show_bits;
    }
    if (show_n == 0) {
        return;
    }

    for (i = 0; i < _lcd_n; i++) {
        lcd_data[i] = 0x20;   
    }

    if (_lcd_align == LCD_SHOW_DIGIT_ALIGN_LEFT) {
        start = 0; 
    } else if (_lcd_align == LCD_SHOW_DIGIT_ALIGN_RIGHT) {
        start = _lcd_n - show_n;
    } else if (_lcd_align == LCD_SHOW_DIGIT_ALIGN_CENTER) {
        start = (_lcd_n - show_n) >> 1;
    } else {
        start = _lcd_n - show_n;
    }
    end = start + show_n - 1;

    digit_get_bit(_lcd_digit);
    if (_lcd_point) {
        for (i = 0; i < _lcd_point; i++) {
            lcd_data[end - i] = get_ascii(_digit_bits[i]);
        }
        lcd_data[end - _lcd_point] = 0x2e;  //ascii(.)
        for (i = _lcd_point; i < show_bits; i++) {
            lcd_data[end - i - 1] = get_ascii(_digit_bits[i]);
        }
        if (_lcd_clear_enable && (_lcd_clear_bit < show_bits)) {
            if (_lcd_clear_bit >= _lcd_point) {
                lcd_data[end - _lcd_clear_bit - 1] = 0x20;
            } else {
                lcd_data[end - _lcd_clear_bit] = 0x20;
            }
        }
    } else {
        for (i = 0; i < show_bits; i++) {
            lcd_data[end - i] = get_ascii(_digit_bits[i]);
        }
        if (_lcd_clear_enable && (_lcd_clear_bit < show_bits)) {
            lcd_data[end - _lcd_clear_bit] = 0x20;
        }
    }

    lcd12864_ddram_write(_lcd_addr, lcd_data, _lcd_n);
}

void lcd_show_cgram(uint8 addr, uint8 index, uint8 n) {
    uint8 xdata cgram_addr[2];

    cgram_addr[0] = 0x00;
    cgram_addr[1] = index;
    lcd_show_ascii(addr, cgram_addr, n);
}

