#include "adc_syscalls.h"

bool libtock_adc_exists(void) {
  return driver_exists(DRIVER_NUM_ADC);
}

returncode_t libtock_adc_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_ADC, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_adc_set_readwrite_allow_set_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t rval = allow_readwrite(DRIVER_NUM_ADC, 0, buffer, len);
  return tock_allow_rw_return_to_returncode(rval);
}

returncode_t libtock_adc_set_readwrite_allow_set_double_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t rval = allow_readwrite(DRIVER_NUM_ADC, 1, buffer, len);
  return tock_allow_rw_return_to_returncode(rval);
}

returncode_t libtock_adc_command_channel_count(int* count) {
  syscall_return_t cval = command(DRIVER_NUM_ADC, 0, 0, 0);
  return tock_command_return_u32_to_returncode(cval, (uint32_t*) count);
}

returncode_t libtock_adc_command_single_sample(uint8_t channel) {
  syscall_return_t cval = command(DRIVER_NUM_ADC, 1, channel, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_adc_command_continuous_sample(uint8_t channel, uint32_t frequency) {
  syscall_return_t cval = command(DRIVER_NUM_ADC, 2, channel, frequency);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_adc_command_buffered_sample(uint8_t channel, uint32_t frequency) {
  syscall_return_t cval = command(DRIVER_NUM_ADC, 3, channel, frequency);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_adc_command_continuous_buffered_sample(uint8_t channel, uint32_t frequency) {
  syscall_return_t cval = command(DRIVER_NUM_ADC, 4, channel, frequency);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_adc_command_stop_sampling(void) {
  syscall_return_t cval = command(DRIVER_NUM_ADC, 5, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_adc_command_get_reference_voltage(uint32_t* reference_voltage) {
  syscall_return_t cval = command(DRIVER_NUM_ADC, 102, 0, 0);
  return tock_command_return_u32_to_returncode(cval, reference_voltage);
}

returncode_t libtock_adc_command_get_resolution_bits(uint32_t* resolution) {
  syscall_return_t cval = command(DRIVER_NUM_ADC, 101, 0, 0);
  return tock_command_return_u32_to_returncode(cval, resolution);
}
