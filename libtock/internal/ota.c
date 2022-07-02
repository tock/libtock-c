#include <stdio.h>
#include <string.h>

#include "tock.h"
#include "internal/ota.h"

// Internal callback for synchronous interfaces
// Check ota driver exist. This dirver provide interface to access the NVM(Flash Memory) in order to write data from an app.
int ota_driver_check(void)
{
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, COMMAND_DRIVER_CHECK, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

// Check how many bytes are in userspace(not kernel space)
int ota_userspace_size_check(int *userspace_size_bytes)
{
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, COMMAND_USERSPACE_SIZE_CHECK, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)userspace_size_bytes);
}

// Provide grant for kernel
int ota_internal_read_buffer(uint8_t *buffer, uint32_t len)
{
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, OTA_APP_ID, (void *)buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}
// Provide grant for kernel
int ota_internal_write_buffer(uint8_t *buffer, uint32_t len)
{
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, OTA_APP_ID, (void *)buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

// call back setting for read
int ota_read_done_subscribe(subscribe_upcall cb, void *userdata)
{
  subscribe_return_t sval = subscribe(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, OTA_READ_DONE_CALLBACK_NUM, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

// call back setting for write
int ota_write_done_subscribe(subscribe_upcall cb, void *userdata)
{
  subscribe_return_t sval = subscribe(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, OTA_WRITE_DONE_CALLBACK_NUM, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

// For direct command for data read
int ota_data_read_execution(uint32_t offset, uint32_t length)
{
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, COMMAND_READ_DATA, (int)offset, (int)length);
  return tock_command_return_novalue_to_returncode(res);
}
// For direct command for data write
int ota_data_write_execution(uint32_t offset, uint32_t length)
{
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, COMMAND_WRITE_DATA, (int)offset, (int)length);
  return tock_command_return_novalue_to_returncode(res);
}

// PROCLOADER KERNEL
// For direct command for data write
int ota_app_loading_request(void)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_APP_LOAD_REQ, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

// For direct command for data write
int ota_set_start_address_of_writable_flash(void)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_SET_START_ADDRESS, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int ota_get_offset_of_start_address_of_writable_flash(uint32_t *startaddress)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_GET_START_ADDRESS, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)startaddress);
}

int ota_get_the_number_of_processes(uint32_t *processes_num)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_GET_PROCESS_NUMS, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)processes_num);
}

int ota_calculate_crc32(uint32_t *crc32_result)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_CAL_GET_CRC32, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)crc32_result);
}
