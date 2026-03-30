#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  libtock_pull_none=0,
  libtock_pull_up,
  libtock_pull_down,
} libtock_gpio_input_mode_t;

typedef enum {
  libtock_change=0,
  libtock_rising_edge,
  libtock_falling_edge,
} libtock_gpio_interrupt_mode_t;

#ifdef __cplusplus
}
#endif
