#include "i2c_master.h"

#define DRIVER_NUM_I2CMASTER 0x20003

#define TOCK_I2C_MASTER_CB       1
#define TOCK_I2C_MASTER_BUF      1


int i2c_master_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t subval = subscribe(DRIVER_NUM_I2CMASTER, TOCK_I2C_MASTER_CB, callback, callback_args);
  if (subval.success == 0) {
    return tock_error_to_rcode(subval.error);
  } else {
    return TOCK_SUCCESS;
  }
}

int i2c_master_set_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_I2CMASTER, TOCK_I2C_MASTER_BUF, (void*) buffer, len);
  if (aval.success == 0) {
    return tock_error_to_rcode(aval.error);
  } else {
    return TOCK_SUCCESS;
  }
}

int i2c_master_write(uint8_t address, uint8_t len) {
  uint32_t a = (((uint32_t) len) << 16) | address;
  syscall_return_t comval = command(DRIVER_NUM_I2CMASTER, 1, a, len);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}

int i2c_master_read(uint8_t address, uint8_t len) {
  uint32_t a = (((uint32_t) len) << 16) | address;
  syscall_return_t comval = command(DRIVER_NUM_I2CMASTER, 2, a, len);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}
int i2c_master_write_read(uint8_t address, uint8_t len) {
  uint32_t a = (((uint32_t) len) << 16) | address;
  syscall_return_t comval = command(DRIVER_NUM_I2CMASTER, 3, a, len);
  if (comval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(comval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}

static void i2c_callback(__attribute__ ((unused)) int a1,
                         __attribute__ ((unused)) int a2,
                         __attribute__ ((unused)) int unused,
                         void* ud) {
  (*(bool*)ud) = true;
}

int i2c_master_write_sync(uint16_t address, uint8_t* buffer, uint16_t len) {
  bool ready = 0;
  int rval   = i2c_master_set_buffer(buffer, len);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  rval = i2c_master_set_callback(i2c_callback, &ready);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  rval = i2c_master_write(address, len);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  yield_for(&ready);
  return TOCK_SUCCESS;
}

int i2c_master_read_sync(uint16_t address, uint8_t* buffer, uint16_t len) {
  bool ready = 0;
  int rval   = i2c_master_set_buffer(buffer, len);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  rval = i2c_master_set_callback(i2c_callback, &ready);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  rval = i2c_master_read(address, len);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  yield_for(&ready);
  return TOCK_SUCCESS;
}

int i2c_master_write_read_sync(uint16_t address, uint8_t* buffer, uint16_t len) {
  bool ready = 0;
  int rval   = i2c_master_set_buffer(buffer, len);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  rval = i2c_master_set_callback(i2c_callback, &ready);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  rval = i2c_master_write_read(address, len);
  if (rval != TOCK_SUCCESS) {
    return rval;
  }

  yield_for(&ready);
  return TOCK_SUCCESS;
}

