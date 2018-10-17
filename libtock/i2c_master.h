#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

int i2c_master_write(uint8_t address,  uint8_t * buffer, uint8_t wlen);
int i2c_master_read(uint8_t address,  uint8_t * buffer, uint8_t rlen);
int i2c_master_write_read(uint8_t address,  uint8_t * buffer, uint8_t wlen, uint8_t rlen);

#ifdef __cplusplus
}
#endif