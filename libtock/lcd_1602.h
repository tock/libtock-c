/*
	lcd_1602.h - C implementation of the LiquidCrystal class, providing basic
	API for communicating with the 16x2 HD44780 LCD; 

	The code was developed based on the basic Arduino Library for the LCD: 
		https://github.com/arduino-libraries/LiquidCrystal.git
*/
#pragma once

#include <timer.h>
#include <string.h>
#include <stdarg.h>
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOW 0
#define HIGH 1

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

typedef struct {

	uint8_t _rs_pin; // LOW: command.  HIGH: character.
	uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
	uint8_t _enable_pin; // activated by a HIGH pulse.
	uint8_t _data_pins[8];

	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;

	uint8_t _initialized;

	uint8_t _numlines;
	uint8_t _row_offsets[4];
} LCD_16x2;

// intializing functions
void lcd_1602_start(LCD_16x2* lcd_struct, int arg_count, ...);
void lcd_1602_init(LCD_16x2* lcd_struct, int four_bit_mode, uint8_t rs, uint8_t rw, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
void lcd_1602_begin(LCD_16x2* lcd_struct, uint8_t cols, uint8_t lines);

void lcd_1602_clear(LCD_16x2* lcd_struct);
void lcd_1602_home(LCD_16x2* lcd_struct);

void lcd_1602_no_display(LCD_16x2* lcd_struct);
void lcd_1602_display(LCD_16x2* lcd_struct);
void lcd_1602_no_blink(LCD_16x2* lcd_struct);
void lcd_1602_blink(LCD_16x2* lcd_struct);
void lcd_1602_no_cursor(LCD_16x2* lcd_struct);
void lcd_1602_cursor(LCD_16x2* lcd_struct);

void lcd_1602_scroll_display_left(LCD_16x2* lcd_struct);
void lcd_1602_scroll_display_right(LCD_16x2* lcd_struct);
void lcd_1602_left_to_right(LCD_16x2* lcd_struct);
void lcd_1602_right_to_left(LCD_16x2* lcd_struct);
void lcd_1602_autoscroll(LCD_16x2* lcd_struct);
void lcd_1602_no_autoscroll(LCD_16x2* lcd_struct);

uint8_t lcd_1602_print_string(LCD_16x2* lcd_struct, char* str);
uint8_t lcd_1602_print_number(LCD_16x2* lcd_struct, uint32_t value);

void lcd_1602_set_row_offsets(LCD_16x2* lcd_struct, uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3);
void lcd_1602_set_cursor(LCD_16x2* lcd_struct, uint8_t line, uint8_t position);
uint8_t lcd_1602_write(LCD_16x2* lcd_struct, uint8_t* buffer, uint8_t size);
void lcd_1602_command(LCD_16x2* lcd_struct, uint8_t command);

uint8_t lcd_1602_send_ret(LCD_16x2* lcd_struct, uint8_t* value, uint8_t command);
void lcd_1602_send(LCD_16x2* lcd_struct, uint8_t value, uint8_t command);
void lcd_1602_write_4_bits(LCD_16x2* lcd_struct, uint8_t data);
void lcd_1602_write_8_bits(LCD_16x2* lcd_struct, uint8_t data);
void lcd_1602_pulse_enable(LCD_16x2* lcd_struct);

uint32_t get_min(uint32_t a, uint32_t b);

#ifdef __cplusplus
}
#endif
