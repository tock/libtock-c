#include "gpio_async.h"

struct gpio_async_data {
  bool fired;
  bool value;
  returncode_t ret;
};

static struct gpio_async_data result = { .fired = false };

static void gpio_async_callback_command(returncode_t ret, bool value) {
  result.fired = true;
  result.value = value;
  result.ret   = ret;
}

static returncode_t gpio_async_op(uint32_t port, uint8_t pin, returncode_t (*op)(uint32_t, uint8_t,
                                                                                 libtock_gpio_async_callback_command)) {
  returncode_t err;
  result.fired = false;

  err = op(port, pin, gpio_async_callback_command);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_gpio_async_make_output_sync(uint32_t port, uint8_t pin) {
  return gpio_async_op(port, pin, libtock_gpio_async_make_output);
}

returncode_t libtocksync_gpio_async_set(uint32_t port, uint8_t pin) {
  return gpio_async_op(port, pin, libtock_gpio_async_set);
}

returncode_t libtocksync_gpio_async_clear(uint32_t port, uint8_t pin) {
  return gpio_async_op(port, pin, libtock_gpio_async_clear);
}

returncode_t libtocksync_gpio_async_toggle(uint32_t port, uint8_t pin) {
  return gpio_async_op(port, pin, libtock_gpio_async_toggle);
}

returncode_t libtocksync_gpio_async_make_input(uint32_t port, uint8_t pin, libtock_gpio_input_mode_t pin_config) {
  returncode_t err;
  result.fired = false;

  err = libtock_gpio_async_make_input(port, pin, pin_config, gpio_async_callback_command);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_gpio_async_read(uint32_t port, uint8_t pin, bool* value) {
  returncode_t err;
  result.fired = false;

  err = libtock_gpio_async_read(port, pin, gpio_async_callback_command);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *value = result.value;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_gpio_async_enable_interrupt(uint32_t port, uint8_t pin,
                                                     libtock_gpio_interrupt_mode_t irq_config) {
  returncode_t err;
  result.fired = false;

  err = libtock_gpio_async_enable_interrupt(port, pin, irq_config, gpio_async_callback_command);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_gpio_async_disable_interrupt(uint32_t port, uint8_t pin) {
  return gpio_async_op(port, pin, libtock_gpio_async_disable_interrupt);
}

returncode_t libtocksync_gpio_async_disable_sync(uint32_t port, uint8_t pin) {
  return gpio_async_op(port, pin, libtock_gpio_async_disable);
}
