#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_LED 0x2

// Check if the button system call driver is available on this board.
bool libtock_led_driver_exists(void);

// Returns the number of LEDs on the host platform.
returncode_t libtock_led_command_count(int* count);

// Turn on the LED specified by index.
returncode_t libtock_led_command_on(int led_num);

// Turn off the LED specified by index.
returncode_t libtock_led_command_off(int led_num);

// Toggle the LED specified by index.
returncode_t libtock_led_command_toggle(int led_num);

#ifdef __cplusplus
}
#endif
