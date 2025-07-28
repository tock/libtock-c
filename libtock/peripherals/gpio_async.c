#include "gpio_async.h"

#include "syscalls/gpio_async_syscalls.h"

static void gpio_async_upcall_interrupt(int                          pin_number,
                                        int                          value,
                                        __attribute__ ((unused)) int unused2,
                                        void*                        opaque) {
  libtock_gpio_async_callback_interrupt cb = (libtock_gpio_async_callback_interrupt) opaque;
  cb(0, (uint32_t) pin_number, value == 1);
}

static void gpio_async_upcall_command(__attribute__ ((unused)) int unused1,
                                      int                          value,
                                      __attribute__ ((unused)) int unused2,
                                      void*                        opaque) {
  libtock_gpio_async_callback_command cb = (libtock_gpio_async_callback_command) opaque;
  cb(RETURNCODE_SUCCESS, value);
}

bool libtock_gpio_async_exists(void) {
  return libtock_gpio_async_driver_exists();
}

returncode_t libtock_gpio_async_set_interrupt_callback(libtock_gpio_async_callback_interrupt cb) {
  return libtock_gpio_async_set_upcall_interrupt(gpio_async_upcall_interrupt, cb);
}

static returncode_t gpio_async_operation(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb,
                                         returncode_t (*op)(uint32_t, uint8_t)) {
  returncode_t ret;

  ret = libtock_gpio_async_set_upcall_command(gpio_async_upcall_command, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = op(port, pin);
  return ret;
}

returncode_t libtock_gpio_async_make_output(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb) {
  return gpio_async_operation(port, pin, cb, libtock_gpio_async_command_make_output);
}

returncode_t libtock_gpio_async_clear(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb) {
  return gpio_async_operation(port, pin, cb, libtock_gpio_async_command_set);
}

returncode_t libtock_gpio_async_set(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb) {
  return gpio_async_operation(port, pin, cb, libtock_gpio_async_command_clear);
}

returncode_t libtock_gpio_async_toggle(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb) {
  return gpio_async_operation(port, pin, cb, libtock_gpio_async_command_toggle);
}

returncode_t libtock_gpio_async_make_input(uint32_t port, uint8_t pin, libtock_gpio_input_mode_t pin_config,
                                           libtock_gpio_async_callback_command cb) {
  returncode_t ret;

  ret = libtock_gpio_async_set_upcall_command(gpio_async_upcall_command, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_gpio_async_command_make_input(port, pin, (uint32_t) pin_config);
  return ret;
}

returncode_t libtock_gpio_async_read(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb) {
  return gpio_async_operation(port, pin, cb, libtock_gpio_async_command_read);
}

returncode_t libtock_gpio_async_enable_interrupt(uint32_t port, uint8_t pin, libtock_gpio_interrupt_mode_t irq_config,
                                                 libtock_gpio_async_callback_command cb) {
  returncode_t ret;

  ret = libtock_gpio_async_set_upcall_command(gpio_async_upcall_command, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_gpio_async_command_enable_interrupt(port, pin, (uint32_t) irq_config);
  return ret;
}

returncode_t libtock_gpio_async_disable_interrupt(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb) {
  return gpio_async_operation(port, pin, cb, libtock_gpio_async_command_disable_interrupt);
}

returncode_t libtock_gpio_async_disable(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb) {
  return gpio_async_operation(port, pin, cb, libtock_gpio_async_command_disable);
}
