#pragma once

#include <libtock/sensors/proximity.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_proximity_exists(void);

// Read proximity synchronously.
//
// This function queries the sensor for a proximity reading which is then
// returned via the callback. A proximity value is in the range [0,255] where
// '255' indicates the closest measurable distance and '0' that no object is
// detected.
returncode_t libtocksync_proximity_read(uint8_t* proximity);

// Return proximity value on interrupt synchronously.
//
// This function can be used to wait for the sensor to detect a proximity
// reading in the user-specified range. This range is determined by the
// `proximity_set_interrupt_thresholds` arguments. A proximity value is in the
// range [0,255] where '255' indicates the closest measurable distance and '0'
// that no object is detected.
returncode_t libtocksync_proximity_read_on_interrupt(uint32_t lower_threshold, uint32_t higher_threshold,
                                                     uint8_t* proximity);

#ifdef __cplusplus
}
#endif
