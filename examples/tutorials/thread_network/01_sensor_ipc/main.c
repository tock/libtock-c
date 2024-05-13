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

// static void sensor_ipc_callback(int pid, int len, int buf,
//                    __attribute__((unused)) void *ud)
// {
// }

int main(void) {
  // Measure the temperature once before registering ourselves as an IPC
  // service. This ensures that we always return a correct (but potentially
  // stale) temperature value.
  libtocksync_temperature_read(&current_temperature);

  // We measure the temperature in the main loop and simply provide the latest
  // reading in an IPC. This means that the control app does not have to wait
  // for the temperature read system call to complete.
  while (1) {
    libtocksync_temperature_read(&current_temperature);
    // printf("Current temperature: %d\r\n", current_temperature);
    libtocksync_alarm_delay_ms(1000);
  }
}
