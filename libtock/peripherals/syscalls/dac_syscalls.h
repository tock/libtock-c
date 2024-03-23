#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_DAC 0x6

// Check if the DAC driver exists.
bool libtock_dac_exists(void);

// Command to initialize the digital-to-analog converter (DAC).
returncode_t libtock_dac_command_initialize(void);

// Set the level of the DAC.
returncode_t libtock_dac_command_set_value(uint32_t value);

#ifdef __cplusplus
}
#endif

