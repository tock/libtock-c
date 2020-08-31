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

int proximity_read(void);
int proximity_read_sync(unsigned*);
int proximity_read_on_interrupt(uint8_t lower, uint8_t upper);
int proximity_read_on_interrupt_sync(unsigned* proximity);
int proximity_set_interrupt_thresholds(uint8_t lower, uint8_t upper);


#ifdef _cplusplus
}
#endif