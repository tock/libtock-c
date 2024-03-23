#include <stdio.h>

#include <libtock/interface/button.h>

static void button_callback(returncode_t ret,
                            int          btn_num,
                            bool         val) {
  if (val) {
    printf("Button Press! Button: %i Status: %i\n", btn_num, ret);
    printf("App address: %p\n", tock_app_flash_begins_at());
  }
}

int main(void) {
  returncode_t ret;
  printf("[TEST] Button Press\n");

  if (!libtock_button_exists()) {
    printf("No buttons on this board!\n");
    return -2;
  }

  ret = libtock_button_notify_on_press(0, button_callback);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Could not subscribe to button presses.\n");
    return -3;
  }

  while (1) {
    yield();
  }
}
