/**
 * Copyright (C) 2020 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <libtock/peripherals/i2c_master.h>
#include <libtock-sync/services/alarm.h>

#include "bme280.h"
#include "bme280_common.h"

/******************************************************************************/
/*!                               Macros                                      */


/******************************************************************************/
/*!                Static variable definition                                 */

/*! Variable that holds the I2C device address or SPI chip selection */
static uint8_t dev_addr;

/**
 * @brief Timeout for I2C calls
 */

/******************************************************************************/
/*!                User interface functions                                   */

/*!
 * I2C read function map to COINES platform
 */
BME280_INTF_RET_TYPE bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    dev_addr = *(uint8_t*)intf_ptr;
    reg_data[0] = reg_addr;

    
    //printf("read sync before\n");
    //int status = i2c_master_write_read_sync(dev_addr << 1, reg_data, 1, length);
    int status = i2c_master_write_sync(dev_addr << 1, &reg_addr, 1);
    status = i2c_master_read_sync(dev_addr << 1, reg_data, length);
    //printf("read sync after\n");
    for (int i = 0; i < length; i++){
        printf("%d", reg_data[i]);
    }
    printf("\n");

    if (status != 0) {
        return BME280_E_COMM_FAIL;
    }

    return BME280_OK;
}

/*!
 * I2C write function map to COINES platform
 */

BME280_INTF_RET_TYPE bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    //printf("I2C write\n");
    dev_addr = *(uint8_t*)intf_ptr;

    int status = i2c_master_write_sync(dev_addr << 1, &reg_addr, 1);
    //printf("i2C write done\n");
    if (status != 0) {
        return BME280_E_COMM_FAIL;
    }

    status = i2c_master_write_sync(dev_addr << 1, (uint8_t *) reg_data, length);

    if (status != 0) {
        return BME280_E_COMM_FAIL;
    }

    return BME280_OK;
}



/*!
 * Delay function map to COINES platform
 */
void bme280_delay_us(uint32_t period, void *intf_ptr)
{
    // calculate us to ms
    uint32_t period_ms = period / 1000;

    // if no period then set to 1 to ensure delay
    if (period_ms == 0) {
        period_ms = 1;
    }

    // delay
    libtocksync_alarm_delay_ms(period_ms);
}

/*!
 *  @brief Prints the execution status of the APIs.
 */
void bme280_error_codes_print_result(const char api_name[], int8_t rslt)
{
    if (rslt != BME280_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case BME280_E_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf("It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case BME280_E_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf("It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case BME280_E_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case BME280_E_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}

/*!
 *  @brief Function to select the interface between SPI and I2C.
 */
int8_t bme280_interface_selection(struct bme280_dev *dev, uint8_t intf)
{
    int8_t rslt = BME280_OK;

    if (dev != NULL)
    {
        /* Bus configuration : I2C */
        if (intf == BME280_I2C_INTF)
        {
            dev_addr = BME280_I2C_ADDR_SEC;
            dev->read = bme280_i2c_read;
            dev->write = bme280_i2c_write;
            dev->intf = BME280_I2C_INTF;
        }
        /* Bus configuration : SPI */
        else if (intf == BME280_SPI_INTF)
        {
            rslt = BME280_E_DEV_NOT_FOUND;
        }

        /* Holds the I2C device addr or SPI chip selection */
        dev->intf_ptr = &dev_addr;

        /* Configure delay in microseconds */
        dev->delay_us = bme280_delay_us;
    }
    else
    {
        rslt = BME280_E_NULL_PTR;
    }

    return rslt;
}