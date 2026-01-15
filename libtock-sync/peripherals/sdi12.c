#include "sdi12.h"

struct sdi12_receive_data {
    bool fired;
    returncode_t ret;
    uint32_t length;
};

struct sdi12_write_data {
    bool fired;
    returncode_t ret;
};

static struct sdi12_receive_data rx_result = { .fired = false };
static struct sdi12_write_data tx_result = { .fired = false };

static void sdi12_receive_cb(returncode_t ret, uint32_t length) {
    rx_result.fired = true;
    rx_result.ret = ret;
    rx_result.length = length;
}

static void sdi12_tx_done_cb(returncode_t ret) {
    tx_result.fired = true;
    tx_result.ret = ret;
}

bool libtocksync_sdi12_exists(void) {
    return libtock_sdi12_exists();
}

returncode_t libtocksync_sdi12_receive(uint8_t* rx_buffer, uint32_t len) {
    returncode_t ret;

    rx_result.fired = false;
    ret = libtock_sdi12_receive(sdi12_receive_cb, rx_buffer, len);
    if (ret != RETURNCODE_SUCCESS) return ret;
    printf("LibTock-C: Yielding for SDI12 receive...\n");
    yield_for(&rx_result.fired);
    printf("LibTock-C: SDI12 receive completed with return code %d\n", rx_result.ret);

    return rx_result.ret;
}

returncode_t libtocksync_sdi12_write(uint8_t* tx_buffer, uint32_t len) {
    returncode_t ret; 

    tx_result.fired = false;
    ret = libtock_sdi12_write(sdi12_tx_done_cb, tx_buffer, len);
    if (ret != RETURNCODE_SUCCESS) return ret;
    yield_for(&tx_result.fired);
    return tx_result.ret;   
}

returncode_t libtocksync_sdi12_write_and_receive(uint8_t* tx_buffer, uint32_t tx_len, uint8_t* rx_buffer, uint32_t rx_len) {
    returncode_t ret;

    // Start receive first
    rx_result.fired = false;
    ret = libtock_sdi12_receive(sdi12_receive_cb, rx_buffer, rx_len);
    if (ret != RETURNCODE_SUCCESS) return ret;

    // Then do write
    tx_result.fired = false;
    ret = libtock_sdi12_write(sdi12_tx_done_cb, tx_buffer, tx_len);
    if (ret != RETURNCODE_SUCCESS) return ret;
    yield_for(&tx_result.fired);
    if (tx_result.ret != RETURNCODE_SUCCESS) return tx_result.ret;
    printf("Yielding for SDI12 receive after write...\n");
    // Wait for receive
    yield_for(&rx_result.fired);
    return rx_result.ret;
}