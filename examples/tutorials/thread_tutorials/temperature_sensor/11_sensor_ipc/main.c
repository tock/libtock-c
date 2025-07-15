#include <stdio.h>

#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/ipc.h>
#include <libtock/tock.h>

static int current_temperature = 0;

static void sensor_ipc_callback(int pid, int len, int buf,
                                __attribute__((unused)) void* ud) {
  // A client has requested us to provide them the current temperature value.
  // We must make sure that it provides us with a buffer sufficiently large to
  // store a single integer:
  if (len < ((int) sizeof(current_temperature))) {
    // We do not inform the caller and simply return. We do print a log message:
    puts("[thread-sensor] ERROR: sensor IPC invoked with too small buffer.\r\n");
    return;
  }

  // The buffer is large enough, copy the current temperature into it:
  memcpy((void*) buf, &current_temperature, sizeof(current_temperature));

  // Let the client know:
  ipc_notify_client(pid);
}


int main(void) {
  // Measure the temperature once before registering ourselves as an IPC
  // service. This ensures that we always return a correct (but potentially
  // stale) temperature value.
  libtocksync_temperature_read(&current_temperature);

  // Register this application as an IPC service under its name:
  ipc_register_service_callback("org.tockos.thread-tutorial.sensor",
                                sensor_ipc_callback,
                                NULL);

  // We measure the temperature in the main loop and
  // print this value to the console.
  while (1) {
    // Measure temperature -- returned in the form 2200 => 22C
    libtocksync_temperature_read(&current_temperature);

    // Delay 1000 ms (1 second).
    libtocksync_alarm_delay_ms(1000);
  }
}
