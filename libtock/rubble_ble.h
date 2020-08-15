#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *   DRIVER DEFINITIONS   -- corresponds to different system calls
 *
 *    *_CMD   - command call
 *    *_ALLOW - allow call
 *    *_SUB   - subscribe call
 *
 ******************************************************************************/

#define RUBBLE_DRIVER_NUMBER 0x30003

#define RUBBLE_ADV_START_CMD 0

#define RUBBLE_ADV_BUF_ALLOW 0

// start advertising
//
// advd               - The advertising data. Must be formatted properly
// len                - Length of the advertising data
// interval           - The advertising interval in milliseconds
int rubble_start_advertising(uint8_t* advd, int len, uint16_t interval);

#ifdef __cplusplus
}
#endif
