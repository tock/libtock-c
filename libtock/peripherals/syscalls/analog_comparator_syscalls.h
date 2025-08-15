#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ANALOG_COMPARATOR 0x7

bool libtock_analog_comparator_driver_exists(void);

// Set the upcall function called by the AC when an interrupt is received.
//
// - callback       - pointer to function to be called
// - callback_args  - pointer to data provided to the callback
returncode_t libtock_analog_comparator_set_upcall(subscribe_upcall callback, void* opaque);

returncode_t libtock_analog_comparator_command_comparison(uint8_t channel, uint32_t* comparison);

returncode_t libtock_analog_comparator_command_start_comparing(uint8_t channel);

returncode_t libtock_analog_comparator_command_stop_comparing(uint8_t channel);

returncode_t libtock_analog_comparator_command_count(uint32_t* count);

#ifdef __cplusplus
}
#endif
