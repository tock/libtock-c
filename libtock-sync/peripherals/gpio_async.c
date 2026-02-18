#include <libtock/peripherals/syscalls/gpio_async_syscalls.h>

#include "gpio_async.h"

#include "syscalls/gpio_async_syscalls.h"

static returncode_t gpio_async_command(uint32_t port, uint8_t pin, returncode_t (*command_fn)(uint32_t, uint8_t)) {
  returncode_t ret;

  ret = command_fn(port, pin);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_gpio_async_yield_wait_for_generic_command();
  return ret;
}

bool libtocksync_gpio_async_exists(void) {
  return libtock_gpio_async_driver_exists();
}

returncode_t libtocksync_gpio_async_make_output(uint32_t port, uint8_t pin) {
  return gpio_async_command(port, pin, libtock_gpio_async_command_make_output);
}

returncode_t libtocksync_gpio_async_set(uint32_t port, uint8_t pin) {
  return gpio_async_command(port, pin, libtock_gpio_async_command_set);
}

returncode_t libtocksync_gpio_async_clear(uint32_t port, uint8_t pin) {
  return gpio_async_command(port, pin, libtock_gpio_async_command_clear);
}

returncode_t libtocksync_gpio_async_toggle(uint32_t port, uint8_t pin) {
  return gpio_async_command(port, pin, libtock_gpio_async_command_toggle);
}

returncode_t libtocksync_gpio_async_make_input(uint32_t port, uint8_t pin, libtock_gpio_input_mode_t pin_config) {
  returncode_t ret;

  ret = libtock_gpio_async_command_make_input(port, pin, pin_config);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_gpio_async_yield_wait_for_generic_command();
  return ret;
}

returncode_t libtocksync_gpio_async_read(uint32_t port, uint8_t pin, bool* value) {
  returncode_t ret;

  ret = libtock_gpio_async_command_read(port, pin);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_gpio_async_yield_wait_for_read(value);
  return ret;
}

returncode_t libtocksync_gpio_async_enable_interrupt(uint32_t port, uint8_t pin,
                                                     libtock_gpio_interrupt_mode_t irq_config) {
  returncode_t ret;

  ret = libtock_gpio_async_command_enable_interrupt(port, pin, irq_config);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_gpio_async_yield_wait_for_generic_command();
  return ret;
}

returncode_t libtocksync_gpio_async_disable_interrupt(uint32_t port, uint8_t pin) {
  return gpio_async_command(port, pin, libtock_gpio_async_command_disable_interrupt);
}

returncode_t libtocksync_gpio_async_disable_sync(uint32_t port, uint8_t pin) {
  return gpio_async_command(port, pin, libtock_gpio_async_command_disable);
}
