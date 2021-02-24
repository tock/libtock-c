#include "lps25hb.h"
#include "tock.h"

struct lps25hb_data {
  bool fired;
  int value;
};

static struct lps25hb_data result = { .fired = false };

// Internal callback for faking synchronous reads
static void lps25hb_cb(int value,
                       __attribute__ ((unused)) int unused1,
                       __attribute__ ((unused)) int unused2,
                       void* ud) {
  struct lps25hb_data* data = (struct lps25hb_data*) ud;
  data->value = value;
  data->fired = true;
}

int lps25hb_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe2(DRIVER_NUM_LPS25HB, 0, callback, callback_args);
  if (sval.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int lps25hb_get_pressure (void) {
  syscall_return_t com = command2(DRIVER_NUM_LPS25HB, 1, 0, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[0]);
  }
}

int lps25hb_get_pressure_sync (void) {
  int err;
  result.fired = false;

  err = lps25hb_set_callback(lps25hb_cb, (void*) &result);
  if (err < 0) return err;

  err = lps25hb_get_pressure();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}
