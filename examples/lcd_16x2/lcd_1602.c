#include "gpio.h"
#include "lcd_1602.h"

/*
 * lcd_1602_start is the first function to be called and initializes the
 * functioning parameters of the LCD.
 * As arguments, there are:
 *  - an address to the LCD_16x2 structure that will simbolize the LCD
 *	- the number of arguments for initializing
 *    - 4 cases: 10 & 11 arguments for 8-bit communication
 *				    6 & 7 arguments for 4-bit communication
 *  - the rest of the arguments
 * Depending on this number, it will call the lcd_1602_init function
 * with different arguments
 *
 * Example:
 *  .. initialize parameters
 *  lcd_1602_start(&lcd, 6, rs, en, d0, d1, d2, d3);
 *  or
 *  lcd_1602_start(&lcd, 11, rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);
 */
void lcd_1602_begin()
{
  int ret = command(DRIVER_LCD_NUM, 0, 16, 1);
  if (ret)
    return;
  ret = command(DRIVER_LCD_NUM, 0, 16, 2);
  if (ret)
    return;
}

/* lcd_1602_clear clears the display and sets the cursor posiziton to zero
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_clear(&lcd);
 */
void lcd_1602_clear()
{
  int ret = command(DRIVER_LCD_NUM, 3, 0, 0);
  if (ret)
    return;
}

/* lcd_1602_home, as lcd_1602_clear, clears the display and sets the cursor posiziton to zero
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_home(&lcd);
 */
void lcd_1602_home()
{
  int ret = command(DRIVER_LCD_NUM, 2, 0, 0);
  if (ret)
    return;
}

/* lcd_1602_no_display turns the display off very quickly
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_no_display(&lcd);
 */
void lcd_1602_no_display()
{
  int ret = command(DRIVER_LCD_NUM, 7, 1, 0);
  if (ret)
    return;
}
/* lcd_1602_display turns the display on very quickly
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_no_display(&lcd);
 */
void lcd_1602_display()
{
  int ret = command(DRIVER_LCD_NUM, 7, 0, 0);
  if (ret)
    return;
}

/* lcd_1602_no_blink turns off the blinking cursor display
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_no_blink(&lcd);
 */
void lcd_1602_no_blink()
{
  int ret = command(DRIVER_LCD_NUM, 8, 1, 0);
  if (ret)
    return;
}
/* lcd_1602_blink turns on the blinking cursor display
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_blink(&lcd);
 */
void lcd_1602_blink()
{
  int ret = command(DRIVER_LCD_NUM, 8, 0, 0);
  if (ret)
    return;
}

/* lcd_1602_no_cursor turns off the underline cursor
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_no_cursor(&lcd);
 */
void lcd_1602_no_cursor() 
{
  int ret = command(DRIVER_LCD_NUM, 6, 1, 0);
  if (ret)
    return;
}
/* lcd_1602_cursor turns on the underline cursor
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_cursor(&lcd);
 */
void lcd_1602_cursor()
{
  int ret = command(DRIVER_LCD_NUM, 6, 0, 0);
  if (ret)
    return;
}

/* lcd_1602_scroll_display_left scrolls the display to the left without
 * changing the RAM
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_scroll_display_left(&lcd);
 */
void lcd_1602_scroll_display_left()
{
  int ret = command(DRIVER_LCD_NUM, 9, 0, 0);
  if (ret)
    return;
}
/* lcd_1602_scroll_display_right scrolls the display to the right without
 * changing the RAM
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_scroll_display_right(&lcd);
 */
void lcd_1602_scroll_display_right()
{
  int ret = command(DRIVER_LCD_NUM, 9, 1, 0);
  if (ret)
    return;
}

/* lcd_1602_left_to_right should flow the text from left to right on the display
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_left_to_right(&lcd);
 */
void lcd_1602_left_to_right()
{
  int ret = command(DRIVER_LCD_NUM, 4, 0, 0);
  if (ret)
    return;
}
/* lcd_1602_right_to_left should flow the text from right to left on the display
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *  lcd_1602_right_to_left(&lcd);
 */
void lcd_1602_right_to_left()
{
  int ret = command(DRIVER_LCD_NUM, 4, 1, 0);
  if (ret)
    return;
}

/* lcd_1602_autoscroll will 'right justify' the text from the cursor
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *	lcd_1602_autoscroll(&lcd);
 */
void lcd_1602_autoscroll()
{
  int ret = command(DRIVER_LCD_NUM, 5, 0, 0);
  if (ret)
    return;
}
/* lcd_1602_no_autoscroll will 'left justify' the text from the cursor
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *	lcd_1602_no_autoscroll(&lcd);
 */
void lcd_1602_no_autoscroll()
{
  int ret = command(DRIVER_LCD_NUM, 5, 1, 0);
  if (ret)
    return;
}

/* lcd_1602_print_string will do some checks for the number of bytes to be sent
 * and will call the lcd_1602_write function with a correct number of bytes to be sent
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 *	- the string to be printed
 *
 * Example:
 *	lcd_1602_print_string(&lcd, "Here I am!");
 */
uint8_t lcd_1602_print_string(char* string)
{
  char buff[128];
  uint8_t len = strlen(string);
  uint8_t n   = 0;
  while (n < len) {
    uint8_t to_write = get_min(sizeof(buff), len - n);
    memcpy(buff, string, to_write);
    int ret = allow(DRIVER_LCD_NUM, 1, (void* )buff, to_write);
    if (ret)
      return 0;
    n      += to_write;
    string += to_write;
    if (to_write < 0)
      break;
  }
  return n;
}

/* lcd_1602_print_number will transform the number given as an argument to a
 * char[], to be printed as a string (1 decimal/slot)
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 *	- the number to be printed
 *
 * Example:
 *	lcd_1602_print_number(&lcd, 123456789);
 */
uint8_t lcd_1602_print_number(uint32_t value)
{
  char string[16];
  snprintf(string, 16, "%u", (unsigned int) value);
  return lcd_1602_print_string(string);
}

/* lcd_1602_set_cursor will set the cursor position to the row and col given as
 * arguments
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 *	- the column
 *  - the line
 *
 * Example:
 *	lcd_1602_set_cursor(&lcd, 5, 1);
 */

void lcd_1602_set_cursor(uint8_t col, uint8_t row)
{
  int ret = command(DRIVER_LCD_NUM, 1, col, row);
  if (ret)
    return;
}

/* get_min returns the lower number from the 2 arguments given
 *
 */
uint32_t get_min(uint32_t a, uint32_t b)
{
  if (a > b)
    return b;
  else return a;
}
