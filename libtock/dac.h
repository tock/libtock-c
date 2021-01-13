#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_DAC 0x6

// Initialize and enable the DAC.
syscall_return_t dac_initialize(void);

// Set the DAC to a value.
syscall_return_t dac_set_value(uint32_t value);

#ifdef __cplusplus
}
#endif
