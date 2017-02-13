#include "lcd_12864.h"
#include "lcd.h"
#include "util.h"

uint8 xdata _lcd_addr;
uint8 xdata _lcd_n;
uint8 xdata _lcd_align;
uint8 xdata _lcd_show_digit_bits;
uint8 xdata _lcd_clear_enable;
uint8 xdata _lcd_clear_bit;
uint32 xdata _lcd_digit;

#define get_ascii(x) (0x30 + (x))

static uint8 xdata lcd_data[16];
static uint8 xdata bits[8];

static uint32 xdata _lcd_digit_get_bit_digit;
static void lcd_digit_get_bit_function(void) {
    uint8 i;

    for (i = 0; i < 8; i++) {
        bits[i] = 0;
    }

    while (_lcd_digit_get_bit_digit >= 100000000) {
        _lcd_digit_get_bit_digit -= 100000000;
    }
    
    while (_lcd_digit_get_bit_digit >= 10000000) {
        _lcd_digit_get_bit_digit -= 10000000;
        bits[7]++;
    }
    
    while (_lcd_digit_get_bit_digit >= 1000000) {
        _lcd_digit_get_bit_digit -= 1000000;
        bits[6]++;
    }
    
    while (_lcd_digit_get_bit_digit >= 100000) {
        _lcd_digit_get_bit_digit -= 100000;
        bits[5]++;
    }
    
    while (_lcd_digit_get_bit_digit >= 10000) {
        _lcd_digit_get_bit_digit -= 10000;
        bits[4]++;
    }
    
    while (_lcd_digit_get_bit_digit >= 1000) {
        _lcd_digit_get_bit_digit -= 1000;
        bits[3]++;
    }
    
    while (_lcd_digit_get_bit_digit >= 100) {
        _lcd_digit_get_bit_digit -= 100;
        bits[2]++;
    }
    
    while (_lcd_digit_get_bit_digit >= 10) {
        _lcd_digit_get_bit_digit -= 10;
        bits[1]++;
    }
    
    while (_lcd_digit_get_bit_digit >= 1) {
        _lcd_digit_get_bit_digit -= 1;
        bits[0]++;
    }
}

#define lcd_digit_get_bit(digit) (       \
    _lcd_digit_get_bit_digit = (digit),     \
    lcd_digit_get_bit_function()         \
)

void lcd_show_digit_function(void) {
    uint8 xdata digit_bits;
    uint8 xdata show_bits;
    uint8 xdata start;
    uint8 i, bit_value;

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

    for (i = 0; i < _lcd_n; i++) {
        lcd_data[i] = 0x20;   
    }

    if (_lcd_align == LCD_SHOW_DIGIT_ALIGN_LEFT) {
        start = 0; 
    } else if (_lcd_align == LCD_SHOW_DIGIT_ALIGN_RIGHT) {
        start = _lcd_n - show_bits;
    } else if (_lcd_align == LCD_SHOW_DIGIT_ALIGN_CENTER) {
        start = (_lcd_n - show_bits) >> 1;
    } else {
        start = _lcd_n - show_bits;
    }

    lcd_digit_get_bit(_lcd_digit);
    for (i = 0; i < show_bits; i++) {
        lcd_data[start + i] = get_ascii(bits[show_bits - 1 - i]);
    }

    if (_lcd_clear_enable && (_lcd_clear_bit < show_bits)) {
        lcd_data[start + show_bits - 1 - _lcd_clear_bit] = 0x20;
    }

    lcd12864_ddram_write(_lcd_addr, lcd_data, _lcd_n);
}

void lcd_show_cgram(uint8 addr, uint8 index, uint8 n) {
    uint8 xdata cgram_addr[2];

    cgram_addr[0] = 0x00;
    cgram_addr[1] = index;
    lcd_show_ascii(addr, cgram_addr, n);
}

