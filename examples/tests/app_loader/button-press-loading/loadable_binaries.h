// `app_loader`'s makefile autogenerates this file. Do not edit manually.
#pragma once

#define BINARY_COUNT 2

#include "blink.xxd"
#define APP_BLINK blink_embed
#include "adc.xxd"
#define APP_ADC adc_embed

const uint8_t* binaries[] = {
  APP_BLINK,
  APP_ADC,
};

size_t binary_sizes[] = {
	blink_embed_size,
	adc_embed_size,
};

const char* binary_names[] = {
  "blink",
  "adc",
};

size_t actual_sizes[] = {
	blink_embed_actual_size,
	adc_embed_actual_size,
};
