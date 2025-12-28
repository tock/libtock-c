#include "ipc_registry_string_name.h"

#include "syscalls/ipc_registry_string_name_syscalls.h"

// callback for synchronous reads
static void ipc_registry_string_name_upcall(bool succeeded,
                                 __attribute__ ((unused)) int unused1,
                                 __attribute__ ((unused)) int unused2, void* opaque) {
  libtock_ipc_registry_string_name_registration_complete_callback cb = (libtock_ipc_registry_string_name_registration_complete_callback) opaque;
  cb(RETURNCODE_SUCCESS, succeeded);
}

bool libtock_ipc_registry_string_name_exists(void) {
  return libtock_ipc_registry_string_name_driver_exists();
}

returncode_t libtock_register_service(
    libtock_ipc_registry_string_name_registration_complete_callback cb,
    uint8_t* string_name,
    uint8_t string_name_len) {
  returncode_t err;

  // TODO

  return err;
}
