#include <openthread/platform/entropy.h>
#include <libtock-sync/peripherals/rng.h>
#include <stdio.h>

otError otPlatEntropyGet(uint8_t *aOutput, uint16_t aOutputLength) {
  if (aOutput == NULL) {
    return OT_ERROR_INVALID_ARGS;
  }

  // Synchronously fill buffer
  int count;
  int returnCode = libtocksync_rng_get_random_bytes(aOutput, (uint32_t)aOutputLength, (uint32_t)aOutputLength, &count);

  if (returnCode == RETURNCODE_SUCCESS) {
    return OT_ERROR_NONE;
  }
  return OT_ERROR_FAILED;
}
