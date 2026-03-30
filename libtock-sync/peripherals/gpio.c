#include <libtock/peripherals/syscalls/gpio_syscalls.h>

#include "gpio.h"

#include "syscalls/gpio_syscalls.h"

static returncode_t wait_until(uint32_t pin, libtock_gpio_input_mode_t pin_config, libtock_gpio_interrupt_mode_t mode) {
  returncode_t ret;
  uint32_t irq_pin;
  bool value;

  ret = libtock_gpio_command_enable_input(pin, (uint32_t) pin_config);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_gpio_command_enable_interrupt(pin, (uint32_t) mode);
  if (ret != RETURNCODE_SUCCESS) return ret;

  while (1) {
    libtocksync_gpio_yield_wait_for(&irq_pin, &value);

    if (irq_pin == pin) {
      if (mode == libtock_rising_edge && value == true) break;
      if (mode == libtock_falling_edge && value == false) break;
      if (mode == libtock_change) break;
    }
  }
  return RETURNCODE_SUCCESS;
}

bool libtocksync_gpio_exists(void) {
  return libtock_gpio_driver_exists();
}

returncode_t libtocksync_gpio_enable_input(uint32_t pin, libtock_gpio_input_mode_t pin_config) {
  return libtock_gpio_command_enable_input(pin, (uint32_t) pin_config);
}

returncode_t libtocksync_gpio_read(uint32_t pin, int* pin_value) {
  return libtock_gpio_command_read(pin, (uint32_t*) pin_value);
}

returncode_t libtocksync_gpio_wait_until_high(uint32_t pin, libtock_gpio_input_mode_t pin_config) {
  return wait_until(pin, pin_config, libtock_rising_edge);
}

returncode_t libtocksync_gpio_wait_until_low(uint32_t pin, libtock_gpio_input_mode_t pin_config) {
  return wait_until(pin, pin_config, libtock_falling_edge);
}

returncode_t libtocksync_gpio_wait_until_changed(uint32_t pin, libtock_gpio_input_mode_t pin_config) {
  return wait_until(pin, pin_config, libtock_change);
}
