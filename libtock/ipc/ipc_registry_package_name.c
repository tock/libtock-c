#include "ipc_registry_package_name.h"

#include "syscalls/ipc_registry_package_name_syscalls.h"

// callback for registration complete
static void registration_complete_upcall(int statuscode,
                                         __attribute__ ((unused)) int unused1,
                                         __attribute__ ((unused)) int unused2, void* opaque) {
  // Cleanup before performing callback
  libtock_ipc_registry_package_name_set_upcall_registration_complete(NULL, NULL);

  libtock_ipc_registry_package_name_registration_complete_callback cb =
    (libtock_ipc_registry_package_name_registration_complete_callback)opaque;
  cb(tock_status_to_returncode(statuscode));
}

// callback for discovery complete
static void discovery_complete_upcall(int statuscode, int lower_val, int upper_val, void* opaque) {
  // Cleanup before performing callback
  libtock_ipc_registry_package_name_set_allow_name(NULL, 0);
  libtock_ipc_registry_package_name_set_upcall_discovery_complete(NULL, NULL);

  libtock_ipc_registry_package_name_discovery_complete_callback cb =
    (libtock_ipc_registry_package_name_discovery_complete_callback)opaque;
  uint32_t lower  = (uint32_t)lower_val;
  uint32_t upper  = (uint32_t)upper_val;
  uint64_t ipc_id = (((uint64_t)upper) << 32) | ((uint64_t)lower);
  cb(tock_status_to_returncode(statuscode), ipc_id);
}

bool libtock_ipc_registry_package_name_exists(void) {
  return libtock_ipc_registry_package_name_driver_exists();
}

returncode_t libtock_ipc_registry_package_name_register_service(
  libtock_ipc_registry_package_name_registration_complete_callback cb) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_registry_package_name_set_upcall_registration_complete(registration_complete_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_registry_package_name_command_register();
  return err;
}

returncode_t libtock_ipc_registry_package_name_discover_service(
  libtock_ipc_registry_package_name_discovery_complete_callback cb,
  uint8_t*                                                      package_name,
  size_t                                                        package_name_len) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_registry_package_name_set_allow_name(package_name, package_name_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_registry_package_name_set_upcall_discovery_complete(discovery_complete_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_registry_package_name_command_discover();
  return err;
}
