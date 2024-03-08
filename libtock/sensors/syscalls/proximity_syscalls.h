#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_PROXIMITY 0x60005

// Check if the proximity driver exists.
bool libtock_proximity_exists(void);

// Configure the upcall for the driver.
returncode_t libtock_proximity_set_upcall(subscribe_upcall upcall, void *opaque);

// Read the proximity sensor.
returncode_t libtock_proximity_command_read(void);

// Configure the proximity sensor to use interrupts.
returncode_t libtock_proximity_command_read_on_interrupt(uint32_t lower_threshold, uint32_t higher_threshold);

#ifdef __cplusplus
}
#endif
