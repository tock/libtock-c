#include "max17205.h"
#include "tock.h"

struct max17205_data {
  int rc;
  int value0;
  int value1;
  bool fired;
};

static struct max17205_data result = { .fired = false, .rc = 0, .value0 = 0, .value1 = 0 };
static subscribe_upcall* user_cb   = NULL;

// Internal callback for faking synchronous reads
static void internal_user_cb(int return_code,
                             int value0,
                             int value1,
                             void* ud) {

  struct max17205_data* data = (struct max17205_data*) ud;
  data->rc     = return_code;
  data->value0 = value0;
  data->value1 = value1;
  data->fired  = true;
}

static bool is_busy = false;
// Lower level CB that allows us to stop more commands while busy
static void max17205_cb(int return_code,
                        int value0,
                        int value1,
                        void* ud) {
  is_busy = false;
  if (user_cb) {
    user_cb(return_code, value0, value1, ud);
  }
}

int max17205_set_callback (subscribe_upcall callback, void* callback_args) {
  // Set the user-level calllback to the provided one
  user_cb = callback;

  // Subscribe to the callback with our lower-layer callback, but pass
  // callback arguments.
  subscribe_return_t sval = subscribe2(DRIVER_NUM_MAX17205, 0, max17205_cb, callback_args);
  if (sval.success) {
    return 0;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int max17205_read_status(void) {
  if (is_busy) {
    return TOCK_EBUSY;
  } else {
    is_busy = true;
    syscall_return_t com = command2(DRIVER_NUM_MAX17205, 1, 0, 0);
    if (com.type == TOCK_SYSCALL_SUCCESS) {
      return TOCK_SUCCESS;
    } else if (com.type > TOCK_SYSCALL_SUCCESS) {
      // Returned an incorrect success code
      is_busy = false;
      return TOCK_FAIL;
    } else {
      is_busy = false;
      return tock_error_to_rcode(com.data[0]);
    }
  }
}

int max17205_read_soc(void) {
  if (is_busy) {
    return TOCK_EBUSY;
  } else {
    is_busy = true;
    syscall_return_t com = command2(DRIVER_NUM_MAX17205, 2, 0, 0);
    if (com.type == TOCK_SYSCALL_SUCCESS) {
      return TOCK_SUCCESS;
    } else if (com.type > TOCK_SYSCALL_SUCCESS) {
      // Returned an incorrect success code
      is_busy = false;
      return TOCK_FAIL;
    } else {
      is_busy = false;
      return tock_error_to_rcode(com.data[0]);
    }
  }
}

int max17205_read_voltage_current(void) {
  if (is_busy) {
    return TOCK_EBUSY;
  } else {
    is_busy = true;
    syscall_return_t com = command2(DRIVER_NUM_MAX17205, 3, 0, 0);
    if (com.type == TOCK_SYSCALL_SUCCESS) {
      return TOCK_SUCCESS;
    } else if (com.type > TOCK_SYSCALL_SUCCESS) {
      // Returned an incorrect success code
      is_busy = false;
      return TOCK_FAIL;
    } else {
      is_busy = false;
      return tock_error_to_rcode(com.data[0]);
    }
  }
}

int max17205_read_coulomb(void) {
  if (is_busy) {
    return TOCK_EBUSY;
  } else {
    is_busy = true;
    syscall_return_t com = command2(DRIVER_NUM_MAX17205, 4, 0, 0);
    if (com.type == TOCK_SYSCALL_SUCCESS) {
      return TOCK_SUCCESS;
    } else if (com.type > TOCK_SYSCALL_SUCCESS) {
      // Returned an incorrect success code
      is_busy = false;
      return TOCK_FAIL;
    } else {
      is_busy = false;
      return tock_error_to_rcode(com.data[0]);
    }
  }
}

int max17205_read_rom_id(void) {
  if (is_busy) {
    return TOCK_EBUSY;
  } else {
    is_busy = true;
    syscall_return_t com = command2(DRIVER_NUM_MAX17205, 5, 0, 0);
    if (com.type == TOCK_SYSCALL_SUCCESS) {
      return TOCK_SUCCESS;
    } else if (com.type > TOCK_SYSCALL_SUCCESS) {
      // Returned an incorrect success code
      is_busy = false;
      return TOCK_FAIL;
    } else {
      is_busy = false;
      return tock_error_to_rcode(com.data[0]);
    }
  }
}

int max17205_read_status_sync(uint16_t* status) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_cb, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_status();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *status = result.value0 & 0xFFFF;

  return result.rc;
}

int max17205_read_soc_sync(uint16_t* percent, uint16_t* soc_mah, uint16_t* soc_mah_full) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_cb, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_soc();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *percent      = result.value0 & 0xFFFF;
  *soc_mah      = (result.value1 & 0xFFFF0000) >> 16;
  *soc_mah_full = result.value1 & 0xFFFF;

  return result.rc;
}

int max17205_read_voltage_current_sync(uint16_t* voltage, int16_t* current) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_cb, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_voltage_current();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *voltage = result.value0 & 0xFFFF;
  *current = result.value1 & 0xFFFF;

  return result.rc;
}

int max17205_read_coulomb_sync(uint16_t* coulomb) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_cb, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_coulomb();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *coulomb = result.value0 & 0xFFFF;

  return result.rc;
}

int max17205_read_rom_id_sync(uint64_t* rom_id) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_cb, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_rom_id();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  uint64_t temp = result.value0;
  temp  <<= 32;
  temp   |= result.value1 & 0x00000000FFFFFFFF;
  *rom_id = temp;

  return result.rc;
}

float max17205_get_voltage_mV(int vcount) {
  return vcount * 1.25;
}

float max17205_get_current_uA(int ccount) {
  return ccount * 108;
}

float max17205_get_percentage_mP(int percent) {
  return ((float)percent / 26000.0) * 100000.0;
}

float max17205_get_capacity_uAh(int cap) {
  return (float)cap * (5.0 / .01);
}
