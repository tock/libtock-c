/*
	lcd_1602.h - C implementation of the LiquidCrystal class, providing basic
	API for communicating with the 16x2 HD44780 LCD; 

	The code was developed based on the basic Arduino Library for the LCD: 
		https://github.com/arduino-libraries/LiquidCrystal.git
*/
#pragma once

#include <stdarg.h>
#include <string.h>
#include <timer.h>

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

#define DRIVER_LCD_NUM 0x00008

void lcd_1602_begin();

void lcd_1602_clear();
void lcd_1602_home();

void lcd_1602_no_display();
void lcd_1602_display();
void lcd_1602_no_blink();
void lcd_1602_blink();
void lcd_1602_no_cursor();
void lcd_1602_cursor();

void lcd_1602_scroll_display_left();
void lcd_1602_scroll_display_right();
void lcd_1602_left_to_right();
void lcd_1602_right_to_left();
void lcd_1602_autoscroll();
void lcd_1602_no_autoscroll();
void lcd_1602_set_cursor(uint8_t line, uint8_t position);

uint8_t lcd_1602_print_string(char* str);
uint8_t lcd_1602_print_number(uint32_t value);

uint32_t get_min(uint32_t a, uint32_t b);

#ifdef __cplusplus
}
#endif
