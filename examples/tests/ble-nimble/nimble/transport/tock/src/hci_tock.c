/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <sysinit/sysinit.h>
#include <syscfg/syscfg.h>
#include "os/os_mbuf.h"
#include "os/os_mempool.h"
#include "nimble/ble.h"

// Tock-specific includes.
#include "tock.h"

#define DRIVER_NUM_BLE_HCI 0x876
#define TOCK_READONLYALLOW_BLE_HCI_TX_CMD 0
#define TOCK_READONLYALLOW_BLE_HCI_TX_ACL 1
#define TOCK_COMMAND_BLE_HCI_TX_CMD 1
#define TOCK_COMMAND_BLE_HCI_TX_ACL 2
#define TOCK_SUBSCRIBE_BLE_HCI 0

#define TX_Q_SIZE   (MYNEWT_VAL(BLE_TRANSPORT_ACL_FROM_LL_COUNT) + \
                     MYNEWT_VAL(BLE_TRANSPORT_EVT_COUNT) + \
                     MYNEWT_VAL(BLE_TRANSPORT_EVT_DISCARDABLE_COUNT))

// int
// ble_transport_to_ll_cmd_impl(void *buf)
// {
//     int rc;
//     uint8_t *cmd = buf;
//     hci_drv_write_t write_buf;
//     uint8_t *ptr = (uint8_t *)write_buf.data;

//     *ptr = HCI_H4_CMD;
//     ptr++;
//     write_buf.len = HCI_CMD_HDR_LEN + cmd[2] + 1;
//     memcpy(ptr, cmd, write_buf.len - 1);

//     rc = apollo3_hci_write(&write_buf);

//     ble_transport_free(cmd);

//     return (rc < 0) ? BLE_ERR_MEM_CAPACITY :  0;
// }

// int
// ble_transport_to_ll_acl_impl(struct os_mbuf *om)
// {
//     return apollo3_ble_hci_acl_tx(om);
// }


// static int
// apollo3_ble_hci_acl_tx(struct os_mbuf *om)
// {
//     int rc = 0;
//     hci_drv_write_t write_buf;
//     uint8_t *ptr = (uint8_t *)write_buf.data;

//     *ptr = HCI_H4_ACL;
//     ptr++;
//     write_buf.len = 1;

//     os_mbuf_copydata(om, 0, OS_MBUF_PKTLEN(om), ptr);
//     write_buf.len += OS_MBUF_PKTLEN(om);

//     rc = apollo3_hci_write(&write_buf);

//     os_mbuf_free_chain(om);

//     return (rc < 0) ? BLE_ERR_MEM_CAPACITY : 0;
// }


static void tock_hci_upcall(int   callback_type,
                       int   arg1,
                       int   arg2,
                       void* callback_args) {

  // adc_data_t* result = (adc_data_t*)callback_args;

  // switch (callback_type) {
  //   case SingleSample:
  //     result->error   = RETURNCODE_SUCCESS;
  //     result->channel = arg1;
  //     result->sample  = arg2;
  //     break;

  //   case ContinuousSample:
  //     result->error   = RETURNCODE_SUCCESS;
  //     result->channel = arg1;
  //     result->sample  = arg2;
  //     break;

  //   case SingleBuffer:
  //     result->error   = RETURNCODE_SUCCESS;
  //     result->channel = (arg1 & 0xFF);
  //     result->length  = ((arg1 >> 8) & 0xFFFFFF);
  //     result->buffer  = (uint16_t*)arg2;
  //     break;

  //   case ContinuousBuffer:
  //     result->error   = RETURNCODE_SUCCESS;
  //     result->channel = (arg1 & 0xFF);
  //     result->length  = ((arg1 >> 8) & 0xFFFFFF);
  //     result->buffer  = (uint16_t*)arg2;
  //     break;

  //   default:
  //     result->error = RETURNCODE_FAIL;
  //     break;
  // }
  // result->fired = true;
}

uint8_t tock_kernel_to_userspace_buffer[256];

int ble_transport_tock_init(void) {
    // we need to subscribe to an upcall to get notified of incoming HCI packets


    subscribe_return_t subval = subscribe(DRIVER_NUM_BLE_HCI, TOCK_SUBSCRIBE_BLE_HCI, tock_hci_upcall, NULL);
    if (!subval.success) {
        return -1;
    }


    // we need to allow an rx (from our perspective) buffer for incoming HCI packets



    allow_ro_return_t rw = allow_readwrite(DRIVER_NUM_BLE_HCI, TOCK_READWRITEALLOW_BLE_HCI_RX, (void*) tock_kernel_to_userspace_buffer, 256);
    if (!rw.success) {
      return -1;
    }

}


uint8_t tock_userspace_to_kernel_cmd_buffer[256];
uint8_t tock_userspace_to_kernel_acl_buffer[256];



// Send a packet to the kernel.
int ble_transport_to_ll_cmd_impl(void *buf) {

    // int rc;
    uint8_t* cmd = (uint8_t*) buf;
    // hci_drv_write_t write_buf;
    // uint8_t *ptr = (uint8_t *)write_buf.data;

    // *ptr = HCI_H4_CMD;
    // ptr++;
    // write_buf.len = HCI_CMD_HDR_LEN + cmd[2] + 1;



    // The total copy size includes the two byte opcode, the one byte length,
    // and the remaining length of the packet, which is the third byte in the
    // packet.
    uint32_t copy_len = 3 + cmd[2];
    // Copy to the buffer we are going to share with the kernel.
    memcpy(tock_userspace_to_kernel_cmd_buffer, (uint8_t*) buf, copy_len);

    // Share our TX buffer with the kernel. Since the kernel only needs to
    // receive the cmd, we use a read-only allow.
    allow_ro_return_t ro = allow_readonly(DRIVER_NUM_BLE_HCI, TOCK_READONLYALLOW_BLE_HCI_TX_CMD, (void*) tock_userspace_to_kernel_cmd_buffer, copy_len);
    if (!ro.success) {
        // Not sure what the correct return code is here.
      return BLE_ERR_MEM_CAPACITY;
    }

    ble_transport_free(cmd);

    // Notify the kernel that we have sent it a buffer.
    syscall_return_t res = command(DRIVER_NUM_BLE_HCI, TOCK_COMMAND_BLE_HCI_TX_CMD, 0, 0);
    if (res.type == TOCK_SYSCALL_SUCCESS) {
        return 0;
    } else {
        // Not sure what the correct return code is here.
        return BLE_ERR_MEM_CAPACITY;
    }

    // rc = apollo3_hci_write(&write_buf);

    // ble_transport_free(cmd);

    // return (rc < 0) ? BLE_ERR_MEM_CAPACITY :  0;




    // return 0;
}

int ble_transport_to_ll_acl_impl(struct os_mbuf *om) {
    // int rc = 0;
    // hci_drv_write_t write_buf;
    // uint8_t *ptr = (uint8_t *)write_buf.data;

    // *ptr = HCI_H4_ACL;
    // ptr++;
    // write_buf.len = 1;

    // os_mbuf_copydata(om, 0, OS_MBUF_PKTLEN(om), ptr);
    // write_buf.len += OS_MBUF_PKTLEN(om);

    // rc = apollo3_hci_write(&write_buf);

    // os_mbuf_free_chain(om);

    // return (rc < 0) ? BLE_ERR_MEM_CAPACITY : 0;



    uint32_t copy_len = OS_MBUF_PKTLEN(om);
    // Copy to the buffer we are going to share with the kernel.
    os_mbuf_copydata(om, 0, OS_MBUF_PKTLEN(om), tock_userspace_to_kernel_acl_buffer);

    // Share our TX buffer with the kernel. Since the kernel only needs to
    // receive the acl, we use a read-only allow.
    allow_ro_return_t ro = allow_readonly(DRIVER_NUM_BLE_HCI, TOCK_READONLYALLOW_BLE_HCI_TX_ACL, (void*) tock_userspace_to_kernel_acl_buffer, copy_len);
    if (!ro.success) {
        // Not sure what the correct return code is here.
      return BLE_ERR_MEM_CAPACITY;
    }

    os_mbuf_free_chain(om);

    // Notify the kernel that we have sent it a buffer.
    syscall_return_t res = command(DRIVER_NUM_BLE_HCI, TOCK_COMMAND_BLE_HCI_TX_ACL, 0, 0);
    if (res.type == TOCK_SYSCALL_SUCCESS) {
        return 0;
    } else {
        // Not sure what the correct return code is here.
        return BLE_ERR_MEM_CAPACITY;
    }



}


