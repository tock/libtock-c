#include <stdio.h>

#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/ipc.h>
#include <libtock/tock.h>

static int current_temperature = 0;

int main(void) {

  // We measure the temperature in the main loop and
  // print this value to the console.
  while (1) {
    // Measure temperature -- returned in the form 2200 => 22C
    libtocksync_temperature_read(&current_temperature);

    // Convert temperature
    int whole_degree   = current_temperature / 100;
    int decimal_degree = current_temperature % 100;

    printf("Current temperature: %i.%i\r\n", whole_degree, decimal_degree);

    // Delay 1000 ms (1 second).
    libtocksync_alarm_delay_ms(1000);
  }
}
