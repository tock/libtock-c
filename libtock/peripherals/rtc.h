#pragma once

#include "../tock.h"
#include "rtc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for get date callbacks.
//
// - `arg1` (`returncode_t`): Returncode indicating status.
// - `arg2` (`libtock_rtc_date_t`): Current date.
typedef void (*libtock_rtc_callback_date)(returncode_t, libtock_rtc_date_t);

// Function signature for operation done callbacks.
//
// - `arg1` (`returncode_t`): Returncode indicating status.
typedef void (*libtock_rtc_callback_done)(returncode_t);


// Check if the driver exists.
bool libtock_rtc_exists(void);

// Get the current date.
//
// The callback will be issued with the current date and time.
returncode_t libtock_rtc_get_date(libtock_rtc_callback_date cb);

// Set the date and time.
//
// The callback will be issued when the time and date have been set.
returncode_t libtock_rtc_set_date(libtock_rtc_date_t* set_date, libtock_rtc_callback_done cb);

#ifdef __cplusplus
}
#endif
