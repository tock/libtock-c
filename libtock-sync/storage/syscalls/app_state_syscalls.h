#pragma once

#include <libtock/storage/syscalls/app_state_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an app state save to complete.
returncode_t libtocksync_app_state_yield_wait_for(void);

#ifdef __cplusplus
}
#endif
