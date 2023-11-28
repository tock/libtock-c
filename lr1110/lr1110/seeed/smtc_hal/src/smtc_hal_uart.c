
#include <string.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_uart.h"
#include "nrf_drv_uart.h"
#include "smtc_hal_uart.h"
#include "smtc_hal_config.h"
#include "smtc_hal_gpio.h"

static void uart0_handleEvent( app_uart_evt_t *pEvent );
static void uart1_handleEvent( app_uart_evt_t *pEvent );

APP_UART_DEF( uart0, 0, UART_TX_RX_BUF_SIZE, uart0_handleEvent );
APP_UART_DEF( uart1, 1, UART_TX_RX_BUF_SIZE, uart1_handleEvent );

static uint8_t s_uart0ReadDataBuffer[UART_TX_RX_BUF_SIZE] = {0};
static uint16_t s_index = 0;

static bool uart_init = false;
static bool debug_init = false;

static app_uart_comm_params_t const commParams_0 =
{
    .rx_pin_no    = SENSOR_TXD,
    .tx_pin_no    = SENSOR_RXD,
    .rts_pin_no   = NRF_UART_PSEL_DISCONNECTED,
    .cts_pin_no   = NRF_UART_PSEL_DISCONNECTED,                    
    .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
    .use_parity   = false,
    .baud_rate    = NRF_UART_BAUDRATE_115200
};

static app_uart_comm_params_t const commParams_1 =
{
    .rx_pin_no    = NRF_UART_PSEL_DISCONNECTED,
    .tx_pin_no    = DEBUG_TX_PIN,
    .rts_pin_no   = NRF_UART_PSEL_DISCONNECTED,
    .cts_pin_no   = NRF_UART_PSEL_DISCONNECTED,                    
    .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
    .use_parity   = false,
    .baud_rate    = NRF_UART_BAUDRATE_1000000
};

void hal_uart_init( void )
{	
    uint32_t errCode = 0;
    uint8_t cnt = 0;

    if( uart_init == false )
    {
        uart0.comm_params = &commParams_0;
        
        while( true )
        {
            errCode = app_uart_init( &uart0, &uart0_buffers, APP_IRQ_PRIORITY_LOWEST );
            
            if( errCode == NRF_SUCCESS )
            {
                uart_init = true;
                break;
            }

            hal_mcu_wait_us( 100 );
            
            cnt ++;
            if( cnt > 10 )
            {
               break;
            }
        }
    }
}

void hal_uart_deinit( void )
{
    if( uart_init == true )
    {
        uart_init = false;
        app_uart_close( &uart0 );
        
        hal_gpio_init_out( SENSOR_RXD, HAL_GPIO_RESET );
        hal_gpio_init_in( SENSOR_TXD, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
    }
}

void hal_debug_init( void )
{
    uint32_t errCode;
    uint8_t cnt = 0;
    
    if( debug_init == false )
    {
        uart1.comm_params = &commParams_1;
        
        while( true )
        {
            errCode = app_uart_init( &uart1, &uart1_buffers, APP_IRQ_PRIORITY_LOWEST );
            
            if( errCode == NRF_SUCCESS )
            {
                debug_init = true;
                break;
            }
            
            hal_mcu_wait_us( 100 );

            cnt ++;
            if( cnt > 10 )
            {
               break;
            }
        }
    }
}

void hal_debug_deinit( void )
{
    if( debug_init == true )
    {
        debug_init = false;
        app_uart_close( &uart1 );
    }
}

void hal_uart_flush( void )
{
    if( debug_init == true )
    {
        app_uart_flush( &uart1 );
    }
}

void hal_uart_get( uint8_t *p_byte )
{
    if( debug_init == true )
    {
        app_uart_get( &uart1, p_byte );
    }
}

void hal_uart_put( uint8_t byte )
{
    if( debug_init == true )
    {
        app_uart_put( &uart1, byte );
    }
}

void hal_uart_tx( uint8_t *buff, uint16_t len )
{
    if( debug_init == true )
    {
        // for( uint16_t i = 0; i < len; i++ )
        // {
        //     app_uart_put( &uart1, buff[i] );
        //     hal_mcu_wait_us( 15 );
        // }
        nrf_drv_uart_tx( &uart1, buff, len );
        hal_mcu_wait_us( len * 10 );
    }
}

void hal_uart_rx( uint8_t *buff, uint16_t len )
{
    if( debug_init == true )
    {
        for( uint16_t i = 0; i < len; i++ )
        {
            app_uart_get( &uart1, &buff[i] );
        }
    }
}

void hal_uart_0_tx( uint8_t* buff, uint16_t len )
{
    if( uart_init == true )
    {
        for( uint16_t i = 0; i < len; i++ )
        {
            app_uart_put( &uart0, buff[i] );
        }
    }
}

void hal_uart_0_rx( uint8_t* buff, uint16_t len )
{
    if( uart_init == true )
    {
        for( uint16_t i = 0; i < len; i++ )
        {
            buff[i] = s_uart0ReadDataBuffer[i];
        }
    }
}

void hal_uart_0_rx_clear_buf( void )
{
    s_index = 0;
    memset( s_uart0ReadDataBuffer, 0, UART_TX_RX_BUF_SIZE );
}

uint16_t hal_uart_0_rx_get_len( void )
{
    return s_index;
}

static void uart0_handleEvent( app_uart_evt_t *pEvent )
{
    uint8_t dataChar = 0;

    switch( pEvent -> evt_type )
    {
        case APP_UART_DATA_READY:
        {
            UNUSED_VARIABLE( app_uart_get( &uart0, &dataChar ));
            s_uart0ReadDataBuffer[s_index++] = dataChar;
            if( s_index >= UART_TX_RX_BUF_SIZE ) s_index = 0;
        } break;

        case APP_UART_FIFO_ERROR:

        break;

        case APP_UART_COMMUNICATION_ERROR:

        break;

        default:
        break;
    }
}

static void uart1_handleEvent( app_uart_evt_t *pEvent )
{
    switch( pEvent -> evt_type )
    {
        case APP_UART_TX_EMPTY:
        break;

        default:
        break;
    }
}
