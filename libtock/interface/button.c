#include "button.h"

#include "syscalls/button_syscalls.h"

bool libtock_button_exists(void) {
  return libtock_button_driver_exists();
}

returncode_t libtock_button_count(int* count) {
  return libtock_button_command_count(count);
}

returncode_t libtock_button_read(int button_num, int* button_value) {
  return libtock_button_command_read(button_num, button_value);
}

static void button_upcall(int                          btn_num,
                          int                          val,
                          __attribute__ ((unused)) int arg2,
                          void*                        opaque) {
  libtock_button_callback cb = (libtock_button_callback) opaque;
  cb(RETURNCODE_SUCCESS, btn_num, val == 1);
}

returncode_t libtock_button_notify_on_press(int button_num, libtock_button_callback cb) {
  returncode_t ret;

  ret = libtock_button_set_upcall(button_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_button_command_enable_interrupt(button_num);
  return ret;
}
