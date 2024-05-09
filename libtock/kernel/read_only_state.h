#pragma once

#include "../tock.h"
#include "syscalls/read_only_state_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// We currently support ROS version 1
// Version 1:
//   |-------------------------|
//   |       Count (u32)       |
//   |-------------------------|
//   |   Pending Tasks (u32)   |
//   |-------------------------|
//   |                         |
//   |     Time Ticks (u64)    |
//   |-------------------------|
#define LIBTOCK_READ_ONLY_STATE_BUFFER_LEN (4 * 4 + 4 * 4 + 8 * 4)

// Share a buffer with the kernel to use for read only state
//
// - `base` the buffer to use.
// - `len` should be `LIBTOCK_READ_ONLY_STATE_BUFFER_LEN`.
returncode_t libtock_read_only_state_allocate_region(uint8_t* base, int len);

// Use the read only state buffer provided by `base`
// to get the number of pending tasks.
uint32_t libtock_read_only_state_get_pending_tasks(void* base);

// Use the read only state buffer provided by `base`
// to get the current time returned from the kernel.
uint64_t libtock_read_only_state_get_ticks(void* base);

#ifdef __cplusplus
}
#endif
