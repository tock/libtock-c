#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_RTC 0x00090007

// Check if the RTC driver exists.
bool libtock_rtc_exists(void);

// Set the upcall for get and set complete callbacks.
returncode_t libtock_rtc_set_upcall(subscribe_upcall callback, void* opaque);

// Command to get the date.
returncode_t libtock_rtc_command_get_date(void);

// Command to set the date.
returncode_t libtock_rtc_command_set_date(uint32_t date, uint32_t time);

#ifdef __cplusplus
}
#endif