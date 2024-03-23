#include "lora_phy_syscalls.h"

bool libtock_lora_phy_exists(void) {
  return driver_exists(DRIVER_NUM_LORA_PHY_SPI) && driver_exists(DRIVER_NUM_LORA_PHY_GPIO);
}

returncode_t libtock_lora_phy_set_upcall_spi(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_LORA_PHY_SPI, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_lora_phy_set_readonly_allow_master_write_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_LORA_PHY_SPI, 0, (const void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_lora_phy_set_readwrite_allow_master_read_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_LORA_PHY_SPI, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_lora_phy_command_read_write(uint32_t length) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_SPI, 2, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_lora_phy_gpio_command_enable_output(uint32_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_GPIO, 1, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_lora_phy_gpio_command_set(uint32_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_GPIO, 2, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_lora_phy_gpio_command_clear(uint32_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_GPIO, 3, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_lora_phy_gpio_command_toggle(uint32_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_GPIO, 4, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_lora_phy_gpio_command_enable_input(uint32_t pin, uint32_t pin_config) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_GPIO, 5, pin, pin_config);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_lora_phy_gpio_command_read(uint32_t pin, uint32_t* pin_value) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_GPIO, 6, pin, 0);
  return tock_command_return_u32_to_returncode(cval, pin_value);
}

returncode_t libtock_lora_phy_gpio_command_enable_interrupt(uint32_t pin, uint32_t irq_config) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_GPIO, 7, pin, irq_config);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_lora_phy_gpio_command_disable_interrupt(uint32_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_LORA_PHY_GPIO, 8, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_lora_phy_gpio_command_interrupt_callback(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_LORA_PHY_GPIO, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}
