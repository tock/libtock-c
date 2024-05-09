
#ifndef __SMTC_HAL_UART__
#define __SMTC_HAL_UART__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UART0	0
#define UART1	1

#define UART_TX_RX_BUF_SIZE	256

void hal_uart_init( void );
void hal_uart_deinit( void );

void hal_debug_init( void );
void hal_debug_deinit( void );

void hal_uart_flush( void );

void hal_uart_get( uint8_t *p_byte );
void hal_uart_put( uint8_t byte );

void hal_uart_tx( uint8_t* buff, uint16_t len );
void hal_uart_rx( uint8_t* buff, uint16_t len );

void hal_uart_0_tx( uint8_t *buff, uint16_t len );
void hal_uart_0_rx( uint8_t *buff, uint16_t len );
void hal_uart_0_rx_clear_buf( void );
uint16_t hal_uart_0_rx_get_len( void );

#ifdef __cplusplus
}
#endif

#endif
