
#ifndef _SMTC_HAL_GPIO_H
#define _SMTC_HAL_GPIO_H

#include <stdint.h>
#include <stdbool.h>

#define GPIO_IRQ_MAX	64

extern uint8_t btn_press_type;

typedef struct hal_gpio_irq_s
{
	uint32_t pin;
    void* context;
    void ( *callback )( void* context );
} hal_gpio_irq_t;

typedef enum gpio_state_e
{
    HAL_GPIO_RESET = 0,
    HAL_GPIO_SET   = 1,
} hal_gpio_state_t;

typedef enum gpio_pull_mode_e
{
    HAL_GPIO_PULL_MODE_NONE = 0,
    HAL_GPIO_PULL_MODE_UP   = 1,
    HAL_GPIO_PULL_MODE_DOWN = 2,
} hal_gpio_pull_mode_t;

typedef enum gpio_irq_mode_e
{
    HAL_GPIO_IRQ_MODE_OFF            = 0,
    HAL_GPIO_IRQ_MODE_RISING         = 1,
    HAL_GPIO_IRQ_MODE_FALLING        = 2,
    HAL_GPIO_IRQ_MODE_RISING_FALLING = 3,
} hal_gpio_irq_mode_t;

typedef enum gpio_pin_direction_e
{
    HAL_GPIO_PIN_DIRECTION_INPUT	= 0,
    HAL_GPIO_PIN_DIRECTION_OUTPUT	= 1,
} hal_gpio_pin_direction_t;

#ifdef __cplusplus
extern "C" {
#endif

void hal_gpio_init( void );

void hal_gpio_task_init( void );

void hal_gpio_init_in( uint32_t pin, const hal_gpio_pull_mode_t pull_mode, const hal_gpio_irq_mode_t irq_mode, hal_gpio_irq_t* irq );

void hal_gpio_init_out( uint32_t pin, hal_gpio_state_t value );

void hal_gpio_set_value( uint32_t pin, const hal_gpio_state_t value );

void hal_gpio_toggle( uint32_t pin );

uint32_t hal_gpio_get_value( uint32_t pin );

void hal_gpio_wait_for_state( uint32_t pin, hal_gpio_state_t state );

bool hal_gpio_is_pending_irq( uint32_t pin );

void hal_gpio_clear_pending_irq( uint32_t pin );

void hal_gpio_irq_attach( const hal_gpio_irq_t* irq );

void hal_gpio_irq_deatach( const hal_gpio_irq_t* irq );

void hal_gpio_irq_enable( void );

void hal_gpio_irq_disable( void );

#ifdef __cplusplus
}
#endif

#endif
