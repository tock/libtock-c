#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_CRC 0x40002

// Check if CRC driver is installed.
bool libtock_crc_exists(void);

// Register a callback to receive CRC results.
returncode_t libtock_crc_set_upcall(subscribe_upcall callback, void* opaque);

// Provide the buffer over which to compute a CRC.
returncode_t libtock_crc_set_readonly_allow(const uint8_t* buf, uint32_t len);

// Request a CRC computation asynchronously
//
// The callback and buffer must be provided first.
//
// If `SUCCESS` is returned, the result will be provided to the registered
// callback.
//
// Returns `EBUSY` if a computation is already in progress. Returns `ESIZE` if
// the buffer is too big for the unit.
returncode_t libtock_crc_command_request(uint32_t algorithm, uint32_t len);

#ifdef __cplusplus
}
#endif
