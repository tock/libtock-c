#include <stdio.h>

#include <libtock/kernel/ipc.h>

#include "sensor_service.h"

typedef void (*sensor_service_callback)(void);

static sensor_service_callback callback;

static void ipc_callback(__attribute__ ((unused)) int   pid,
                         __attribute__ ((unused)) int   len,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* ud) {
  callback();
}

returncode_t connect_to_sensor_service(char* ipc_buf, sensor_service_callback cb) {
  int err;
  size_t svc_num = 0;

  // Save the callback to use when we get notified.
  callback = cb;

  // Find the sensing service.
  err = ipc_discover("soil_moisture_sensor", &svc_num);
  if (err < 0) {
    printf("No soil moisture service\n");
    return err;
  }
  // Setup our local callback for when new sensor data is ready.
  err = ipc_register_client_callback(svc_num, ipc_callback, NULL);
  if (err < 0) {
    printf("No ipc_register_client_callback\n");
    return err;
  }
  // Share a buffer with the service to send us data.
  err = ipc_share(svc_num, ipc_buf, 64);
  if (err < 0) {
    printf("No ipc_share\n");
    return err;
  }
  // Notify that we exist so the service will send us data.
  err = ipc_notify_service(svc_num);
  if (err < 0) {
    printf("No ipc_notify_service\n");
    return err;
  }

  return RETURNCODE_SUCCESS;
}