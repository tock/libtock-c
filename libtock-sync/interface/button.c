#include <interface/button.h>

#include "button.h"


struct data {
  bool fired;
  bool pressed;
  int button_num;
  returncode_t result;
};

static struct data result = { .fired = false };


static void button_cb(returncode_t ret, int button_num, bool pressed) {
  result.fired      = true;
  result.pressed    = pressed;
  result.button_num = button_num;
  result.result     = ret;
}


returncode_t libtocksync_button_wait_for_press(int button_num) {
  returncode_t err;
  result.fired = false;

  err = libtock_button_notify_on_press(button_num, button_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback and for the button to be pressed.
  while (true) {
    yield_for(&result.fired);
    if (result.result != RETURNCODE_SUCCESS) return result.result;

    if (result.pressed && result.button_num == button_num) {
      break;
    }
  }

  return RETURNCODE_SUCCESS;
}
