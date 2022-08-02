#include <stdio.h>
#include <string.h>

#include "tock.h"
#include "internal/ota.h"

/***************************** Interface of nonvolatile_storage_driver.rs (capsule) *****************************/
// Check the nonvolateil driver existance
int ota_nonvol_driver_check(void)
{
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, COMMAND_DRIVER_CHECK, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

// Check the size of flash memory allocated to userspace(not kernel space)
int ota_nonvol_userspace_size_check(int *userspace_size_bytes)
{
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, COMMAND_USERSPACE_SIZE_CHECK, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)userspace_size_bytes);
}

// Set sharable read buffer with nonvolatile_storage_driver.rs
int ota_flash_internal_read_buffer(uint8_t *buffer, uint32_t len)
{
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, IDs_FOR_NONVOL, (void *)buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

// Set sharable write buffer with nonvolatile_storage_driver.rs
int ota_flash_internal_write_buffer(uint8_t *buffer, uint32_t len)
{
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, IDs_FOR_NONVOL, (void *)buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

// Entitle callback function (when read action is done)
int ota_flash_read_done_subscribe(subscribe_upcall cb, void *userdata)
{
  subscribe_return_t sval = subscribe(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, OTA_READ_DONE_CALLBACK_NUM, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

// Entitle callback function (when write action is done)
int ota_flash_write_done_subscribe(subscribe_upcall cb, void *userdata)
{
  subscribe_return_t sval = subscribe(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, OTA_WRITE_DONE_CALLBACK_NUM, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

// Command to let the kernel read data from flash
int ota_data_read_execution(uint32_t offset, uint32_t length)
{
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, COMMAND_READ_DATA, (uint32_t)offset, (uint32_t)length);
  return tock_command_return_novalue_to_returncode(res);
}
// Command to let the kernel write data into flash
int ota_data_write_execution(uint32_t offset, uint32_t length)
{
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER, COMMAND_WRITE_DATA, (uint32_t)offset, (uint32_t)length);
  return tock_command_return_novalue_to_returncode(res);
}

/***************************** Interface of process_load_utilities.rs (kernel) *****************************/
// Commnad to let the kernel load a flashed app
int ota_app_loading_request(void)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_APP_LOAD_REQ, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

// Commnad to let the kernel set the dynamically changed writable flash start address
int ota_set_dynamic_start_address_of_writable_flash(uint32_t appsize)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_SET_DYNAMIC_START_ADDRESS, appsize, 0);
  return tock_command_return_novalue_to_returncode(res);
}

// Commnad to get the dynamically changed writable flash start address
int ota_get_dynamic_start_address_of_writable_flash(uint32_t *startaddress)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_GET_DYNAMIC_START_ADDRESS, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)startaddress);
}

// Commnad to get index (PROCESSES array at main.rs) for kernel to write the entry point of a flashed app
int ota_get_process_index(uint32_t *processes_index)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_GET_PROCESS_INDEX, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)processes_index);
}

// Commnad to let the kernel calculate CRC32-POSIX and to return the result
int ota_calculate_crc32(uint32_t *crc32_result)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_CAL_GET_CRC32, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)crc32_result);
}

// Commnad to let the kernel use the end address of sram (by apps loaded by tockloader) and index as the start address fo unused sram and next index respectively
int ota_init_sram_start_addr_and_index(void)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_SET_SRAM_START_ADDRESS, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

// Commnad to get the start address of flash memory region containing app images
int ota_get_flash_start_address(uint32_t *rom_start_address)
{
  syscall_return_t res = command(DRIVER_NUM_PROCLOADER_KERNEL, COMMAND_PROC_GET_FLASH_START_ADDRESS, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t *)rom_start_address);
}

/***************************** Interface of uart console.rs (capsule) *****************************/
// Set sharable read buffer with console.rs
int ota_uart_internal_read_buffer(uint8_t *buffer, uint32_t len)
{
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_CONSOLE, IDs_FOR_CONSOLE, (void *)buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

// Set sharable write buffer with console.rs
int ota_uart_internal_write_buffer(uint8_t *buffer, uint32_t len)
{
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_CONSOLE, IDs_FOR_CONSOLE, (void *)buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

// Entitle callback function (when read action is done)
int ota_uart_read_done_subscribe(subscribe_upcall cb, void *userdata)
{
  subscribe_return_t sval = subscribe(DRIVER_NUM_CONSOLE, COMMAND_CONSOLE_GET_STR, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

// Entitle callback function (when write action is done)
int ota_uart_write_done_subscribe(subscribe_upcall cb, void *userdata)
{
  subscribe_return_t res = subscribe(DRIVER_NUM_CONSOLE, COMMAND_CONSOLE_PUT_STR, cb, userdata);
  return tock_subscribe_return_to_returncode(res);
}

// Commnad to specify when callback function (by ota_uart_read_done_subscribe) have to be triggered
int ota_uart_data_read_cb_entry(uint32_t length)
{
  syscall_return_t res = command(DRIVER_NUM_CONSOLE, COMMAND_CONSOLE_GET_STR, length, 0);
  return tock_command_return_novalue_to_returncode(res);
}

// Commnad to write and send data via UART
int ota_uart_write_data(uint32_t len)
{
  syscall_return_t com = command(DRIVER_NUM_CONSOLE, COMMAND_CONSOLE_PUT_STR, len, 0);
  return tock_command_return_novalue_to_returncode(com);
}

// Commnad to send data via UART
int ota_putnstr_async(const char *str, size_t len, subscribe_upcall cb, void *userdata)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic pop

  allow_ro_return_t ro = allow_readonly(DRIVER_NUM_CONSOLE, COMMAND_CONSOLE_PUT_STR, str, len);
  if (!ro.success)
  {
    return tock_status_to_returncode(ro.status);
  }

  subscribe_return_t sub = subscribe(DRIVER_NUM_CONSOLE, COMMAND_CONSOLE_PUT_STR, cb, userdata);
  if (!sub.success)
  {
    return tock_status_to_returncode(sub.status);
  }

  syscall_return_t com = command(DRIVER_NUM_CONSOLE, COMMAND_CONSOLE_PUT_STR, len, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ota_getnstr_async(uint8_t *buf, size_t len, subscribe_upcall cb, void *userdata)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic pop
  allow_rw_return_t rw = allow_readwrite(DRIVER_NUM_CONSOLE, 1, buf, len);
  if (!rw.success)
  {
    return tock_status_to_returncode(rw.status);
  }

  subscribe_return_t sub = subscribe(DRIVER_NUM_CONSOLE, 2, cb, userdata);
  if (!sub.success)
  {
    return tock_status_to_returncode(sub.status);
  }

  syscall_return_t com = command(DRIVER_NUM_CONSOLE, 2, len, 0);
  return tock_command_return_novalue_to_returncode(com);
}