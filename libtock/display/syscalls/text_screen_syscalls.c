#include "text_screen_syscalls.h"

returncode_t libtock_text_screen_set_upcall(subscribe_upcall callback, void *opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TEXT_SCREEN, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_text_screen_set_readonly_allow(const uint8_t* ptr, uint32_t size) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_TEXT_SCREEN, 0, ptr, size);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_text_screen_command_get_size(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_on(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_off(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_blink_on(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_blink_off(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 5, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_show_cursor(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 6, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_hide_cursor(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 7, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_write(uint32_t len) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 8, len, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_clear(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 9, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_home(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 10, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_text_screen_command_set_cursor(uint32_t col, uint32_t row) {
  syscall_return_t cval = command(DRIVER_NUM_TEXT_SCREEN, 11, col, row);
  return tock_command_return_novalue_to_returncode(cval);
}
