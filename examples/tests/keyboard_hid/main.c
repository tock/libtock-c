#include <stdio.h>
#include <string.h>

#include <button.h>
#include <usb_keyboard_hid.h>


uint8_t keyboard_buffer[64];

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(__attribute__ ((unused)) int   btn_num,
                            __attribute__ ((unused)) int   val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {

  char message[] = "Hi from Tock!";
  int ret;

  ret = usb_keyboard_hid_send_string_sync(keyboard_buffer, message, strlen(message));
  if (ret < 0) {
    printf("ERROR sending string with USB keyboard HID: %i\n", ret);
  }
}

int main(void) {
  int err;

  err = button_subscribe(button_callback, NULL);
  if (err < 0) return err;

  button_enable_interrupt(0);

  return 0;
}
