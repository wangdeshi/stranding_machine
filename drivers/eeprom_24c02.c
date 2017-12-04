#include "eeprom_24c02.h"
#include "util.h"

static void iic_init(void) {
    SCL = 1;
    delay(3);
    SDA = 1;
    delay(3);
}

static void iic_start(void) {
	SDA = 1;
	delay(3);
	SCL = 1;
	delay(3);
	SDA = 0;
	delay(3);
	SCL = 0;
}

static void iic_stop(void) {
    SDA = 0;
    delay(3);
    SCL = 1;
    delay(3);
    SDA = 1;
    delay(3);
}

static void iic_ack(void) {
	int16 i;

	SCL = 1;
	delay(3);
    i = 0;
	while((SDA == 1) && (i < 250)) {
        i++;
    }
	SCL = 0;
	delay(3);	
}

static void iic_write(uint8 date) {
	uint8 i;

	for (i = 0; i < 8; i++) {
		SCL = 0;
		delay(3);
		date = date << 1;
		SDA = CY;
		delay(3);
		SCL = 1;
		delay(3);
	}

	SCL = 0;
	delay(3);
	SDA = 1;
	delay(3);	
}

static uint8 iic_read(void) {
	uint8 i, date;

    date = 0;
	for (i = 0; i < 8; i++) {
		SCL = 1;
		delay(3);
		date = (date << 1) | SDA;
		SCL = 0;
		delay(3);
	}

	return date;
}

void eeprom_write(uint8 addr, uint8 date) {
    //EA = 0;
    iic_init();
    iic_start();
    iic_write(0xa0);
    iic_ack();
    iic_write(addr);
    iic_ack();
    iic_write(date);
    iic_ack();
    iic_stop();
    //EA = 1;
}

void eeprom_write_uint32(uint8 addr, uint32 date) {
    eeprom_write(addr++, date >> 24);
    eeprom_write(addr++, date >> 16);
    eeprom_write(addr++, date >> 8);
    eeprom_write(addr, date);
}

uint8 eeprom_read(uint8 addr) {
    uint8 date;

    EA = 0;
    iic_start();
    iic_write(0xa0);
    iic_ack();
    iic_write(addr);
    iic_ack();
    iic_start();
    iic_write(0xa1);
    iic_ack();
    date = iic_read();
    SDA = 1;
    iic_stop();
    EA = 1;

    return date;
}

uint32 eeprom_read_uint32(uint8 addr) {
    uint32 xdata date;

    date = eeprom_read(addr++);
    date <<= 8; 
    date += eeprom_read(addr++);
    date <<= 8; 
    date += eeprom_read(addr++);
    date <<= 8; 
    date += eeprom_read(addr);

    return date;
}

