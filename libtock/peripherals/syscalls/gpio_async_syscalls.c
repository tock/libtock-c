#include "gpio_async_syscalls.h"

#define CONCAT_PORT_DATA(port, data) (((data & 0xFFFF) << 16) | (port & 0xFFFF))



bool libtock_gpio_async_exists(void) {
  return driver_exists(DRIVER_NUM_GPIO_ASYNC);
}

returncode_t libtock_gpio_async_set_upcall_command(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_GPIO_ASYNC, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_gpio_async_set_upcall_interrupt(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_GPIO_ASYNC, 1, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_gpio_async_command_make_output(uint32_t port, uint8_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 1, pin, port);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_async_command_set(uint32_t port, uint8_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 2, pin, port);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_async_command_clear(uint32_t port, uint8_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 3, pin, port);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_async_command_toggle(uint32_t port, uint8_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 4, pin, port);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_async_command_make_input(uint32_t port, uint8_t pin, libtock_gpio_input_mode_t pin_config) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 5, pin, CONCAT_PORT_DATA(port, pin_config));
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_async_command_read(uint32_t port, uint8_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 6, pin, port);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_async_command_enable_interrupt(uint32_t port, uint8_t pin,
                                                         libtock_gpio_interrupt_mode_t irq_config) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 7, pin, CONCAT_PORT_DATA(port, irq_config));
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_async_command_disable_interrupt(uint32_t port, uint8_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 8, pin, port);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_async_command_disable(uint32_t port, uint8_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_GPIO_ASYNC, 9, pin, port);
  return tock_command_return_novalue_to_returncode(cval);
}
