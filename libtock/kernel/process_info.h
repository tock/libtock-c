#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Check if the driver exists.
bool libtock_process_info_exists(void);

// Get an array of `uint32_t` process IDs.
//
// `count` is set to the number of process IDs.
returncode_t libtock_process_info_get_process_ids(uint8_t* buffer, size_t buffer_length, uint32_t* count);

// Get an array of `uint32_t` `ShortId`s.
//
// `count` is set to the number of `ShortId`s.
returncode_t libtock_process_info_get_short_ids(uint8_t* buffer, size_t buffer_length, uint32_t* count);

// Get the process name for the process with process ID `process_id`.
returncode_t libtock_process_info_get_process_name(uint32_t process_id, uint8_t* buffer, size_t buffer_length);

// Populate the buffer with information about the process with process ID `process_id`.
returncode_t libtock_process_info_get_process_stats(uint32_t process_id, uint8_t* buffer, size_t buffer_length);

// Change the operating state of the process with process ID `process_id`.
returncode_t libtock_process_info_set_process_state(uint32_t process_id, uint32_t state);

#ifdef __cplusplus
}
#endif
