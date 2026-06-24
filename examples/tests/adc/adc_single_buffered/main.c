#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libtock/interface/console.h>
#include <libtock/peripherals/adc.h>
#include <libtock/tock.h>

// Sample the first channel. On Hail, this is external pin A0 (AD0)
#define ADC_CHANNEL 0

// Sampling frequency
#define ADC_HIGHSPEED_FREQUENCY 44100

// Buffer size
// Given a sampling frequency, we will receive callbacks every
// BUF_SIZE/FREQ seconds. At 44100 Hz and 4410 samples, this is a callback
// every 50 ms
#define BUF_SIZE 2205

// data buffers
static uint16_t sample_buffer1[BUF_SIZE] = {0};


static void single_buffered_sample_cb(uint8_t  channel,
                                      uint32_t length);

static libtock_adc_callbacks callbacks = {
  .single_sample_callback     = NULL,
  .continuous_sample_callback = NULL,
  .buffered_sample_callback   = single_buffered_sample_cb,
  .continuous_buffered_sample_callback = NULL,
};



static void single_buffered_sample_cb(uint8_t  channel,
                                      uint32_t length) {

  // retrieve the filled buffer
  libtock_adc_set_buffer(NULL, 0);

  // calculate and print statistics about the data
  uint32_t sum = 0;
  uint16_t min = 0xFFFF;
  uint16_t max = 0;
  for (uint32_t i = 0; i < length; i++) {
    uint16_t sample = (sample_buffer1[i] * 3300 / 4095);
    sum += sample;
    if (sample < min) {
      min = sample;
    }
    if (sample > max) {
      max = sample;
    }
  }
  printf("Channel: %u\tCount: %d\tAvg: %" PRIu32 "\tMin: %u\tMax: %u\n",
         channel, BUF_SIZE, sum / BUF_SIZE, min, max);

  memset(sample_buffer1, sizeof(sample_buffer1), 0);

  // re-allow the buffer
  libtock_adc_set_buffer(sample_buffer1, BUF_SIZE);

  // start another buffered sample
  int err = libtock_adc_buffered_sample(ADC_CHANNEL, ADC_HIGHSPEED_FREQUENCY, &callbacks);
  if (err < RETURNCODE_SUCCESS) {
    printf("single buffered sample error: %d\n", err);
  }
}

int main(void) {
  int err;
  printf("[Tock] ADC Single Buffered Test\n");

  // check if ADC driver exists
  if (!libtock_adc_exists()) {
    printf("No ADC driver!\n");
    exit(-1);
  }
  int count;
  libtock_adc_channel_count(&count);
  printf("ADC driver exists with %d channels\n", count);

  // set main buffer for ADC samples
  err = libtock_adc_set_buffer(sample_buffer1, BUF_SIZE);
  if (err < RETURNCODE_SUCCESS) {
    printf("set buffer error: %d\n", err);
    exit(-1);
  }

  // start buffered sampling
  printf("Beginning a single buffered sampling on channel %d at %d Hz\n",
         ADC_CHANNEL, ADC_HIGHSPEED_FREQUENCY);
  err = libtock_adc_buffered_sample(ADC_CHANNEL, ADC_HIGHSPEED_FREQUENCY, &callbacks);
  if (err < RETURNCODE_SUCCESS) {
    printf("single buffered sample error: %d\n", err);
    exit(-1);
  }

  // loop forever
  while (1) {
    yield();
  }

  return 0;
}
