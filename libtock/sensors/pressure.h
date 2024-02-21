#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_PRESSURE 0x60008

// check if pressure sensor exists
bool pressure_exists(void);

// units: pressure in hPa

// function to be called when the pressure measurement is finished
//
// callback       - pointer to function to be called
// callback_args  - pointer to data provided to the callback
int pressure_set_callback (subscribe_upcall callback, void* callback_args);

// initiate a pressure measurement used both for synchronous and asynchronous readings
int pressure_read(void);


// initiate a synchronous pressure measurement
//
// pressure     - pointer/address where the result of the pressure reading should be stored
int pressure_read_sync (int* pressure);

#ifdef __cplusplus
}
#endif
