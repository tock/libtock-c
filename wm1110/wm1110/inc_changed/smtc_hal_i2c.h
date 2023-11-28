
#ifndef __SMTC_HAL_I2C_H
#define __SMTC_HAL_I2C_H

#define TWI_TIMEOUT         100000

#ifdef __cplusplus
extern "C" {
#endif

void hal_i2c_master_init( void );

void hal_i2c_master_deinit( void );

void hal_i2c_master_init_test( void );
bool hal_i2c_scanner_test( uint8_t address );

void hal_i2c_scanner( void );

// uint16_t hal_i2c_write_buffer_reg( uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len );

uint16_t hal_i2c_write_buffer( uint8_t addr, uint8_t *buf, uint8_t len );

uint16_t hal_i2c_read_buffer( uint8_t addr, uint8_t *buf, uint8_t len );
void hal_i2c_master_uninit( void );
#endif

#ifdef __cplusplus
}
#endif