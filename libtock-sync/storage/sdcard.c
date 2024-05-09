#include "sdcard.h"

// used for creating synchronous versions of functions
//
// - `fired` - set when callback has been called
// - `block_size` - block size of SD card, set upon initialization complete
// - `size_in_kB` - size in kilobytes of SD card, set upon initialization complete
// - `error` - error code signaled in callback, 0 if successful
struct sdcard_data {
  bool fired;
  uint32_t block_size;
  uint32_t size_in_kB;
  returncode_t ret;
};

struct sdcard_data result = {.fired = false};

static void sdcard_cb_init(returncode_t ret, uint32_t block_size, uint32_t size_in_kB) {
  result.fired      = true;
  result.block_size = block_size;
  result.size_in_kB = size_in_kB;
  result.ret        = ret;
}

static void sdcard_cb_general(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}


returncode_t libtocksync_sdcard_initialize(uint32_t* block_size, uint32_t* size_in_kB) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_sdcard_initialize(sdcard_cb_init);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // wait for callback
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  // copy args
  if (block_size != NULL) {
    *block_size = result.block_size;
  }
  if (size_in_kB != NULL) {
    *size_in_kB = result.size_in_kB;
  }

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_sdcard_read_block(uint32_t sector, uint8_t* buffer, uint32_t len) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_sdcard_read_block(sector, buffer, len, sdcard_cb_general);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // wait for callback
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_sdcard_set_readwrite_allow_read_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_sdcard_write_block(uint32_t sector, uint8_t* buffer, uint32_t len) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_sdcard_write_block(sector, buffer, len, sdcard_cb_general);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // wait for callback
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_sdcard_set_readonly_allow_write_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return RETURNCODE_SUCCESS;
}
