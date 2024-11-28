#include "isolated_nonvolatile_storage.h"
#include "syscalls/isolated_nonvolatile_storage_syscalls.h"

static void get_number_bytes_done(int                          ret,
                                  int                          number_bytes,
                                  __attribute__ ((unused)) int arg3,
                                  void*                        opaque) {
  libtock_isolated_nonvolatile_storage_callback_get_number_bytes cb = (libtock_isolated_nonvolatile_storage_callback_get_number_bytes) opaque;
  cb(tock_status_to_returncode(ret), number_bytes);
}

static void write_done(int                          ret,
                       int                          length,
                       __attribute__ ((unused)) int arg2,
                       void*                        opaque) {
  libtock_isolated_nonvolatile_storage_callback_write cb = (libtock_isolated_nonvolatile_storage_callback_write) opaque;
  cb(tock_status_to_returncode(ret), length);
}

static void read_done(int                          ret,
                      int                          length,
                      __attribute__ ((unused)) int arg2,
                      void*                        opaque) {
  libtock_isolated_nonvolatile_storage_callback_read cb = (libtock_isolated_nonvolatile_storage_callback_read) opaque;
  cb(tock_status_to_returncode(ret), length);
}

returncode_t libtock_isolated_nonvolatile_storage_get_number_bytes(libtock_isolated_nonvolatile_storage_callback_get_number_bytes cb) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_set_upcall_get_number_bytes_done(get_number_bytes_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_command_get_number_bytes();
  return ret;
}

returncode_t libtock_isolated_nonvolatile_storage_write(uint32_t offset, uint32_t length, uint8_t* buffer,
                                               uint32_t buffer_length, libtock_isolated_nonvolatile_storage_callback_write cb) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_set_upcall_write_done(write_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readonly_write_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_command_write(offset, length);
  return ret;
}

returncode_t libtock_isolated_nonvolatile_storage_read(uint32_t offset, uint32_t length, uint8_t* buffer, uint32_t buffer_length,
                                              libtock_isolated_nonvolatile_storage_callback_read cb) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_set_upcall_read_done(read_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readwrite_read_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_isolated_nonvolatile_storage_command_read(offset, length);
  return ret;
}

