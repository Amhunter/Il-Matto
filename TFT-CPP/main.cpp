#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <tft.h>
#include "capture.h"

class tft_t tft;

void init(void)
{
	DDRB |= 0x80;			// LED
	PORTB |= 0x80;
	tft.init();
	tft.setOrient(tft.FlipLandscape);
	tft.setBackground(0x667F);
	tft.setForeground(0x0000);
	tft.clean();
	capture::init(&tft);
	capture::enable();
	stdout = tftout(&tft);
	tft.setBGLight(true);
	sei();
}

int main(void)
{
	init();

start:
	tft.clean();
	tft.setZoom(1);
	puts("*** TFT library testing ***");
	puts("STDOUT output, orientation, FG/BG colour, BG light");
	tft.setZoom(3);
	puts("Font size test");
	tft.setZoom(1);
	tft.setXY(300, 38);
	puts("Change text position & word warp test");
	tft.frame(115, 56, 200, 10, 2, 0xF800);
	puts("Draw frame test");
	tft.rectangle(118, 68, 180, 6, 0x07E0);
	puts("Draw rectangle test");
	tft.point(120, 76, 0x001F);
	tft.point(122, 76, 0x001F);
	tft.point(124, 76, 0x001F);
	tft.point(126, 76, 0x001F);
	tft.point(128, 76, 0x001F);
	tft.point(130, 76, 0x001F);
	puts("Draw points test");
	tft.line(200, 100, 300, 200, 0xF800);
	tft.line(300, 210, 200, 110, 0x001F);
	tft.line(200, 200, 300, 100, 0xF800);
	tft.line(300, 110, 200, 210, 0x001F);

	tft.line(100, 100, 300, 200, 0xF800);
	tft.line(300, 210, 100, 110, 0x001F);
	tft.line(100, 200, 300, 100, 0xF800);
	tft.line(300, 110, 100, 210, 0x001F);

	tft.line(200, 0, 300, 200, 0xF800);
	tft.line(300, 210, 200, 10, 0x001F);
	tft.line(200, 200, 300, 0, 0xF800);
	tft.line(300, 10, 200, 210, 0x001F);

	tft.line(100, 150, 300, 150, 0xF800);
	tft.line(300, 160, 100, 160, 0x001F);
	tft.line(250, 0, 250, 200, 0xF800);
	tft.line(260, 200, 260, 0, 0x001F);
	puts("Draw lines test");

#ifdef TFT_VERTICALSCROLLING
	tft.setVerticalScrollingArea(64, 32);
	uint16_t max = 320 - 32;
	uint16_t v = 64;
loop:
	tft.setVerticalScrolling(v);
	if (max == ++v)
		v = 64;
	_delay_ms(10);
#else
loop:
#endif
	goto loop;
	goto start;

	return 1;
}
