#pragma once

#include "tock.h"

#define DRIVER_NUM_PROXIMITY 0x60005

#ifdef _cplusplus
extern "C" {
#endif

// function to be called when the system call is finished
//
// callback       - pointer to function to be called
// callback_args  - pointer to data provided to the callback
int proximity_set_callback (subscribe_cb callback, void* callback_args);

// Read proximity async
int proximity_read(void);

// Read proximity sync
// This function queries the sensor for a proximity reading which is then returned via the callback.
// A proximity value is of the following range [0,255] where '255' indicates the closest measurable distance and '0' that no object is detected.
int proximity_read_sync(uint8_t* proximity);

// Return prox value on interrupt async
int proximity_read_on_interrupt(uint8_t lower, uint8_t upper);

// Return prox value on interrupt sync
// This function can be used to wait for the sensor to detect a proximity reading in the user-specified range.
// This range is determined by the `proximity_set_interrupt_thresholds` arguments.
int proximity_read_on_interrupt_sync(uint8_t* proximity);

// Set thresholds for interrupts (no syscalls involved)
//
// The proximity sensor fires an interrupt (which executes the callback sent by the `proximity_read_on_interrupt_sync()` function)
// The interrupt is fired when the following logic statement is true `(reading >= upper_threshold) || (reading <= lower_threshold)`
// lower          - lower interrupt threshold for sensor --> range is [0,255]
// upper          - upper interrupt threshold for sensor --> range is [0,255]
int proximity_set_interrupt_thresholds(uint8_t lower, uint8_t upper);


#ifdef _cplusplus
}
#endif