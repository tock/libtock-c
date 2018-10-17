#include "i2c_master.h"
#include "tock.h"
#include <stdlib.h>

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#define ALLOW_NUM                 (1)
#define SUBSCRIBE_NUM             (1)
#define DRIVER_NUM_I2CMASTER      (0x80040006)

enum cmd {
  WRITE_CMD = 1,
  READ_CMD = 2,
  WRITE_READ_CMD = 3
};

struct i2c_master_slave_data {
  bool fired;
  int callback_type;
  int length;
};

static struct i2c_master_slave_data result = { .fired = false };

static void i2c_master_cb(int driver,
                          int subscribe,
                          __attribute__ ((unused)) int unused,
                          void* user_data) {
  (void) driver;
  (void) subscribe;
  (void) user_data;
  result.fired = true;
}

int i2c_master_read(uint8_t address, uint8_t * buffer, uint8_t rlen) {
  int err;

  err = allow(DRIVER_NUM_I2CMASTER, ALLOW_NUM, buffer, rlen);
  if (err < 0) return err;

  result.fired = false;
  err = subscribe(DRIVER_NUM_I2CMASTER, SUBSCRIBE_NUM, &i2c_master_cb, NULL);
  if (err < 0) return err;

  err = command(DRIVER_NUM_I2CMASTER, READ_CMD, (uint32_t) address, (uint32_t) rlen);
  if (err < 0) return err;

  yield_for(&result.fired);
  return result.length;
}

int i2c_master_write(uint8_t address, uint8_t * buffer, uint8_t wlen) {
  int err;

  err = allow(DRIVER_NUM_I2CMASTER, ALLOW_NUM, buffer, wlen);
  if (err < 0) return err;

  result.fired = false;
  err = subscribe(DRIVER_NUM_I2CMASTER, SUBSCRIBE_NUM, &i2c_master_cb, NULL);
  if (err < 0) return err;

  err = command(DRIVER_NUM_I2CMASTER, WRITE_CMD, (uint32_t) address, (uint32_t) wlen);
  if (err < 0) return err;

  yield_for(&result.fired);
  return result.length;
}

int i2c_master_write_read(uint8_t address, uint8_t * buffer, uint8_t wlen, uint8_t rlen) {
  int err;

  err = allow(DRIVER_NUM_I2CMASTER, ALLOW_NUM, buffer, max(wlen, rlen));
  if (err < 0) return err;

  result.fired = false;
  err = subscribe(DRIVER_NUM_I2CMASTER, SUBSCRIBE_NUM, &i2c_master_cb, NULL);
  if (err < 0) return err;

  uint32_t a =   ((uint32_t) wlen << 8) | ((uint32_t) address);
  err = command(DRIVER_NUM_I2CMASTER, WRITE_READ_CMD, a, (uint32_t) rlen);
  if (err < 0) return err;

  yield_for(&result.fired);
  return result.length;
}
