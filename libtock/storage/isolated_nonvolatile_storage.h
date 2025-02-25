#pragma once

#include "../tock.h"
#include "syscalls/isolated_nonvolatile_storage_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for nonvolatile storage get_number_bytes callbacks.
//
// - `arg1` (`returncode_t`): Status of get_number_bytes.
// - `arg2` (`uint64_t`): Number of bytes available in storage.
typedef void (*libtock_isolated_nonvolatile_storage_callback_get_number_bytes)(returncode_t, uint64_t);

// Function signature for nonvolatile storage write callbacks.
//
// - `arg1` (`returncode_t`): Status of write.
typedef void (*libtock_isolated_nonvolatile_storage_callback_write)(returncode_t);

// Function signature for nonvolatile storage read callbacks.
//
// - `arg1` (`returncode_t`): Status of read.
typedef void (*libtock_isolated_nonvolatile_storage_callback_read)(returncode_t);

// Get the number of bytes available for storage.
returncode_t libtock_isolated_nonvolatile_storage_get_number_bytes(
  libtock_isolated_nonvolatile_storage_callback_get_number_bytes cb);

// Write bytes from `buffer` to the storage starting at `offset`.
returncode_t libtock_isolated_nonvolatile_storage_write(uint64_t                                            offset,
                                                        uint8_t*                                            buffer,
                                                        size_t                                              buffer_length,
                                                        libtock_isolated_nonvolatile_storage_callback_write cb);

// Read bytes into `buffer` from the storage starting at `offset`.
returncode_t libtock_isolated_nonvolatile_storage_read(uint64_t                                           offset,
                                                       uint8_t*                                           buffer,
                                                       size_t                                             buffer_length,
                                                       libtock_isolated_nonvolatile_storage_callback_read cb);

#ifdef __cplusplus
}
#endif
