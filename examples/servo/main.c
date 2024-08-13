#include "../../libtock/interface/syscalls/servo_syscalls.h"
#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  // Checks if the driver exists and, if not, returns -1.
  if (!libtock_servo_exists()) {
    printf("There is no available servo\n");
    return -1;
  }

  uint16_t angle = 0;

  // Changes the angle of the servomotor from 0 to 180 degrees (waiting 0.1 ms between every change).
  for (int i = 0; i <= 180; i++) {
    if (libtock_servo_angle(i) == RETURNCODE_SUCCESS) {
      libtocksync_alarm_delay_ms(100);
      // Verifies if the function successfully returned the current angle.
      if (libtock_current_servo_angle(&angle) == RETURNCODE_SUCCESS) {
        printf("The current angle is: %d\n", angle);
      } else {
        printf("\nThe servomotor is OFF\n");
      }
    } else {
      printf("\nThe angle could not be changed\n");
      return -1;
    }
  }
}
