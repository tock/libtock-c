#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <libtock/interface/button.h>

static void button_callback(returncode_t ret,
                            int          btn_num,
                            bool         pressed) {
  if (ret != RETURNCODE_SUCCESS) return;

  if (pressed) {
    printf("Button %i pressed!\r\n", btn_num);
  }
}

int main(void) {
  for (int i = 0; i < 4; i++) {
    libtock_button_notify_on_press(i, button_callback);
  }

  for ( ;;) {
    yield();
  }
}
