/*
    Example for hd44780 library
    Tested on NUCLEO-F429ZI
 */

#include <hd44780.h>
#include <string.h>
#include <timer.h>


int main(void) {
  char string[3];
  int ret, to_write;
  ret = hd44780_begin();
  ret = hd44780_set_cursor(0, 0);

  for (int i = 0; i < 200; i++) {
    snprintf(string, 16, "%u", (unsigned int) i);
    ret = hd44780_print_string(string);
    if (i > 99) {
      to_write = 3;
    } else if (i > 9) {
      to_write = 2;
    } else to_write = 1;
    while (ret != to_write) {
      delay_ms(500);
      int current = hd44780_print_string(&string[ret]);
      ret += current;
    }
  }
  return 0;
}