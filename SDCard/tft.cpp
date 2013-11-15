#include <avr/io.h>
#include <stdlib.h>
#include "ascii.h"
#include "ili9341.h"
#include "tft.h"

#define WIDTH 6
#define HEIGHT 8
#define SIZE_H 320
#define SIZE_W 240
#define DEF_FGC 0xFFFF
#define DEF_BGC 0x0000
#define SWAP(x, y) { \
	(x) = (x) ^ (y); \
	(y) = (x) ^ (y); \
	(x) = (x) ^ (y); \
}

#include <avr/pgmspace.h>

class tfthw tft;

tfthw::tfthw(void)
{
	x = 0;
	y = 0;
	zoom = 1;
	orient = Portrait;
	w = SIZE_W;
	h = SIZE_H;
	fgc = DEF_FGC;
	bgc = DEF_BGC;
}

void tfthw::frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, \
		uint8_t s, uint16_t c)
{
	rectangle(x, y, w - s, s, c);
	rectangle(x + w - s, y, s, h - s, c);
	rectangle(x, y + s, s, h - s, c);
	rectangle(x + s, y + h - s, w - s, s, c);
}

void tfthw::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, \
		uint16_t c)
{
	if (x0 == x1) {
		if (y0 > y1)
			SWAP(y0, y1);
		rectangle(x0, y0, 1, y1 - y0, c);
		return;
	}
	if (y0 == y1) {
		if (x0 > x1)
			SWAP(x0, x1);
		rectangle(x0, y0, x1 - x0, 1, c);
		return;
	}
	uint16_t dx = abs(x1 - x0), dy = abs(y1 - y0);
	if (dx < dy) {
		if (y0 > y1) {
			SWAP(x0, x1);
			SWAP(y0, y1);
		}
		for (uint16_t y = y0; y <= y1; y++)
			point(x0 + x1 * (y - y0) / dy - \
					x0 * (y - y0) / dy, y, c);
	} else {
		if (x0 > x1) {
			SWAP(x0, x1);
			SWAP(y0, y1);
		}
		for (uint16_t x = x0; x <= x1; x++)
			point(x, y0 + y1 * (x - x0) / dx - \
					y0 * (x - x0) / dx, c);
	}
}

void tfthw::rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, \
		uint16_t c)
{
	area(x, y, w, h);
	send(1, 0x2C);			// Memory Write
	while (w--)
		for (y = 0; y < h; y++) {
			send(0, c / 0x0100);
			send(0, c % 0x0100);
		}
}

void tfthw::putch(char ch)
{
	unsigned char c;
	uint8_t i, j;
	area(x, y, WIDTH * zoom, HEIGHT * zoom);
	send(1, 0x2C);			// Memory Write
	for (i = 0; i < HEIGHT * zoom; i++) {
		c = pgm_read_byte(&(ascii[ch - ' '][i / zoom]));
		for (j = 0; j < WIDTH * zoom; j++) {
			if (c & 0x80) {
				send(0, fgc / 0x0100);
				send(0, fgc % 0x0100);
			} else {
				send(0, bgc / 0x0100);
				send(0, bgc % 0x0100);
			}
			if (j % zoom == zoom - 1)
				c <<= 1;
		}
	}
}
