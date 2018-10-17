#ifndef BMM150_PORT_H_
#define BMM150_PORT_H_

#include "bmm150.h"

#ifdef __cplusplus
extern "C" {
#endif

int8_t bmm150_port_init(struct bmm150_dev *dev);
void bmm150_print_data(struct bmm150_mag_data *data);

#ifdef __cplusplus
}
#endif
#endif