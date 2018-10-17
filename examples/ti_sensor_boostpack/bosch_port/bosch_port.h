#ifndef BOSCH_PORT_H_
#define BOSCH_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

uint8_t tock_i2c_write (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
uint8_t tock_i2c_read  (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif