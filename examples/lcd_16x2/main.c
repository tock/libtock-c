/*
    Example for lcd_1602 library
    Tested on NUCLEO-F429ZI
 */

#include "lcd_1602.h"
#include <string.h>
#include <timer.h>


int main(void) {
  char str[14] = "Hello, world!";
  lcd_1602_begin();

  lcd_1602_print_string(str);
  lcd_1602_set_cursor(0, 1);
  lcd_1602_print_number(2020);

  while (1) {
    //
  }
  return 0;
}