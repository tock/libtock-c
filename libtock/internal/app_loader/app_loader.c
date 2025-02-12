#include "libtock/internal/app_loader/app_loader.h"

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_set_setup_upcall(subscribe_upcall cb, void* userdata) {
  return libtock_app_loader_setup_subscribe(cb, userdata);
}

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_set_write_upcall(subscribe_upcall cb, void* userdata) {
  return libtock_app_loader_write_subscribe(cb, userdata);
}

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_set_load_upcall(subscribe_upcall cb, void* userdata) {
  return libtock_app_loader_load_subscribe(cb, userdata);
}

/*
 * Command to request the kernel to set up for a new app.
 * This functions takes the size of the new app as the argument.
 */
returncode_t libtock_app_loader_setup(uint32_t app_length) {
  return libtock_app_loader_command_setup(app_length);
}


/*
 * Function to set up a shared buffer with the capsule.
 * This buffer is used to write data to the flash.
 */
returncode_t libtock_app_loader_set_buffer(uint8_t* buffer, uint32_t len) {
  return libtock_app_loader_write_buffer(buffer, len);
}

/*
 * Command to write new app binary to the capsule.
 * Specify the offset at which the data segment should be written.
 * Also specify how long the data segment will be in bytes.
 *
 * After writing data, the userspace app should yield for
 * the write done callback from the capsule.
 */
returncode_t libtock_app_loader_write(uint32_t flash_offset, uint32_t write_length) {
  return libtock_app_loader_command_write(flash_offset, write_length);
}

/*
 * Command to request the kernel to load the newly flashed app.
 */
returncode_t libtock_app_loader_load(void) {
  return libtock_app_loader_command_load();
}