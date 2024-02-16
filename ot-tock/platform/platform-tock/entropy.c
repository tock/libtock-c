// TODO COPYRIGHT

#include <openthread/platform/entropy.h>
#include <rng.h>
#include <stdio.h>

otError otPlatEntropyGet(uint8_t *aOutput, uint16_t aOutputLength) {
    printf("entropy getting %u bits of random\n", aOutputLength);
    if (aOutput == NULL) {
        return OT_ERROR_INVALID_ARGS;
}
    
    // Synchronously fill buffer
    int count;
    int returnCode = rng_sync(aOutput, (uint32_t)aOutputLength, (uint32_t)aOutputLength, &count);
  
    // TODO: test printing count in tock exa mple app. The documentation doesn't make sense.
    if (returnCode == RETURNCODE_SUCCESS) { // RETURNCODE_SUCCESS is 0
        printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
        return OT_ERROR_NONE;
    }
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_FAILED;
}