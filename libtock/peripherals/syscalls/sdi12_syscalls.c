#include "sdi12_syscalls.h"

bool libtock_sdi12_driver_exists(void) {
    return driver_exists(DRIVER_NUM_SDI12);
}

returncode_t libtock_sdi12_set_write_done_upcall(subscribe_upcall callback, void* opaque) {
    subscribe_return_t sval = subscribe(DRIVER_NUM_SDI12, SDI12_SUBSCRIBE_TX_DONE, callback, opaque);
    printf("Return from subsribe pass/fail: %d, status: %d\n", sval.success, sval.status);
    return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_sdi12_set_receive_upcall(subscribe_upcall callback, void* opaque) {
    subscribe_return_t sval = subscribe(DRIVER_NUM_SDI12, SDI12_SUBSCRIBE_RX, callback, opaque);
    return tock_subscribe_return_to_returncode(sval);
}


returncode_t libtock_sdi12_command_write(uint8_t* tx_buffer, uint32_t len){
    syscall_return_t cval = command(DRIVER_NUM_SDI12, SDI12_COMMAND_TX, tx_buffer, len);
    //printf("SDI12 command write returned %d\n", cval.type);
    int ret = tock_command_return_novalue_to_returncode(cval);
    printf("SDI12 command write returncode: %d\n", ret);
    return ret;
}

// Waits asynchronously to receive data over the sdi12 interface. A `readwrite`
// buffer must be provided to the kernel driver prior to calling this function.
returncode_t libtock_sdi12_command_receive(uint8_t* rx_buffer, uint32_t len) {
    syscall_return_t cval = command(DRIVER_NUM_SDI12, SDI12_COMMAND_RX, 0, len);
    printf("SDI12 command receive returned %d\n", cval.type);
    return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_sdi12_command_get_measurement(uint8_t* tx_buffer, uint8_t* rx_buffer) {
    syscall_return_t cval = command(DRIVER_NUM_SDI12, SDI12_GET_MEASUREMENT, tx_buffer, rx_buffer);
    printf("SDI12 get measurement command returned %d\n", cval.type);
    return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_sdi12_set_readwrite_allow_rx(uint8_t* buffer, uint32_t len) {
    allow_rw_return_t rval = allow_readwrite(DRIVER_NUM_SDI12, SDI12_ALLOW_RW_RX_BUFFER, buffer, len);
    return tock_allow_rw_return_to_returncode(rval);
}

returncode_t libtock_sdi12_set_readonly_allow_tx(const uint8_t* buffer, uint32_t len) {
    printf("Setting SDI12 readonly allow tx buffer at %p of length %u\n", buffer, len);
    allow_ro_return_t rval = allow_readonly(DRIVER_NUM_SDI12, 0, (void*)buffer, len);
    return tock_allow_ro_return_to_returncode(rval);
}