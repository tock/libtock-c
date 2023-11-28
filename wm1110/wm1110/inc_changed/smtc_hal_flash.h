
#ifndef __SMTC_HAL_FLASH_H
#define __SMTC_HAL_FLASH_H


#define ADDR_FLASH_LORAWAN_CONTEXT 0
#define ADDR_FLASH_MODEM_CONTEXT 512
#define ADDR_FLASH_DEVNONCE_CONTEXT 1024
#define ADDR_FLASH_SECURE_ELEMENT_CONTEXT 1536


#ifdef __cplusplus
extern "C" {
#endif

smtc_hal_status_t hal_flash_init( void );

smtc_hal_status_t hal_flash_deinit( void );

smtc_hal_status_t hal_flash_erase_page( uint32_t addr, uint8_t nb_page );

smtc_hal_status_t hal_flash_write_buffer( uint32_t addr, const uint8_t* buffer, uint32_t size );

void hal_flash_read_buffer( uint32_t addr, uint8_t* buffer, uint32_t size );

#endif

#ifdef __cplusplus
}
#endif