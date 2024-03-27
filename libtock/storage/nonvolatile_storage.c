#include "nonvolatile_storage.h"

static void write_done(int                          length,
                       __attribute__ ((unused)) int arg1,
                       __attribute__ ((unused)) int arg2,
                       void*                        opaque) {
  libtock_nonvolatile_storage_callback_write cb = (libtock_nonvolatile_storage_callback_write) opaque;
  cb(RETURNCODE_SUCCESS, length);
}

static void read_done(int                          length,
                      __attribute__ ((unused)) int arg1,
                      __attribute__ ((unused)) int arg2,
                      void*                        opaque) {
  libtock_nonvolatile_storage_callback_read cb = (libtock_nonvolatile_storage_callback_read) opaque;
  cb(RETURNCODE_SUCCESS, length);
}

returncode_t libtock_nonvolatile_storage_get_number_bytes(uint32_t* number_bytes) {
  return libtock_nonvolatile_storage_command_get_number_bytes(number_bytes);
}

returncode_t libtock_nonvolatile_storage_write(uint32_t offset, uint32_t length, uint8_t* buffer,
                                               uint32_t buffer_length, libtock_nonvolatile_storage_callback_write cb) {
  returncode_t ret;

  ret = libtock_nonvolatile_storage_set_upcall_write_done(write_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_nonvolatile_storage_set_allow_readonly_write_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_nonvolatile_storage_command_write(offset, length);
  return ret;
}

returncode_t libtock_nonvolatile_storage_read(uint32_t offset, uint32_t length, uint8_t* buffer, uint32_t buffer_length,
                                              libtock_nonvolatile_storage_callback_read cb) {
  returncode_t ret;

  ret = libtock_nonvolatile_storage_set_upcall_read_done(read_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_nonvolatile_storage_set_allow_readwrite_read_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_nonvolatile_storage_command_read(offset, length);
  return ret;
}
