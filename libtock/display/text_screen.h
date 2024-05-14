#pragma once

#include "../tock.h"
#include "syscalls/text_screen_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_TEXT_SCREEN 0x90003

// Callback when an operation has completed.
//
// This callback is used for several operations, but is generic as it only
// returns the returncode for the operation as provided in the upcall.
typedef void (*libtock_text_screen_callback_done)(returncode_t);

// Callback with size.
//
// - `arg1` (`returncode_t`): Status of getting the size.
// - `arg2` (`uint32_t`): Width of display.
// - `arg3` (`uint32_t`): Height of display.
typedef void (*libtock_text_screen_callback_size)(returncode_t, uint32_t, uint32_t);

returncode_t libtock_text_screen_display_on (libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_display_off (libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_blink_on (libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_blink_off (libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_show_cursor (libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_hide_cursor (libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_clear (libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_home (libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_set_cursor (uint8_t col, uint8_t row, libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_write (uint8_t* buffer, uint32_t buffer_len, uint32_t write_len,
                                        libtock_text_screen_callback_done cb);

returncode_t libtock_text_screen_get_size (libtock_text_screen_callback_size cb);

#ifdef __cplusplus
}
#endif