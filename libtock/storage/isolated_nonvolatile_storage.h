#pragma once

#include "../tock.h"
#include "syscalls/isolated_nonvolatile_storage_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for nonvolatile storage get_number_bytes callbacks.
//
// - `arg1` (`returncode_t`): Status of get_number_bytes.
// - `arg2` (`int`): Number of bytes available in storage.
typedef void (*libtock_isolated_nonvolatile_storage_callback_get_number_bytes)(returncode_t, int);

// Function signature for nonvolatile storage write callbacks.
//
// - `arg1` (`returncode_t`): Status of write.
// - `arg2` (`int`): Length written.
typedef void (*libtock_isolated_nonvolatile_storage_callback_write)(returncode_t, int);

// Function signature for nonvolatile storage read callbacks.
//
// - `arg1` (`returncode_t`): Status of read.
// - `arg2` (`int`): Length ead.
typedef void (*libtock_isolated_nonvolatile_storage_callback_read)(returncode_t, int);

// Get the number of bytes available for storage.
returncode_t libtock_isolated_nonvolatile_storage_get_number_bytes(libtock_isolated_nonvolatile_storage_callback_get_number_bytes cb);

// Write `length` bytes from `buffer` to the storage starting at `offset`.
returncode_t libtock_isolated_nonvolatile_storage_write(uint32_t offset, uint32_t length, uint8_t* buffer,
                                               uint32_t buffer_length, libtock_isolated_nonvolatile_storage_callback_write cb);

// Read `length` bytes into `buffer` from the storage starting at `offset`.
returncode_t libtock_isolated_nonvolatile_storage_read(uint32_t offset, uint32_t length, uint8_t* buffer, uint32_t buffer_length,
                                              libtock_isolated_nonvolatile_storage_callback_read cb);

#ifdef __cplusplus
}
#endif

