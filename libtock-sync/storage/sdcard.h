#pragma once

#include <libtock/storage/sdcard.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the SD card.
returncode_t libtocksync_sdcard_initialize(uint32_t* block_size, uint32_t* size_in_kB);

// Read a block from the SD card.
returncode_t libtocksync_sdcard_read_block(uint32_t sector, uint8_t* buffer, uint32_t len);

// Write a block to the SD card.
returncode_t libtocksync_sdcard_write_block(uint32_t sector, uint8_t* buffer, uint32_t len);

#ifdef __cplusplus
}
#endif
