#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_CONSOLE 0x1

int uart_putstr(uint8_t uart_num, const char* str);
int uart_putnstr(uint8_t uart_num, const char* str, size_t len);
int uart_putnstr_async(uint8_t uart_num, const char* str, size_t len, subscribe_cb cb, void* userdata);

int uart_getnstr(uint8_t uart_num, char *str, size_t len);
int uart_getnstr_async(uint8_t uart_num, char *str, size_t len, subscribe_cb cb, void* userdata);

/* Returns TOCK_FAIL on failure, or else the character received */
int uart_getch(uint8_t uart_num);

// Abort an ongoing receive call.
int uart_getnstr_abort(uint8_t uart_num);

#ifdef __cplusplus
}
#endif
