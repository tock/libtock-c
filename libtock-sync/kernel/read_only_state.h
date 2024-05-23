#pragma once

#include <libtock/kernel/read_only_state.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Use ROS to check if there are any pending tasks. If there are we yield, if
// not then we return immediately without any syscalls.
//
// `pending_tasks` is set to the number of pending tasks before yield was
// called. If there are no pending tasks, `pending_tasks` is set to 0.
//
// Returns `RETURNCODE_SUCCESS` on success, and error otherwise.
returncode_t libtocksync_read_only_state_quick_yield(void* base, int* pending_tasks);

#ifdef __cplusplus
}
#endif
