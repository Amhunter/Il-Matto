#ifndef DISPLAY_H
#define DISPLAY_H

#define CHECK_DDR	DDRC
#define CHECK_PORT	PORTC
#define CHECK_PIN	PINC
#define CHECK_BIT	_BV(PC4)

#define LED_W	64
#define LED_H	32

#define LCD_W	128
#define LCD_H	64

#include <inttypes.h>
#include <stdio.h>

typedef int8_t int_t;
typedef uint8_t uint_t;

namespace display
{
	enum Levels {Foreground = 0, Background = 2};
	enum Colours {None = 0, Red = 1, Green = 2, Orange = Red | Green, Yellow = Orange, \
		AllRed = Red << Foreground | Red << Background, AllGreen = Green << Foreground | Green << Background, \
		FGC = ~(0xFF << Background), BGC = ~FGC};
	enum BuffColours {BuffRed = 0, BuffGreen = 1};

	// Row, Column, Colour
	extern volatile uint_t buff[LED_H][LED_W / 8][2];
	extern bool lcdOutput;
}

class Display
{
public:
	void init(void);
	void update(void);

	void fill(const uint_t clr = display::None);
	static void drawPoint(const uint_t x, const uint_t y, const uint_t clr);
	static void drawChar(const uint_t x, const uint_t y, const uint_t zoom, const uint_t clr, const char ch);
	static void drawString(const uint_t x, const uint_t y, const uint_t zoom, const uint_t clr, const char *str);
	static void drawEllipse(uint_t xx, uint_t yy, int_t w, int_t h, const uint_t clr);
	static void drawLine(uint_t x1, uint_t y1, uint_t x2, uint_t y2, const uint_t clr);
};

FILE *displayOut(void);

extern Display disp;

#endif
