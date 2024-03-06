#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_LPS25HB 0x70004

// Check if this driver exists.
bool libtock_lps25hb_exists(void);

// Set the upcall for when sensor data is ready.
returncode_t libtock_lps25hb_set_upcall(subscribe_upcall callback, void* opaque);

// Request a pressure reading.
returncode_t libtock_lps25hb_command_get_pressure(void);

#ifdef __cplusplus
}
#endif