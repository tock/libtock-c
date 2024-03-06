#include <chips/lps25hb.h>
#include <tock.h>

struct lps25hb_data {
  bool fired;
  int value;
};

static struct lps25hb_data result = { .fired = false };

// Internal callback for faking synchronous reads
static void lps25hb_callback(returncode_t ret, int value) {
  result.fired = true;
  result.value = value;
}

returncode_t libtocksync_lps25hb_get_pressure(int* pressure) {
  returncode_t err;
  result.fired = false;

  err = libtock_lps25hb_get_pressure(libtock_lps25hb_get_pressure);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *pressure = result.value;

  return RETURNCODE_SUCCESS;
}
