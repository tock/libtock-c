#include "gpio.h"
#include "hd44780.h"

#define ALLOW_BAD_VALUE 200

/*
 * hd44780_start is the first function to be called and initializes the
 * functioning parameters and communication parameters of the LCD, according
 * to its datasheet (HD44780).
 *
 * Example:
 *  hd44780_begin(void);
 *
 */
void hd44780_begin(void)
{
  int ret = command(DRIVER_HD44780_NUM, 0, 16, 1);
  if (ret)
    return;
  ret = command(DRIVER_HD44780_NUM, 0, 16, 2);
  if (ret)
    return;
}

/* hd44780_clear clears the display and sets the cursor posiziton to zero
 *
 * Example:
 *  hd44780_clear(void);
 */
void hd44780_clear(void)
{
  int ret = command(DRIVER_HD44780_NUM, 3, 0, 0);
  if (ret)
    return;
}

/* hd44780_home, as hd44780_clear, clears the display and sets the cursor posiziton to zero
 *
 * Example:
 *  hd44780_home(void);
 */
void hd44780_home(void)
{
  int ret = command(DRIVER_HD44780_NUM, 2, 0, 0);
  if (ret)
    return;
}

/* hd44780_no_display turns the display off very quickly
 *
 * Example:
 *  hd44780_no_display(void);
 */
void hd44780_no_display(void)
{
  int ret = command(DRIVER_HD44780_NUM, 7, 1, 0);
  if (ret)
    return;
}
/* hd44780_display turns the display on very quickly
 *
 * Example:
 *  hd44780_no_display(void);
 */
void hd44780_display(void)
{
  int ret = command(DRIVER_HD44780_NUM, 7, 0, 0);
  if (ret)
    return;
}

/* hd44780_no_blink turns off the blinking cursor display
 *
 * Example:
 *  hd44780_no_blink(void);
 */
void hd44780_no_blink(void)
{
  int ret = command(DRIVER_HD44780_NUM, 8, 1, 0);
  if (ret)
    return;
}
/* hd44780_blink turns on the blinking cursor display
 *
 * Example:
 *  hd44780_blink(void);
 */
void hd44780_blink(void)
{
  int ret = command(DRIVER_HD44780_NUM, 8, 0, 0);
  if (ret)
    return;
}

/* hd44780_no_cursor turns off the underline cursor
 *
 * Example:
 *  hd44780_no_cursor(void);
 */
void hd44780_no_cursor(void)
{
  int ret = command(DRIVER_HD44780_NUM, 6, 1, 0);
  if (ret)
    return;
}
/* hd44780_cursor turns on the underline cursor
 *
 * Example:
 *  hd44780_cursor(void);
 */
void hd44780_cursor(void)
{
  int ret = command(DRIVER_HD44780_NUM, 6, 0, 0);
  if (ret)
    return;
}

/* hd44780_scroll_display_left scrolls the display to the left without
 * changing the RAM
 *
 * Example:
 *  hd44780_scroll_display_left(void);
 */
void hd44780_scroll_display_left(void)
{
  int ret = command(DRIVER_HD44780_NUM, 9, 0, 0);
  if (ret)
    return;
}
/* hd44780_scroll_display_right scrolls the display to the right without
 * changing the RAM
 *
 * Example:
 *  hd44780_scroll_display_right(void);
 */
void hd44780_scroll_display_right(void)
{
  int ret = command(DRIVER_HD44780_NUM, 9, 1, 0);
  if (ret)
    return;
}

/* hd44780_left_to_right should flow the text from left to right on the display
 *
 * Example:
 *  hd44780_left_to_right(void);
 */
void hd44780_left_to_right(void)
{
  int ret = command(DRIVER_HD44780_NUM, 4, 0, 0);
  if (ret)
    return;
}
/* hd44780_right_to_left should flow the text from right to left on the display
 *
 * Example:
 *  hd44780_right_to_left(void);
 */
void hd44780_right_to_left(void)
{
  int ret = command(DRIVER_HD44780_NUM, 4, 1, 0);
  if (ret)
    return;
}

/* hd44780_autoscroll will 'right justify' the text from the cursor
 *
 * Example:
 *	hd44780_autoscroll(void);
 */
void hd44780_autoscroll(void)
{
  int ret = command(DRIVER_HD44780_NUM, 5, 0, 0);
  if (ret)
    return;
}
/* hd44780_no_autoscroll will 'left justify' the text from the cursor
 *
 * Example:
 *	hd44780_no_autoscroll(void);
 */
void hd44780_no_autoscroll(void)
{
  int ret = command(DRIVER_HD44780_NUM, 5, 1, 0);
  if (ret)
    return;
}

/* hd44780_print_full_string will do some checks for the number of bytes to
 * be sent and will try to send to the kernel all the bytes in the buffer.
 * When the syscall returns a certain value (ALLOW_BAD_VALUE) it means that the
 * command buffer in the kernel is full and a delay is needed in order for the
 * buffer to be emptied (the commands already saved to be actually executed).
 * As argument, there is the string to be printed.
 *
 * Example:
 *	hd44780_print_full_string("Here I am!");
 */
uint8_t hd44780_print_full_string(char* string)
{
  char buff[128];
  uint8_t len = strlen(string);
  uint8_t n   = 0;
  while (n < len) {
    uint8_t to_write = get_min(sizeof(buff), len - n);
    memcpy(buff, string, to_write);
    int ret = allow(DRIVER_HD44780_NUM, 0, (void* )buff, to_write);
    if (ret == ALLOW_BAD_VALUE) {
      while (ret == ALLOW_BAD_VALUE) {
        delay_ms(1000);
        ret = allow(DRIVER_HD44780_NUM, 0, (void* )buff, to_write);
      }
    }
    n      += ret;
    string += ret;
  }
  return n;
}

/* hd44780_print_string will do some checks for the number of bytes to be sent
 * and will try to send to the kernel all the bytes in the buffer.
 * When the syscall returns a certain value (ALLOW_BAD_VALUE) it stops.
 * As argument, there is the string to be printed.
 *
 * Example:
 *	hd44780_print_string("Here I am!");
 */
uint8_t hd44780_print_string(char* string)
{
  char buff[128];
  uint8_t len = strlen(string);
  uint8_t n   = 0;
  while (n < len) {
    uint8_t to_write = get_min(sizeof(buff), len - n);
    memcpy(buff, string, to_write);
    int ret = allow(DRIVER_HD44780_NUM, 0, (void* )buff, to_write);
    if (!ret || ret == ALLOW_BAD_VALUE) return n;
    n      += ret;
    string += ret;
  }
  return n;
}

/* hd44780_set_cursor will set the cursor position to the row and col given as
 * arguments
 * As arguments, there are:
 *	- the column
 *  - the line
 *
 * Example:
 *	hd44780_set_cursor(5, 1);
 */

void hd44780_set_cursor(uint8_t col, uint8_t row)
{
  int ret = command(DRIVER_HD44780_NUM, 1, col, row);
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
