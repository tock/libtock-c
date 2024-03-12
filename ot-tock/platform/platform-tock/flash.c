// TODO COPYRIGHT

#include <openthread/platform/flash.h>
#include <string.h>

const uint32_t SWAP_SIZE = 10000;
uint8_t swapPart0[10000];
uint8_t swapPart1[10000];

// NOTE: This is just a temporary version of this file. OpenThread will
// not operate properly if it is unable to "save" the state. This provides
// an in memory representation of "flash" that will be lost across power
// cycles.

void zeroArray(uint8_t *arr, uint32_t size);

// function to zero array 
void zeroArray(uint8_t *arr, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

void otPlatFlashInit(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);

    // initialize swapPart0 and swapPart1
    zeroArray(swapPart0, SWAP_SIZE);
    zeroArray(swapPart1, SWAP_SIZE);
    
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    OT_UNUSED_VARIABLE(aInstance);

    if (aSwapIndex == 0) {
        zeroArray(swapPart0, SWAP_SIZE);
    } else {
        zeroArray(swapPart1, SWAP_SIZE);
    }
}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, const void *aData, uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);

    if (aSwapIndex == 0) {
        memcpy(swapPart0 + aOffset, aData, aSize);
    } else {
        memcpy(swapPart1 + aOffset, aData, aSize);
    }

}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData, uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aSwapIndex);
    OT_UNUSED_VARIABLE(aOffset);
    OT_UNUSED_VARIABLE(aData);
    OT_UNUSED_VARIABLE(aSize);

    if (aSwapIndex == 0) {
        memcpy(aData, swapPart0 + aOffset, aSize);
    } else {
        memcpy(aData, swapPart1 + aOffset, aSize);
    }
}

uint32_t otPlatFlashGetSwapSize(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return SWAP_SIZE;
}