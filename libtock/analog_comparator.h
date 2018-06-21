#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ACIFC 0x7

// Does the driver exist?
int ac_exists(void);

// Initialize and enable the DAC.
int ac_enable(void);

// Compare the voltages of two pins (if one is higher than the other) on the
// corresponding AC, by polling every second.
bool ac_comparison(uint8_t);

// Compare the voltages of three pins (if one is between the other two) on the
// corresponding window, by polling every second.
bool ac_window_comparison(uint8_t);

// Enable interrupts
int ac_enable_interrupts(uint8_t);

// Compare the voltages of two pins (if one is higher than the other) on the
// corresponding AC, by means of receiving an interrupt.
int ac_interrupt_callback(subscribe_cb callback, void* callback_args);

#ifdef __cplusplus
}
#endif
