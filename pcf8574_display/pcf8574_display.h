#ifndef pcf8574_display_h

#define pcf8574_display_h

#ifndef F_CPU          		// if F_CPU was not defined in Project -> Properties
#define F_CPU 16000000UL    // define it now as 16 MHz unsigned long
#endif

#include <util/delay.h>

#include "../pcf8574/pcf8574.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

// LCD display lines
#define LCD_EN_LINE 0x04
#define LCD_RW_LINE 0x02
#define LCD_RS_LINE 0x01

#define PCF8574A_MODULE_ADDR_W 0x7E		// bits 1-3 of address depends of operation, bits 4-7 are 0111 for 8547A and bit 0 is 0 for write and 1 for read operation

void pcf8574_display_init(uint8_t _digits_on_line, uint8_t _ten_dot_diget, uint8_t _lines_on_display);
void pcf8574_display_send_data(uint8_t _data);
void pcf8574_display_send_command(uint8_t _cmd);
uint8_t pcf8574_display_init_4bit();
uint8_t pcf8574_display_send(uint8_t _data);
uint8_t pcf8574_display_write_byte(uint8_t _data);
uint8_t pcf8574_display_pulse(uint8_t _data);
void pcf8574_display_clear();
void pcf8574_display_return();
void pcf8574_display_set_cursor(uint8_t _line_no, uint8_t _pos_in_line);

#endif