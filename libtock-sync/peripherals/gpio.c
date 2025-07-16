#include <libtock/peripherals/syscalls/gpio_syscalls.h>

#include "gpio.h"

struct gpio_data {
  bool fired;
  uint32_t pin;
  bool value;
};

static struct gpio_data result = { .fired = false };

static void cb(uint32_t pin, bool value) {
  result.fired = true;
  result.pin   = pin;
  result.value = value;
}

static returncode_t wait_until(uint32_t pin, libtock_gpio_input_mode_t pin_config, libtock_gpio_interrupt_mode_t mode) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_gpio_set_interrupt_callback(cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_gpio_enable_input(pin, pin_config);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_gpio_enable_interrupt(pin, mode);
  if (ret != RETURNCODE_SUCCESS) return ret;

  while (1) {
    yield_for(&result.fired);

    if (result.pin == pin) {
      if (mode == libtock_rising_edge && result.value == true) break;
      if (mode == libtock_falling_edge && result.value == false) break;
      if (mode == libtock_change) break;
    }
    result.fired = false;
  }
  return RETURNCODE_SUCCESS;
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
