#pragma once

#include <libtock/kernel/syscalls/app_loader_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for the app loader setup operation to complete.
returncode_t libtocksync_app_loader_yield_wait_for_setup(void);

// Wait for the app loader flash write operation to complete.
returncode_t libtocksync_app_loader_yield_wait_for_write(void);

// Wait for the app loader finalize operation to complete.
returncode_t libtocksync_app_loader_yield_wait_for_finalize(void);

// Wait for the app loader load operation to complete.
returncode_t libtocksync_app_loader_yield_wait_for_load(void);

// Wait for the app loader abort operation to complete.
returncode_t libtocksync_app_loader_yield_wait_for_abort(void);

#ifdef __cplusplus
}
#endif
