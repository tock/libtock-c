#pragma once

#include "../tock.h"

#ifdef _cplusplus
extern "C" {
#endif

// Function signature for proximity data callback.
//
// - `arg1` (`returncode_t`): Status from sampling the sensor.
// - `arg2` (`uint8_t`): Proximity value in the range [0,255] where '255'
//   indicates the closest measurable distance and '0' that no object is
//   detected.
typedef void (*libtock_proximity_callback)(returncode_t, uint8_t);

// Check if the driver exists.
bool libtock_proximity_exists(void);

// Read proximity asynchronously.
//
// This function queries the sensor for a proximity reading which is then
// returned via the callback.
returncode_t libtock_proximity_read(libtock_proximity_callback cb);

// Read proximity asynchronously within a specified window. This function can be
// used to wait for the sensor to detect a proximity reading in the
// user-specified range.
returncode_t libtock_proximity_read_on_interrupt(uint32_t lower_threshold, uint32_t higher_threshold,
                                                 libtock_proximity_callback cb);

#ifdef _cplusplus
}
#endif
