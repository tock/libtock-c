#pragma once

#include <libtock/storage/syscalls/isolated_nonvolatile_storage_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an isolated nonvolatile storage get_number_bytes to complete.
returncode_t libtocksync_isolated_nonvolatile_storage_yield_wait_for_get_number_bytes(uint64_t* number_bytes);

// Wait for an isolated nonvolatile storage read to complete.
returncode_t libtocksync_isolated_nonvolatile_storage_yield_wait_for_read(void);

// Wait for an isolated nonvolatile storage write to complete.
returncode_t libtocksync_isolated_nonvolatile_storage_yield_wait_for_write(void);

#ifdef __cplusplus
}
#endif
