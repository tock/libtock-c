#include "sdi12.h"
#include <stdio.h>

#include "syscalls/sdi12_syscalls.h"

bool libtock_sdi12_exists(void) {
    return libtock_sdi12_driver_exists();
}

// Internal callback for rx.
static void rx_done_upcall(int ret,
                           int length,
                           __attribute__((unused)) int arg2,
                           void* opaque) {
    libtock_sdi12_receive_callback cb = (libtock_sdi12_receive_callback)opaque;
    cb(ret, length);
}

// Internal callback for tx done.
static void tx_done_upcall(int ret,
                            __attribute__((unused)) int arg1,
                            __attribute__((unused)) int arg2,
                            void* opaque) {
    libtock_sdi12_write_done_callback cb = (libtock_sdi12_write_done_callback)opaque;
    cb(ret);
}              


// Start a sdi12 receive operation, placing the received data into the provided buffer.
// The provided callback will be invoked upon receiving data or an error.
returncode_t libtock_sdi12_receive(libtock_sdi12_receive_callback cb, uint8_t* rx_buffer, uint32_t len) {
    returncode_t ret;

    ret = libtock_sdi12_set_receive_upcall(rx_done_upcall, cb);
    if (ret != RETURNCODE_SUCCESS) return ret;

    // allow rx_buffer to the kernel as rw buffer.
    ret = libtock_sdi12_set_readwrite_allow_rx(rx_buffer, len);
    if (ret != RETURNCODE_SUCCESS) return ret;

    ret = libtock_sdi12_command_receive(rx_buffer, len);
    return ret;
}

returncode_t libtock_sdi12_write(libtock_sdi12_write_done_callback cb, uint8_t* tx_buffer, uint32_t len) {
    // allow ro buffer to the kernel.
    returncode_t ret;

    ret = libtock_sdi12_set_write_done_upcall(tx_done_upcall, cb);
    if (ret != RETURNCODE_SUCCESS) return ret;
    
    ret = libtock_sdi12_set_readonly_allow_tx(tx_buffer, len);
    if (ret != RETURNCODE_SUCCESS) return ret;

    ret = libtock_sdi12_command_write(tx_buffer, len);
    printf("libtock_sdi12_write command returned %d\n", ret);
    return ret;

}

returncode_t libtock_sdi12_get_measurement(libtock_sdi12_receive_callback cb, uint8_t* tx_buffer, uint8_t* rx_buffer) {
    returncode_t ret;

    ret = libtock_sdi12_set_receive_upcall(rx_done_upcall, cb);
    if (ret != RETURNCODE_SUCCESS) return ret;

    // allow rx_buffer to the kernel as rw buffer.
    ret = libtock_sdi12_set_readwrite_allow_rx(rx_buffer, rx_buffer != NULL ? strlen((const char*)rx_buffer) : 0);
    if (ret != RETURNCODE_SUCCESS) return ret;

    ret = libtock_sdi12_set_readonly_allow_tx(tx_buffer, tx_buffer != NULL ? strlen((const char*)tx_buffer) : 0);
    if (ret != RETURNCODE_SUCCESS) return ret;

    return libtock_sdi12_command_get_measurement(tx_buffer, rx_buffer);
}

