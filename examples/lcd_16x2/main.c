/*
    Example for lcd_1602 library
    Tested on NUCLEO-F429ZI
 */

#include <lcd_1602.h>
#include <string.h>
#include <timer.h>

int rs = 7;
int en = 5;
int d0 = 4;
int d1 = 3;
int d2 = 2;
int d3 = 1;

int main(void) {
  char str[12] = "Hello world";

  LCD_16x2 lcd;
  lcd_1602_start(&lcd, 6, rs, en, d0, d1, d2, d3);
  lcd_1602_begin(&lcd, 16, 2);

  lcd_1602_print_string(&lcd, str);
  lcd_1602_set_cursor(&lcd, 0, 1);
  lcd_1602_print_number(&lcd, 123456);

  delay_ms(2000);

  while (1) {
    //
  }
  return 0;
}