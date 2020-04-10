/* vim: set sw=2 expandtab tw=80: */

#include <lsm303dlhc.h>
#include <math.h>
#include <stdio.h>
#include <timer.h>

static int decimals (float f, int n)
{
  return (int)((fabs(f - (int)(f))) * pow (10, n));
}

int main(void) {
  LSM303DLHCXYZ xyz;
  float temp;
  if (lsm303dlhc_is_present ()) {
    printf ("LSM303DLHC sensor is present\n");
    if (lsm303dlhc_set_power_mode (LSM303DLHC_100HZ, LSM303DLHC_NORMAL)) {
      printf ("LSM303DLHC device set power mode\n");
      if (lsm303dlhc_set_accelerometer_scale_and_resolution (LSM303DLHC_SCALE_8G, false)) {
        printf ("LSM303DLHC device set scale\n");
        lsm303dlhc_set_temperature_and_magnetometer_rate (true, LSM303DLHC_M_22_0HZ);
        lsm303dlhc_set_magnetometer_range (LSM303DLHC_RANGE_4_7G);
        while (1)
        {
          if (lsm303dlhc_read_acceleration_xyz (&xyz) == TOCK_SUCCESS) {
            printf ("acceleration x %d.%d y %d.%d z %d.%d\r\n", (int)xyz.x, decimals(xyz.x, 3),
                    (int)xyz.y, decimals(xyz.y, 3), (int)xyz.z, decimals(xyz.z, 3));
          } else {
            printf ("Error while reading acceleration\n");
          }
          if (lsm303dlhc_read_temperature (&temp) == TOCK_SUCCESS) {
            printf ("temperature %d.%d\r\n", (int)temp, decimals(temp, 1));
          } else {
            printf ("Error while reading temperature\n");
          }
          if (lsm303dlhc_read_magnetometer_xyz (&xyz) == TOCK_SUCCESS) {
            printf ("magnetometer x %d.%d y %d.%d z %d.%d\r\n", (int)xyz.x, decimals(xyz.x, 3),
                    (int)xyz.y, decimals(xyz.y, 3), (int)xyz.z, decimals(xyz.z, 3));
          } else {
            printf ("Error while reading magnetometer\n");
          }
          delay_ms (1000);
        }
      }
      else
      {
        printf ("LSM303DLHC device set scale failed\n");
      }
      // l3gd20_set_hpf_parameters (L3GD20_HPF_MODE_NORMAL, L3GD20_HPF_DIV_64);
      // l3gd20_enable_hpf (true);
    } else {
      printf ("LSM303DLHC device set power mode failed\n");
    }

  } else {
    printf ("LSM303DLHC sensor driver not present\n");
  }
  return 0;
}
