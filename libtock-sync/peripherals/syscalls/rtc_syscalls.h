#pragma once

#include <libtock/peripherals/rtc.h>
#include <libtock/peripherals/syscalls/rtc_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an RTC get date operation to complete.
returncode_t libtocksync_rtc_yield_wait_for_get(libtock_rtc_date_t* date);

// Wait for an RTC set date operation to complete.
returncode_t libtocksync_rtc_yield_wait_for_set(void);

#ifdef __cplusplus
}
#endif
