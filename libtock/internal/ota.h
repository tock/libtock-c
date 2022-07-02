#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "tock.h"

#define DRIVER_NUM_NONVOLATILE_STORAGE_DRIVER 0x50001
#define DRIVER_NUM_PROCLOADER_KERNEL 0x10001

#define OTA_APP_ID 0

#define OTA_READ_DONE_CALLBACK_NUM 0
#define OTA_WRITE_DONE_CALLBACK_NUM 1

#define OTA_READ_OFFSET_DONE_CALLBACK_NUM 0

#define COMMAND_DRIVER_CHECK 0
#define COMMAND_USERSPACE_SIZE_CHECK 1
#define COMMAND_READ_DATA 2
#define COMMAND_WRITE_DATA 3

#define COMMAND_PROC_APP_LOAD_REQ 1
#define COMMAND_PROC_SET_START_ADDRESS 2
#define COMMAND_PROC_GET_START_ADDRESS 3
#define COMMAND_PROC_GET_PROCESS_NUMS 4
#define COMMAND_PROC_CAL_GET_CRC32 5

#define COMMAND_APP_SCANNING 1

    int ota_driver_check(void);
    int ota_userspace_size_check(int *userspace_size_bytes);
    int ota_internal_read_buffer(uint8_t *buffer, uint32_t len);
    int ota_internal_write_buffer(uint8_t *buffer, uint32_t len);
    int ota_read_done_subscribe(subscribe_upcall cb, void *userdata);
    int ota_write_done_subscribe(subscribe_upcall cb, void *userdata);
    int ota_data_read_execution(uint32_t offset, uint32_t length);
    int ota_data_write_execution(uint32_t offset, uint32_t length);
    int ota_app_loading_request(void);
    int ota_set_start_address_of_writable_flash(void);
    int ota_get_offset_of_start_address_of_writable_flash(uint32_t *startaddress);
    int ota_get_the_number_of_processes(uint32_t *processes_num);
    int ota_calculate_crc32(uint32_t *crc32_result);

#ifdef __cplusplus
}
#endif
