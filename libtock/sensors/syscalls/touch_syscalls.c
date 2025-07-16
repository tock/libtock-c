#include "touch_syscalls.h"

bool libtock_touch_driver_exists(void) {
  return driver_exists(DRIVER_NUM_TOUCH);
}

returncode_t libtock_touch_set_upcall_single_touch(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TOUCH, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_touch_set_upcall_gesture(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TOUCH, 1, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_touch_set_upcall_multi_touch(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TOUCH, 2, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_touch_set_allow_readwrite_multi_touch(void* data, int len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_TOUCH, 2, data, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_touch_command_get_number_of_touches(uint32_t* touches) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 100, 0, 0);
  return tock_command_return_u32_to_returncode(cval, touches);
}

returncode_t libtock_touch_command_enable_single_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_touch_command_disable_single_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_touch_command_enable_multi_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 11, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_touch_command_disable_multi_touch(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 12, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_touch_command_multi_touch_next(void) {
  syscall_return_t cval = command(DRIVER_NUM_TOUCH, 10, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
