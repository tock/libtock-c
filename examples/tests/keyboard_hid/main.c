#include <stdio.h>
#include <string.h>

#include <libtock-sync/interface/button.h>
#include <libtock-sync/interface/usb_keyboard_hid.h>

char message[] = "Hi from Tock!";

int main(void) {
  returncode_t ret;
  printf("[TEST] USB Keyboard HID\n");

  if (!libtock_usb_keyboard_hid_exists()) {
    printf("No USB keyboard HID on this board!\n");
    return -2;
  }

  while (1) {
    ret = libtocksync_button_wait_for_press(0);
    if (ret != RETURNCODE_SUCCESS) {
      printf("Could not wait for button presses.\n");
      return -3;
    }

    ret = libtocksync_usb_keyboard_hid_send_string(message, strlen(message));
    if (ret != RETURNCODE_SUCCESS) {
      printf("ERROR sending string with USB keyboard HID: %i\n", ret);
      return -4;
    }
  }
}
