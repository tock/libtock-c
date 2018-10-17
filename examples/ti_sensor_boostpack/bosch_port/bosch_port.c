#include "bosch_port.h"
#include <i2c_master.h>
#include <stdlib.h>


uint8_t tock_i2c_write (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len){
  uint8_t *buf;
  buf    = malloc(len + 1);
  buf[0] = reg_addr;
  memcpy(buf + 1, data, len);

  int err = i2c_master_write(dev_id, buf, len + 1);

  free(buf);
  return err;
}


uint8_t tock_i2c_read (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len){
  data[0] = reg_addr;
  return i2c_master_write_read(dev_id, data, 1, len);
}
