#pragma once

#include <libtock/tock.h>
#include <libtock/storage/nonvolatile_storage.h>

#ifdef __cplusplus
extern "C" {
#endif

// Write `length` bytes from `buffer` to the nonvolatile storage starting at
// `offset`.
returncode_t libtocksync_nonvolatile_storage_write(uint32_t offset, uint32_t length, uint8_t* buffer, uint32_t buffer_length, int* length_written);

// Read `length` bytes into `buffer` from the nonvolatile storage starting at
// `offset`.
returncode_t libtocksync_nonvolatile_storage_read(uint32_t offset, uint32_t length, uint8_t* buffer, uint32_t buffer_length, int* length_read);

#ifdef __cplusplus
}
#endif
