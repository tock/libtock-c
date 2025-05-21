#include "isolated_nonvolatile_storage.h"
#include <stdio.h>

struct nv_data {
  bool fired;
  returncode_t ret;
  uint64_t storage_size;
};

static struct nv_data result = {.fired = false};

static void get_number_bytes_cb(returncode_t ret, uint64_t number_bytes) {
  result.fired        = true;
  result.ret          = ret;
  result.storage_size = number_bytes;
}

static void write_cb(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

static void read_cb(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

returncode_t libtocksync_isolated_nonvolatile_storage_get_number_bytes(uint64_t* number_bytes) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_isolated_nonvolatile_storage_get_number_bytes(get_number_bytes_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *number_bytes = result.storage_size;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_isolated_nonvolatile_storage_write(uint64_t offset, uint8_t* buffer,
                                                            uint32_t buffer_length) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_isolated_nonvolatile_storage_write(offset, buffer, buffer_length, write_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readonly_write_buffer(NULL, 0);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_isolated_nonvolatile_storage_read(uint64_t offset, uint8_t* buffer,
                                                           uint32_t buffer_length) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_isolated_nonvolatile_storage_read(offset, buffer, buffer_length, read_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readwrite_read_buffer(NULL, 0);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  return RETURNCODE_SUCCESS;
}
