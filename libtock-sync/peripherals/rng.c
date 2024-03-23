#include <peripherals/rng.h>
#include <tock.h>

struct rng_data {
  bool fired;
  returncode_t ret;
  int received;
};

// Global state for faking synchronous reads using a callback and yield.
static struct rng_data result = { .fired = false };



static void rng_cb(returncode_t ret, int received) {
  result.fired    = true;
  result.ret      = ret;
  result.received = received;
}



returncode_t libtocksync_rng_get_random_bytes(uint8_t* buf, uint32_t len, uint32_t num, int* num_received) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_rng_get_random_bytes(buf, len, num, rng_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);

  ret = libtock_usb_keyboard_hid_set_readwrite_allow_send_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *num_received = result.received;

  return RETURNCODE_SUCCESS;
}
