#include "lcd_1602.h"

/*
 * lcd_1602_start is the first function to be called and initializes the
 * functioning parameters of the LCD. 
 * As arguments, there are: 
 * 	- an address to the LCD_16x2 structure that will simbolize the LCD
 *	- the number of arguments for initializing
 * 		- 4 cases: 10 & 11 arguments for 8-bit communication
 *				    6 & 7 arguments for 4-bit communication
 * 	- the rest of the arguments
 * Depending on this number, it will call the lcd_1602_init function 
 * with different arguments
 * 
 * Example:
 * 	.. initialize parameters
 * 	lcd_1602_start(&lcd, 6, rs, en, d0, d1, d2, d3);
 *  or
 *  lcd_1602_start(&lcd, 11, rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);
 */
void lcd_1602_start(LCD_16x2* lcd_struct, int arg_count, ...)
{
	int rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7;
	va_list arg_list;
	va_start(arg_list, arg_count);
	switch (arg_count)
	{
		case 11:
			rs = va_arg(arg_list, int);
			rw = va_arg(arg_list, int);
			en = va_arg(arg_list, int);
			d0 = va_arg(arg_list, int);
			d1 = va_arg(arg_list, int);
			d2 = va_arg(arg_list, int);
			d3 = va_arg(arg_list, int);
			d4 = va_arg(arg_list, int);
			d5 = va_arg(arg_list, int);
			d6 = va_arg(arg_list, int);
			d7 = va_arg(arg_list, int);
			va_end(arg_list);
			lcd_1602_init(lcd_struct, 0, rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);
			break;
		case 10:
			rs = va_arg(arg_list, int);
			en = va_arg(arg_list, int);
			d0 = va_arg(arg_list, int);
			d1 = va_arg(arg_list, int);
			d2 = va_arg(arg_list, int);
			d3 = va_arg(arg_list, int);
			d4 = va_arg(arg_list, int);
			d5 = va_arg(arg_list, int);
			d6 = va_arg(arg_list, int);
			d7 = va_arg(arg_list, int);
			va_end(arg_list);
			lcd_1602_init(lcd_struct, 0, rs, 255, en, d0, d1, d2, d3, d4, d5, d6, d7);
			break;
		case 7:
			rs = va_arg(arg_list, int);
			rw = va_arg(arg_list, int);
			en = va_arg(arg_list, int);
			d0 = va_arg(arg_list, int);
			d1 = va_arg(arg_list, int);
			d2 = va_arg(arg_list, int);
			d3 = va_arg(arg_list, int);
			va_end(arg_list);
			lcd_1602_init(lcd_struct, 1, rs, rw, en, d0, d1, d2, d3, 0, 0, 0, 0);
			break;
		case 6:
			rs = va_arg(arg_list, int);
			en = va_arg(arg_list, int);
			d0 = va_arg(arg_list, int);
			d1 = va_arg(arg_list, int);
			d2 = va_arg(arg_list, int);
			d3 = va_arg(arg_list, int);
			va_end(arg_list);
			lcd_1602_init(lcd_struct, 1, rs, 255, en, d0, d1, d2, d3, 0, 0, 0, 0);
			break;
		default:
			{}
	}
}

/* lcd_1602_init sets the communication parameters with the LCD and usually
 * is called by lcd_1602_start.
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 *	- integer (0 or 1) for 4 or 8-bit communication (1 for 4-bit)
 *	- 11 integers representing the 11 pins of the LCD connected to the pins of the board
 *
 * Example:
 *	lcd_1602_init(lcd_struct, 0, rs, 255, en, d0, d1, d2, d3, d4, d5, d6, d7);
 */
void lcd_1602_init(LCD_16x2* lcd_struct, int four_bit_mode, uint8_t rs, uint8_t rw, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	lcd_struct->_rs_pin = rs;
	lcd_struct->_rw_pin = rw;
  	lcd_struct->_enable_pin = en;

  	lcd_struct->_data_pins[0] = d0;
	lcd_struct->_data_pins[1] = d1;
	lcd_struct->_data_pins[2] = d2;
	lcd_struct->_data_pins[3] = d3;
	lcd_struct->_data_pins[4] = d4;
	lcd_struct->_data_pins[5] = d5;
	lcd_struct->_data_pins[6] = d6;
	lcd_struct->_data_pins[7] = d7;

	if (four_bit_mode)
		lcd_struct->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	else
		lcd_struct->_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

	lcd_1602_begin(lcd_struct, 16, 1);
}

/* lcd_1602_begin established the communication with the LCD according to the HD44780
 * LCD datasheet. 
 * As arguments, there are:
 *  - the address to the LCD_16x2 structure of the LCD
 *  - the number of columns and lines of the LCD that is used
 *
 * Example:
 *  lcd_1602_begin(&lcd, 16, 2);
 */
void lcd_1602_begin(LCD_16x2* lcd_struct, uint8_t cols, uint8_t lines)
{
	if (lines > 1) {
    	lcd_struct->_displayfunction |= LCD_2LINE;
  	}

  	lcd_struct->_numlines = lines;
  	lcd_1602_set_row_offsets(lcd_struct, 0x00, 0x40, 0x00 + cols, 0x40 + cols);
  	gpio_enable_output(lcd_struct->_rs_pin);

  	if (lcd_struct->_rw_pin != 255) {
  		gpio_enable_output(lcd_struct->_rw_pin);
  	}

  	gpio_enable_output(lcd_struct->_enable_pin);

  	for (int i = 0; i < 4; i++) {
  		gpio_enable_output(lcd_struct->_data_pins[i]);
  	}

  	delay_ms(100);
  	gpio_clear(lcd_struct->_rs_pin);
  	gpio_clear(lcd_struct->_enable_pin);
  	if (lcd_struct->_rw_pin != 255)
  		gpio_clear(lcd_struct->_rw_pin);

  	if (! (lcd_struct->_displayfunction & LCD_8BITMODE)) {
  		lcd_1602_write_4_bits(lcd_struct, 0x03);
  		delay_ms(5);

  		lcd_1602_write_4_bits(lcd_struct, 0x03);
  		delay_ms(5);

  		lcd_1602_write_4_bits(lcd_struct, 0x03);
  		delay_ms(1);

  		lcd_1602_write_4_bits(lcd_struct, 0x02);
  	} else {
  		lcd_1602_command(lcd_struct, LCD_FUNCTIONSET | lcd_struct->_displayfunction);

  		delay_ms(5);

	    lcd_1602_command(lcd_struct, LCD_FUNCTIONSET | lcd_struct->_displayfunction);
	    delay_ms(1);

	    lcd_1602_command(lcd_struct, LCD_FUNCTIONSET | lcd_struct->_displayfunction);
  	}

  	lcd_1602_command(lcd_struct, LCD_FUNCTIONSET | lcd_struct->_displayfunction);  

	lcd_struct->_displaycontrol = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF;  
	lcd_1602_display(lcd_struct);

	lcd_1602_clear(lcd_struct);

	lcd_struct->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	lcd_1602_command(lcd_struct, LCD_ENTRYMODESET | lcd_struct->_displaymode);
}

/* lcd_1602_clear clears the display and sets the cursor posiziton to zero
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_clear(&lcd);
 */
void lcd_1602_clear(LCD_16x2* lcd_struct)
{
	lcd_1602_command(lcd_struct, LCD_CLEARDISPLAY); 
    delay_ms(2);
}

/* lcd_1602_home, as lcd_1602_clear, clears the display and sets the cursor posiziton to zero
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_home(&lcd);
 */
void lcd_1602_home(LCD_16x2* lcd_struct)
{
	lcd_1602_command(lcd_struct, LCD_RETURNHOME);
    delay_ms(2);
}

/* lcd_1602_no_display turns the display off very quickly
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_no_display(&lcd);
 */
void lcd_1602_no_display(LCD_16x2* lcd_struct)
{
	lcd_struct->_displaycontrol &= ~LCD_DISPLAYON;
	lcd_1602_command(lcd_struct, LCD_DISPLAYCONTROL | lcd_struct->_displaycontrol);
}
/* lcd_1602_display turns the display on very quickly
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_no_display(&lcd);
 */
void lcd_1602_display(LCD_16x2* lcd_struct)
{
	lcd_struct->_displaycontrol |= LCD_DISPLAYON;
    lcd_1602_command(lcd_struct, LCD_DISPLAYCONTROL | lcd_struct->_displaycontrol);
}

/* lcd_1602_no_blink turns off the blinking cursor display
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_no_blink(&lcd);
 */
void lcd_1602_no_blink(LCD_16x2* lcd_struct)
{
  	lcd_struct->_displaycontrol &= ~LCD_BLINKON;
  	lcd_1602_command(lcd_struct, LCD_DISPLAYCONTROL | lcd_struct->_displaycontrol);
}
/* lcd_1602_blink turns on the blinking cursor display
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_blink(&lcd);
 */
void lcd_1602_blink(LCD_16x2* lcd_struct)
{
    lcd_struct->_displaycontrol |= LCD_BLINKON;
  	lcd_1602_command(lcd_struct, LCD_DISPLAYCONTROL | lcd_struct->_displaycontrol);
}

/* lcd_1602_no_cursor turns off the underline cursor
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_no_cursor(&lcd);
 */
void lcd_1602_no_cursor(LCD_16x2* lcd_struct)
{
    lcd_struct->_displaycontrol &= ~LCD_CURSORON;
    lcd_1602_command(lcd_struct, LCD_DISPLAYCONTROL | lcd_struct->_displaycontrol);
}
/* lcd_1602_cursor turns on the underline cursor
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_cursor(&lcd);
 */
void lcd_1602_cursor(LCD_16x2* lcd_struct)
{
    lcd_struct->_displaycontrol |= LCD_CURSORON;
    lcd_1602_command(lcd_struct, LCD_DISPLAYCONTROL | lcd_struct->_displaycontrol);
}

/* lcd_1602_scroll_display_left scrolls the display to the left without
 * changing the RAM
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_scroll_display_left(&lcd);
 */
void lcd_1602_scroll_display_left(LCD_16x2* lcd_struct)
{
	lcd_1602_command(lcd_struct, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
/* lcd_1602_scroll_display_right scrolls the display to the right without
 * changing the RAM
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_scroll_display_right(&lcd);
 */
void lcd_1602_scroll_display_right(LCD_16x2* lcd_struct)
{
	lcd_1602_command(lcd_struct, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/* lcd_1602_left_to_right should flow the text from left to right on the display
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_left_to_right(&lcd);
 */
void lcd_1602_left_to_right(LCD_16x2* lcd_struct)
{
    lcd_struct->_displaymode |= LCD_ENTRYLEFT;
    lcd_1602_command(lcd_struct, LCD_ENTRYMODESET | lcd_struct->_displaymode);
}
/* lcd_1602_right_to_left should flow the text from right to left on the display
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example:
 * 	lcd_1602_right_to_left(&lcd);
 */
void lcd_1602_right_to_left(LCD_16x2* lcd_struct)
{
    lcd_struct->_displaymode &= ~LCD_ENTRYLEFT;
    lcd_1602_command(lcd_struct, LCD_ENTRYMODESET | lcd_struct->_displaymode);
}

/* lcd_1602_autoscroll will 'right justify' the text from the cursor
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *	lcd_1602_autoscroll(&lcd);
 */
void lcd_1602_autoscroll(LCD_16x2* lcd_struct)
{
    lcd_struct->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
    lcd_1602_command(lcd_struct, LCD_ENTRYMODESET | lcd_struct->_displaymode);
}
/* lcd_1602_no_autoscroll will 'left justify' the text from the cursor
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 *
 * Example:
 *	lcd_1602_no_autoscroll(&lcd);
 */
void lcd_1602_no_autoscroll(LCD_16x2* lcd_struct)
{
    lcd_struct->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    lcd_1602_command(lcd_struct, LCD_ENTRYMODESET | lcd_struct->_displaymode);
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
uint8_t lcd_1602_print_string(LCD_16x2* lcd_struct, char* string)
{
	char buff[128];
	uint8_t len = strlen(string);
	uint8_t n = 0;
	while(n < len) {
		uint8_t to_write = get_min(sizeof(buff), len - n);
		memcpy(buff, string, to_write);
		int8_t written = lcd_1602_write(lcd_struct, (uint8_t *) buff, to_write);
		n += written;
		string += written;
		if (written < 0)
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
uint8_t lcd_1602_print_number(LCD_16x2* lcd_struct, uint32_t value)
{
	char string[16];
	snprintf(string, 16, "%u", (unsigned int) value);
	return lcd_1602_print_string(lcd_struct, string);
}

void lcd_1602_set_row_offsets(LCD_16x2* lcd_struct, uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3)
{
	lcd_struct->_row_offsets[0] = row0;
  	lcd_struct->_row_offsets[1] = row1;
 	lcd_struct->_row_offsets[2] = row2;
 	lcd_struct->_row_offsets[3] = row3;
}

/* lcd_1602_set_cursor will set the cursor position to the row and col given as
 * arguments
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 *	- the column
 * 	- the line
 *
 * Example:
 *	lcd_1602_set_cursor(&lcd, 5, 1);
 */
void lcd_1602_set_cursor(LCD_16x2* lcd_struct, uint8_t col, uint8_t row)
{
	const size_t max_lines = sizeof(lcd_struct->_row_offsets) / sizeof(*(lcd_struct->_row_offsets));
	if ( row >= max_lines ) {
		row = max_lines - 1;    // we count rows starting w/0
	}
	if ( row >= lcd_struct->_numlines ) {
	  	row = lcd_struct->_numlines - 1;    // we count rows starting w/0
	}
	  
	lcd_1602_command(lcd_struct, LCD_SETDDRAMADDR | (col + lcd_struct->_row_offsets[row]));
}

/*********** mid level commands, for sending data/cmds */

/* lcd_1602_write will write byte by byte the buffer given as an argument
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 *	- the buffer from which it will be written on the display
 * 	- the number of bytes to be written 
 *
 * Example:
 *	lcd_1602_write(&lcd, buffer, 10);
 */
uint8_t lcd_1602_write(LCD_16x2* lcd_struct, uint8_t* buffer, uint8_t size)
{
	uint8_t n = 0;
	while(size--) {
		uint8_t ret = lcd_1602_send_ret(lcd_struct, buffer++, HIGH);
		if (ret == 0)
			break;
		n += ret;
	}
	return n;
}

/* lcd_1602_command will call the lcd_1602_send function with the command to be
 * sent to the LCD (given as an argument) and the LOW value
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 *	- the command to send to the LCD
 *
 * Example:
 *	lcd_1602_command(&lcd, LCD_FUNCTIONSET | lcd->_displayfunction);
 */
void lcd_1602_command(LCD_16x2* lcd_struct, uint8_t command)
{
	lcd_1602_send(lcd_struct, command, LOW);
}

/************ low level data pushing commands **********/

/* lcd_1602_send_ret will just call the lcd_1602_send function with the arguments given
 * and will return 1
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 * 	- the value (LOW/HIGH) for the command to be sent
 *	- the command to send to the LCD
 *
 * Example:
 *	lcd_1602_send_ret(&lcd, LCD_FUNCTIONSET | lcd->_displayfunction, HIGH);
 */
uint8_t lcd_1602_send_ret(LCD_16x2* lcd_struct, uint8_t* value, uint8_t command)
{
	lcd_1602_send(lcd_struct, *value, command);
	return 1;
}

/* lcd_1602_send will write commands or data, with automatic 4/8-bit selection
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 * 	- the value (LOW/HIGH) for the command to be sent
 *	- the command to send to the LCD
 *
 * Example:
 *	lcd_1602_send(&lcd, LCD_FUNCTIONSET | lcd->_displayfunction, LOW);
 */
void lcd_1602_send(LCD_16x2* lcd_struct, uint8_t value, uint8_t command)
{
	if (command == LOW)
		gpio_clear(lcd_struct->_rs_pin);
	else gpio_set(lcd_struct->_rs_pin);

	if (lcd_struct->_rw_pin != 255)
		gpio_clear(lcd_struct->_rw_pin);

	if (lcd_struct->_displayfunction & LCD_8BITMODE) {
    	lcd_1602_write_8_bits(lcd_struct, value); 
 	} else {
    	lcd_1602_write_4_bits(lcd_struct, value>>4);
    	lcd_1602_write_4_bits(lcd_struct, value);
  }
}

/* lcd_1602_write_4_bits will be called to actually write the data to the LCD in the
 * 4-bit communication
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 * 	- the data to be written
 *
 * Example:
 *	lcd_1602_write_4_bits(&lcd, 26);
 */
void lcd_1602_write_4_bits(LCD_16x2* lcd_struct, uint8_t data)
{
	for (int i = 0; i < 4; i++) {
		if ((data >> i) & 0x01) {
			gpio_set(lcd_struct->_data_pins[i]);
		} else {
			gpio_clear(lcd_struct->_data_pins[i]);
		}
	}

	lcd_1602_pulse_enable(lcd_struct);
}

/* lcd_1602_write_*_bits will be called to actually write the data to the LCD in the
 * 8-bit communication
 * As arguments, there are:
 *	- the address to the LCD_16x2 structure of the LCD
 * 	- the data to be written
 *
 * Example:
 *	lcd_1602_write_8_bits(&lcd, 26);
 */
void lcd_1602_write_8_bits(LCD_16x2* lcd_struct, uint8_t data)
{
	for (int i = 0; i < 8; i++) {
		if ((data >> i) & 0x01) {
			gpio_set(lcd_struct->_data_pins[i]);
		} else {
			gpio_clear(lcd_struct->_data_pins[i]);
		}
	}

	lcd_1602_pulse_enable(lcd_struct);
}

/* lcd_1602_pulse_enable will be called after every write operation and will behave
 * accrording to the HD44780 datasheet, figure 26
 * As arguments, there is the address to the LCD_16x2 structure of the LCD
 * 
 * Example: 
 *	lcd_1602_pulse_enable(&lcd);
 */
void lcd_1602_pulse_enable(LCD_16x2* lcd_struct)
{
	gpio_clear(lcd_struct->_enable_pin);
	delay_ms(1);
	gpio_set(lcd_struct->_enable_pin);
	delay_ms(1);
	gpio_clear(lcd_struct->_enable_pin);
	delay_ms(1);
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