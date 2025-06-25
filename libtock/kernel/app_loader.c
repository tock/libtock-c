#include "app_loader.h"

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_setup(subscribe_upcall cb, void* userdata) {
  return libtock_app_loader_set_setup_upcall(cb, userdata);
}

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_write(subscribe_upcall cb, void* userdata) {
  return libtock_app_loader_set_write_upcall(cb, userdata);
}

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_finalize(subscribe_upcall cb, void* userdata) {
  return libtock_app_loader_set_finalize_upcall(cb, userdata);
}

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_load(subscribe_upcall cb, void* userdata) {
  return libtock_app_loader_set_load_upcall(cb, userdata);
}

/*
 * Function to setup the callback from capsule.
 * This function takes in the function that will be executed
 * when the callback is triggered.
 */
returncode_t libtock_app_loader_subscribe_abort(subscribe_upcall cb, void* userdata) {
  return libtock_app_loader_set_abort_upcall(cb, userdata);
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
returncode_t libtock_app_loader_finalize(void) {
  return libtock_app_loader_command_finalize();
}

/*
 * Command to request the kernel to load the newly flashed app.
 */
returncode_t libtock_app_loader_load(void) {
  return libtock_app_loader_command_load();
}

/*
 * Command to request the kernel to abort setup/write process.
 */
returncode_t libtock_app_loader_abort(void) {
  return libtock_app_loader_command_abort();
}