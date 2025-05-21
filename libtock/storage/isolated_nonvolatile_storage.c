#include "isolated_nonvolatile_storage.h"
#include "syscalls/isolated_nonvolatile_storage_syscalls.h"

static void get_number_bytes_done(int   ret,
                                  int   number_bytes_lo,
                                  int   number_bytes_hi,
                                  void* opaque) {
  libtock_isolated_nonvolatile_storage_callback_get_number_bytes cb =
    (libtock_isolated_nonvolatile_storage_callback_get_number_bytes) opaque;
  uint64_t num = (uint64_t) number_bytes_lo | ((uint64_t) number_bytes_hi) << 32;

  cb(tock_status_to_returncode(ret), num);
}

static void write_done(int                          ret,
                       __attribute__ ((unused)) int arg1,
                       __attribute__ ((unused)) int arg2,
                       void*                        opaque) {
  libtock_isolated_nonvolatile_storage_callback_write cb = (libtock_isolated_nonvolatile_storage_callback_write) opaque;
  cb(tock_status_to_returncode(ret));
}

static void read_done(int                          ret,
                      __attribute__ ((unused)) int arg1,
                      __attribute__ ((unused)) int arg2,
                      void*                        opaque) {
  libtock_isolated_nonvolatile_storage_callback_read cb = (libtock_isolated_nonvolatile_storage_callback_read) opaque;
  cb(tock_status_to_returncode(ret));
}

returncode_t libtock_isolated_nonvolatile_storage_get_number_bytes(
  libtock_isolated_nonvolatile_storage_callback_get_number_bytes cb) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_set_upcall_get_number_bytes_done(get_number_bytes_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_command_get_number_bytes();
  return ret;
}

returncode_t libtock_isolated_nonvolatile_storage_write(uint64_t offset, uint8_t* buffer,
                                                        size_t buffer_length,
                                                        libtock_isolated_nonvolatile_storage_callback_write cb) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_set_upcall_write_done(write_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readonly_write_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_command_write(offset);
  return ret;
}

returncode_t libtock_isolated_nonvolatile_storage_read(uint64_t offset, uint8_t* buffer, size_t buffer_length,
                                                       libtock_isolated_nonvolatile_storage_callback_read cb) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_set_upcall_read_done(read_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readwrite_read_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_command_read(offset);
  return ret;
}
