#include <stdio.h>

#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/ipc.h>
#include <libtock/tock.h>

// Global variable storing the current temperature. This is written to in the
// main loop, and read from in the IPC handler. Because the app is single
// threaded and has no yield point when writing the value, we do not need to
// worry about synchronization -- reads never happen during a write.
static int current_temperature = 0;

static void sensor_ipc_callback(int pid, int len, int buf,
                                __attribute__((unused)) void* ud) {
  // A client has requested us to provide them the current temperature value.
  // We must make sure that it provides us with a buffer sufficiently large to
  // store a single integer:
  if (len < ((int) sizeof(current_temperature))) {
    // We do not inform the caller and simply return. We do print a log message:
    puts("[thread-sensor] ERROR: sensor IPC invoked with too small buffer.\r\n");
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

  // We measure the temperature in the main loop and simply provide the latest
  // reading in an IPC. This means that the control app does not have to wait
  // for the temperature read system call to complete.
  while (1) {
    libtocksync_temperature_read(&current_temperature);
    // printf("Current temperature: %d\r\n", current_temperature);
    libtocksync_alarm_delay_ms(1000);
  }
}
