#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

// app binaries
extern uint8_t tock_hello[8192];
extern uint8_t blink[2048];
extern uint8_t adc[16384];


int install_binary(uint8_t id);
#ifdef __cplusplus
}
#endif
