#pragma once

#include <libtock/storage/syscalls/nonvolatile_storage_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a nonvolatile storage read to complete.
returncode_t libtocksync_nonvolatile_storage_yield_wait_for_read(int* length);

// Wait for a nonvolatile storage write to complete.
returncode_t libtocksync_nonvolatile_storage_yield_wait_for_write(int* length);

#ifdef __cplusplus
}
#endif
