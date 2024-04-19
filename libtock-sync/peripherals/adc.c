#include <stdint.h>
#include <stdio.h>

#include "adc.h"
#include "tock.h"

// used for creating synchronous versions of functions
//
// fired - set when the callback has been called
// channel - channel that the collected sample corresponds to
// sample - collected sample value, valid if single sample operation
// length - number of collected sample values, valid if multiple sample
//          operation
// buffer - pointer to buffer filled with samples, valid if multiple sample
//          operation
// error - set to FAIL if an invalid callback type is detected
struct adc_data {
  bool fired;
  uint8_t channel;
  uint16_t sample;
  uint32_t length;
  uint16_t* buffer;
  int error;
};

static struct adc_data result;


static void sample(uint8_t channel, uint16_t sample) {
  result.fired   = true;
  result.channel = channel;
  result.sample  = sample;
}

static void buffered_sample(uint8_t channel, uint32_t length, uint16_t* buffer) {
  result.fired   = true;
  result.channel = channel;
  result.length  = length;
  result.buffer  = buffer;
}


static libtock_adc_callbacks callbacks = {
  .single_sample_callback     = sample,
  .continuous_sample_callback = sample,
  .buffered_sample_callback   = buffered_sample,
  .continuous_buffered_sample_callback = buffered_sample,
};



returncode_t libtocksync_adc_sample(uint8_t channel, uint16_t* sample) {
  int err;
  result.fired = false;
  result.error = RETURNCODE_SUCCESS;

  err = libtock_adc_single_sample(channel, &callbacks);
  if (err != RETURNCODE_SUCCESS) return err;

  // wait for callback
  yield_for(&result.fired);

  // copy over result
  *sample = result.sample;

  return result.error;
}

returncode_t libtocksync_adc_sample_buffer(uint8_t channel, uint32_t frequency, uint16_t* buffer, uint32_t length) {
  returncode_t err;
  result.fired = false;
  result.error = RETURNCODE_SUCCESS;

  err = libtock_adc_set_buffer(buffer, length);
  if (err < RETURNCODE_SUCCESS) return err;

  err = libtock_adc_buffered_sample(channel, frequency, &callbacks);
  if (err != RETURNCODE_SUCCESS) return err;

  // wait for callback
  yield_for(&result.fired);

  // copy over result
  if (result.buffer != buffer) {
    return RETURNCODE_FAIL;
  }

  return result.error;
}
