#pragma once

#include <libtock/sensors/distance.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Read the distance sensor synchronously.
//
// ## Arguments
//
// - `distance`: Set to the distance value in millimeters.
//
// ## Return Value
//
// A returncode indicating whether the distance read was completed
// successfully.
returncode_t libtocksync_distance_read(int* distance);

// Get the minimum measurable distance.
//
// ## Return Value
//
// The minimum measurable distance in millimeters.
int libtocksync_distance_get_minimum_distance(void);

// Get the maximum measurable distance.
//
// ## Return Value
//
// The maximum measurable distance in millimeters.
int libtocksync_distance_get_maximum_distance(void);

#ifdef __cplusplus
}
#endif
