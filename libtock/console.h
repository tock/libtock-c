#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_CONSOLE 0x1

int putstr(const char* str);
int putnstr(const char* str, size_t len);
int putnstr_async(const char* str, size_t len, subscribe_cb cb, void* userdata);

int getnstr(char *str, size_t len);
int getnstr_async(char *str, size_t len, subscribe_cb cb, void* userdata);

/* Returns TOCK_FAIL on failure, or else the character received */
int getch(void);

// Abort an ongoing receive call.
int getnstr_abort(void);

void console_override_default_uart(uint8_t new_uart);

#ifdef __cplusplus
}
#endif
