#include "syscalls/text_screen_syscalls.h"
#include "text_screen.h"

static void text_screen_callback(int                          status,
                                 __attribute__ ((unused)) int data1,
                                 __attribute__ ((unused)) int data2,
                                 void*                        opaque) {
  libtock_text_screen_callback_done cb = (libtock_text_screen_callback_done) opaque;
  cb(tock_status_to_returncode(status));
}

static void text_screen_callback_size(int   status,
                                      int   data1,
                                      int   data2,
                                      void* opaque) {
  libtock_text_screen_callback_size cb = (libtock_text_screen_callback_size) opaque;
  cb(tock_status_to_returncode(status), (uint32_t) data1, (uint32_t) data2);
}

bool libtock_text_screen_exists(void) {
  return libtock_text_screen_driver_exists();
}

returncode_t libtock_text_screen_display_on(libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_on();
  return err;
}

returncode_t libtock_text_screen_display_off(libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_off();
  return err;
}

returncode_t libtock_text_screen_blink_on(libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_blink_on();
  return err;
}

returncode_t libtock_text_screen_blink_off(libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_blink_off();
  return err;
}

returncode_t libtock_text_screen_show_cursor(libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_show_cursor();
  return err;
}

returncode_t libtock_text_screen_hide_cursor(libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_hide_cursor();
  return err;
}

returncode_t libtock_text_screen_clear(libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_clear();
  return err;
}

returncode_t libtock_text_screen_home(libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_home();
  return err;
}

returncode_t libtock_text_screen_set_cursor(uint8_t col, uint8_t row, libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_set_cursor((uint8_t) col, (uint8_t) row);
  return err;
}

returncode_t libtock_text_screen_write(uint8_t* buffer, uint32_t buffer_len, uint32_t write_len,
                                       libtock_text_screen_callback_done cb) {
  returncode_t err;

  err = libtock_text_screen_set_readonly_allow(buffer, buffer_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_set_upcall(text_screen_callback, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_write(write_len);
  return err;
}

returncode_t libtock_text_screen_get_size(libtock_text_screen_callback_size cb) {
  returncode_t err;

  err = libtock_text_screen_set_upcall(text_screen_callback_size, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_text_screen_command_get_size();
  return err;
}
