#include "nonvolatile_storage.h"


struct nv_data {
  bool fired;
  returncode_t ret;
  int length;
};

struct nv_data result = {.fired = false};

static void write_cb(returncode_t ret, int length) {
  result.fired  = true;
  result.ret    = ret;
  result.length = length;
}

static void read_cb(returncode_t ret, int length) {
  result.fired  = true;
  result.ret    = ret;
  result.length = length;
}



returncode_t libtocksync_nonvolatile_storage_write(uint32_t offset, uint32_t length, uint8_t* buffer,
                                                   uint32_t buffer_length, int* length_written) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_nonvolatile_storage_write(offset, length, buffer, buffer_length, write_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_nonvolatile_storage_set_allow_readonly_write_buffer(NULL, 0);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *length_written = result.length;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_nonvolatile_storage_read(uint32_t offset, uint32_t length, uint8_t* buffer,
                                                  uint32_t buffer_length, int* length_read) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_nonvolatile_storage_read(offset, length, buffer, buffer_length, read_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_nonvolatile_storage_set_allow_readwrite_read_buffer(NULL, 0);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *length_read = result.length;
  return RETURNCODE_SUCCESS;
}
