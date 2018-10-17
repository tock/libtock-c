/*
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== opt3001.c ========
 */

/* Module Header */
#include "../bosch_port/bosch_port.h"
#include "opt3001.h"

#include <stdbool.h>
#include <stdint.h>

#define LUX_BASE_RES            (00.01F)
#define RANGE_BASE              (00040.95F)
#define RANGE_MAX               (83865.60F)
#define RANGE_BITS              (0xF000)

/* Default OPT3001 parameters structure */
const OPT3001_Params OPT3001_defaultParams = {
  OPT3001_CONTINUOUS,          /* Continuous conversions                     */
  OPT3001_CONVRD_DIS,          /* Disable Conversion Ready Interrupts        */
  OPT3001_800MS,               /* 0.8 s conversion time                      */
  OPT3001_FAULT1,              /* One fault per interrupt                    */
  OPT3001_AUTO,                /* Automatic Lux Range                        */
  OPT3001_LATCH,               /* Latch Mode                                 */
};

/*
 *  ======== convertToLux ========
 *  Converts register into lux
 */
static float convertToLux(uint16_t reg)
{
  float lsbSize;

  /* The register consist of data bits and exponent bits               */
  /* Shift out data bits from reg and raise two to the resulting power */
  lsbSize = (LUX_BASE_RES * (2 << (reg >> 12))) / 2;
  /* Mask out exponent bits */
  reg = reg & 0x0FFF;

  /* Return lux */
  return lsbSize * reg;
}

/*
 *  ======== OPT3001_getLux ========
 *  Get current lux reading
 */
bool OPT3001_getLux(struct opt3001_dev *dev, float *data)
{
  uint16_t reg;

  if (OPT3001_readRegister(dev, &reg, OPT3001_RESULT)) {
    *data = convertToLux(reg);

    return true;
  }

  return false;
}

/*
 *  ======== OPT3001_getLuxLimits ========
 */
bool OPT3001_getLuxLimits(struct opt3001_dev *dev, float *high, float *low)
{
  uint16_t reg;

  if (OPT3001_readRegister(dev, &reg, OPT3001_HILIMIT)) {
    *high = convertToLux(reg);

    if (OPT3001_readRegister(dev, &reg, OPT3001_LOLIMIT)) {
      *low = convertToLux(reg);

      return true;
    }
  }

  return false;
}

/*
 *  ======== OPT3001_open ========
 *  Setups OPT3001 sensor and returns OPT3001_Handle
 */
bool OPT3001_init(struct opt3001_dev *dev, OPT3001_Params *params)
{
  uint16_t data;

  if (params == NULL) {
    params = (OPT3001_Params *) &OPT3001_defaultParams;
  }

  dev->addr  = 0x47;
  dev->read  = (opt3001_com_fptr_t) tock_i2c_read;
  dev->write = (opt3001_com_fptr_t) tock_i2c_write;

  data = (uint16_t)params->conversionMode | (uint16_t)params->conversionTime | (uint16_t)params->faultCount
         | (uint16_t)params->interruptMode | params->range;

  return OPT3001_writeRegister(dev, data, OPT3001_CONFIG);
}

/*
 *  ======== OPT3001_Params_init ========
 *  Initialize a OPT3001_Params struct to default settings.
 */
void OPT3001_Params_init(OPT3001_Params *params)
{
  *params = OPT3001_defaultParams;
}

/*
 *  ======== OPT3001_readRegister ========
 *  Reads a specified register from a OPT3001 sensor.
 */
bool OPT3001_readRegister(struct opt3001_dev *dev, uint16_t *data,
                          uint8_t registerAddress)
{
  uint8_t buf[2];
  int8_t result = dev->read(dev->addr, registerAddress, buf, sizeof(buf));
  *data = (buf[0] << 8) | buf[1];
  return result == 0;
}

/*
 *  ======== OPT3001_setLuxLimits ========
 */
bool OPT3001_setLuxLimits(struct opt3001_dev *dev, float high, float low)
{
  uint16_t limit;       /* Bits[11:00] */
  uint16_t exp;         /* Bits[15:12] */

  if ( (uint32_t) high != OPT3001_IGNORE) {

    if (high > RANGE_MAX) {
      high = RANGE_MAX;
    }
    /* Determine exp and limit range */
    for (exp = 0; exp <= (OPT3001_RANGE11 >> 12); exp++) {

      if (high < (RANGE_BASE * (2 << exp)) / 2) {
        break;
      }
    }

    /* Truncate and convert for Limit Register */
    limit = (int) (high / ((LUX_BASE_RES * (2 << exp)) / 2));
    exp   = exp << 12;

    if (!OPT3001_writeRegister(dev, (limit | exp), OPT3001_HILIMIT)) {
      return false;
    }
  }

  if ( (uint32_t) low != OPT3001_IGNORE) {

    if (low < RANGE_BASE) {
      low = RANGE_BASE;
    }
    /* Determine exp and limit range */
    for (exp = 0; exp <= (OPT3001_RANGE11 >> 12); exp++) {

      if (low < (RANGE_BASE * (2 << exp)) / 2) {
        break;
      }
    }

    /* Truncate and convert for Limit Register */
    limit = (int) (low / ((LUX_BASE_RES * (2 << exp)) / 2));
    exp   = exp << 12;

    if (!OPT3001_writeRegister(dev, (limit | exp), OPT3001_LOLIMIT)) {
      return false;
    }
  }

  return true;
}


/*
 *  ======== OPT3001_setRange ========
 */
bool OPT3001_setRange(struct opt3001_dev *dev, OPT3001_FullRange range)
{
  uint16_t reg;

  /* Read Configuration Register */
  if (OPT3001_readRegister(dev, &reg, OPT3001_CONFIG)) {

    /* Clear current range bits */
    reg &= ~RANGE_BITS;
    /* Write new range bits */
    reg |= range;

    if (OPT3001_writeRegister(dev, reg, OPT3001_CONFIG)) {
      return true;
    }
  }

  return false;
}

/*
 *  ======== OPT3001_setConversionMode ========
 *  Set conversion mode.
 */
bool OPT3001_setConversionMode(struct opt3001_dev *dev,
                               OPT3001_ConversionMode mode)
{
  uint16_t reg;

  /* Read Configuration Register */
  if (OPT3001_readRegister(dev, &reg, OPT3001_CONFIG)) {

    /* Clear conversion mode bits */
    reg &= ~OPT3001_SINGLESHOT;
    /* Write new conversion bits */
    reg |= mode;

    if (OPT3001_writeRegister(dev, reg, OPT3001_CONFIG)) {
      return true;
    }
  }

  return false;
}

/*
 *  ======== OPT3001_writeRegister ========
 *  Writes data to the specified register and OPT3001 sensor.
 */
bool OPT3001_writeRegister(struct opt3001_dev *dev, uint16_t data,
                           uint8_t registerAddress)
{

  uint8_t output_data[2] = { data >> 8, data & 0xFF};
  int8_t result = dev->write(dev->addr, registerAddress, (uint8_t*) &output_data, 2);
  return result == 0;
}
