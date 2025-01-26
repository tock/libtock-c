#pragma once

#include <libtock/peripherals/rtc.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Get the current date.
returncode_t libtocksync_rtc_get_date(libtock_rtc_date_t* date);

// Set the date.
returncode_t libtocksync_rtc_set_date(libtock_rtc_date_t* set_date);

#ifdef __cplusplus
}
#endif
