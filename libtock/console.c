#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "console.h"
#include <uart.h>


static uint8_t uart_num = 0;

int putstr(const char* str){
  return uart_putstr(uart_num, str);
}
int putnstr(const char* str, size_t len){
  return uart_putnstr(uart_num, str, len);
}
int putnstr_async(const char* str, size_t len, subscribe_cb cb, void* userdata){
  return uart_putnstr_async(uart_num, str, len, cb, userdata);
}

int getnstr(char *str, size_t len){
  return uart_getnstr(uart_num, str, len);
}
int getnstr_async(char *str, size_t len, subscribe_cb cb, void* userdata){
  return uart_getnstr_async(uart_num, str, len, cb, userdata);
}

/* Returns TOCK_FAIL on failure, or else the character received */
int getch(void){
  return uart_getch(uart_num);
}

// Abort an ongoing receive call.
int getnstr_abort(void){
  return uart_getnstr_abort(uart_num);
}

void console_override_default_uart(uint8_t new_uart){
  uart_num = new_uart;
}