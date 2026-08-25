#include <libtock/defer.h>

#include "ipc_registry_package_name.h"

#include "syscalls/ipc_registry_package_name_syscalls.h"

bool libtocksync_ipc_registry_package_name_exists(void) {
  return libtock_ipc_registry_package_name_driver_exists();
}

returncode_t libtocksync_ipc_registry_package_name_register_service(void) {
  returncode_t err = RETURNCODE_SUCCESS;
  err = libtock_ipc_registry_package_name_command_register();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ipc_registry_package_name_yield_wait_for_registration_complete();
  return err;
}

returncode_t libtocksync_ipc_registry_package_name_discover_service(uint8_t* package_name, size_t package_name_len,
                                                                    uint64_t* ipc_id) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_registry_package_name_set_allow_name(package_name, package_name_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ipc_registry_package_name_set_allow_name(NULL, 0);
  }

  err = libtock_ipc_registry_package_name_command_discover();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ipc_registry_package_name_yield_wait_for_discovery_complete(ipc_id);
  return err;
}
