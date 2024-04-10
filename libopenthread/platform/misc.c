#include <openthread/platform/misc.h>

void otPlatReset(otInstance *aInstance) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
}

otPlatResetReason otPlatGetResetReason(otInstance *aInstance){
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  return OT_PLAT_RESET_REASON_POWER_ON;
}

void otPlatWakeHost(void){
  // TODO
}