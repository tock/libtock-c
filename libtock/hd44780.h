/*
	hd44780.h - C implementation for hd44780 capsule
*/
#pragma once

#include <stdarg.h>
#include <string.h>
#include <timer.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_HD44780_NUM 0x80005

int hd44780_begin(void);

int hd44780_clear(void);
int hd44780_home(void);

int hd44780_no_display(void);
int hd44780_display(void);
int hd44780_no_blink(void);
int hd44780_blink(void);
int hd44780_no_cursor(void);
int hd44780_cursor(void);

int hd44780_scroll_display_left(void);
int hd44780_scroll_display_right(void);
int hd44780_left_to_right(void);
int hd44780_right_to_left(void);
int hd44780_autoscroll(void);
int hd44780_no_autoscroll(void);
int hd44780_set_cursor(uint8_t line, uint8_t position);

uint8_t hd44780_print_string(char* str);
uint8_t hd44780_print_full_string(char* str);

#ifdef __cplusplus
}
#endif
