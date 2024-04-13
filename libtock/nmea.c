#include "nmea.h"
#include "tock.h"

struct data {
  bool fired;
  int len;
};

static struct data result = { .fired = false };

static void nmea_upcall(__attribute__ ((unused)) int status,
                        int                          len,
                        __attribute__ ((unused)) int unused1,
                        void*                        ud) {
  struct data* data = (struct data*) ud;
  data->len   = len;
  data->fired = true;
}

bool nmea_exists(void) {
  return driver_exists(DRIVER_NUM_NMEA);
}

int nmea_set_read_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NMEA, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int nmea_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NMEA, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int nmea_read_sentance(void) {
  syscall_return_t cval = command(DRIVER_NUM_NMEA, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int nmea_read_sentance_sync(uint8_t* buffer, uint16_t len) {
  int err;
  result.fired = false;

  err = nmea_set_read_buffer(buffer, len);
  if (err < 0) return err;

  err = nmea_set_callback(nmea_upcall, (void*) &result);
  if (err < 0) return err;

  err = nmea_read_sentance();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.len;
}
