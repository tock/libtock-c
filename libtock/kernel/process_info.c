#include "process_info.h"
#include "syscalls/process_info_syscalls.h"


bool libtock_process_info_exists(void) {
  return libtock_process_info_driver_exists();
}

returncode_t libtock_process_info_get_process_count(uint32_t* count) {
  return libtock_process_info_command_get_process_count(count);
}

returncode_t libtock_process_info_get_process_ids(uint8_t* buffer, size_t buffer_length, uint32_t* count) {
  returncode_t ret;

  ret = libtock_process_info_set_allow_readwrite_info_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_process_info_command_get_process_ids(count);
  return ret;
}

returncode_t libtock_process_info_get_short_ids(uint8_t* buffer, size_t buffer_length, uint32_t* count) {
  returncode_t ret;

  ret = libtock_process_info_set_allow_readwrite_info_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_process_info_command_get_short_ids(count);
  return ret;
}

returncode_t libtock_process_info_get_process_name(uint32_t process_id, uint8_t* buffer, size_t buffer_length) {
  returncode_t ret;

  ret = libtock_process_info_set_allow_readwrite_info_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  uint32_t name_len = 0;
  ret = libtock_process_info_command_get_process_name(process_id, &name_len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_process_info_set_allow_readwrite_info_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  if (name_len < buffer_length) {
    buffer[name_len] = '\0';
  }

  return ret;
}

returncode_t libtock_process_info_get_process_stats(uint32_t process_id, uint8_t* buffer, size_t buffer_length) {
  returncode_t ret;

  ret = libtock_process_info_set_allow_readwrite_info_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_process_info_command_get_process_stats(process_id);
  return ret;
}

returncode_t libtock_process_info_set_process_state(uint32_t process_id, uint32_t state) {
  return libtock_process_info_command_set_process_state(process_id,  state);
}
