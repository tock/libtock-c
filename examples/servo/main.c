#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock-sync/interface/servo.c>

int main(void) {
  // Checks if the driver exists and, if not, returns -1.
	if (!libtock_servo_exists()) {
    printf("There is no available servo\n");
    return -1;
  }
  // Changes the angle of the servomotor from 0 to 180 degrees (waiting 0.1 ms between every change).
  for (int i = 0; i<=180; i++) {
    libtocksync_servo_angle(i);
    libtocksync_alarm_delay_ms(100);
  }
}
