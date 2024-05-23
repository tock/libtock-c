#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ADC 0x5

bool libtock_adc_exists(void);

returncode_t libtock_adc_set_upcall(subscribe_upcall callback, void* opaque);

returncode_t libtock_adc_set_readwrite_allow_set_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_adc_set_readwrite_allow_set_double_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_adc_command_channel_count(int* count);

returncode_t libtock_adc_command_single_sample(uint8_t channel);

returncode_t libtock_adc_command_continuous_sample(uint8_t channel, uint32_t frequency);

returncode_t libtock_adc_command_buffered_sample(uint8_t channel, uint32_t frequency);

returncode_t libtock_adc_command_continuous_buffered_sample(uint8_t channel, uint32_t frequency);

returncode_t libtock_adc_command_stop_sampling(void);

returncode_t libtock_adc_command_get_reference_voltage(uint32_t* reference_voltage);

returncode_t libtock_adc_command_get_resolution_bits(uint32_t* resolution);

#ifdef __cplusplus
}
#endif
