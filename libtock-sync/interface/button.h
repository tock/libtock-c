#pragma once

#include <libtock/interface/button.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_button_exists(void);

// Wait for a specific button to be pressed.
//
// This blocks until the button has been pressed. Note, this will not return if
// the button is currently pressed and then released. It will wait until the
// button is pressed again.
//
// ## Arguments
//
// - `button_num`: Which button to wait for.
//
// ## Return Value
//
// A returncode indicating whether the button wait was completed successfully.
returncode_t libtocksync_button_wait_for_press(int button_num);

#ifdef __cplusplus
}
#endif
