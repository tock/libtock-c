#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>

#include "../../libtock/interface/syscalls/servo_syscalls.h"

int main(void) {
  // Checks if the driver exists and, if not, returns -1.
  if (!libtock_servo_exists()) {
    printf("There is no available servo\n");
    return -1;
  }
  returncode_t result  = RETURNCODE_EOFF;
  uint32_t servo_count = 0;
  libtock_servo_count(&servo_count);
  printf("The number of available servomotors is: %ld\n", servo_count);
  uint16_t angle = 0;
  uint16_t index = 0; // the first index available.

  if (libtock_servo_read_angle(index, &angle) == RETURNCODE_ENODEVICE) {
    printf("\n The index number is bigger than the available servomotors\n");
    return -1;
  }

  // Changes the angle of the servomotor from 0 to 180 degrees (waiting 0.1 ms between every change).
  for (int i = 0; i <= 180; i++) {
    // `result` stores the returned code received from the driver.
    result = libtock_servo_set_angle(index, i);
    if (result == RETURNCODE_SUCCESS) {
      libtocksync_alarm_delay_ms(100);
      // Verifies if the function successfully returned the current angle.
      if (libtock_servo_read_angle(index, &angle) == RETURNCODE_SUCCESS) {
        printf("Requested angle %d. Actual current angle is: %d\n", i, angle);
      }
    } else if (result == RETURNCODE_FAIL) {
      printf("\nAngle %d exceeds the servo's limit angle.\n", i);
    }
  }
  if (libtock_servo_read_angle(index, &angle) != RETURNCODE_SUCCESS) {
    printf("\n This servo cannot return its angle.\n");
  }
}
