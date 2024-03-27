#pragma once

#include "tock.h"
#include "syscalls/dac_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the Digital-to-Analog converter.
returncode_t libtock_dac_initialize(void);

// Set the level for the DAC.
returncode_t libtock_dac_set_value(uint32_t value);

#ifdef __cplusplus
}
#endif
