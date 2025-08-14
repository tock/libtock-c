#include "nonvolatile_storage_syscalls.h"

bool libtock_nonvolatile_storage_driver_exists(void) {
  return driver_exists(DRIVER_NUM_NONVOLATILE_STORAGE);
}

returncode_t libtock_nonvolatile_storage_set_upcall_read_done(subscribe_upcall cb, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NONVOLATILE_STORAGE, 0, cb, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_nonvolatile_storage_set_upcall_write_done(subscribe_upcall cb, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NONVOLATILE_STORAGE, 1, cb, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_nonvolatile_storage_set_allow_readwrite_read_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NONVOLATILE_STORAGE, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_nonvolatile_storage_set_allow_readonly_write_buffer(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_NONVOLATILE_STORAGE, 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_nonvolatile_storage_command_get_number_bytes(uint32_t* number_bytes) {
  syscall_return_t cval = command(DRIVER_NUM_NONVOLATILE_STORAGE, 1, 0, 0);
  return tock_command_return_u32_to_returncode(cval, number_bytes);
}

returncode_t libtock_nonvolatile_storage_command_read(uint32_t offset, uint32_t length) {
  syscall_return_t cval = command(DRIVER_NUM_NONVOLATILE_STORAGE, 2, offset, length);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_nonvolatile_storage_command_write(uint32_t offset, uint32_t length) {
  syscall_return_t cval = command(DRIVER_NUM_NONVOLATILE_STORAGE, 3, offset, length);
  return tock_command_return_novalue_to_returncode(cval);
}
