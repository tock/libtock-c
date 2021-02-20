#include "crc.h"
#include "tock.h"

int crc_exists(void) {
  syscall_return_t ret = command2(DRIVER_NUM_CRC, 0, 0, 0);
  if (ret.type == TOCK_SYSCALL_SUCCESS) {
    return 1;
  } else {
    return 0;
  }
}

int crc_request(enum crc_alg alg) {
  syscall_return_t ret = command2(DRIVER_NUM_CRC, 2, alg, 0);
  if (ret.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    // printf("Failure on crc_request: %s\n", tock_strerr(ret.data[0]));
    return tock_error_to_rcode(ret.data[0]);
  }
}

int crc_subscribe(subscribe_upcall callback, void *ud) {
  subscribe_return_t ret = subscribe2(DRIVER_NUM_CRC, 0, callback, ud);
  if (ret.success) {
    return TOCK_SUCCESS;
  } else {
    // printf("Failure on crc_subscribe: %s\n", tock_strerr(ret.error));
    return tock_error_to_rcode(ret.error);
  }
}

int crc_set_buffer(const void* buf, size_t len) {
  allow_ro_return_t ret =  allow_readonly(DRIVER_NUM_CRC, 0, (void*) buf, len);
  if (ret.success) {
    return TOCK_SUCCESS;
  } else {
    // printf("Failure on crc_set_buffer: %s\n", tock_strerr(ret.error));
    return tock_error_to_rcode(ret.error);
  }
}

struct data {
  bool fired;
  int status;
  uint32_t result;
};

static void callback(int status, int v1, __attribute__((unused)) int v2, void *data)
{
  struct data *d = data;

  d->fired  = true;
  d->status = status;
  d->result = v1;
}

int crc_compute(const void *buf, size_t buflen, enum crc_alg alg, uint32_t *result)
{
  struct data d = { .fired = false };

  crc_set_buffer(buf, buflen);
  crc_subscribe(callback, (void *) &d);
  crc_request(alg);
  yield_for(&d.fired);

  if (d.status == TOCK_SUCCESS)
    *result = d.result;

  return d.status;
}
