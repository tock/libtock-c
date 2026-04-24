#include "adc_syscalls.h"

returncode_t libtocksync_adc_yield_wait_for_single_sample(uint16_t* sample) {
  yield_waitfor_return_t ret;
  ret     = yield_wait_for(DRIVER_NUM_ADC, 0);
  *sample = (uint16_t) ret.data2;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_adc_yield_wait_for_buffered_sample(uint32_t* length) {
  yield_waitfor_return_t ret;
  ret     = yield_wait_for(DRIVER_NUM_ADC, 0);
  *length = (ret.data1 >> 8) & 0xFFFFFF;
  return RETURNCODE_SUCCESS;
}
