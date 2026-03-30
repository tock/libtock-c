#include <libtock/defer.h>
#include <libtock/peripherals/syscalls/adc_syscalls.h>

#include "adc.h"

#include "syscalls/adc_syscalls.h"

bool libtocksync_adc_exists(void) {
  return libtock_adc_driver_exists();
}

returncode_t libtocksync_adc_sample(uint8_t channel, uint16_t* sample) {
  returncode_t err;

  err = libtock_adc_command_single_sample(channel);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_adc_yield_wait_for_single_sample(sample);
  return err;
}

returncode_t libtocksync_adc_sample_buffer(uint8_t channel, uint32_t frequency, uint16_t* buffer, uint32_t length) {
  returncode_t err;
  uint32_t actual_length;

  err = libtock_adc_set_readwrite_allow_set_buffer((uint8_t*) buffer, length * 2);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_adc_set_readwrite_allow_set_buffer(NULL, 0);
  }

  err = libtock_adc_command_buffered_sample(channel, frequency);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_adc_yield_wait_for_buffered_sample(&actual_length);
  return err;
}
