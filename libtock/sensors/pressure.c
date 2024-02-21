#include "pressure.h"
#include "tock.h"

struct data {
  bool fired;
  int press;
};

static struct data result = { .fired = false };

// Internal upcall for faking synchronous reads
static void press_upcall(int                          press,
                         __attribute__ ((unused)) int unused,
                         __attribute__ ((unused)) int unused1,
                         void*                        ud) {
  struct data* data = (struct data*) ud;
  data->press = press;
  data->fired = true;
}

bool pressure_exists(void) {
  return driver_exists(DRIVER_NUM_PRESSURE);
}

int pressure_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_PRESSURE, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int pressure_read(void) {
  syscall_return_t cval = command(DRIVER_NUM_PRESSURE, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int pressure_read_sync(int* pressure) {
  int err;
  result.fired = false;

  err = pressure_set_callback(press_upcall, (void*) &result);
  if (err < 0) return err;

  err = pressure_read();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *pressure = result.press;

  return RETURNCODE_SUCCESS;
}
