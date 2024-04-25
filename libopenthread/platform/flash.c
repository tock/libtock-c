#include <internal/nonvolatile_storage.h>
#include <openthread/platform/flash.h>
#include <stdio.h>

// Force flash operations to be synchronous
static bool done = false;

const uint32_t SWAP_SIZE = 1024;
const uint8_t SWAP_NUM = 2;

uint32_t otPlatFlashGetSwapSize(otInstance *aInstance){
	OT_UNUSED_VARIABLE(aInstance);
	return SWAP_SIZE;
}

static void read_done(__attribute((unused)) int length, __attribute__((unused)) int arg1,
                      __attribute__((unused)) int arg2, __attribute__((unused)) void *ud) {
    done = true;
}

static void write_done(__attribute((unused)) int length, __attribute__((unused)) int arg1,
                       __attribute__((unused)) int arg2, __attribute__((unused)) void *ud) {
    done = true;
}

void otPlatFlashInit(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    OT_UNUSED_VARIABLE(aInstance);

    int num_bytes;
    nonvolatile_storage_internal_get_number_bytes(&num_bytes);

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

    // Sets up the buffer to store the output of the read
    ret = nonvolatile_storage_internal_write_buffer((void *)aData, aSize);
    if (ret != RETURNCODE_SUCCESS) {
        return;
    }

    ret = nonvolatile_storage_internal_write_done_subscribe(write_done, NULL);
    if (ret != RETURNCODE_SUCCESS) {
        return;
    }

    done = false;
    ret = nonvolatile_storage_internal_write(offset, aSize);
    if (ret != 0) {
        return;
    }
    // wait until callback fires
    yield_for(&done);
}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData,
                     uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aSwapIndex);
    int ret;

    assert(aSwapIndex < SWAP_NUM);
    uint32_t offset = aSwapIndex ? SWAP_SIZE : 0;
    offset += aOffset;

    // Sets up the buffer to store the output of the read
    ret = nonvolatile_storage_internal_read_buffer(aData, aSize);
    if (ret != RETURNCODE_SUCCESS) {
   	return;
    }

    ret = nonvolatile_storage_internal_read_done_subscribe(read_done, NULL);
    if (ret != RETURNCODE_SUCCESS) {
        return;
    }

    done = false;
    ret = nonvolatile_storage_internal_read(offset, aSize);
    if (ret != RETURNCODE_SUCCESS) {
        return;
    }
    // wait until callback fires
    yield_for(&done);
}
