#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SDCARD 0x50002

// Check if an SD Card driver is installed.
bool libtock_sdcard_driver_exists(void);

// Set a callback function for SD card commands.
returncode_t libtock_sdcard_set_upcall(subscribe_upcall callback, void* opque);

// Set a buffer that data read from the SD card will be placed into.
returncode_t libtock_sdcard_set_readwrite_allow_read_buffer(uint8_t* buffer, uint32_t len);

// Set a buffer containing data to write to the SD card.
returncode_t libtock_sdcard_set_readonly_allow_write_buffer(uint8_t* buffer, uint32_t len);

// Initialize an SD card asynchronously.
//
// Note that for a newly powered-on SD card, initialization can take over 100 ms
// to complete. Expects a callback to have already been set. Callback will be
// called when either initialization is complete or an error occurs.
returncode_t libtock_sdcard_command_initialize(void);

// Read a single block from an SD card asynchronously.
//
// Expects a read_buffer and a callback to already have been set up. Callback
// will be called when either the block has been read or an error occurs. When
// the callback is successful, data will be in the read_buffer.
//
// ## Arguments
//
// - `sector`: sector address of the block to be read.
returncode_t libtock_sdcard_command_read_block(uint32_t sector);

// Write a single block to an SD card asynchronously.
//
// Expects a write_buffer and a callback to already have been set up. Data in
// the write_buffer will be written to the SD card. Callback will be called when
// either the block has been written or an error occurs.
//
// ## Arguments
//
// - `sector`: sector address of the block to be written
returncode_t libtock_sdcard_command_write_block(uint32_t sector);

#ifdef __cplusplus
}
#endif
