#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_TOUCH 0x90002

// Check if touch sensor exists.
bool libtock_touch_driver_exists(void);

// Set the upcall for upcalls on a single touch.
returncode_t libtock_touch_set_upcall_single_touch(subscribe_upcall callback, void* opaque);

// Set the upcall for upcalls on a multiple touch event.
returncode_t libtock_touch_set_upcall_multi_touch(subscribe_upcall callback, void* opaque);

// Set the upcall for a gesture callback.
returncode_t libtock_touch_set_upcall_gesture(subscribe_upcall callback, void* opaque);

// Allow a buffer for receiving multi touch events.
returncode_t libtock_touch_set_allow_readwrite_multi_touch(void* data, int len);

// Command to get a count of touches.
returncode_t libtock_touch_command_get_number_of_touches(uint32_t* touches);

// Enable single touch events.
returncode_t libtock_touch_command_enable_single_touch(void);

// Disable single touch events.
returncode_t libtock_touch_command_disable_single_touch(void);

// Enable multi touch events.
returncode_t libtock_touch_command_enable_multi_touch(void);

// Disable multi touch events.
returncode_t libtock_touch_command_disable_multi_touch(void);

// Request next multi touch event.
returncode_t libtock_touch_command_multi_touch_next(void);

#ifdef __cplusplus
}
#endif
