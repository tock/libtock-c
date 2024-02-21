
#ifndef __SMTC_HAL_FLASH_H
#define __SMTC_HAL_FLASH_H

#define ADDR_FLASH_PAGE_SIZE (( uint32_t )0x00001000 ) // Size of Page = 4 KBytes
#define ADDR_FLASH_PAGE_0 (( uint32_t )0x00000000 ) // Base @ of Page 0, 4 KBytes */
#define ADDR_FLASH_PAGE( page ) ( ADDR_FLASH_PAGE_0 + ( page ) *ADDR_FLASH_PAGE_SIZE )

#ifdef SOFTDEVICE_PRESENT   
    #define APP_FLASH_ADDR_START	0xEA000   
    #define APP_FLASH_ADDR_END          0xEDFFF
#else
    #define APP_FLASH_ADDR_START	0xFC000
    #define APP_FLASH_ADDR_END      0xFFFFF
#endif

#define APP_FLASH_SIZE_MAX	0x4000
#define APP_FLASH_PAGE_MAX	0x04

#ifdef SOFTDEVICE_PRESENT
    #define ADDR_FLASH_LORAWAN_CONTEXT ADDR_FLASH_PAGE(237)
    #define ADDR_FLASH_MODEM_CONTEXT ADDR_FLASH_PAGE(236)
    #define ADDR_FLASH_DEVNONCE_CONTEXT ADDR_FLASH_PAGE(235)
    #define ADDR_FLASH_SECURE_ELEMENT_CONTEXT ADDR_FLASH_PAGE(234)
#else
    #define ADDR_FLASH_LORAWAN_CONTEXT ADDR_FLASH_PAGE(255)
    #define ADDR_FLASH_MODEM_CONTEXT ADDR_FLASH_PAGE(254)
    #define ADDR_FLASH_DEVNONCE_CONTEXT ADDR_FLASH_PAGE(253)
    #define ADDR_FLASH_SECURE_ELEMENT_CONTEXT ADDR_FLASH_PAGE(252)
#endif

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