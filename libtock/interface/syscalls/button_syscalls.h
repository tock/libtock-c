#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_BUTTON 0x3

// Check if the button system call driver is available on this board.
bool libtock_button_exists(void);

// Set the upcall for the button driver.
returncode_t libtock_button_set_upcall(subscribe_upcall callback, void* opaque);

// Set `count` to the number of buttons.
returncode_t libtock_button_command_count(int* count);

// Enable the interrupt for the button so that button presses trigger upcalls.
//
// ## Arguments
//
// - `button_num`: The index of the button.
returncode_t libtock_button_command_enable_interrupt(int button_num);

// Disable the interrupt for the button so that button presses do not trigger
// upcalls.
//
// ## Arguments
//
// - `button_num`: The index of the button.
returncode_t libtock_button_command_disable_interrupt(int button_num);

// Read the current button state into `button_value`.
//
// ## Arguments
//
// - `button_num`: The index of the button.
// - `button_value`: Will be set to 1 if button is pressed, 0 otherwise.
returncode_t libtock_button_command_read(int button_num, int* button_value);

#ifdef __cplusplus
}
#endif
