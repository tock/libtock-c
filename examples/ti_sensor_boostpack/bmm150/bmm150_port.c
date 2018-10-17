#include "../bmi160/bmi160_port.h"
#include "bmm150.h"
#include "bmm150_port.h"

#include "../bosch_port/bosch_port.h"

#include <console.h>
#include <stdlib.h>
#include <timer.h>

extern struct bmi160_dev bmi160;

void bmm150_print_data(struct bmm150_mag_data *data)
{

  char beg[] = "BMM150 Gyro  ";
  putnstr(beg, sizeof(beg));

  char x[] = "x:       ";
  itoa (data->x, &x[3], 10);
  putnstr(x, sizeof(x));

  char y[] = ", y:               ";
  itoa (data->y, &y[5], 10);
  putnstr(y, sizeof(y));

  char z[] = ", z:             ";
  itoa (data->z, &z[5], 10);
  putnstr(z, sizeof(z));

  putnstr("\r\n", 2);
}