/* vim: set sw=2 expandtab tw=80: */

#include <l3gd20.h>
#include <math.h>
#include <stdio.h>
#include <timer.h>

#define ABS(N) ((N < 0) ? (-N) : (N))

static int integer (float f)
{
  return trunc (f);
}

static int decimals (float f, int n)
{
  return trunc((ABS(f - trunc (f))) * pow (10, n));
}

int main(void) {
  L3GD20XYZ xyz;
  int temp;
  printf ("is present %d\r\n", l3gd20_is_present ());
  printf ("power on %d\r\n", l3gd20_power_on ());
  printf ("hpf set parameters %d\r\n", l3gd20_set_hpf_parameters (L3GD20_HPF_MODE_NORMAL, L3GD20_HPF_DIV_64));
  printf ("enable hpf %d\r\n", l3gd20_enable_hpf (true));
  while (1)
  {
    if (l3gd20_read_xyz (&xyz) == TOCK_SUCCESS && l3gd20_read_temperature (&temp) == TOCK_SUCCESS) {
      printf ("temperature %d x %d.%d y %d.%d z %d.%d\r\n", temp, integer(xyz.x), decimals(xyz.x, 3),
              integer(xyz.y), decimals(xyz.y, 3), integer(xyz.z), decimals(xyz.z, 3));
    }
    delay_ms (10);
  }
  return 0;
}
