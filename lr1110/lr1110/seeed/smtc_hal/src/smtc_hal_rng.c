
#include <stdlib.h>
#include <time.h>
#include "nrf_drv_rng.h"
#include "smtc_hal_rng.h"

#define RANDOM_BUFF_SIZE	4

static uint8_t rng_buff[RANDOM_BUFF_SIZE];

void hal_rng_init( void )
{
    nrf_drv_rng_init(NULL);
}

void hal_rng_deinit( void )
{
    nrf_drv_rng_uninit( );
}

uint32_t hal_rng_get_random( void )
{
    uint8_t available = 0;
    uint8_t length = 0;
    uint32_t rng = 0;

    // hal_rng_init( );

    nrf_drv_rng_bytes_available( &available );
    length = MIN( RANDOM_BUFF_SIZE, available );

    if( length >= RANDOM_BUFF_SIZE )
    {
        nrf_drv_rng_rand( rng_buff, length );
        rng = rng_buff[0] << 24 | rng_buff[1] << 16 | rng_buff[2] << 8 | rng_buff[3];
    }
    // else
    // {
    // printf( "hal_rng_get_random error\n" );
    // srand(( unsigned )time( NULL ));
    // rng = rand();
    // }

    // hal_rng_deinit( );

    return rng;
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

int32_t hal_rng_get_signed_random_in_range( const int32_t val_1, const int32_t val_2 )
{
    int32_t max, min, rng = 0;

    if( val_1 < val_2 ) 
    {
        min = val_1; max = val_2;
    }
    else
    {
        min = val_2; max = val_1;
    }

    rng = hal_rng_get_random( );
    rng = abs( rng %( max - min + 1 )) + min;

    return rng;
}
