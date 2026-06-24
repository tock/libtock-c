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
#define ADC_LOWSPEED_FREQUENCY 10

static void continuous_sample_cb(uint8_t  channel,
                                 uint16_t sample);

static libtock_adc_callbacks callbacks = {
  .single_sample_callback     = NULL,
  .continuous_sample_callback = continuous_sample_cb,
  .buffered_sample_callback   = NULL,
  .continuous_buffered_sample_callback = NULL,
};



static void continuous_sample_cb(uint8_t  channel,
                                 uint16_t sample) {

  // single ADC sample is ready
  static uint8_t counter = 0;

  // print data
  uint16_t voltage = (uint16_t)(sample * 3300 / 4095);
  printf("Channel: %u\tValue: %u\n", channel, voltage);

  // determine when to switch sampling method
  counter++;
  if (counter == 10) {
    counter = 0;

    // stop single sampling
    int err = libtock_adc_stop_sampling();
    if (err < RETURNCODE_SUCCESS) {
      printf("Failed to stop sampling: %d\n", err);
      return;
    }

    // start buffered sampling
    printf("Beginning continuous sampling on channel %d at %d Hz\n",
           ADC_CHANNEL, ADC_LOWSPEED_FREQUENCY);
    err = libtock_adc_continuous_sample(ADC_CHANNEL, ADC_LOWSPEED_FREQUENCY, &callbacks);
    if (err < RETURNCODE_SUCCESS) {
      printf("continuous sample error: %d\n", err);
      return;
    }
  }
}


int main(void) {
  int err;
  printf("[Tock] ADC Continuous Test\n");

  // check if ADC driver exists
  if (!libtock_adc_exists()) {
    printf("No ADC driver!\n");
    return -1;
  }
  int count;
  libtock_adc_channel_count(&count);
  printf("ADC driver exists with %d channels\n", count);

  // begin continuous sampling
  printf("Beginning continuous sampling on channel %d at %d Hz\n",
         ADC_CHANNEL, ADC_LOWSPEED_FREQUENCY);
  err = libtock_adc_continuous_sample(ADC_CHANNEL, ADC_LOWSPEED_FREQUENCY, &callbacks);
  if (err < RETURNCODE_SUCCESS) {
    printf("continuous sample error: %d\n", err);
    return -1;
  }

  // loop forever
  while (1) {
    yield();
  }

  return 0;
}
