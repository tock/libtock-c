#include <libtock/interface/button.h>
#include <libtock/interface/led.h>

// Callback for button presses.
static void button_callback(__attribute__ ((unused)) returncode_t ret, int btn_num, bool pressed) {
  if (pressed) {
    libtock_led_toggle(btn_num);
  }
}

int main(void) {
  int err;

  // Enable interrupts on each button.
  int count;
  err = libtock_button_count(&count);
  if (err < 0) return err;

  for (int i = 0; i < count; i++) {
    libtock_button_notify_on_press(i, button_callback);
  }

  return 0;
}
