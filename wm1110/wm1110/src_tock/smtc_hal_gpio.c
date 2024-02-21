
// #include "nrf_gpio.h"
// #include "nrf_gpiote.h"
// #include "nrf_drv_gpiote.h"
#include "smtc_hal_gpio.h"
#include "smtc_hal_config.h"
//#include "smtc_hal.h"
#include "lora_phy.h"

static hal_gpio_irq_t const* gpio_irq[GPIO_IRQ_MAX];

// static nrf_drv_gpiote_in_config_t inConfig0 =GPIOTE_CONFIG_IN_SENSE_LOTOHI( false );
// static nrf_drv_gpiote_in_config_t inConfig1 =GPIOTE_CONFIG_IN_SENSE_HITOLO( false );
// static nrf_drv_gpiote_in_config_t inConfig2 =GPIOTE_CONFIG_IN_SENSE_TOGGLE( false );
static bool gpioe_init = false;

// static void irqCallbackFunc( nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action )
// {
// 	if(( gpio_irq[pin] != NULL ) && ( gpio_irq[pin]->callback != NULL ))
// 	{
// 		gpio_irq[pin]->callback( gpio_irq[pin]->context );
// 	}
// }

void hal_gpio_init_in( uint32_t pin, const hal_gpio_pull_mode_t pull_mode, const hal_gpio_irq_mode_t irq_mode, hal_gpio_irq_t* irq )
{
	GPIO_InputMode_t pull_value;
	switch( pull_mode )
	{
		case HAL_GPIO_PULL_MODE_NONE:
			// pull_value = NRF_GPIO_PIN_NOPULL;
			pull_value = PullNone;
			break;
			
		case HAL_GPIO_PULL_MODE_UP:
			// pull_value = NRF_GPIO_PIN_PULLUP;
			pull_value = PullUp;
			break;
			
		case HAL_GPIO_PULL_MODE_DOWN:
			// pull_value = NRF_GPIO_PIN_PULLDOWN;
			pull_value = PullDown;
			break;
			
		default:
			break;
	}

	if( irq_mode == HAL_GPIO_IRQ_MODE_OFF )
	{
		// nrf_gpio_cfg_input( pin, pull_value );
		lora_phy_gpio_enable_input((GPIO_Pin_t)pin, pull_value);
	}
	else
	{
		// if( gpioe_init == false )
		// {
		// 	gpioe_init = true;
		// 	nrf_drv_gpiote_init( );
		// }

        GPIO_InterruptMode_t tock_irq_mode = Change;
		switch( irq_mode )
		{
			case HAL_GPIO_IRQ_MODE_RISING:
				// inConfig0.pull = pull_value;
				// inConfig0.sense = NRF_GPIOTE_POLARITY_LOTOHI;
				// nrf_drv_gpiote_in_init( pin, &inConfig0, irqCallbackFunc );
				tock_irq_mode = RisingEdge;
				break;
				
			case HAL_GPIO_IRQ_MODE_FALLING:
				// inConfig1.pull = pull_value;
				// inConfig1.sense = NRF_GPIOTE_POLARITY_HITOLO;
				// nrf_drv_gpiote_in_init( pin, &inConfig1, irqCallbackFunc );
				tock_irq_mode = FallingEdge;
				break;
				
			case HAL_GPIO_IRQ_MODE_RISING_FALLING:
				// inConfig2.pull = pull_value;
				// inConfig2.sense = NRF_GPIOTE_POLARITY_TOGGLE;
				// nrf_drv_gpiote_in_init( pin, &inConfig2, irqCallbackFunc );
				tock_irq_mode = Change;
				break;
			
			default:
				break;
		}

		lora_phy_gpio_enable_interrupt((GPIO_Pin_t)pin, tock_irq_mode);
	
		if(( irq != NULL ) && ( irq->callback != NULL ))
		{
			// gpio_irq[(irq->pin) & ( GPIO_IRQ_MAX - 1 )] = irq;
			lora_phy_gpio_interrupt_callback(irq->callback, irq->context);
		}
		
		// nrf_drv_gpiote_in_event_enable( pin, true );
	}
}

void hal_gpio_irq_attach( const hal_gpio_irq_t* irq )
{
	if(( irq != NULL ) && ( irq->callback != NULL ))
	{
		// gpio_irq[(irq->pin) & ( GPIO_IRQ_MAX - 1 )] = irq;
		lora_phy_gpio_interrupt_callback(irq->callback, irq->context);
	}
}

void hal_gpio_irq_deatach( const hal_gpio_irq_t* irq )
{
	if( irq != NULL )
	{
		// gpio_irq[(irq->pin) & GPIO_IRQ_MAX] = NULL;
		lora_phy_gpio_disable_interrupt((GPIO_Pin_t)(irq->pin));
	}
}

void hal_gpio_init_out( uint32_t pin, hal_gpio_state_t value )
{
	// nrf_gpio_cfg_output( pin );
	// nrf_gpio_pin_write( pin, value );

	lora_phy_gpio_enable_output((GPIO_Pin_t)pin);
	if (value == HAL_GPIO_RESET) {
		lora_phy_gpio_clear((GPIO_Pin_t)pin);
	} else { // value = HAL_GPIO_SET
		lora_phy_gpio_set((GPIO_Pin_t)pin);
	}
}

void hal_gpio_init( void )
{
	// re-power up LR1110
	hal_gpio_init_out( LR1110_IRQ_PIN, HAL_GPIO_RESET );
	hal_gpio_init_out( LR1110_BUSY_PIN, HAL_GPIO_RESET );
	hal_gpio_init_out( LR1110_SPI_NSS_PIN, HAL_GPIO_RESET );
	hal_gpio_init_out( LR1110_SPI_SCK_PIN, HAL_GPIO_RESET );
	hal_gpio_init_out( LR1110_SPI_MOSI_PIN, HAL_GPIO_RESET );
	hal_gpio_init_out( LR1110_SPI_MISO_PIN, HAL_GPIO_RESET );
	hal_gpio_init_out( LR1110_NRESER_PIN, HAL_GPIO_RESET );

	hal_mcu_wait_ms( 500 ); // re-power up LR1110 and wait for CCL sensor ready

	hal_gpio_init_out( LR1110_SPI_NSS_PIN, HAL_GPIO_SET );
	hal_gpio_init_in( LR1110_BUSY_PIN, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
	hal_gpio_init_in( LR1110_IRQ_PIN, HAL_GPIO_PULL_MODE_DOWN, HAL_GPIO_IRQ_MODE_RISING, NULL );
	hal_gpio_set_value( LR1110_NRESER_PIN, HAL_GPIO_SET );
}

void hal_gpio_set_value( uint32_t pin, const hal_gpio_state_t value )
{
	switch( value )
	{
		case HAL_GPIO_RESET:
			// nrf_gpio_pin_clear( pin );
			lora_phy_gpio_clear((GPIO_Pin_t)pin);
			break;
		case HAL_GPIO_SET:
			// nrf_gpio_pin_set( pin );
			lora_phy_gpio_set((GPIO_Pin_t)pin);
			break;
		default:
			break;
	}
}

void hal_gpio_toggle( uint32_t pin )
{
	// nrf_gpio_pin_toggle( pin );
	lora_phy_gpio_toggle((GPIO_Pin_t)pin);
}

uint32_t hal_gpio_get_value( uint32_t pin )
{
	int value;
	lora_phy_gpio_read((GPIO_Pin_t)pin, &value);
	if(value != 0) {
		return HAL_GPIO_SET;
	} else {
		return HAL_GPIO_RESET;
	}
	
	// if( nrf_gpio_pin_read( pin ))
	// {
	// 	return HAL_GPIO_SET;
	// }
	// else
	// {
	// 	return HAL_GPIO_RESET;
	// }
}

void hal_gpio_wait_for_state( uint32_t pin, hal_gpio_state_t state )
{
	int value;
	if( state == HAL_GPIO_RESET )
	{
		// while( nrf_gpio_pin_read( pin )) { };
		while(lora_phy_gpio_read((GPIO_Pin_t)pin, &value)) {
			
		}
	}
	else
	{
		// while( !nrf_gpio_pin_read( pin )) { };
		while(!lora_phy_gpio_read((GPIO_Pin_t)pin, &value)) {

		}
	}
}

// pending_irq functions has no equivalent in Tock
bool hal_gpio_is_pending_irq( uint32_t pin )
{
	bool pending = false;
	uint32_t irq_pending = 0;
	
	// sd_nvic_GetPendingIRQ( GPIOTE_IRQn, &irq_pending );
	if( irq_pending ) pending = true;
	
	return pending;
}

void hal_gpio_clear_pending_irq( uint32_t pin )
{
	// sd_nvic_ClearPendingIRQ( GPIOTE_IRQn );
}

void hal_gpio_irq_enable( void )
{

}

void hal_gpio_irq_disable( void )
{
	
}
