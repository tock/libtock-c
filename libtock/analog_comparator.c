#include "analog_comparator.h"
#include "tock.h"

bool analog_comparator_exists(void) {
  return command(DRIVER_NUM_ANALOG_COMPARATOR, 0, 0, 0) >= 0;
}

int analog_comparator_count(void) {
  return command(DRIVER_NUM_ANALOG_COMPARATOR, 0, 0, 0);
}

bool analog_comparator_comparison(uint8_t channel) {
  return command(DRIVER_NUM_ANALOG_COMPARATOR, 1, channel, 0);
}

int analog_comparator_start_comparing(uint8_t channel) {
  return command(DRIVER_NUM_ANALOG_COMPARATOR, 2, channel, 0);
}

int analog_comparator_stop_comparing(uint8_t channel) {
  return command(DRIVER_NUM_ANALOG_COMPARATOR, 3, channel, 0);
}

int analog_comparator_interrupt_callback(subscribe_cb callback, void* callback_args) {
  return subscribe(DRIVER_NUM_ANALOG_COMPARATOR, 0, callback, callback_args);
}
