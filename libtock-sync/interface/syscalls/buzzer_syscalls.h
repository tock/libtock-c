#pragma once

#include <libtock/interface/syscalls/buzzer_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for buzzer event to start.
returncode_t libtocksync_buzzer_yield_wait_for(void);

#ifdef __cplusplus
}
#endif
