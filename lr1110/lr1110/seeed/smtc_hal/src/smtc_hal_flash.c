
#include "nrf_fstorage.h"
#include "smtc_hal.h"

#ifdef SOFTDEVICE_PRESENT
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_fstorage_sd.h"
#else
#include "nrf_drv_clock.h"
#include "nrf_fstorage_nvmc.h"
#endif

static void fstorage_evt_handler( nrf_fstorage_evt_t * p_evt );

NRF_FSTORAGE_DEF( nrf_fstorage_t fstorage ) =
{
    .evt_handler    = fstorage_evt_handler,
    .start_addr     = APP_FLASH_ADDR_START,
    .end_addr       = APP_FLASH_ADDR_END,
};

smtc_hal_status_t hal_flash_init( void )
{
	nrf_fstorage_api_t * p_fs_api;
	
#ifdef SOFTDEVICE_PRESENT
	p_fs_api = &nrf_fstorage_sd;
#else
	p_fs_api = &nrf_fstorage_nvmc;
#endif

	nrf_fstorage_init( &fstorage, p_fs_api, NULL );
}

smtc_hal_status_t hal_flash_erase_page( uint32_t addr, uint8_t nb_page )
{
	if( addr >= APP_FLASH_ADDR_START && addr <= APP_FLASH_ADDR_END && nb_page <= APP_FLASH_PAGE_MAX )
	{
		nrf_fstorage_erase( &fstorage, addr, nb_page, NULL );
		while( nrf_fstorage_is_busy( &fstorage ))
		{
#ifdef SOFTDEVICE_PRESENT
			( void )sd_app_evt_wait( );
#else
			__WFE();
#endif
		}
	}
}

smtc_hal_status_t hal_flash_write_buffer( uint32_t addr, const uint8_t* buffer, uint32_t size )
{
	if( addr >= APP_FLASH_ADDR_START && addr <= APP_FLASH_ADDR_END && size <= APP_FLASH_SIZE_MAX )
	{
		nrf_fstorage_write( &fstorage, addr, buffer, size, NULL );
		while( nrf_fstorage_is_busy( &fstorage ))
		{
#ifdef SOFTDEVICE_PRESENT
			( void )sd_app_evt_wait( );
#else
			__WFE();
#endif
		}
	}
}

void hal_flash_read_buffer( uint32_t addr, uint8_t* buffer, uint32_t size )
{
	if( addr >= APP_FLASH_ADDR_START && addr <= APP_FLASH_ADDR_END && size <= APP_FLASH_SIZE_MAX )
	{
		nrf_fstorage_read( &fstorage, addr, buffer, size );
	}
}

static void fstorage_evt_handler( nrf_fstorage_evt_t * p_evt )
{
	
}

smtc_hal_status_t hal_flash_deinit( void )
{
    nrf_fstorage_uninit( &fstorage,  NULL );
}
