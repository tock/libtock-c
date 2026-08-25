#pragma once

#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_app_loader_exists(void);

// Ask the kernel to set up state for a new app of `app_length` bytes.
// Blocks until the setup operation completes.
returncode_t libtocksync_app_loader_setup(uint32_t app_length);

// Write `write_length` bytes from `buffer` to flash at `flash_offset`.
// Blocks until the write operation completes.
returncode_t libtocksync_app_loader_write(uint32_t flash_offset, uint32_t write_length, uint8_t* buffer,
                                          uint32_t buffer_len);

// Signal that writing the new process binary is complete. Blocks until the
// finalize operation completes.
returncode_t libtocksync_app_loader_finalize(void);

// Ask the kernel to load and start the newly flashed app. Blocks until the
// load operation completes.
returncode_t libtocksync_app_loader_load(void);

// Ask the kernel to abort the in-progress setup/write operation. Blocks
// until the abort operation completes.
returncode_t libtocksync_app_loader_abort(void);

#ifdef __cplusplus
}
#endif
