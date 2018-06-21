#include "analog_comparator.h"
#include "tock.h"

int ac_exists(void) {
  return command(DRIVER_NUM_ACIFC, 0, 0, 0) >= 0;
}

int ac_enable(void) {
  return command(DRIVER_NUM_ACIFC, 1, 0, 0);
}

bool ac_comparison(uint8_t ac) {
  return command(DRIVER_NUM_ACIFC, 2, ac, 0);
}

bool ac_window_comparison(uint8_t window) {
  return command(DRIVER_NUM_ACIFC, 3, window, 0);
}

int ac_enable_interrupts(uint8_t ac) {
  return command(DRIVER_NUM_ACIFC, 4, ac, 0);
}

int ac_interrupt_callback(subscribe_cb callback, void* callback_args) {
  return subscribe(DRIVER_NUM_ACIFC, 0, callback, callback_args);
}
