
// #include "nrf_drv_twi.h"
// #include "smtc_hal.h"

// static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE( 0 );
// static const nrf_drv_twi_config_t twi_config = {
//         .scl                = SENSOR_SCL,
// 		.sda                = SENSOR_SDA,
// 		.frequency          = NRF_DRV_TWI_FREQ_100K,
// 		.interrupt_priority = APP_IRQ_PRIORITY_HIGH,
// 		.clear_bus_init     = false
// 	};
    
// static bool s_twiTxDone = false;
// static bool s_twiRxDone = false;

// static bool i2c_init = false; 

// static void twi_handler( nrf_drv_twi_evt_t const * p_event, void * p_context );

// void hal_i2c_master_init( void )
// {
//     if( i2c_init == false )
//     {
//         i2c_init = true;
        
//         ret_code_t err_code;
        
//         err_code = nrf_drv_twi_init( &m_twi, &twi_config, twi_handler, NULL );
//         APP_ERROR_CHECK( err_code );

//         nrf_drv_twi_enable( &m_twi );
//     }
// }

// void hal_i2c_master_deinit( void )
// {
//     if( i2c_init == true )
//     {
//         i2c_init = false;
//         nrf_drv_twi_uninit( &m_twi );
//     }
// }

// void hal_i2c_master_init_test( void )
// {
//     if( i2c_init == false )
//     {
//         i2c_init = true;
        
//         ret_code_t err_code;
        
//         err_code = nrf_drv_twi_init( &m_twi, &twi_config, NULL, NULL );
//         APP_ERROR_CHECK( err_code );

//         nrf_drv_twi_enable( &m_twi );
//     }
// }
// bool hal_i2c_scanner_test( uint8_t address )
// {
// 	ret_code_t err_code;
// 	uint8_t sample_data;
	
//         err_code = nrf_drv_twi_rx( &m_twi, address, &sample_data, sizeof( sample_data ));
//         if( err_code == NRF_SUCCESS )
//         {
//                 return true;
//         }
//         return false;
// }
// void hal_i2c_scanner( void )
// {
// 	ret_code_t err_code;
// 	uint8_t address;
// 	uint8_t sample_data;
// 	bool detected_device = false;
	
// 	for( address = 1; address <= 127; address++ )
// 	{
// 		err_code = nrf_drv_twi_rx( &m_twi, address, &sample_data, sizeof( sample_data ));
// 		if( err_code == NRF_SUCCESS )
// 		{
// 			detected_device = true;
// 			PRINTF( "I2C device detected at address 0x%x.\r\n", address );
// 			hal_mcu_wait_ms( 10 );
// 		}
// 	}

// 	if( !detected_device )
// 	{
// 		PRINTF( "No device was found.\r\n" );
// 	}
// }

// /* uint16_t hal_i2c_write_buffer_reg( uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len )
// {
//     uint16_t errCode;
//     uint32_t timeout = TWI_TIMEOUT;
    
//     nrf_drv_twi_xfer_desc_t xferDesc;
//     xferDesc.address = addr;
//     xferDesc.type = NRF_DRV_TWI_XFER_TX;
//     xferDesc.primary_length = 1;
//     xferDesc.p_primary_buf = &reg;
//     xferDesc.secondary_length = len;
//     xferDesc.p_secondary_buf = buf;
    
//     s_twiTxDone = false;
//     errCode = nrf_drv_twi_xfer( &m_twi, &xferDesc, 0 );
//     if( errCode != NRF_SUCCESS ) return errCode;
    
//     while(( !s_twiTxDone ) && --timeout );
//     if( !timeout ) return NRF_ERROR_TIMEOUT;
    
//     return errCode;
// } */

// uint16_t hal_i2c_write_buffer( uint8_t addr, uint8_t *buf, uint8_t len )
// {
//     uint16_t errCode = NRF_SUCCESS;
//     uint32_t timeout = TWI_TIMEOUT;
    
//     s_twiTxDone = false;
//     errCode = nrf_drv_twi_tx( &m_twi, addr, buf, len, false );
//     if( addr != 0 )
//     {
//         if( errCode == NRF_SUCCESS )
//         {
//             timeout = TWI_TIMEOUT;
//             while(( !s_twiTxDone ) && --timeout );
//             if( !timeout ) errCode = NRF_ERROR_TIMEOUT;
//         }
//     }
//     else
//     {
//         errCode = NRF_SUCCESS;
//     }

//     // PRINTF( "hal_i2c_write_buffer, errCode=0x%d\r\n", errCode );
//     if( errCode == NRF_SUCCESS ) return true;
//     return false;
// }

// uint16_t hal_i2c_read_buffer( uint8_t addr, uint8_t *buf, uint8_t len )
// {
//     uint16_t errCode = NRF_SUCCESS;
//     uint32_t timeout = TWI_TIMEOUT;
    
//     s_twiRxDone = false;
//     errCode = nrf_drv_twi_rx( &m_twi, addr, buf, len );
//     if( errCode == NRF_SUCCESS )
//     {
//         timeout = TWI_TIMEOUT;
//         while(( !s_twiRxDone ) && --timeout );
//         if( !timeout ) errCode = NRF_ERROR_TIMEOUT;
//     }
    
//     // PRINTF( "hal_i2c_read_buffer, errCode=0x%d\r\n", errCode );
//     if( errCode == NRF_SUCCESS ) return true;
//     return false;
// }

// static void twi_handler( nrf_drv_twi_evt_t const * p_event, void * p_context )
// {
//     switch( p_event->type )
//     {	
//         case NRF_DRV_TWI_EVT_DONE:
//         switch( p_event->xfer_desc.type )
//         {
//             case NRF_DRV_TWI_XFER_TX:
//                 s_twiTxDone = true;
//                 break;

//             case NRF_DRV_TWI_XFER_TXTX:
//                 s_twiTxDone = true;
//                 break;

//             case NRF_DRV_TWI_XFER_RX:
//                 s_twiRxDone = true;
//                 break;

//             case NRF_DRV_TWI_XFER_TXRX:
//                 s_twiRxDone = true;
//                 break;

//             default:
//                 break;
//         }
//         break;

//         case NRF_DRV_TWI_EVT_ADDRESS_NACK:
//             break;

//         case NRF_DRV_TWI_EVT_DATA_NACK:
//             break;

//         default:
//             break;
//     }
// }
// void hal_i2c_master_uninit( void )
// {
//     if( i2c_init == true )
//     {
//         i2c_init = false;        
//         nrf_drv_twi_uninit( &m_twi);
//     }
// }
