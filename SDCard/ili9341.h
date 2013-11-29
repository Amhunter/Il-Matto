#ifndef ILI9341_H
#define ILI9341_H

// 8-bit parallel interface I

#define TFT_PCTRL	DDRA
#define TFT_WCTRL	PORTA
#define TFT_RCTRL	PINA
#define TFT_PDATA	DDRC
#define TFT_WDATA	PORTC
#define TFT_RDATA	PINC

#define TFT_CS	(1 << 0)	// Chip-select (0)
#define TFT_BLC	(1 << 1)	// Background light control (0)
#define TFT_RST	(1 << 2)	// Reset (0)
#define TFT_WR	(1 << 3)	// Parallel data write strobe (Rising)
#define TFT_RS	(1 << 4)	// Data(1) / Command(0) selection
#define TFT_RD	(1 << 5)	// Parallel data read strobe (Rising)
#define TFT_VSY	(1 << 6)	// VSYNC
#define TFT_FMK	(1 << 7)	// Frame mark

#include <avr/io.h>
#include <util/delay.h>

class ili9341
{
public:
	enum Orientation {Landscape, Portrait, FlipLandscape, FlipPortrait, \
		BMPLandscape, BMPPortrait, BMPFlipLandscape, BMPFlipPortrait};

	static inline void init(void);
	static inline void sleep(bool e);

protected:
	static inline void cmd(uint8_t dat);
	static inline void data(uint8_t dat);
	static inline void send(bool c, uint8_t dat);
	static inline uint8_t recv(void);
	static inline void _setBGLight(bool ctrl);
	static inline void _setOrient(uint8_t o);
};

// Defined as inline to excute faster

#define LOW(b)	TFT_WCTRL &= ~(b)
#define HIGH(b)	TFT_WCTRL |= (b)
#define SEND() TFT_PDATA = 0xFF
#define RECV() { \
	TFT_PDATA = 0x00; \
	TFT_WDATA = 0xFF; \
}

inline void ili9341::sleep(bool e)
{
	send(1, e ? 0x10 : 0x11);
}

inline void ili9341::_setOrient(uint8_t o)
{
	send(1, 0x36);			// Memory Access Control
	switch (o) {
	case Landscape:
		send(0, 0x28);		// Column Address Order, BGR
		break;
	case Portrait:
		send(0, 0x48);		// Column Address Order, BGR
		break;
	case FlipLandscape:
		send(0, 0xE8);		// Column Address Order, BGR
		break;
	case FlipPortrait:
		send(0, 0x88);		// Column Address Order, BGR
		break;
	case BMPLandscape:
		send(0, 0x68);		// Column Address Order, BGR
		break;
	case BMPFlipLandscape:
		send(0, 0xA8);		// Column Address Order, BGR
		break;
	case BMPPortrait:
		send(0, 0xD8);		// Column Address Order, BGR
		break;
	case BMPFlipPortrait:
		send(0, 0x18);		// Column Address Order, BGR
		break;
	}
}

inline void ili9341::cmd(uint8_t dat)
{
	LOW(TFT_RS);
	TFT_WDATA = dat;
	LOW(TFT_WR);
	HIGH(TFT_WR);
	HIGH(TFT_RS);
}

inline void ili9341::data(uint8_t dat)
{
	TFT_WDATA = dat;
	LOW(TFT_WR);
	HIGH(TFT_WR);
}

inline void ili9341::send(bool c, uint8_t dat)
{
	//SEND();
	if (c)
		cmd(dat);
	else
		data(dat);
}

inline uint8_t ili9341::recv(void)
{
	unsigned char dat;
	RECV();
	LOW(TFT_RD);
	//_delay_us(1);
	dat = TFT_RDATA;
	HIGH(TFT_RD);
	SEND();
	return dat;
}

inline void ili9341::_setBGLight(bool ctrl)
{
	if (ctrl)
		TFT_WCTRL |= TFT_BLC;
	else
		TFT_WCTRL &= ~TFT_BLC;
}

inline void ili9341::init(void)
{
	uint8_t c;
	uint16_t r;

	MCUCR |= 0x80;			// Disable JTAG
	MCUCR |= 0x80;			// Thanks to someone point that out

	TFT_PCTRL = 0xFF & ~TFT_FMK;
	TFT_WCTRL = 0xFF & ~TFT_BLC;	// Disable background light
	RECV();
	LOW(TFT_RST);			// Hardware reset
	LOW(TFT_CS);
	HIGH(TFT_WR);
	HIGH(TFT_RD);
	HIGH(TFT_RS);
	HIGH(TFT_VSY);
	_delay_us(10);			// Min: 10us
	HIGH(TFT_RST);
	_delay_ms(120);
	SEND();
	send(1, 0x28);		// Display OFF
	send(1, 0x11);		// Sleep Out
	_delay_ms(120);
	send(1, 0x34);		// Tearing Effect Line OFF
	send(1, 0x38);		// Idle Mode OFF
	send(1, 0x13);		// Normal Display Mode ON
	send(1, 0x20);		// Display Inversion OFF
	send(1, 0x3A);		// COLMOD: Pixel Format Set
	send(0, 0x55);		// 16 bits/pixel
	send(1, 0x36);		// Memory Access Control
	send(0, 0x48);		// Column Adress Order, BGR
	send(1, 0x2C);		// Memory Write
	for (r = 0; r < 320; r++)	// Black screen
		for (c = 0; c < 240; c++) {
			send(0, 0x00);
			send(0, 0x00);
			send(0, 0x00);
		}
	send(1, 0x29);		// Display On
}

#undef LOW
#undef HIGH
#undef SEND
#undef RECV

#endif
