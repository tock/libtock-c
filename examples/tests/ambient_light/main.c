#include <stdio.h>

#include <libtock-sync/sensors/ambient_light.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>

int main (void) {
  printf("[Ambient Light] Test\n");

  while (1) {
    // Start a light measurement
    int lux;
    int ret = libtocksync_ambient_light_read_intensity(&lux);
    if (ret == RETURNCODE_ENODEVICE) {
      printf("ERROR: No ambient light sensor on this board.\n");
    } else if (ret < 0) {
      printf("ERROR: unable to read the sensor (error code: %i)\n", lux);
    } else {
      // Print the lux value
      printf("\tValue(%d lux) [0x%X]\n\n", lux, lux);
    }
    libtocksync_alarm_delay_ms(1000);
  }
}
