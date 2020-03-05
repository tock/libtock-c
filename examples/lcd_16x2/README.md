LCD 16x2 HD44780 App
======================================

An example application that demonstrates how the library for the 16x2 HD44780 LCD 
(lcd_1602.h) works and can be used in a bigger app.

The application does the following:
  * Configures the communication parameters by calling lcd_1602_lcd()
  * Begins the communication with the LCD according to the HD44780U datasheet, Figure 24
    by calling lcd_1602_begin()
  * Prints the string to the LCD on the first line by calling lcd_1602_print_string
  * Sets the cursor on the second line by calling lcd_1602_set_cursor
  * Prints a number (2020) to the LCD on the second line by calling lcd_1602_print_number

Datasheet for the LCD: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf 

Supported Boards
-----------------
- It should support any board that has GPIO support. The communication is being done 
only through GPIO pins. 

Tested Boards
-----------------
- NUCLEO-F429ZI
