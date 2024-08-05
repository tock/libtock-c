#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_DISTANCE 0x60009

// Check if distance sensor exists.
bool libtock_distance_exists(void);

// units: disntance in millimeters.

// Set the callback function to be called when the distance measurement is
// finished.
//
// callback  - pointer to function to be called
// opaque    - pointer to data provided to the callback
returncode_t libtock_distance_set_upcall(subscribe_upcall callback, void* opaque);

// Initiate an ambient distance measurement.
returncode_t libtock_distance_command_read(void);

// Function to get minimum distance
int libtocksync_distance_get_minimum_distance(void);

// Function to get maximum distance
int libtocksync_distance_get_maximum_distance(void);

#ifdef __cplusplus
}
#endif
