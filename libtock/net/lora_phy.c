#include "lora_phy.h"
#include "syscalls/lora_phy_syscalls.h"



static void lora_phy_spi_upcall(__attribute__ ((unused)) int unused0,
                                __attribute__ ((unused)) int unused1,
                                __attribute__ ((unused)) int unused2,
                                void*                        opaque) {
  libtock_lora_phy_callback_spi cb = (libtock_lora_phy_callback_spi) opaque;
  cb(RETURNCODE_SUCCESS);
}

returncode_t libtock_lora_phy_write(const uint8_t*                buf,
                                    uint32_t                      len,
                                    libtock_lora_phy_callback_spi cb) {
  returncode_t ret;

  ret = libtock_lora_phy_set_readonly_allow_master_write_buffer(buf, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_lora_phy_set_upcall_spi(lora_phy_spi_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_lora_phy_command_read_write(len);
  return ret;
}

returncode_t libtock_lora_phy_read_write(const uint8_t*                write,
                                         uint8_t*                      read,
                                         uint32_t                      len,
                                         libtock_lora_phy_callback_spi cb) {
  returncode_t ret;

  ret = libtock_lora_phy_set_readwrite_allow_master_read_buffer(read, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtock_lora_phy_write(write, len, cb);
}

static void lora_phy_gpio_upcall( int                          pin_number,
                                  int                          val,
                                  __attribute__ ((unused)) int unused2,
                                  void*                        opaque) {
  libtock_gpio_callback_interrupt cb = (libtock_gpio_callback_interrupt) opaque;
  cb(pin_number, val == 1);
}

returncode_t libtock_lora_phy_gpio_enable_output(uint32_t pin) {
  return libtock_lora_phy_gpio_command_enable_output(pin);
}

returncode_t libtock_lora_phy_gpio_set(uint32_t pin) {
  return libtock_lora_phy_gpio_command_set(pin);
}

returncode_t libtock_lora_phy_gpio_clear(uint32_t pin) {
  return libtock_lora_phy_gpio_command_clear(pin);
}

returncode_t libtock_lora_phy_gpio_toggle(uint32_t pin){
  return libtock_lora_phy_gpio_command_toggle(pin);
}

returncode_t libtock_lora_phy_gpio_enable_input(uint32_t pin, libtock_gpio_input_mode_t pin_config){
  return libtock_lora_phy_gpio_command_enable_input(pin, (uint32_t) pin_config);
}

returncode_t libtock_lora_phy_gpio_read(uint32_t pin, int* pin_value){
  return libtock_lora_phy_gpio_command_read(pin, (uint32_t*) pin_value);
}

returncode_t libtock_lora_phy_gpio_enable_interrupt(uint32_t pin, libtock_gpio_interrupt_mode_t irq_config){
  return libtock_lora_phy_gpio_command_enable_interrupt(pin, (uint32_t) irq_config);
}

returncode_t libtock_lora_phy_gpio_disable_interrupt(uint32_t pin){
  return libtock_lora_phy_gpio_command_disable_interrupt(pin);
}

returncode_t libtock_lora_phy_gpio_set_callback(libtock_gpio_callback_interrupt cb) {
  return libtock_lora_phy_gpio_command_interrupt_callback(lora_phy_gpio_upcall, cb);
}
