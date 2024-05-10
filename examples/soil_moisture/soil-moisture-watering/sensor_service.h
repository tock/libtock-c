#pragma once

#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*sensor_service_callback)(uint32_t);

returncode_t connect_to_sensor_service(char* ipc_buf, sensor_service_callback cb);

#ifdef __cplusplus
}
#endif
