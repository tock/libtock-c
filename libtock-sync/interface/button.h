#pragma once

#include <tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a specific button to be pressed.
//
// This blocks until the button has been pressed. Note, this will not return if
// the button is currently pressed and then released. It will wait until the
// button is pressed again.
//
// ## Arguments
//
// - `temperature`: Set to the temperature value in hundredths of degrees
//   centigrade.
//
// ## Return Value
//
// A returncode indicating whether the temperature read was completed
// successfully.
returncode_t libtocksync_button_wait_for_press(int button_num);

#ifdef __cplusplus
}
#endif