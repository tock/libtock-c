#include <rng.h>
#include <stdlib.h>
#include <tock.h>

struct rng_data {
  bool fired;
  int received;
};

// Global state for faking synchronous reads using a callback and
// yield
static struct rng_data result = { .fired = false, .received = 0 };

// Internal callback for faking synchronous reads
static void rng_upcall(__attribute__ ((unused)) int callback_type,
                       int received,
                       __attribute__ ((unused)) int val2,
                       void* ud) {
  struct rng_data* data = (struct rng_data*) ud;
  data->fired    = true;
  data->received = received;
}

allow_rw_return_t rng_set_buffer(uint8_t* buf, uint32_t len) {
  return allow_readwrite(DRIVER_NUM_RNG, 0, (void*) buf, len);
}

subscribe_return_t rng_set_callback(subscribe_upcall callback, void* callback_args) {
  return subscribe2(DRIVER_NUM_RNG, 0, callback, callback_args);
}

syscall_return_t rng_get_random(int num_bytes) {
  return command2(DRIVER_NUM_RNG, 1, num_bytes, 0);
}

int rng_async(subscribe_upcall callback, uint8_t* buf, uint32_t len, uint32_t num) {
  subscribe_return_t sres = rng_set_callback(callback, NULL);
  if (!sres.success) return tock_error_to_rcode(sres.error);

  allow_rw_return_t ares = rng_set_buffer(buf, len);
  if (!ares.success) return tock_error_to_rcode(ares.error);

  syscall_return_t res = rng_get_random(num);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (res.type == TOCK_SYSCALL_FAILURE) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    // Unexpected return code variant
    exit(-1);
  }
}

int rng_sync(uint8_t* buf, uint32_t len, uint32_t num) {
  allow_rw_return_t ares = rng_set_buffer(buf, len);
  if (!ares.success) return tock_error_to_rcode(ares.error);

  subscribe_return_t sres = rng_set_callback(rng_upcall, (void*) &result);
  if (!sres.success) return tock_error_to_rcode(sres.error);

  result.fired = false;
  syscall_return_t res = rng_get_random(num);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    yield_for(&result.fired);
    return result.received;
  } else if (res.type == TOCK_SYSCALL_FAILURE) {
    // We assume that after an error the callback is not called
    return tock_error_to_rcode(res.data[0]);
  } else {
    // Unexpected return code variant
    exit(-1);
  }
}
