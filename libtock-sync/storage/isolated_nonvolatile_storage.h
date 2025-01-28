#pragma once

#include <libtock/storage/isolated_nonvolatile_storage.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Get number of bytes available to this app in nonvolatile storage 
returncode_t libtocksync_isolated_nonvolatile_storage_get_number_bytes(uint32_t* number_bytes);

// Write `length` bytes from `buffer` to the nonvolatile storage starting at
// `offset`.
returncode_t libtocksync_isolated_nonvolatile_storage_write(uint32_t offset, uint32_t length, uint8_t* buffer,
                                                   uint32_t buffer_length, int* length_written);

// Read `length` bytes into `buffer` from the nonvolatile storage starting at
// `offset`.
returncode_t libtocksync_isolated_nonvolatile_storage_read(uint32_t offset, uint32_t length, uint8_t* buffer,
                                                  uint32_t buffer_length, int* length_read);

#ifdef __cplusplus
}
#endif
