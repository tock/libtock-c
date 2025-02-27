#include "process_info.h"


returncode_t libtock_process_info_get_process_ids(uint8_t* buffer, size_t buffer_length,uint32_t* count) {
  returncode_t ret;

  ret = libtock_process_info_set_allow_readwrite_info_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_process_info_command_get_process_ids(count);
  return ret;
}

returncode_t libtock_process_info_get_short_ids(uint8_t* buffer, size_t buffer_length,uint32_t* count) {
  returncode_t ret;

  ret = libtock_process_info_set_allow_readwrite_info_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_process_info_command_get_short_ids(count);
  return ret;
}

returncode_t libtock_process_info_get_process_name(uint32_t process_id, uint8_t* buffer, size_t buffer_length){
  returncode_t ret;

  ret = libtock_process_info_set_allow_readwrite_info_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_process_info_command_get_process_name(process_id);
  return ret;
}
