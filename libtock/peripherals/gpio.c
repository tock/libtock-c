#include "gpio.h"
#include "syscalls/gpio_syscalls.h"

static void gpio_upcall(int                          pin_number,
                        int                          pin_level,
                        __attribute__ ((unused)) int unused2,
                        void*                        opaque) {
  libtock_gpio_callback_interrupt cb = (libtock_gpio_callback_interrupt) opaque;
  cb((uint32_t) pin_number, pin_level == 1);
}

bool libtock_gpio_exists(void) {
  return libtock_gpio_driver_exists();
}

returncode_t libtock_gpio_count(int* count) {
  return libtock_gpio_command_count((uint32_t*) count);
}

returncode_t libtock_gpio_set_interrupt_callback(libtock_gpio_callback_interrupt cb) {
  return libtock_gpio_set_upcall(gpio_upcall, cb);
}

returncode_t libtock_gpio_enable_input(uint32_t pin, libtock_gpio_input_mode_t pin_config) {
  return libtock_gpio_command_enable_input(pin, (uint32_t) pin_config);
}

returncode_t libtock_gpio_enable_output(uint32_t pin) {
  return libtock_gpio_command_enable_output(pin);
}

returncode_t libtock_gpio_set(uint32_t pin) {
  return libtock_gpio_command_set(pin);
}

returncode_t libtock_gpio_clear(uint32_t pin) {
  return libtock_gpio_command_clear(pin);
}

returncode_t libtock_gpio_toggle(uint32_t pin) {
  return libtock_gpio_command_toggle(pin);
}

returncode_t libtock_gpio_read(uint32_t pin, int* pin_value) {
  return libtock_gpio_command_read(pin, (uint32_t*) pin_value);
}

returncode_t libtock_gpio_enable_interrupt(uint32_t pin, libtock_gpio_interrupt_mode_t irq_config) {
  return libtock_gpio_command_enable_interrupt(pin, (uint32_t) irq_config);
}

returncode_t libtock_gpio_disable_interrupt(uint32_t pin) {
  return libtock_gpio_command_disable_interrupt(pin);
}

returncode_t libtock_gpio_disable(uint32_t pin) {
  return libtock_gpio_command_disable(pin);
}
