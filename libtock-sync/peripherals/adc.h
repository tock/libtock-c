#pragma once

#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_adc_exists(void);

// Query the number of ADC channels available.
returncode_t libtocksync_adc_channel_count(int* count);

// Query the ADC reference voltage in millivolts. Returns 0 if not available.
returncode_t libtocksync_adc_reference_voltage(uint32_t* reference_voltage);

// Query the ADC resolution in bits.
returncode_t libtocksync_adc_resolution_bits(uint32_t* resolution);

returncode_t libtocksync_adc_sample(uint8_t channel, uint16_t* sample);

returncode_t libtocksync_adc_sample_buffer(uint8_t channel, uint32_t frequency, uint16_t* buffer, uint32_t length);

#ifdef __cplusplus
}
#endif
