#include "ros.h"

int ros_get_version(void) {
  syscall_return_t res = command(ROS_DRIVER_NUM, 0, 0, 0);

  if (res.type != TOCK_SYSCALL_SUCCESS_U32) {
    return -1;
  } else {
    return res.data[0];
  }
}

int ros_allocate_region(void* base, int len) {
  allow_rw_return_t rw;

  if (len < ROS_BUFFER_LEN) {
    // The buffer is not long enough
    return -1;
  }

  rw = allow_shared(ROS_DRIVER_NUM, 0, base, len);

  if (!rw.success) {
    return tock_status_to_returncode(rw.status);
  }
  return RETURNCODE_SUCCESS;
}

uint32_t ros_get_pending_tasks(void* base) {
  uint32_t* ptr = base;
  return ptr[1];
}

uint64_t ros_get_ticks(void* base) {
  uint32_t* ptr = base;

  // Start with the high bytes set to 0
  uint32_t high, low;

  do {
    // Set the high bytes the value in memory
    high = ptr[3];
    // Read the low bytes
    low = ptr[2];
    // If the high bytes don't match what is still in memory re-try
    // the load
  } while (high != ptr[3]);

  return ((uint64_t)high << 32) | low;
}
