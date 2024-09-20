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
  returncode_t result = RETURNCODE_EOFF;
  uint16_t angle = 0;
  uint16_t index = 0; // the first index available.

  if (libtock_current_servo_angle(index, &angle) == RETURNCODE_ENODEVICE) {
    printf("\n The index number is bigger than the available servomotors\n");
    return -1;
  }

  // Changes the angle of the servomotor from 0 to 180 degrees (waiting 0.1 ms between every change).
  for (int i = 0; i <= 180; i++) {
    // `result` stores the returned code received from the driver.
    result = libtock_servo_angle(index, i);
    if (result == RETURNCODE_SUCCESS) {
      libtocksync_alarm_delay_ms(100);
      // Verifies if the function successfully returned the current angle.
      if (libtock_current_servo_angle(index, &angle) == RETURNCODE_SUCCESS) {
        printf("\nThe current angle is: %d", angle);
      } else {
        printf("\n This servo cannot return it's angle.\n");
      }
    } else if (result == RETURNCODE_EINVAL){
      printf("\nThe angle you provided exceeds 360 degrees\n");
      return -1;
    } else if (result == RETURNCODE_FAIL){
      printf("\nThe angle could not be changed. The provided angle exceeds the servo's limit\n");
      return -1;
    }
  }
}
