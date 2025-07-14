#include "adc.h"
#include "syscalls/adc_syscalls.h"


// Internal callback for routing to operation-specific callbacks
//
// callback_type - number indicating which type of callback occurred
// arg1, arg2 - meaning varies based on callback_type
// callback_args - user data passed into the set_callback function
//
// Possible callbacks
// SingleSample: single sample operation is complete
//      arg1 - channel number that collected sample corresponds to
//      arg2 - sample value
// MultipleSample: sampling a buffer worth of data is complete
//      arg1 - channel in lower 8 bits,
//             number of samples collected in upper 24 bits
//      arg2 - pointer to buffer filled with samples
// ContinuousSample: a buffer of sample data is ready
//      arg1 - channel in lower 8 bits,
//             number of samples collected in upper 24 bits
//      arg2 - pointer to buffer filled with samples
static void adc_routing_upcall(int   callback_type,
                               int   arg1,
                               int   arg2,
                               void* opaque) {
  libtock_adc_callbacks* callbacks = (libtock_adc_callbacks*) opaque;

  switch (callback_type) {
    case libtock_adc_SingleSample:
      if (callbacks->single_sample_callback) {
        uint8_t channel = (uint8_t)arg1;
        uint16_t sample = (uint16_t)arg2;
        callbacks->single_sample_callback(channel, sample);
      }
      break;

    case libtock_adc_ContinuousSample:
      if (callbacks->continuous_sample_callback) {
        uint8_t channel = (uint8_t)arg1;
        uint16_t sample = (uint16_t)arg2;
        callbacks->continuous_sample_callback(channel, sample);
      }
      break;

    case libtock_adc_SingleBuffer:
      if (callbacks->buffered_sample_callback) {
        uint8_t channel  = (uint8_t)(arg1 & 0xFF);
        uint32_t length  = ((arg1 >> 8) & 0xFFFFFF);
        uint16_t* buffer = (uint16_t*)arg2;
        callbacks->buffered_sample_callback(channel, length, buffer);
      }
      break;

    case libtock_adc_ContinuousBuffer:
      if (callbacks->continuous_buffered_sample_callback) {
        uint8_t channel  = (uint8_t)(arg1 & 0xFF);
        uint32_t length  = ((arg1 >> 8) & 0xFFFFFF);
        uint16_t* buffer = (uint16_t*)arg2;
        callbacks->continuous_buffered_sample_callback(channel, length, buffer);
      }
      break;
  }
}


bool libtock_adc_exists(void) {
  return libtock_adc_driver_exists();
}

returncode_t libtock_adc_set_buffer(uint16_t* buffer, uint32_t length) {
  return libtock_adc_set_readwrite_allow_set_buffer((uint8_t*) buffer, length * 2);
}

returncode_t libtock_adc_set_double_buffer(uint16_t* buffer, uint32_t length) {
  return libtock_adc_set_readwrite_allow_set_double_buffer((uint8_t*) buffer, length * 2);
}

returncode_t libtock_adc_channel_count(int* count) {
  return libtock_adc_command_channel_count(count);
}

returncode_t libtock_adc_single_sample(uint8_t channel, libtock_adc_callbacks* callbacks) {
  returncode_t ret = libtock_adc_set_upcall(adc_routing_upcall, callbacks);
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtock_adc_command_single_sample(channel);
}

returncode_t libtock_adc_continuous_sample(uint8_t channel, uint32_t frequency, libtock_adc_callbacks* callbacks) {
  returncode_t ret = libtock_adc_set_upcall(adc_routing_upcall, callbacks);
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtock_adc_command_continuous_sample(channel, frequency);
}

returncode_t libtock_adc_buffered_sample(uint8_t channel, uint32_t frequency, libtock_adc_callbacks* callbacks) {
  returncode_t ret = libtock_adc_set_upcall(adc_routing_upcall, callbacks);
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtock_adc_command_buffered_sample(channel, frequency);
}

returncode_t libtock_adc_continuous_buffered_sample(uint8_t channel, uint32_t frequency,
                                                    libtock_adc_callbacks* callbacks) {
  returncode_t ret = libtock_adc_set_upcall(adc_routing_upcall, callbacks);
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtock_adc_command_continuous_buffered_sample(channel, frequency);
}

returncode_t libtock_adc_stop_sampling(void) {
  return libtock_adc_command_stop_sampling();
}
