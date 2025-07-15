#include "read_only_state.h"

returncode_t libtocksync_read_only_state_quick_yield(void* base, int* pending_tasks) {

  uint32_t tasks = libtock_read_only_state_get_pending_tasks(base);

  if (tasks > 0) {
    // Waiting tasks, call yield
    yield();
  }

  *pending_tasks = tasks;
  return RETURNCODE_SUCCESS;
}
