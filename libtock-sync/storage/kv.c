#include <libtock/defer.h>
#include <libtock/storage/syscalls/kv_syscalls.h>

#include "kv.h"

#include "syscalls/kv_syscalls.h"

bool libtocksync_kv_exists(void) {
  return libtock_kv_driver_exists();
}

returncode_t libtocksync_kv_get(const uint8_t* key_buffer, uint32_t key_len, uint8_t* ret_buffer, uint32_t ret_len,
                                uint32_t* value_len) {
  returncode_t err;
  int length;

  err = libtock_kv_set_readonly_allow_key_buffer(key_buffer, key_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readonly_allow_key_buffer(NULL, 0);
  }

  err = libtock_kv_set_readwrite_allow_output_buffer(ret_buffer, ret_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readwrite_allow_output_buffer(NULL, 0);
  }

  err = libtock_kv_command_get();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_kv_yield_wait_for_get(&length);
  if (err != RETURNCODE_SUCCESS) return err;

  *value_len = (uint32_t) length;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_kv_set(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                uint32_t val_len) {
  returncode_t err;

  err = libtock_kv_set_readonly_allow_key_buffer(key_buffer, key_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readonly_allow_key_buffer(NULL, 0);
  }

  err = libtock_kv_set_readonly_allow_input_buffer(val_buffer, val_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readonly_allow_input_buffer(NULL, 0);
  }

  err = libtock_kv_command_set();
  if (err != RETURNCODE_SUCCESS) return err;

  return libtocksync_kv_yield_wait_for_done();
}

returncode_t libtocksync_kv_add(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                uint32_t val_len) {
  returncode_t err;

  err = libtock_kv_set_readonly_allow_key_buffer(key_buffer, key_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readonly_allow_key_buffer(NULL, 0);
  }

  err = libtock_kv_set_readonly_allow_input_buffer(val_buffer, val_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readonly_allow_input_buffer(NULL, 0);
  }

  err = libtock_kv_command_add();
  if (err != RETURNCODE_SUCCESS) return err;

  return libtocksync_kv_yield_wait_for_done();
}

returncode_t libtocksync_kv_update(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                   uint32_t val_len) {
  returncode_t err;

  err = libtock_kv_set_readonly_allow_key_buffer(key_buffer, key_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readonly_allow_key_buffer(NULL, 0);
  }

  err = libtock_kv_set_readonly_allow_input_buffer(val_buffer, val_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readonly_allow_input_buffer(NULL, 0);
  }

  err = libtock_kv_command_update();
  if (err != RETURNCODE_SUCCESS) return err;

  return libtocksync_kv_yield_wait_for_done();
}

returncode_t libtocksync_kv_delete(const uint8_t* key_buffer, uint32_t key_len) {
  returncode_t err;

  err = libtock_kv_set_readonly_allow_key_buffer(key_buffer, key_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_kv_set_readonly_allow_key_buffer(NULL, 0);
  }

  err = libtock_kv_command_delete();
  if (err != RETURNCODE_SUCCESS) return err;

  return libtocksync_kv_yield_wait_for_done();
}

returncode_t libtocksync_kv_garbage_collect(void) {
  returncode_t err;

  err = libtock_kv_command_garbage_collect();
  if (err != RETURNCODE_SUCCESS) return err;

  return libtocksync_kv_yield_wait_for_done();
}
