#include "sdcard_syscalls.h"

bool libtock_sdcard_driver_exists(void) {
  return driver_exists(DRIVER_NUM_SDCARD);
}

returncode_t libtock_sdcard_set_upcall(subscribe_upcall callback, void* opque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SDCARD, 0, callback, opque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_sdcard_set_readwrite_allow_read_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SDCARD, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_sdcard_set_readonly_allow_write_buffer(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SDCARD, 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_sdcard_command_initialize(void) {
  syscall_return_t cval = command(DRIVER_NUM_SDCARD, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_sdcard_command_read_block(uint32_t sector) {
  syscall_return_t cval = command(DRIVER_NUM_SDCARD, 3, sector, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_sdcard_command_write_block(uint32_t sector) {
  syscall_return_t cval = command(DRIVER_NUM_SDCARD, 4, sector, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
