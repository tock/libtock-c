#include <string.h>

#include "app_state.h"


// Internal callback for synchronous interfaces
static void app_state_upcall(__attribute__ ((unused)) int callback_type,
                             __attribute__ ((unused)) int value,
                             __attribute__ ((unused)) int unused,
                             void*                        opaque) {
  libtock_app_state_callback cb = (libtock_app_state_callback) opaque;
  cb(RETURNCODE_SUCCESS);
}


static returncode_t app_state_init(void) {
  returncode_t ret;

  ret = libtock_app_state_set_readonly_allow(_app_state_ram_pointer, _app_state_size);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Check that we have a region to use for this.
  int number_regions = tock_app_number_writeable_flash_regions();
  if (number_regions == 0) return RETURNCODE_ENOMEM;

  // Get the pointer to flash which we need to ask the kernel where it is.
  _app_state_flash_pointer = tock_app_writeable_flash_region_begins_at(0);

  _app_state_inited = true;
  return RETURNCODE_SUCCESS;
}


returncode_t libtock_app_state_load(void) {
  if (!_app_state_inited) {
    int err;
    err = app_state_init();
    if (err != RETURNCODE_SUCCESS) return err;
  }

  memcpy(_app_state_ram_pointer, _app_state_flash_pointer, _app_state_size);
  return RETURNCODE_SUCCESS;
}

returncode_t libtock_app_state_save(libtock_app_state_callback cb) {
  returncode_t err;

  if (!_app_state_inited) {
    err = app_state_init();
    if (err != RETURNCODE_SUCCESS) return err;
  }

  err = libtock_app_state_set_upcall(app_state_upcall, (void*) cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_app_state_command_save((uint32_t) _app_state_flash_pointer);
  return err;
}
