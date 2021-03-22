#include "internal/alarm.h"

int alarm_internal_subscribe(subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_ALARM, 0, cb, userdata);
  if (sval.success) {
    return 0;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int alarm_internal_set(uint32_t reference, uint32_t tics) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 6, reference, tics);
  if (rval.type == TOCK_SYSCALL_SUCCESS) {
    return 0;
  } else {
    return tock_error_to_rcode(rval.data[0]);
  }
}

int alarm_internal_stop(void) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 3, 0, 0);
  if (rval.type == TOCK_SYSCALL_SUCCESS) {
    return 0;
  } else {
    return tock_error_to_rcode(rval.data[0]);
  }
}

unsigned int alarm_internal_frequency(void) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 1, 0, 0);
  if (rval.type == TOCK_SYSCALL_SUCCESS_U32) {
    return (unsigned int)rval.data[0];
  } else {
    return 0;
  }
}
