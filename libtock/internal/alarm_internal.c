#include "internal/alarm.h"

int alarm_internal_subscribe(subscribe_cb cb, void *userdata) {
  return subscribe(DRIVER_NUM_ALARM, 0, cb, userdata);
}

int alarm_internal_set(uint32_t tics) {
  return command(DRIVER_NUM_ALARM, 4, (int)tics, 0);
}

int alarm_internal_relative_set(uint32_t delta_tics) {
  return command(DRIVER_NUM_ALARM, 5, (int)delta_tics, 0);
}

int alarm_internal_stop(void) {
  return command(DRIVER_NUM_ALARM, 3, 0, 0);
}

unsigned int alarm_internal_frequency(void) {
  return (unsigned int) command(DRIVER_NUM_ALARM, 1, 0, 0);
}
