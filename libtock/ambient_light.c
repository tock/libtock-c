#include "ambient_light.h"
#include "tock.h"

typedef struct {
  int intensity;
  bool fired;
} ambient_light_data_t;

// internal callback for faking synchronous reads
static void ambient_light_cb(int intensity,
                             __attribute__ ((unused)) int unused1,
                             __attribute__ ((unused)) int unused2, void* ud) {
  ambient_light_data_t* result = (ambient_light_data_t*)ud;
  result->intensity = intensity;
  result->fired     = true;
}

int ambient_light_read_intensity_sync(int* lux_value) {
  int err;
  ambient_light_data_t result = {0};
  result.fired = false;

  err = ambient_light_subscribe(ambient_light_cb, (void*)(&result));
  if (err != TOCK_SUCCESS) {
    return err;
  }

  err = ambient_light_start_intensity_reading();
  if (err != TOCK_SUCCESS) {
    return err;
  }
  
  yield_for(&result.fired);

  *lux_value = result.intensity;

  return TOCK_SUCCESS;
}

int ambient_light_subscribe(subscribe_cb callback, void* userdata) {
  subscribe_return_t sval = subscribe2(DRIVER_NUM_AMBIENT_LIGHT, 0, callback, userdata);
  if (!sval.success) {
    return tock_error_to_rcode(sval.error);
  } else {
    return TOCK_SUCCESS;
  }
}

int ambient_light_start_intensity_reading(void) {
  syscall_return_t rval = command2(DRIVER_NUM_AMBIENT_LIGHT, 1, 0, 0);
  if (rval.type > TOCK_SYSCALL_SUCCESS) { // Wrong success tpe
    return TOCK_FAIL;
  } else if (rval.type < TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(rval.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}

