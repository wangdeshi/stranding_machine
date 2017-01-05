#ifndef __TYPE_H__
#define __TYPE_H__

#define __BUILD_C51__
#define NULL    0

#ifdef __BUILD_C51__
#define inline
#endif

typedef signed char         int8;
typedef unsigned char       uint8;
typedef signed int          int16;
typedef unsigned int        uint16;
typedef signed long         int32;
typedef unsigned long       uint32;

#ifdef __SDCC__
#include<8052.h> 

#define xdata __xdata
#define code __code
#define interrupt __interrupt

/* SFR */
__sfr __at 0x91 P1M1;
__sfr __at 0x92 P1M0;
__sfr __at 0xd8 CCON;
__sfr __at 0xd9 CMOD;
__sfr __at 0xe9 CL;
__sfr __at 0xf9 CH;
__sfr __at 0xea CCAP0L;
__sfr __at 0xfa CCAP0H;
__sfr __at 0xda CCAPM0;
__sfr __at 0xf2 PCAPWM0;
__sfr __at 0x80 P0;
__sfr __at 0x90 P1;
__sfr __at 0xa0 P2;
__sfr __at 0xb0 P3;

__sbit __at 0xd7 CY;
__sbit __at 0xd8 CCF0;
__sbit __at 0xde CR;
__sbit __at 0xdf CF;

/* LCD12864 */
__sbit __at 0xb0 RS;
__sbit __at 0xb1 RW;
__sbit __at 0x96 E;
__sbit __at 0x97 PSB;

/* EEPROM */
__sbit __at 0xa6 SDA;
__sbit __at 0xa7 SCL;

/* OUTPUT */
__sbit __at 0x90 OUTPUT_DIR;
__sbit __at 0x91 OUTPUT_START;
__sbit __at 0x92 OUTPUT_STOP;
__sbit __at 0x94 OUTPUT_SC_BACK;
__sbit __at 0x95 OUTPUT_BEER;

/* INPUT */
__sbit __at 0xb2 INPUT_STOP;
__sbit __at 0xb6 INPUT_RESET;
__sbit __at 0xb7 INPUT_START;

/* KEY */
__sbit __at 0xa5 K0;
__sbit __at 0xa4 K1;
__sbit __at 0xa3 K2;
__sbit __at 0xa2 K3;
__sbit __at 0xa1 K4;
__sbit __at 0xa0 K5;
#else
#include<reg52.h> 
/* SFR */
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr CCON = 0xd8;
sbit CCF0 = CCON^0;
sbit CR = CCON^6;
sbit CF = CCON^7;
sfr CMOD = 0xd9;
sfr CL = 0xe9;
sfr CH = 0xf9;
sfr CCAP0L = 0xea;
sfr CCAP0H = 0xfa;
sfr CCAPM0 = 0xda;
sfr PCAPWM0 = 0xf2;

/* LCD12864 */
sbit RS = P3^0;
sbit RW = P3^1;
sbit E = P1^6;
sbit PSB = P1^7;

/* EEPROM */
sbit SDA = P2^6;
sbit SCL = P2^7;

/* OUTPUT */
sbit OUTPUT_DIR = P1^0;
sbit OUTPUT_START = P1^1;
sbit OUTPUT_STOP = P1^2;
sbit OUTPUT_SC_BACK = P1^4;
sbit OUTPUT_BEER = P1^5;

/* INPUT */
sbit INPUT_STOP = P3^2;
sbit INPUT_RESET = P3^6;
sbit INPUT_START = P3^7;

/* KEY */
sbit K0 = P2^5;
sbit K1 = P2^4;
sbit K2 = P2^3;
sbit K3 = P2^2;
sbit K4 = P2^1;
sbit K5 = P2^0;
#endif

#endif
