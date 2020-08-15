/*
 * Rubble BLE driver functions
 */

#include "rubble_ble.h"
#include "tock.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define _RUBBLE_UNUSED_PARAMETER_VAL 0

int rubble_start_advertising(uint8_t* advd, int len, uint16_t interval) {
  int err = allow(RUBBLE_DRIVER_NUMBER, RUBBLE_ADV_BUF_ALLOW, advd, len);
  if (err < TOCK_SUCCESS)
    return err;

  return command(RUBBLE_DRIVER_NUMBER, RUBBLE_ADV_START_CMD, interval, _RUBBLE_UNUSED_PARAMETER_VAL);
}
