
#include <stdlib.h>
//#include "nrf_drv_rng.h"
#include "smtc_hal_rng.h"

#include <libtock-sync/peripherals/rng.h>

#define RANDOM_BUFF_SIZE	4
static uint8_t rng_buff[RANDOM_BUFF_SIZE];

void hal_rng_init( void )
{
}

uint32_t hal_rng_get_random( void )
{
    int num_received = 0;
    libtocksync_rng_get_random_bytes(rng_buff, RANDOM_BUFF_SIZE, RANDOM_BUFF_SIZE, &num_received);
    return rng_buff[0] << 24 | rng_buff[1] << 16 | rng_buff[2] << 8 | rng_buff[3];
}

uint32_t hal_rng_get_random_in_range( const uint32_t val_1, const uint32_t val_2 )
{
    uint32_t max, min, rng = 0;

    if( val_1 < val_2 )
    {
        min = val_1; max = val_2;
    }
    else
    {
        min = val_2; max = val_1;
    }

    rng = hal_rng_get_random( );
    rng = rng %( max - min + 1 ) + min;

    return rng;
}
