#pragma once

#include <stdint.h>

#include "../tock.h"
#include "syscalls/adc_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif



// mode of the ADC
// Used to tell which operation a callback corresponds to
typedef enum {
  libtock_adc_SingleSample     = 0,
  libtock_adc_ContinuousSample = 1,
  libtock_adc_SingleBuffer     = 2,
  libtock_adc_ContinuousBuffer = 3
} libtock_adc_mode;



// Function signature for ADC single sample callback.
//
// - `arg1` (`uint8_t`): Channel number.
// - `arg2` (`uint16_t`): ADC sample.
typedef void (*libtock_adc_callback_single_sample)(uint8_t, uint16_t);

// Function signature for ADC continuous sample callback.
//
// - `arg1` (`uint8_t`): Channel number.
// - `arg2` (`uint16_t`): ADC sample.
typedef void (*libtock_adc_callback_continuous_sample)(uint8_t, uint16_t);

// Function signature for ADC buffered sample callback.
//
// - `arg1` (`uint8_t`): Channel number.
// - `arg2` (`uint32_t`): Number of samples.
// - `arg2` (`uint16_t`): Buffer of samples.
typedef void (*libtock_adc_callback_buffered_sample)(uint8_t, uint32_t, uint16_t*);

// Function signature for ADC continuous buffered sample callback.
//
// - `arg1` (`uint8_t`): Channel number.
// - `arg2` (`uint32_t`): Number of samples.
// - `arg2` (`uint16_t`): Buffer of samples.
typedef void (*libtock_adc_callback_continuous_buffered_sample)(uint8_t, uint32_t, uint16_t*);



typedef struct {
  libtock_adc_callback_single_sample single_sample_callback;
  libtock_adc_callback_continuous_sample continuous_sample_callback;
  libtock_adc_callback_buffered_sample buffered_sample_callback;
  libtock_adc_callback_buffered_sample continuous_buffered_sample_callback;
} libtock_adc_callbacks;



// ***** System Call Interface *****



// provides an application buffer to the ADC driver to fill with samples
//
// buffer - pointer to buffer to fill with samples
// length - number of samples requested, must be less than or equal to buffer
//          length
returncode_t libtock_adc_set_buffer(uint16_t* buffer, uint32_t length);

// provide an application buffer to fill with samples when continuously
// sampling
//
// buffer - pointer to buffer to fill with samples
// length - number of samples requested, must be less than or equal to buffer
//          length
returncode_t libtock_adc_set_double_buffer(uint16_t* buffer, uint32_t length);



// query how many channels are available in the ADC driver
returncode_t libtock_adc_channel_count(int* count);

// request a single analog sample
//
// channel - number of the channel to be sampled
returncode_t libtock_adc_single_sample(uint8_t channel, libtock_adc_callbacks* callbacks);

// request an repeated analog samples at a given frequency
//
// channel - number of the channel to be sampled
// frequency - rate in samples per second to collect data at
returncode_t libtock_adc_continuous_sample(uint8_t channel, uint32_t frequency, libtock_adc_callbacks* callbacks);

// request a buffer full of samples from the ADC
//
// channel - number of the channel to be sampled
// frequency - rate in samples per second to collect data at
returncode_t libtock_adc_buffered_sample(uint8_t channel, uint32_t frequency, libtock_adc_callbacks* callbacks);

// request continuous ADC sampling
// Alternates between the two provided application buffers
//
// channel - number of the channel to be sampled
// frequency - rate in samples per second to collect data at
returncode_t libtock_adc_continuous_buffered_sample(uint8_t channel, uint32_t frequency,
                                                    libtock_adc_callbacks* callbacks);

// cancel an outstanding ADC operation
// No callback will occur from the prior ADC operation. The ADC may not be
// immediately ready to use again if a single sample was canceled. Usually used
// to stop a continuous sampling operation
returncode_t libtock_adc_stop_sampling(void);



#ifdef __cplusplus
}
#endif
