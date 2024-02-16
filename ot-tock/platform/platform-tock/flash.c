// TODO COPYRIGHT

#include<openthread/platform/flash.h>

const uint32_t SWAP_SIZE = 4096;
uint8_t swapPart0[4096];
uint8_t swapPart1[4096];

// function to zero array 
void zeroArray(uint8_t *arr, uint32_t size) {
    printf("zeroing array\n");
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

void otPlatFlashInit(otInstance *aInstance) {
    printf("initializing flash\n");
    // initialize swapPart0 and swapPart1
    zeroArray(swapPart0, SWAP_SIZE);
    zeroArray(swapPart1, SWAP_SIZE);
    
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    printf("erasing flash\n");
    if (aSwapIndex == 0) {
        zeroArray(swapPart0, SWAP_SIZE);
    } else {
        zeroArray(swapPart1, SWAP_SIZE);
    }
}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, const void *aData, uint32_t aSize) {
    printf("writing to flash\n");
    if (aSwapIndex == 0) {
        memcpy(swapPart0 + aOffset, aData, aSize);
    } else {
        memcpy(swapPart1 + aOffset, aData, aSize);
    }

}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData, uint32_t aSize) {
    printf("reading from flash\n");
    if (aSwapIndex == 0) {
        memcpy(aData, swapPart0 + aOffset, aSize);
    } else {
        memcpy(aData, swapPart1 + aOffset, aSize);
    }
}

uint32_t otPlatFlashGetSwapSize(otInstance *aInstance)
{
    printf("getting swap size\n");
    OT_UNUSED_VARIABLE(aInstance);

    return SWAP_SIZE;
}