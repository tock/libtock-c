#include "tock.h"
#include "tsl2561.h"

struct tsl2561_data {
  bool fired;
  int value;
};

static struct tsl2561_data result = { .fired = false };

// Internal callback for faking synchronous reads
static void tsl2561_upcall(__attribute__ ((unused)) int callback_type,
                       int value,
                       __attribute__ ((unused)) int unused2,
                       void* ud) {
  struct tsl2561_data* data = (struct tsl2561_data*) ud;
  data->value = value;
  data->fired = true;
}

int tsl2561_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe2(DRIVER_NUM_TSL2561, 0, callback, callback_args);
  if (sval.success) {
    return 0;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int tsl2561_get_lux (void) {
  syscall_return_t com = command2(DRIVER_NUM_TSL2561, 1, 0, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[0]);
  }
}

int tsl2561_get_lux_sync (void) {
  int err;
  result.fired = false;

  err = tsl2561_set_callback(tsl2561_upcall, (void*) &result);
  if (err < 0) return err;

  err = tsl2561_get_lux();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}
