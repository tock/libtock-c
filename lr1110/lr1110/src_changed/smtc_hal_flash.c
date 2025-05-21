#include <stdlib.h>

#include <libtock-sync/storage/nonvolatile_storage.h>

#include "smtc_hal.h"

smtc_hal_status_t hal_flash_init( void )
{
	return SMTC_HAL_SUCCESS;
}

smtc_hal_status_t hal_flash_erase_page( uint32_t addr, uint8_t nb_page )
{
	return SMTC_HAL_SUCCESS;
}

smtc_hal_status_t hal_flash_write_buffer( uint32_t addr, const uint8_t* buffer, uint32_t size )
{
	returncode_t ret;

	int length_written = 0;
	ret = libtocksync_nonvolatile_storage_write(addr, size, buffer, size, &length_written);
	if (ret != RETURNCODE_SUCCESS) return SMTC_HAL_FAILURE;

	return SMTC_HAL_SUCCESS;
}

void hal_flash_read_buffer( uint32_t addr, uint8_t* buffer, uint32_t size )
{
	returncode_t ret;
	int length_read = 0;
	int max_tries = 3;

	do {
	    ret = libtocksync_nonvolatile_storage_read(addr, size, buffer, size, &length_read);
	    max_tries--;
	    if (max_tries == 0) {
		exit(ret);
	    }
	} while (ret != RETURNCODE_SUCCESS);
}

smtc_hal_status_t hal_flash_deinit( void )
{
    return SMTC_HAL_SUCCESS;
}
