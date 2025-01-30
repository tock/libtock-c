#include "kv.h"

static void kv_upcall_get(int                          err,
                          int                          length,
                          __attribute__ ((unused)) int unused2,
                          void*                        opaque) {

  libtock_kv_callback_get cb = (libtock_kv_callback_get) opaque;
  cb(tock_status_to_returncode(err), length);
}


static void kv_upcall_done(int                          err,
                           __attribute__ ((unused)) int length,
                           __attribute__ ((unused)) int unused2,
                           void*                        opaque) {

  libtock_kv_callback_done cb = (libtock_kv_callback_done) opaque;
  cb(tock_status_to_returncode(err));
}

returncode_t libtock_kv_get(const uint8_t* key_buffer, uint32_t key_len, uint8_t* ret_buffer, uint32_t ret_len,
                            libtock_kv_callback_get cb) {
  returncode_t err;

  err = libtock_kv_set_upcall(kv_upcall_get, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_kv_set_readonly_allow_key_buffer(key_buffer, key_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_kv_set_readwrite_allow_output_buffer(ret_buffer, ret_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_kv_command_get();
  return err;
}

static returncode_t kv_insert(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                              uint32_t val_len, returncode_t (*op_fn)(void), libtock_kv_callback_done cb) {
  returncode_t err;

  err = libtock_kv_set_upcall(kv_upcall_done, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_kv_set_readonly_allow_key_buffer(key_buffer, key_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_kv_set_readonly_allow_input_buffer(val_buffer, val_len);
  if (err != RETURNCODE_SUCCESS) return err;

  // Do the requested set/add/update operation.
  err = op_fn();
  return err;
}

returncode_t libtock_kv_set(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len,
                            libtock_kv_callback_done cb) {
  return kv_insert(key_buffer, key_len, val_buffer, val_len, libtock_kv_command_set, cb);
}

returncode_t libtock_kv_add(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len,
                            libtock_kv_callback_done cb) {
  return kv_insert(key_buffer, key_len, val_buffer, val_len, libtock_kv_command_add, cb);
}

returncode_t libtock_kv_update(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len,
                               libtock_kv_callback_done cb) {
  return kv_insert(key_buffer, key_len, val_buffer, val_len, libtock_kv_command_update, cb);
}

returncode_t libtock_kv_delete(const uint8_t* key_buffer, uint32_t key_len, libtock_kv_callback_done cb) {
  returncode_t err;

  err = libtock_kv_set_upcall(kv_upcall_done, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_kv_set_readonly_allow_key_buffer(key_buffer, key_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_kv_command_delete();
  return err;
}

returncode_t libtock_kv_garbage_collect(libtock_kv_callback_done cb) {
  returncode_t err;

  err = libtock_kv_set_upcall(kv_upcall_done, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_kv_command_garbage_collect();
  return err;
}
