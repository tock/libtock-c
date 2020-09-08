#pragma once

#include "tock.h"

#define DRIVER_NUM_PROXIMITY 0x60003

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
int proximity_read_sync(unsigned*);

// Return prox value on interrupt async
int proximity_read_on_interrupt(uint8_t lower, uint8_t upper);

// Return prox value on interrupt sync
int proximity_read_on_interrupt_sync(unsigned* proximity);

// Set thresholds for interrupts (no syscalls involved)
int proximity_set_interrupt_thresholds(uint8_t lower, uint8_t upper);


#ifdef _cplusplus
}
#endif