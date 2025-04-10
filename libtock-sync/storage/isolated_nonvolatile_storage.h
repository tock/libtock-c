#pragma once

#include <libtock/storage/isolated_nonvolatile_storage.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Get number of bytes available to this app in nonvolatile storage.
returncode_t libtocksync_isolated_nonvolatile_storage_get_number_bytes(uint64_t* number_bytes);

// Write bytes from `buffer` to the nonvolatile storage starting at `offset`.
returncode_t libtocksync_isolated_nonvolatile_storage_write(uint64_t offset, uint8_t* buffer,
                                                            uint32_t buffer_length);

// Read bytes into `buffer` from the nonvolatile storage starting at `offset`.
returncode_t libtocksync_isolated_nonvolatile_storage_read(uint64_t offset, uint8_t* buffer,
                                                           uint32_t buffer_length);

#ifdef __cplusplus
}
#endif
