#pragma once

#include "../tock.h"
#include "syscalls/process_info_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtock_process_info_get_process_ids(uint8_t* buffer, size_t buffer_length, uint32_t* count);
returncode_t libtock_process_info_get_short_ids(uint8_t* buffer, size_t buffer_length, uint32_t* count);
returncode_t libtock_process_info_get_process_name(uint32_t process_id, uint8_t* buffer, size_t buffer_length);
returncode_t libtock_process_info_get_process_stats(uint32_t process_id, uint8_t* buffer, size_t buffer_length);
returncode_t libtock_process_info_set_process_state(uint32_t process_id, uint32_t state);

#ifdef __cplusplus
}
#endif
