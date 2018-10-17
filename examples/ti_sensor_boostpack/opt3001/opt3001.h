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

#ifndef OPT3001_H_
#define OPT3001_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>


/* OPT3001 Register Addresses */
#define OPT3001_RESULT    0x0000  /* Result Register (Read-Only)  */
#define OPT3001_CONFIG    0x0001  /* Configuration Register       */
#define OPT3001_LOLIMIT   0x0002  /* Low-Limit Register           */
#define OPT3001_HILIMIT   0x0003  /* High-Limit Register          */
#define OPT3001_MFTID     0x007E  /* Manufacturer ID Register     */
#define OPT3001_DEVICEID  0x007F  /* Device ID Register           */

/* OPT3001 Configuration Register Bits */
#define OPT3001_FL        0x0020  /* Flag Low Field               */
#define OPT3001_FH        0x0040  /* Flag High Field              */
#define OPT3001_CRF       0x0080  /* Conversion Ready Field       */
#define OPT3001_OVF       0x0100  /* Overflow Flag Field          */

/*!
 *  @brief    Ignore lux limit define
 *
 *  OPT3001_IGNORE should be used to ignore or unset a lux limit.
 *
 *  In the following example, the lux limits are set and reset:
 *  @code
 *  OPT3001_setLuxLimits(handle, 43210, 76543.7);
 *  OPT3001_enableInterrupt(handle);
 *  //Additional application code...
 *  OPT3001_setLuxLimits(handle, OPT3001_IGNORE, OPT3001_IGNORE);
 *  @endcode
 */
#define OPT3001_IGNORE         0xFFFF

/*!
 *  @brief    OPT3001 ADC conversion mode
 *
 *  The conversion mode controls whether the device is operating in continuous
 *  conversion, single-shot, or low-power shutdown mode. The hardware default
 *  is shutdown mode such that upon power-up, the device does not perform
 *  conversions. When single-shot mode is selected, the device performs a
 *  conversion and then enters shutdown mode upon completion.
 */
typedef enum OPT3001_ConversionMode {
    OPT3001_SHUTDOWN   = 0x0000, /*!< No conversions (hardware default)     */
    OPT3001_SINGLESHOT = 0x0200, /*!< One conversion, then shutdown         */
    OPT3001_CONTINUOUS = 0x0600  /*!< Continuous conversions                */
} OPT3001_ConversionMode;

/*!
 *  @brief    OPT3001 Conversion Ready Interrupt Mode
 *
 *  Using OPT3001_CONV_EN generates an interrupt when the conversion ready
 *  field indicates a conversion is complete. The conversion ready field is
 *  set to 1 at the end of a conversion and cleared when the configuration
 *  register is subsequuently read or written with any value except one
 *  containing the OPT3001_SHUTDOWN field command. This mode can be used in
 *  conjuction with OPT3001_LATCH or OPT3001_NOLATCH.
 */
typedef enum OPT3001_ConversionReady {
    OPT3001_CONVRD_DIS = 0x000,
    OPT3001_CONVRD_EN = 0xC000
} OPT3001_ConversionReady;

/*!
 *  @brief    OPT3001 Conversion Time
 *
 *  The conversion time determines the length of the light to digital
 *  conversion process. A longer conversion time allows for a lower noise
 *  measurement. The conversion time also relates to the effective resolution
 *  of the data conversion process. Using 800ms ensures full resolution of
 *  the result register while 100ms may limit the resolution based upon
 *  the range.
 */
typedef enum OPT3001_ConversionTime {
    OPT3001_100MS =  0,      /*!< 100 ms conversion time (0.1 s)  */
    OPT3001_800MS =  0x0800  /*!< 800 ms conversion time (0.8 s)  */
} OPT3001_ConversionTime;


/*!
 *  @brief    OPT3001 Fault Count
 *
 *  The fault count field instructs the device as to how many consecutive fault
 *  events are required to trigger the interrupt reporting mechanism: the INT
 *  pin, the flag high field, and flag low field. When the fault count is
 *  reached, the INT pin is asserted.
 */
typedef enum OPT3001_FaultCount {
    OPT3001_FAULT1   = 0x0000,  /*!< One fault count (default)  */
    OPT3001_FAULT2   = 0x0001,  /*!< Two fault counts           */
    OPT3001_FAULT4   = 0x0002,  /*!< Four fault counts          */
    OPT3001_FAULT8   = 0x0003   /*!< Eight fault counts         */
} OPT3001_FaultCount;

/*!
 *  @brief    OPT3001 Result Register Range
 *
 *  The range field selects the full-scale lux range of the device. The result
 *  register may contain a value up to that specified by the range. Greater
 *  range constitutes lower resolution while less range consitutes higher
 *  resolution.
 *
 *  Automatic scaling will determine the optimum range based on a 10-ms
 *  measurement of current lighting conditions. With every measurement there
 *  after, the range is automatically adjusted based on the result (E.g If the
 *  result is towards the upper side of the range, the full-scale range is
 *  increased. If the result is towards the lower side of the range, the
 *  full-scale range is decreased). If the result is outside of the
 *  automatically set range, the current result is aborted and a 10-ms
 *  measurement is taken to re-assess and properly reset the full-scale range.
 *  Therefore, during fast optical transients in automatic mode, a measurement
 *  can possibly take 10 ms rather the time set in OPT3001_ConversionTime.
 */
typedef enum OPT3001_FullRange {
    OPT3001_RANGE0   = 0x0000,  /*!< Range = 00040.95,  lux per LSB = 00.01  */
    OPT3001_RANGE1   = 0x1000,  /*!< Range = 00081.90,  lux per LSB = 00.02  */
    OPT3001_RANGE2   = 0x2000,  /*!< Range = 00163.80,  lux per LSB = 00.04  */
    OPT3001_RANGE3   = 0x3000,  /*!< Range = 00327.60,  lux per LSB = 00.08  */
    OPT3001_RANGE4   = 0x4000,  /*!< Range = 00665.20,  lux per LSB = 00.16  */
    OPT3001_RANGE5   = 0x5000,  /*!< Range = 01310.40,  lux per LSB = 00.32  */
    OPT3001_RANGE6   = 0x6000,  /*!< Range = 02620.80,  lux per LSB = 00.64  */
    OPT3001_RANGE7   = 0x7000,  /*!< Range = 05241.60,  lux per LSB = 01.28  */
    OPT3001_RANGE8   = 0x8000,  /*!< Range = 10483.20,  lux per LSB = 02.56  */
    OPT3001_RANGE9   = 0x9000,  /*!< Range = 20966.40,  lux per LSB = 05.12  */
    OPT3001_RANGE10  = 0xA000,  /*!< Range = 41932.80,  lux per LSB = 10.24  */
    OPT3001_RANGE11  = 0xB000,  /*!< Range = 83865.60,  lux per LSB = 20.48  */
    OPT3001_AUTO     = 0xC000   /*!< Automatic Scaling                       */
} OPT3001_FullRange;

/*!
 *  @brief    OPT3001 Interrupt Mode
 *
 *  Interrupt mode controls the behavior of the INT pin.
 *
 *  If no latch (0), the INT pin is asserted whenever the enabled alert
 *  conditions are met. The INT pin will de-assert automatically when the
 *  condition clears.
 *
 *  In latched mode (1), the INT pin is asserted whenever the appropriate
 *  conditions are met. The INT pin will remain asserted until
 *  the application performs a clearing event (E.g reading the configuration
 *  register).
 */
typedef enum OPT3001_InterruptMode {
    OPT3001_NOLATCH  =  0,      /*!< Non-Latch Mode */
    OPT3001_LATCH    =  0x0010  /*!< Latch Mode     */
} OPT3001_InterruptMode;

/*!
 *  @brief    OPT3001 I2C slave addresses.
 *
 *  The OPT3001 Slave Address is determined by the input to the ADDR pin.
 *  In two cases, the ADDR pin may be coupled with the SDA or SCL bus.
 *  OPT3001 sensors on the same I2C bus cannot share the same slave address.
 */
typedef enum OPT3001_SlaveAddress {
    OPT3001_SA1 = 0x44,  /*!< ADDR = GND */
    OPT3001_SA2 = 0x45,  /*!< ADDR = VDD */
    OPT3001_SA3 = 0x46,  /*!< ADDR = SDA */
    OPT3001_SA4 = 0x47,  /*!< ADDR = SCL */
} OPT3001_SlaveAddress;

typedef int8_t (*opt3001_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len);

/*!
 *  @brief    Members should not be accessed by the application.
 */
struct opt3001_dev {
    /*! addr */
    uint8_t addr;
    /*! Device Id */
    uint8_t dev_id;
    /*! Bus read function pointer */
    opt3001_com_fptr_t read;
    /*! Bus write function pointer */
    opt3001_com_fptr_t write;
};

typedef struct OPT3001_Params {
        OPT3001_ConversionMode    conversionMode;
        OPT3001_ConversionReady   conversionReady;
        OPT3001_ConversionTime    conversionTime;
        OPT3001_FaultCount        faultCount;
        OPT3001_FullRange         range;
        OPT3001_InterruptMode     interruptMode;
} OPT3001_Params;

bool OPT3001_getLux(struct opt3001_dev *, float *data);

bool OPT3001_getLuxLimits(struct opt3001_dev *, float *high, float *low);

void OPT3001_Params_init(OPT3001_Params *params);

bool OPT3001_init(struct opt3001_dev *dev, OPT3001_Params *params);

bool OPT3001_readRegister(struct opt3001_dev *,
        uint16_t *data, uint8_t registerAddress);

bool OPT3001_setConversionMode(struct opt3001_dev *, OPT3001_ConversionMode mode);

bool OPT3001_setLuxLimits(struct opt3001_dev *, float high, float low);

bool OPT3001_setRange(struct opt3001_dev *, OPT3001_FullRange range);

bool OPT3001_writeRegister(struct opt3001_dev *dev, uint16_t data,
        uint8_t registerAddress);

#ifdef __cplusplus
}
#endif

#endif /* OPT3001_H_ */
