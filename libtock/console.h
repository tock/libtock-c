#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_CONSOLE 0x1

int putnstr(const char* str, size_t len);

int getnstr(char *str, size_t len);

/* Returns TOCK_FAIL on failure, or else the character received */
int getch(void);

#ifdef __cplusplus
}
#endif
