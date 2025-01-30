#include "kv.h"

struct kv_data {
  bool fired;
  int length;
  returncode_t ret;
};

static struct kv_data result = {.fired = false};

static void kv_cb_get(returncode_t ret, int length) {
  result.fired  = true;
  result.length = length;
  result.ret    = ret;
}

static void kv_cb_done(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

returncode_t libtocksync_kv_get(const uint8_t* key_buffer, uint32_t key_len, uint8_t* ret_buffer, uint32_t ret_len,
                                uint32_t* value_len) {
  returncode_t err;
  result.fired = false;

  err = libtock_kv_get(key_buffer, key_len, ret_buffer, ret_len, kv_cb_get);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  // Return the length of the retrieved value.
  *value_len = result.length;

  return RETURNCODE_SUCCESS;
}

static returncode_t kv_insert(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                              uint32_t val_len, returncode_t (*op_fn)(const uint8_t*, uint32_t, const uint8_t*,
                                                                      uint32_t, libtock_kv_callback_done)) {
  returncode_t err;
  result.fired = false;

  // Do the requested set/add/update operation.
  err = op_fn(key_buffer, key_len, val_buffer, val_len, kv_cb_done);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_kv_set(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                uint32_t val_len) {
  return kv_insert(key_buffer, key_len, val_buffer, val_len, libtock_kv_set);
}

returncode_t libtocksync_kv_add(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                uint32_t val_len) {
  return kv_insert(key_buffer, key_len, val_buffer, val_len, libtock_kv_add);
}

returncode_t libtocksync_kv_update(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                   uint32_t val_len) {
  return kv_insert(key_buffer, key_len, val_buffer, val_len, libtock_kv_update);
}

returncode_t libtocksync_kv_delete(const uint8_t* key_buffer, uint32_t key_len) {
  returncode_t err;
  result.fired = false;

  err = libtock_kv_delete(key_buffer, key_len, kv_cb_done);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_kv_garbage_collect(void) {
  returncode_t err;
  result.fired = false;

  err = libtock_kv_garbage_collect(kv_cb_done);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}
