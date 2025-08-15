#include <stdio.h>
#include <string.h>

#include <libtock-sync/sensors/ambient_light.h>
#include <libtock-sync/sensors/humidity.h>
#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/ipc.h>

size_t _svc_num = 0;

char buf[64] __attribute__((aligned(64)));

typedef enum {
  SENSOR_TEMPERATURE = 0,
  SENSOR_IRRADIANCE  = 1,
  SENSOR_HUMIDITY    = 2,
} sensor_type_e;

typedef struct {
  int type;  // sensor type
  int value; // sensor reading
} sensor_update_t;

bool _ipc_done = false;
libtock_alarm_t _alarm;

static void ipc_callback(__attribute__ ((unused)) int   pid,
                         __attribute__ ((unused)) int   len,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* ud) {
  _ipc_done = true;
}


static void do_sensing_cb(__attribute__ ((unused)) uint32_t now,
                          __attribute__ ((unused)) uint32_t scheduled,
                          __attribute__ ((unused)) void*    opaque) {

  printf("[BLE ESS Test] Sampling Sensors\n");

  sensor_update_t* update = (sensor_update_t*) buf;

  int light = 0;
  int temp  = 0;
  int humi  = 0;

  if (libtocksync_ambient_light_exists()) {
    libtocksync_ambient_light_read_intensity(&light);

    update->type  = SENSOR_IRRADIANCE;
    update->value = light;
    ipc_notify_service(_svc_num);
    _ipc_done = false;
    yield_for(&_ipc_done);
  }

  if (libtocksync_temperature_exists()) {
    libtocksync_temperature_read(&temp);

    update->type  = SENSOR_TEMPERATURE;
    update->value = temp;
    ipc_notify_service(_svc_num);
    _ipc_done = false;
    yield_for(&_ipc_done);
  }

  if (libtocksync_humidity_exists()) {
    libtocksync_humidity_read(&humi);

    update->type  = SENSOR_HUMIDITY;
    update->value = humi;
    ipc_notify_service(_svc_num);
    _ipc_done = false;
    yield_for(&_ipc_done);
  }

  printf("Setting ESS to:\n");
  printf("  light: %i\n", light);
  printf("  temp:  %i\n", temp);
  printf("  humi:  %i\n", humi);

  libtock_alarm_in_ms(3000, do_sensing_cb,  NULL, &_alarm);
}


int main(void) {
  int err = ipc_discover("org.tockos.services.ble-ess", &_svc_num);
  if (err < 0) {
    printf("No BLE ESS service installed.\n");
    return -1;
  }

  printf("Found BLE ESS service (%u)\n", _svc_num);

  libtocksync_alarm_delay_ms(1500);

  sensor_update_t* update = (sensor_update_t*) buf;
  ipc_register_client_callback(_svc_num, ipc_callback, update);
  ipc_share(_svc_num, buf, 64);

  libtock_alarm_in_ms(1000, do_sensing_cb, NULL,  &_alarm);

  return 0;
}
