#include "read_only_state.h"


returncode_t libtock_read_only_state_allocate_region(uint8_t* base, int len) {
  if (len < LIBTOCK_READ_ONLY_STATE_BUFFER_LEN) {
    // The buffer is not long enough
    return RETURNCODE_ESIZE;
  }

  return libtock_read_only_state_set_userspace_read_allow_allocate_region(base, len);
}

uint32_t libtock_read_only_state_get_pending_tasks(void* base) {
  uint32_t* ptr = base;
  return ptr[1];
}

uint64_t libtock_read_only_state_get_ticks(void* base) {
  uint32_t* ptr = base;

  // Start with the high bytes set to 0
  uint32_t high, low;

  do {
    // Get the high bytes the value in memory
    high = ptr[3];
    // Read the low bytes
    low = ptr[2];
    // If the high bytes don't match what is still in memory re-try
    // the load
  } while (high != ptr[3]);

  return ((uint64_t)high << 32) | low;
}

returncode_t libtock_read_only_state_quick_yield(void* base, int* pending_tasks) {
  if (yield_check_tasks()) {
    *pending_tasks = 1;
    return RETURNCODE_SUCCESS;
  } else {
    uint32_t tasks = libtock_read_only_state_get_pending_tasks(base);

    if (tasks > 0) {
      // Waiting tasks, call yield
      yield();
    }

    *pending_tasks = tasks;
    return RETURNCODE_SUCCESS;
  }
}
