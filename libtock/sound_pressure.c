#include "sound_pressure.h"
#include "tock.h"

struct data {
  bool fired;
  unsigned char temp;
};

static struct data result = { .fired = false };

// Internal callback for faking synchronous reads
static void cb(int temp,
               __attribute__ ((unused)) int unused,
               __attribute__ ((unused)) int unused1,
               void* ud) {
  struct data* data = (struct data*) ud;
  data->temp  = (unsigned char)temp;
  data->fired = true;
}

int sound_pressure_set_callback(subscribe_cb callback, void* callback_args) {
  subscribe_return_t sv = subscribe2(DRIVER_NUM_SOUND_PRESSURE, 0, callback, callback_args);
  if (sv.success == 0) {
    return tock_error_to_rcode(sv.error);
  }
  return TOCK_SUCCESS;
}

int sound_pressure_read(void) {
  syscall_return_t res = command2(DRIVER_NUM_SOUND_PRESSURE, 1, 0, 0);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return res.data[0];
  } else {
    return tock_error_to_rcode (res.data[0]);
  }
}

// enable sound pressure sensor
int sound_pressure_enable(void) {
  syscall_return_t res = command2(DRIVER_NUM_SOUND_PRESSURE, 2, 0, 0);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode (res.data[0]);
  }
}

// disable sound pressure sensor
int sound_pressure_disable(void) {
  syscall_return_t res = command2(DRIVER_NUM_SOUND_PRESSURE, 3, 0, 0);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode (res.data[0]);
  }
}

int sound_pressure_read_sync(unsigned char* sound_pressure) {
  int err;
  result.fired = false;

  err = sound_pressure_set_callback(cb, (void*) &result);
  if (err < 0) return err;

  err = sound_pressure_read();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *sound_pressure = result.temp;

  return 0;
}

