#include <stdio.h>

#include <libtock/interface/button.h>

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: true if pressed, false if depressed
static void button_callback(
  __attribute__ ((unused)) returncode_t   ret,
  __attribute__ ((unused)) int   btn_num,
                            bool                            val) {
  // Only print on the down press.
  if (val) {
    printf("Hello!\n");
  }
}

int main(void) {
  // Ensure there is a button to use.
  int count;
  libtock_button_count(&count);
  if (count < 1) {
    // There are no buttons on this platform.
    printf("Error! No buttons on this platform.\n");
  } else {
    // Enable an interrupt on the first button.
    libtock_button_notify_on_press(0, button_callback);
  }

  // Loop forever waiting on button presses.
  while (1) {
    yield();
  }
}
