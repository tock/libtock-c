#include "humidity.h"
#include "tock.h"

struct data {
  bool fired;
  int humidity;
};

static struct data result = { .fired = false };

// Internal upcall for faking synchronous reads
static void humidity_upcall(int humidity,
                            __attribute__ ((unused)) int unused,
                            __attribute__ ((unused)) int unused1,
                            void* ud) {
  struct data* data = (struct data*) ud;
  data->humidity = humidity;
  data->fired    = true;
}

int humidity_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe2(DRIVER_NUM_HUMIDITY, 0, callback, callback_args);
  if (sval.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int humidity_read(void) {
  syscall_return_t sval = command2(DRIVER_NUM_HUMIDITY, 1, 0, 0);
  if (sval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (sval.type == TOCK_SYSCALL_FAILURE) {
    return tock_error_to_rcode(sval.data[0]);
  } else {
    return TOCK_EBADRVAL;
  }
}

int humidity_read_sync(unsigned* humidity) {
  int err;
  result.fired = false;

  err = humidity_set_callback(humidity_upcall, (void*) &result);
  if (err < 0) return err;

  err = humidity_read();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *humidity = result.humidity;

  return 0;
}
