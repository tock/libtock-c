// `app_loader`'s makefile autogenerates this file. Do not edit manually.
#pragma once

#define BINARY_COUNT 1

#include "adc.xxd"
#define APP_ADC adc_embed

const uint8_t* binaries[] = {
  APP_ADC,
};

size_t binary_sizes[] = {
	adc_embed_size,
};

const char* binary_names[] = {
  "adc",
};

size_t actual_sizes[] = {
	adc_embed_actual_size,
};
