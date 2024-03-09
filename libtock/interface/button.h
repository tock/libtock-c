#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for button press callbacks.
//
// - `arg1` (`returncode_t`): Returncode indicating status of button press.
// - `arg2` (`int`): Button index.
// - `arg3` (`bool`): True if pressed, false otherwise.
typedef void (*libtock_button_callback)(returncode_t, int, bool);

// Read the current button state into `button_value`.
//
// ## Arguments
//
// - `button_num`: The index of the button.
// - `button_value`: Will be set to 1 if button is pressed, 0 otherwise.
returncode_t libtock_button_read(int button_num, int* button_value);

// Set `count` to the number of buttons.
returncode_t libtock_button_count(int* count);

// Setup a callback when a button is pressed.
//
// ## Arguments
//
// - `button_num`: The index of the button.
// - `cb`: The function to be called when the button is pressed. Will be called
//   both when the button is pressed and when released.
returncode_t libtock_button_notify_on_press(int button_num, libtock_button_callback cb);


#ifdef __cplusplus
}
#endif

