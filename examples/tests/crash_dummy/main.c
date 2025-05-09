// Crash on button press.

#include <libtock/interface/button.h>

volatile int* zeroptr = 0;

static void button_callback(__attribute__ ((unused)) returncode_t ret,
                            __attribute__ ((unused)) int          btn_num,
                            __attribute__ ((unused)) bool         val) {
  __attribute__ ((unused)) volatile int k = *zeroptr;
}

int main(void) {
  libtock_button_notify_on_press(0, button_callback);

  while (1) {
    yield();
  }
}
