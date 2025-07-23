#include "isolated_nonvolatile_storage_syscalls.h"

static uint32_t lo(uint64_t v) {
  return (uint32_t) v;
}

static uint32_t hi(uint64_t v) {
  return (uint32_t) (v >> 32);
}

bool libtock_isolated_nonvolatile_storage_exists(void) {
  return driver_exists(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE);
}

returncode_t libtock_isolated_nonvolatile_storage_set_upcall_get_number_bytes_done(subscribe_upcall cb, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 0, cb, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_isolated_nonvolatile_storage_set_upcall_read_done(subscribe_upcall cb, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 1, cb, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_isolated_nonvolatile_storage_set_upcall_write_done(subscribe_upcall cb, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 2, cb, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_isolated_nonvolatile_storage_set_allow_readwrite_read_buffer(uint8_t* buffer, size_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_isolated_nonvolatile_storage_set_allow_readonly_write_buffer(uint8_t* buffer, size_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_isolated_nonvolatile_storage_command_get_number_bytes(void) {
  syscall_return_t cval = command(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_isolated_nonvolatile_storage_command_read(uint64_t offset) {
  syscall_return_t cval = command(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 2, lo(offset), hi(offset));
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_isolated_nonvolatile_storage_command_write(uint64_t offset) {
  syscall_return_t cval = command(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 3, lo(offset), hi(offset));
  return tock_command_return_novalue_to_returncode(cval);
}
