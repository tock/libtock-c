#pragma once

#include "tock.h"


#define DRIVER_NUM_PROXIMITY 0x60002  // Need to change this

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
int proximity_read_on_interrupt(void);
int proximity_read_on_interrupt_sync(unsigned* proximity);
int proximity_set_gain(uint8_t);
int proximity_set_gain_sync(uint8_t);
int proximity_set_interrupt_thresholds(uint8_t , uint8_t);
int proximity_set_interrupt_thresholds_sync(uint8_t , uint8_t);





#ifdef _cplusplus
}
#endif