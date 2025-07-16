#include "aes.h"

#include "syscalls/aes_syscalls.h"

bool libtock_aes_exists(void) {
  return libtock_aes_driver_exists();
}

returncode_t libtock_aes_set_algorithm(libtock_aes_algorithm_t operation, bool encrypting) {
  return libtock_aes_command_set_algorithm((uint8_t) operation, encrypting);
}

returncode_t libtock_aes_setup(void) {
  return libtock_aes_command_setup();
}

returncode_t libtock_aes_crypt(void) {
  return libtock_aes_command_crypt();
}

returncode_t libtock_aes_finish(void) {
  return libtock_aes_command_finish();
}

returncode_t libtock_aes_ccm_set_a_off(uint32_t value) {
  return libtock_aes_command_ccm_set_a_off(value);
}

returncode_t libtock_aes_ccm_set_m_off(uint32_t value) {
  return libtock_aes_command_ccm_set_m_off(value);
}

returncode_t libtock_aes_ccm_set_mic_len(uint32_t value) {
  return libtock_aes_command_ccm_set_mic_len(value);
}

returncode_t libtock_aes_ccm_set_confidential(bool value) {
  return libtock_aes_command_ccm_set_confidential(value);
}
