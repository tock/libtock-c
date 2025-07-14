#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for write done callbacks.
//
// - `length` (`int`): Number of bytes written
typedef void (*libtock_console_callback_write)(returncode_t, uint32_t);

// Function signature for read done callbacks.
//
// - `length` (`int`): Number of bytes read
typedef void (*libtock_console_callback_read)(returncode_t, uint32_t);

// Check if the driver exists.
bool libtock_console_exists(void);

returncode_t libtock_console_write(const uint8_t* buffer, uint32_t len, libtock_console_callback_write cb);

returncode_t libtock_console_read(uint8_t* buffer, uint32_t len, libtock_console_callback_read cb);

returncode_t libtock_console_abort_read(void);

#ifdef __cplusplus
}
#endif
