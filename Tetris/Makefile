# Modified by zhiyb (Yubo Zhi)

TRG	= Tetris
SRC	= ascii.c display.c ili9341.c main.c rand.c rgbconv.c rotary.c sound.c tetris.c tft.c timer.c
LIB	= common
LIBDIR	= ../lib

PRGER		= usbasp
MCU_TARGET	= atmega644p
MCU_FREQ	= 12000000

DEFS	+= -DBAUD=9600

EFUSE	= 0xFF
HFUSE	= 0x9C
LFUSE	= 0xE7

include ../Makefile.defs
