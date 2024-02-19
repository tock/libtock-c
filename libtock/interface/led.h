#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Returns the number of LEDs on the host platform.
returncode_t libtock_led_count(int* count);

// Turn on the specified LED.
returncode_t libtock_led_on(int led_num);

// Turn off the specified LED.
returncode_t libtock_led_off(int led_num);

// Toggle the specified LED.
returncode_t libtock_led_toggle(int led_num);

#ifdef __cplusplus
}
#endif
