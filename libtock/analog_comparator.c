#include "analog_comparator.h"
#include "tock.h"

bool analog_comparator_exists(void) {
  syscall_return_t com = command2(DRIVER_NUM_ANALOG_COMPARATOR, 0, 0, 0);
  return com.type == TOCK_SYSCALL_SUCCESS_U32;
}

int analog_comparator_count(void) {
  syscall_return_t com = command2(DRIVER_NUM_ANALOG_COMPARATOR, 0, 0, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS_U32) {
    return com.data[0];
  } else if (com.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(com.data[0]);
  } else {
    return TOCK_FAIL;
  }
}

bool analog_comparator_comparison(uint8_t channel) {
  syscall_return_t com = command2(DRIVER_NUM_ANALOG_COMPARATOR, 1, channel, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS_U32) {
    return com.data[0];
  } else if (com.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(com.data[0]);
  } else {
    return TOCK_FAIL;
  }
}

int analog_comparator_start_comparing(uint8_t channel) {
  syscall_return_t com = command2(DRIVER_NUM_ANALOG_COMPARATOR, 2, channel, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(com.data[0]);
  } else {
    return TOCK_FAIL;
  }
}

int analog_comparator_stop_comparing(uint8_t channel) {
  syscall_return_t com = command2(DRIVER_NUM_ANALOG_COMPARATOR, 3, channel, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(com.data[0]);
  } else {
    return TOCK_FAIL;
  }
}

int analog_comparator_interrupt_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sub = subscribe2(DRIVER_NUM_ANALOG_COMPARATOR, 0, callback, callback_args);
  if (sub.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sub.error);
  }
}
