#include "button.h"
#include "button_syscalls.h"



returncode_t button_count(int* count) {
  return button_command_count(count);
}



returncode_t button_read(int button_num, int* button_value) {
  return button_command_read(button_num, button_value);
}

static void button_temp_upcall(int                            btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                           void *opaque) {
  button_callback cb = (button_callback) opaque;
  cb(RETURNCODE_SUCCESS, btn_num, val == 1);
}

returncode_t button_notify_on_press(int button_num, button_callback cb) {
  returncode_t ret;

  ret = button_set_upcall(button_temp_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = button_command_enable_interrupt(button_num);
  return ret;
}


