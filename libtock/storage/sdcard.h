#pragma once

#include "../tock.h"
#include "syscalls/sdcard_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for SD card initialization callback.
//
// - `arg1` (`returncode_t`): Status of the init operation.
// - `arg2` (`uint32_t`): block_size, block size of SD card in bytes.
// - `arg3` (`uint32_t`): size_in_kB, total size of SD card in kilobytes.
typedef void (*libtock_sdcard_callback_initialized)(returncode_t, uint32_t, uint32_t);

// Function signature for SD card general callback.
//
// - `arg1` (`returncode_t`): Status of the init operation.
typedef void (*libtock_sdcard_callback_operations)(returncode_t);

// Initialize an SD card asynchronously.
//
// Note that for a newly powered-on SD card, initialization can take over 100 ms
// to complete. Expects a callback to have already been set. Callback will be
// called when either initialization is complete or an error occurs.
returncode_t libtock_sdcard_initialize(libtock_sdcard_callback_initialized cb);

// Read a single block from an SD card asynchronously.
//
// Expects a read_buffer and a callback to already have been set up. Callback
// will be called when either the block has been read or an error occurs. When
// the callback is successful, data will be in the read_buffer.
//
// ## Arguments
//
// - `sector`: sector address of the block to be read.
// - `buffer`: buffer to read into.
// - `len`: len of buffer.
// - `cb`: callback.
returncode_t libtock_sdcard_read_block(uint32_t sector, uint8_t* buffer, uint32_t len,
                                       libtock_sdcard_callback_operations cb);

// Write a single block to an SD card asynchronously.
//
// Expects a write_buffer and a callback to already have been set up. Data in
// the write_buffer will be written to the SD card. Callback will be called when
// either the block has been written or an error occurs.
//
// ## Arguments
//
// - `sector`: sector address of the block to be read.
// - `buffer`: buffer to write from.
// - `len`: len of buffer.
// - `cb`: callback.
returncode_t libtock_sdcard_write_block(uint32_t sector, uint8_t* buffer, uint32_t len,
                                        libtock_sdcard_callback_operations cb);

#ifdef __cplusplus
}
#endif
