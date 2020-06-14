#include "lsm303dlhc.h"

// struct to hold values send with the callback
typedef struct l3gd20dlhc_response {
  int data1;
  int data2;
  int data3;
  bool done;  // the callback has been called
} LSM303DLHCResponse;

static void command_callback_yield (int data1, int data2, int data3, void* ud) {
  LSM303DLHCResponse *response = (LSM303DLHCResponse*)ud;
  if (response) {
    response->data1 = data1;
    response->data2 = data2;
    response->data3 = data3;
    response->done  = true;
  }
}


static int lsm303dlhc_subscribe (subscribe_cb cb, void *userdata) {
  return subscribe(DRIVER_NUM_LSM303DLHC, 0, cb, userdata);
}

static int lsm303dlhc_command (uint32_t command_num, uint32_t data1, uint32_t data2) {
  return command(DRIVER_NUM_LSM303DLHC, command_num, data1, data2);
}

// Accelerometer Scale Factor
// Manual table 27, page 27
const float SCALE_FACTOR[4] = {
  2.0 / 32768.0,
  4.0 / 32768.0,
  8.0 / 32768.0,
  16.0 / 32768.0
};

// Magnetometer Range Factor
// Manual table 75, page 38
const int RANGE_FACTOR_X_Y[8] = {
  1000, // placeholder
  1100,
  855,
  670,
  450,
  400,
  330,
  230,
};

// Magnetometer Range Factor
// Manual table 75, page 38
const int RANGE_FACTOR_Z[8] = {
  1000, // placeholder
  980,
  760,
  600,
  400,
  355,
  295,
  205
};

unsigned char scale_factor = 0;
unsigned char range_factor = 0;
int temp_offset = LSM303DLHC_TEMPERATURE_OFFSET;

bool lsm303dlhc_is_present (void) {
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;
  // subscribe
  lsm303dlhc_subscribe (command_callback_yield, &response);
  if (lsm303dlhc_command (1, 0, 0) == TOCK_SUCCESS) {
    yield_for (&(response.done));
  }
  return response.data1 ? true : false;
}

bool lsm303dlhc_set_power_mode (unsigned char power_mode, bool low_power) {
  int evalue;
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;
  // subscribe
  lsm303dlhc_subscribe (command_callback_yield, &response);
  evalue = lsm303dlhc_command (2, power_mode, low_power ? 1 : 0);
  if (evalue == TOCK_SUCCESS) {
    yield_for (&(response.done));
  }
  return response.data1 ? true : false;
}

bool lsm303dlhc_set_accelerometer_scale_and_resolution (unsigned char scale, bool high_resolution) {
  if (scale > 3) scale = 3;
  int evalue;
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;
  // subscribe
  lsm303dlhc_subscribe (command_callback_yield, &response);
  evalue = lsm303dlhc_command (3, scale, high_resolution ? 1 : 0);
  if (evalue == TOCK_SUCCESS) {
    yield_for (&(response.done));
    if (response.data1 == 1) {
      scale_factor = scale;
    }
  }
  return response.data1 ? true : false;
}

bool lsm303dlhc_set_temperature_and_magnetometer_rate (bool temperature, unsigned char rate) {
  int evalue;
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;
  // subscribe
  lsm303dlhc_subscribe (command_callback_yield, &response);
  evalue = lsm303dlhc_command (4, rate, temperature ? 1 : 0);
  if (evalue == TOCK_SUCCESS) {
    yield_for (&(response.done));
  }
  return response.data1 ? true : false;
}
bool lsm303dlhc_set_magnetometer_range (unsigned char range) {
  if (range > 7) range = 7;
  int evalue;
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;
  // subscribe
  lsm303dlhc_subscribe (command_callback_yield, &response);
  evalue = lsm303dlhc_command (5, range, 0);
  if (evalue == TOCK_SUCCESS) {
    yield_for (&(response.done));
    if (response.data1 == 1) {
      range_factor = range;
    }
  }
  return response.data1 ? true : false;
}

int lsm303dlhc_read_acceleration_xyz (LSM303DLHCXYZ *xyz) {
  int evalue;
  LSM303DLHCResponse response;
  response.done = false;
  // subscribe
  lsm303dlhc_subscribe (command_callback_yield, &response);
  evalue = lsm303dlhc_command (6, 0, 0);
  if (evalue == TOCK_SUCCESS) {
    yield_for (&(response.done));
    if (xyz != NULL) {
      xyz->x = (float)response.data1 * SCALE_FACTOR[scale_factor];
      xyz->y = (float)response.data2 * SCALE_FACTOR[scale_factor];
      xyz->z = (float)response.data3 * SCALE_FACTOR[scale_factor];
    }
  }
  return evalue;
}

int lsm303dlhc_read_temperature (float *temperature) {
  int evalue;
  LSM303DLHCResponse response;
  response.done = false;
  // subscribe
  lsm303dlhc_subscribe (command_callback_yield, &response);
  evalue = lsm303dlhc_command (7, 0, 0);
  if (evalue == TOCK_SUCCESS) {
    yield_for (&(response.done));
    if (temperature != NULL) {
      *temperature = (float)response.data1 / 8 + temp_offset;
    }
  }
  return evalue;
}
int lsm303dlhc_read_magnetometer_xyz (LSM303DLHCXYZ *xyz) {
  int evalue;
  LSM303DLHCResponse response;
  response.done = false;
  // subscribe
  lsm303dlhc_subscribe (command_callback_yield, &response);
  evalue = lsm303dlhc_command (8, 0, 0);
  if (evalue == TOCK_SUCCESS) {
    yield_for (&(response.done));
    if (xyz != NULL) {
      printf ("x %d range %d z %d\r\n", response.data1, RANGE_FACTOR_X_Y[range_factor], response.data3);
      xyz->x = (float)response.data1 / RANGE_FACTOR_X_Y[range_factor];
      xyz->y = (float)response.data2 / RANGE_FACTOR_X_Y[range_factor];
      xyz->z = (float)response.data3 / RANGE_FACTOR_Z[range_factor];
    }
  }
  return evalue;
}
