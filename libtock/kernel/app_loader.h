#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "libtock/kernel/syscalls/app_loader_syscalls.h"
#include "libtock/tock.h"

returncode_t libtock_app_loader_setup(uint32_t app_length, subscribe_upcall cb);

returncode_t libtock_app_loader_write(double size, uint8_t binary[]);

returncode_t libtock_app_loader_finalize(subscribe_upcall cb);

returncode_t libtock_app_loader_load(subscribe_upcall cb);

returncode_t libtock_app_loader_abort(subscribe_upcall cb);

returncode_t libtock_app_loader_uninstall(uint32_t app_short_id, uint32_t app_version, subscribe_upcall cb);


#ifdef __cplusplus
}
#endif
