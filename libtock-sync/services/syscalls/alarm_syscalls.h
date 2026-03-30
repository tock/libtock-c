#pragma once

#include <libtock/peripherals/syscalls/alarm_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an alarm upcall.
returncode_t libtocksync_alarm_yield_wait_for(void);

#ifdef __cplusplus
}
#endif
