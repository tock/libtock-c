/*
	hd44780.h - C implementation of the LiquidCrystal class, providing basic
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

#define DRIVER_HD44780_NUM 0x80005

void hd44780_begin(void);

void hd44780_clear(void);
void hd44780_home(void);

void hd44780_no_display(void);
void hd44780_display(void);
void hd44780_no_blink(void);
void hd44780_blink(void);
void hd44780_no_cursor(void);
void hd44780_cursor(void);

void hd44780_scroll_display_left(void);
void hd44780_scroll_display_right(void);
void hd44780_left_to_right(void);
void hd44780_right_to_left(void);
void hd44780_autoscroll(void);
void hd44780_no_autoscroll(void);
void hd44780_set_cursor(uint8_t line, uint8_t position);

uint8_t hd44780_print_string(char* str);
uint8_t hd44780_print_full_string(char* str);

uint32_t get_min(uint32_t a, uint32_t b);

#ifdef __cplusplus
}
#endif
