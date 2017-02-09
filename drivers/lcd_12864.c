#include "lcd_12864.h"
#include "util.h"

static void lcd12864_write_command(uint8 cmd) {
	RS = 0;
	RW = 0;
	E = 0;
	delay(1);
	P0 = cmd;
	E = 1;
	delay(1);
	E = 0;
}

static void lcd12864_write_date(uint8 date) {
	RS = 1;
	RW = 0;
	E = 0;
	delay(1);
	P0 = date;
	E = 1;
	delay(1);
	E = 0;
}

static uint8 lcd12864_read_date(void) {
	uint8 date;

	RS = 1;
	RW = 1;
	E = 0;
	delay(1);
	E = 1;
	date = P0;
	delay(1);
	E = 0;
	delay(1);

	return date;
}

void lcd12864_init(void) {
	delay(40);
	PSB = 1;
	lcd12864_write_command(0x30);
	delay(5);
	lcd12864_write_command(0x30);
	delay(5);
	lcd12864_write_command(0x0c);
	delay(5);
	lcd12864_write_command(0x01);
	delay(5);
	lcd12864_write_command(0x06);
	delay(5);
}

/* addr[0x80, 0x87] */
/* addr[0x90, 0x97] */
/* addr[0x88, 0x8f] */
/* addr[0x98, 0x9f] */
void lcd12864_ddram_write(uint8 addr, uint8 * date, uint8 n) {
    uint8 i;

    lcd12864_write_command(0x30);
    lcd12864_write_command(addr);
    for (i = 0; i < n; i++) {
        lcd12864_write_date(date[i]);
    }
}

void lcd12864_ddram_clear(void) {
	lcd12864_write_command(0x01);
	delay(30);
}

/* addr[0x00, 0x0f] -> 0x0000 */
/* addr[0x10, 0x1f] -> 0x0002 */
/* addr[0x20, 0x2f] -> 0x0004 */
/* addr[0x30, 0x3f] -> 0x0006 */
void lcd12864_cgram_write(uint8 addr, uint8 * date, uint8 n) {
    uint8 i;

    lcd12864_write_command(0x30);
    lcd12864_write_command(0x40 + addr);
    for (i = 0; i < n; i++) {
        lcd12864_write_date(date[i]);
    }
}

void lcd12864_gdram_write(uint8 * date) {
	uint8 xdata x, y, n;

	lcd12864_write_command(0x34);
	for (x = 0; x < 16; x += 8) {
		for (y = 0; y < 32; y++) {
			lcd12864_write_command(0x80 + y);
            lcd12864_write_command(0x80 + x);
			for (n = 0; n < 8; n++) {
                if (date) {
                    lcd12864_write_date(date[x * 64 + y * 16 + n * 2]);
                    lcd12864_write_date(date[x * 64 + y * 16 + n * 2 + 1]);
                } else {
                    lcd12864_write_date(0x00);
                    lcd12864_write_date(0x00);
                }
			}
		}
	}
	lcd12864_write_command(0x36);
	lcd12864_write_command(0x30);
}

void lcd12864_gdram_clear(void) {
    lcd12864_gdram_write(NULL);
}

