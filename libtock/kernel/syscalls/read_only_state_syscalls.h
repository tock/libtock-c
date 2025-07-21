#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_READ_ONLY_STATE 0x00009

// Check if this driver is available on the kernel.
bool libtock_read_only_state_driver_exists(void);

// Share a buffer with the kernel to use for read only state
//
// - `buffer` the buffer to use.
// - `len` should be `LIBTOCK_READ_ONLY_STATE_BUFFER_LEN`.
returncode_t libtock_read_only_state_set_userspace_read_allow_allocate_region(uint8_t* buffer, uint32_t length);

// Get the latest version of the read only state supported by the kernel.
returncode_t libtock_read_only_state_command_get_version(uint32_t* version);

#ifdef __cplusplus
}
#endif
