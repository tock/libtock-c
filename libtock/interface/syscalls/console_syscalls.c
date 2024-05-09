#include "console_syscalls.h"

returncode_t libtock_console_write_done_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_CONSOLE, 1, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_console_read_done_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_CONSOLE, 2, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_console_set_read_allow(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_CONSOLE, 1, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_console_set_readwrite_allow(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_CONSOLE, 1, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

bool libtock_console_command_exists(void) {
  return driver_exists(DRIVER_NUM_CONSOLE);
}

returncode_t libtock_console_command_write(int length) {
  syscall_return_t cval = command(DRIVER_NUM_CONSOLE, 1, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_console_command_read(int length) {
  syscall_return_t cval = command(DRIVER_NUM_CONSOLE, 2, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_console_command_abort_read(void) {
  syscall_return_t cval = command(DRIVER_NUM_CONSOLE, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
