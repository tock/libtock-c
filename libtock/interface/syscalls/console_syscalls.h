#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_CONSOLE 0x1

// Configure the upcall for write completion events.
returncode_t libtock_console_write_done_set_upcall(subscribe_upcall callback, void* opaque);

// Configure the upcall for write completion events.
returncode_t libtock_console_read_done_set_upcall(subscribe_upcall callback, void* opaque);

// Set the buffer for writing data to the console.
returncode_t libtock_console_set_read_allow(const uint8_t* buffer, uint32_t len);

// Set the buffer for reading data from the console.
returncode_t libtock_console_set_readwrite_allow(uint8_t* buffer, uint32_t len);

// Check if the console driver exists
bool libtock_console_command_exists(void);

// Initiate a write operation of the allowed read-only buffer with the provided length
returncode_t libtock_console_command_write(int length);

// Initiate a read operation into the allowed read-write buffer of the provided length
returncode_t libtock_console_command_read(int length);

// Abort any oustanding read operations
returncode_t libtock_console_command_abort_read(void);


#ifdef __cplusplus
}
#endif
