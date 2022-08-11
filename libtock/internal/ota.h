#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "tock.h"

/* Driver number for system call */
#define DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER 0x50001
#define DRIVER_NUM_PROCLOADER_KERNEL 0x10001
#define DRIVER_NUM_CONSOLE 0x1

/* COMMAND SET OF NONVOLATILE_STORAGE_DRIVER */
#define IDs_FOR_NONVOL 0

#define OTA_READ_DONE_CALLBACK_NUM 0
#define OTA_WRITE_DONE_CALLBACK_NUM 1

#define COMMAND_DRIVER_CHECK 0
#define COMMAND_USERSPACE_SIZE_CHECK 1
#define COMMAND_READ_DATA 2
#define COMMAND_WRITE_DATA 3

/* COMMAND SET OF PROCESS_LOAD_UTILITIES */
#define COMMAND_PROC_APP_LOAD_REQ 1
#define COMMAND_PROC_SET_DYNAMIC_START_ADDRESS 2
#define COMMAND_PROC_GET_DYNAMIC_START_ADDRESS 3
#define COMMAND_PROC_SET_SRAM_START_ADDRESS 4
#define COMMAND_PROC_CAL_GET_CRC32 5
#define COMMAND_PROC_GET_PROCESS_INDEX 6
#define COMMAND_PROC_GET_FLASH_START_ADDRESS 7
#define COMMAND_PROC_GET_SUPPORTED_PROCESS_NUM 8
#define COMMAND_PROC_GET_PROCESS_START_ADDRESS 9
#define COMMAND_PROC_GET_PROCESS_SIZE 10
#define COMMAND_PROC_GET_KERNEL_VERSION 11
#define COMMAND_PROC_GET_PADDING_APP_HEADER_LEN 12

#define CRC_MODE_FOR_LOAD_APP 1
#define CRC_MODE_FOR_PADDING_APP 2

/* COMMAND SET OF CONSOLE */
#define IDs_FOR_CONSOLE 1
#define COMMAND_CONSOLE_PUT_STR 1
#define COMMAND_CONSOLE_GET_STR 2

    int ota_nonvol_driver_check(void);
    int ota_nonvol_userspace_size_check(int *userspace_size_bytes);

    int ota_flash_internal_read_buffer(uint8_t *buffer, uint32_t len);
    int ota_flash_internal_write_buffer(uint8_t *buffer, uint32_t len);
    int ota_flash_read_done_subscribe(subscribe_upcall cb, void *userdata);
    int ota_flash_write_done_subscribe(subscribe_upcall cb, void *userdata);
    int ota_data_read_execution(uint32_t offset, uint32_t length);
    int ota_data_write_execution(uint32_t offset, uint32_t length);

    int ota_app_loading_request(void);
    int ota_set_dynamic_start_address_of_writable_flash(uint32_t appsize);
    int ota_get_dynamic_start_address_of_writable_flash(uint32_t *startaddress);
    int ota_get_process_index(uint32_t *processes_num);
    int ota_get_supported_process_num(uint32_t *supported_processes_num);
    int ota_calculate_crc32(uint32_t *crc32_result, uint32_t start_address, uint32_t mode);
    int ota_init_sram_start_addr_and_index(void);
    int ota_get_flash_start_address(uint32_t *rom_start_address);
    int ota_get_process_start_address(uint32_t *process_start_address, uint32_t index);
    int ota_get_process_size(uint32_t *process_size, uint32_t index);
    int ota_get_kernel_version(uint32_t *kernel_version);
    int ota_get_padding_app_header_length(uint32_t *padding_app_header_len);

    int ota_uart_internal_read_buffer(uint8_t *buffer, uint32_t len);
    int ota_uart_internal_write_buffer(uint8_t *buffer, uint32_t len);
    int ota_uart_read_done_subscribe(subscribe_upcall cb, void *userdata);
    int ota_uart_write_done_subscribe(subscribe_upcall cb, void *userdata);
    int ota_uart_data_read_cb_entry(uint32_t length);
    int ota_uart_write_data(uint32_t len);

    int ota_putnstr_async(const char *str, size_t len, subscribe_upcall cb, void *userdata);
    int ota_getnstr_async(uint8_t *buf, size_t len, subscribe_upcall cb, void *userdata);

#ifdef __cplusplus
}
#endif
