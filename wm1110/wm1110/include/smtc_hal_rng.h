
#ifndef _SMTC_HAL_RNG_H
#define _SMTC_HAL_RNG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void hal_rng_init( void );

void hal_rng_deinit( void );

uint32_t hal_rng_get_random( void );

uint32_t hal_rng_get_random_in_range( const uint32_t val_1, const uint32_t val_2 );

int32_t hal_rng_get_signed_random_in_range( const int32_t val_1, const int32_t val_2 );

#ifdef __cplusplus
}
#endif

#endif