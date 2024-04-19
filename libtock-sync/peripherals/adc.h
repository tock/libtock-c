#pragma once

#include <libtock/tock.h>
#include <libtock/peripherals/adc.h>

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtocksync_adc_sample(uint8_t channel, uint16_t* sample);

returncode_t libtocksync_adc_sample_buffer(uint8_t channel, uint32_t frequency, uint16_t* buffer, uint32_t length);

#ifdef __cplusplus
}
#endif
