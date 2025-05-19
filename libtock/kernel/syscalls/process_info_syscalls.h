#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_PROCESS_INFO 0x10002

// Check if this driver is available on the kernel.
bool libtock_process_info_exists(void);

// // Share a buffer with the kernel to use for read only state
// //
// // - `buffer` the buffer to use.
// // - `len` should be `LIBTOCK_READ_ONLY_STATE_BUFFER_LEN`.
// returncode_t libtock_read_only_state_set_userspace_read_allow_allocate_region(uint8_t* buffer, uint32_t length);

returncode_t libtock_process_info_set_allow_readwrite_info_buffer(uint8_t* buffer, uint32_t length);

returncode_t libtock_process_info_command_get_process_count(uint32_t* count);

returncode_t libtock_process_info_command_get_process_ids(uint32_t* count);
returncode_t libtock_process_info_command_get_short_ids(uint32_t* count);
returncode_t libtock_process_info_command_get_process_name(uint32_t process_id, uint32_t* name_len);
returncode_t libtock_process_info_command_get_process_stats(uint32_t process_id);
returncode_t libtock_process_info_command_set_process_state(uint32_t process_id, uint32_t state);

#ifdef __cplusplus
}
#endif
