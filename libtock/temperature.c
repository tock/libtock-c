#include "temperature.h"
#include "tock.h"

struct data {
  bool fired;
  int temp;
};

static struct data result = { .fired = false };

// Internal upcall  for faking synchronous reads
static void upcall(int temp,
               __attribute__ ((unused)) int unused,
               __attribute__ ((unused)) int unused1,
               void* ud) {
  struct data* data = (struct data*) ud;
  data->temp  = temp;
  data->fired = true;
}

int temperature_set_upcall(subscribe_cb callback, void* callback_args) {
  
  subscribe_return_t sval = subscribe2(DRIVER_NUM_TEMPERATURE, 0, callback, callback_args);
  if (sval.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int temperature_read(void) {
  syscall_return_t sval = command2(DRIVER_NUM_TEMPERATURE, 1, 0, 0);
  if (sval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (sval.type == TOCK_SYSCALL_FAILURE) {
    return tock_error_to_rcode(sval.data[0]);
  } else {
    return TOCK_EBADRVAL;
  }
}

int temperature_read_sync(int* temperature) {
  int err;
  result.fired = false;

  err = temperature_set_upcall(upcall, (void*) &result);
  if (err < 0) return err;

  err = temperature_read();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *temperature = result.temp;

  return 0;
}

