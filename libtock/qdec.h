/**
 * Quadrature Decoder (QDEC)
 *
 * A QDEC provides buffered decoding of quadrature-encoded signals. It is suitable
 * for mechanical and optical sensors.
 *
 * Use for the QDEC would be hooking up a rotary encoder or other optical encoder to
 * the respective ports and using the below series of function to decode the signals
 * sent by the encoders and turn them into useful metrics (i.e. displacement)
 *
 */
#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_QDEC 0x90003

/*Verifies the existence of the QDEC*/
bool qdec_exists(void);
/*Enables the QDEC*/
bool qdec_enable(void);
/*Disables the QDEC*/
bool qdec_disable(void);
/*Enables QDEC interrupts needed to get displacement values*/
bool qdec_interrupt_enable(void);
/*Subscribes the app to QDEC interrupt calls*/
int qdec_subscribe(subscribe_cb callback, void* callback_args);
/*Gets the displacement of the QDEC from its last stopped position*/
int qdec_displacement(void);

#ifdef __cplusplus
}
#endif
