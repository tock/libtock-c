#pragma once

#include <libtock/storage/syscalls/kv_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a KV get operation to complete.
returncode_t libtocksync_kv_yield_wait_for_get(int* length);

// Wait for a KV done operation to complete (set, add, update, delete, gc).
returncode_t libtocksync_kv_yield_wait_for_done(void);

#ifdef __cplusplus
}
#endif
