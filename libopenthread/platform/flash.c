#include <storage/nonvolatile_storage.h>
#include <libtock-sync/storage/nonvolatile_storage.h>

#include <openthread/platform/flash.h>

#include <stdio.h>

const uint32_t SWAP_SIZE = 1024;
const uint8_t SWAP_NUM = 2;

uint32_t otPlatFlashGetSwapSize(otInstance *aInstance){
	OT_UNUSED_VARIABLE(aInstance);
	return SWAP_SIZE;
}

void otPlatFlashInit(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    OT_UNUSED_VARIABLE(aInstance);

    uint32_t num_bytes;
    libtock_nonvolatile_storage_get_number_bytes(&num_bytes);

    // Write all zeroes to flash
    uint8_t zeroes[SWAP_SIZE];

    otPlatFlashWrite(aInstance, aSwapIndex, 0, zeroes, num_bytes);

}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset,
                      const void *aData, uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aSwapIndex);
    int ret;

    assert(aSwapIndex < SWAP_NUM);
    uint32_t offset = aSwapIndex ? SWAP_SIZE : 0;
    offset += aOffset;

    int _written;
    ret = libtocksync_nonvolatile_storage_write(offset, aSize, (uint8_t*)aData, aSize, &_written);
    if (ret != RETURNCODE_SUCCESS) {
        return;
    }
}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData,
                     uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aSwapIndex);
    int ret;

    assert(aSwapIndex < SWAP_NUM);
    uint32_t offset = aSwapIndex ? SWAP_SIZE : 0;
    offset += aOffset;

    int _read;
    ret = libtocksync_nonvolatile_storage_read(offset, aSize, (uint8_t*)aData, aSize, &_read);
    if (ret != RETURNCODE_SUCCESS) {
        return;
    }
}
