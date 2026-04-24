#pragma once

#include <libtock/peripherals/syscalls/adc_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a single ADC sample to complete.
returncode_t libtocksync_adc_yield_wait_for_single_sample(uint16_t* sample);

// Wait for a buffered ADC sample to complete.
returncode_t libtocksync_adc_yield_wait_for_buffered_sample(uint32_t* length);

#ifdef __cplusplus
}
#endif
