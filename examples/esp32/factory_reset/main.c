#include <button.h>
#include <esp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool button_pressed = false;

static void button_callback(__attribute__ ((unused)) int btn_num,
                            __attribute__ ((unused)) int val,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void *ud) {
  button_pressed = true;
}

int main(void) {
  int ret;

  printf("[ESP32 Factory Reset] Erase all saved settings from the ESP32.\n");
  printf("This app will do a factory reset on the ESP32.\n");
  printf("To initiate the factory reset, press the USER button.\n");

  // Setup the button interrupt.
  button_subscribe(button_callback, NULL);
  button_enable_interrupt(0);

  // Now wait for the button to be pressed.
  button_pressed = false;
  yield_for(&button_pressed);

  printf("Detected button press, initiating factory reset of the ESP32...\n");

  ret = esp32_factory_reset();
  if (ret < 0) {
    printf("factory_reset failed %i\n", ret);
  } else {
    printf("ESP32 successfully reset\n");
  }

  return 0;
}
