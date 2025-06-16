#include <storage/isolated_nonvolatile_storage.h>
#include <libtock-sync/storage/isolated_nonvolatile_storage.h>

#include <openthread/platform/flash.h>

#include <stdio.h>

// TODO: The kernel currently has a 512 byte buffer 
// for isolated nonvolatile storage. This means that we can only
// allow 512 bytes. 2x 512B swaps seems to be enough for openthread
// but we may need to revist this in the future.
const uint32_t SWAP_SIZE = 512;
const uint8_t SWAP_NUM = 2;

uint32_t otPlatFlashGetSwapSize(otInstance *aInstance) {
	OT_UNUSED_VARIABLE(aInstance);
	return SWAP_SIZE;
}

void otPlatFlashInit(otInstance *aInstance) { 
    OT_UNUSED_VARIABLE(aInstance);

    // Confirm we have enough space for the 2 swaps.
    uint64_t num_bytes;
    libtocksync_isolated_nonvolatile_storage_get_number_bytes(&num_bytes);

    assert(num_bytes >= SWAP_SIZE * SWAP_NUM);
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    uint8_t overwrite[SWAP_SIZE];

    // Fill the swap area with 0xFF to erase it.
    for (uint32_t i = 0; i < SWAP_SIZE; i++) {
        overwrite[i] = 0xFF;
    }

    otPlatFlashWrite(aInstance, aSwapIndex, 0, overwrite, SWAP_SIZE);
}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset,
                      const void *aData, uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);

    assert(aSwapIndex < SWAP_NUM);
    assert(aOffset + aSize <= SWAP_SIZE);
    uint32_t offset = aSwapIndex ? SWAP_SIZE : 0;
    offset += aOffset;

    libtocksync_isolated_nonvolatile_storage_write(offset, (uint8_t*)aData, aSize);
}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData,
                     uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);
    
    assert(aSwapIndex < SWAP_NUM);
    assert(aOffset + aSize <= SWAP_SIZE);
    uint32_t offset = aSwapIndex ? SWAP_SIZE : 0;
    offset += aOffset;
    
    libtocksync_isolated_nonvolatile_storage_read(offset, (uint8_t*)aData, aSize);
}
