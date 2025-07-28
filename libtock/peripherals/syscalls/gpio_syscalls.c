#include "gpio_syscalls.h"

bool libtock_gpio_driver_exists(void) {
  return driver_exists(GPIO_DRIVER_NUM);
}

returncode_t libtock_gpio_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(GPIO_DRIVER_NUM, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_gpio_command_count(uint32_t* count) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 10, 0, 0);
  return tock_command_return_u32_to_returncode(cval, count);
}

returncode_t libtock_gpio_command_enable_output(uint32_t pin) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 1, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_command_enable_input(uint32_t pin, uint32_t pin_config) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 5, pin, pin_config);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_command_set(uint32_t pin) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 2, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_command_clear(uint32_t pin) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 3, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_command_toggle(uint32_t pin) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 4, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_command_read(uint32_t pin, uint32_t* pin_value) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 6, pin, 0);
  return tock_command_return_u32_to_returncode(cval, pin_value);
}

returncode_t libtock_gpio_command_enable_interrupt(uint32_t pin, uint32_t irq_config) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 7, pin, irq_config);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_command_disable_interrupt(uint32_t pin) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 8, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_gpio_command_disable(uint32_t pin) {
  syscall_return_t cval = command(GPIO_DRIVER_NUM, 9, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
