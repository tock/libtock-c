#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Check if the driver exists.
bool libtock_dac_exists(void);

// Initialize the Digital-to-Analog converter.
returncode_t libtock_dac_initialize(void);

// Set the level for the DAC.
returncode_t libtock_dac_set_value(uint32_t value);

#ifdef __cplusplus
}
#endif
