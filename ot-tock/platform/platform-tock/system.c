// TODO COPYRIGHT
#include<openthread/instance.h>
#include<ieee802154.h>
#include<plat.h>
#include<openthread/platform/radio.h>
#include <stdio.h>
#include<openthread-system.h>
#include<openthread/platform/alarm-milli.h>
#include <stdio.h>

static ieee802154_rxbuf rx_buf_a;
static ieee802154_rxbuf rx_buf_b;

static char buf[780];
ring_buffer usr_rx_buffer = {
    .buffer = buf,
    .write_index = 0,
    .read_index = 0,
    .new = false
};

static uint8_t rx_mPsdu[OT_RADIO_FRAME_MAX_SIZE];
static otRadioFrame receiveFrame = {
    .mPsdu = rx_mPsdu,
    .mLength = OT_RADIO_FRAME_MAX_SIZE
};

typedef struct otTock {
    ring_buffer* usr_rx_buffer;
    ieee802154_rxbuf* kernel_rx_buf;
} otTock;

typedef struct otTock otTock;
otTock otTockInstance = {
    .usr_rx_buffer = &usr_rx_buffer,
    .kernel_rx_buf = &rx_buf_a
};

ieee802154_rxbuf* swap_shared_kernel_buf(otTock* instance);
static void rx_callback(__attribute__ ((unused)) int   pans,
                     __attribute__ ((unused)) int   dst_addr,
                     __attribute__ ((unused)) int   src_addr,
                     __attribute__ ((unused)) void* _ud);

ieee802154_rxbuf* swap_shared_kernel_buf(otTock* instance) {
    ieee802154_rxbuf* rx_buf;

    if (instance->kernel_rx_buf == &rx_buf_a) {
        instance->kernel_rx_buf = &rx_buf_b;
        rx_buf = &rx_buf_a;
    } else {
        instance->kernel_rx_buf = &rx_buf_a;
        rx_buf = &rx_buf_b;
    }

    reset_ring_buf(instance->kernel_rx_buf, (2+IEEE802154_FRAME_LEN*3), rx_callback, NULL);
    return rx_buf;
}

void otSysInit(int argc, char *argv[]){
    // TODO
    OT_UNUSED_VARIABLE(argc);
    OT_UNUSED_VARIABLE(argv);    
}

bool otSysPseudoResetWasRequested(void) { 
    // TODO 
    return false; 
}

void otSysProcessDrivers(otInstance *aInstance){
    // here we handle everything needed to do for libtock openthread
    // this will primarily be inovlving the radio receive

    // check if new data was received since last call

    // the general design I am currently envisioning involves the rx_callback
    // simply taking the data and placing it into a userprocess ring buffer

    // here we will see if the ring buffer has changed since the last time we checked
    // if it has, we will call otPlatRadioReceiveDone to pass the data to openthread
    if (usr_rx_buffer.new){

        // printf("\n\nthere is new data to process\n");
        int offset;

        while(usr_rx_buffer.read_index != usr_rx_buffer.write_index){
            offset = usr_rx_buffer.read_index * IEEE802154_FRAME_LEN;

            // printf("top of copy\n");
            char* rx_buf = usr_rx_buffer.buffer;
            int payload_offset = rx_buf[offset];
            int payload_length = rx_buf[offset+1];
            int mic_len = rx_buf[offset+2];

            receiveFrame.mInfo.mRxInfo.mTimestamp = otPlatAlarmMilliGetNow() * 1000;
            receiveFrame.mInfo.mRxInfo.mRssi = 50;
            receiveFrame.mLength = payload_length+payload_offset+mic_len;
            receiveFrame.mInfo.mRxInfo.mTimestamp = 0;
            receiveFrame.mInfo.mRxInfo.mLqi = 0x7f;

            for (int i = 0; i < receiveFrame.mLength+3; i++) {
                receiveFrame.mPsdu[i] = rx_buf[i+3+offset];
            }

            otPlatRadioReceiveDone(aInstance, &receiveFrame, OT_ERROR_NONE);

            usr_rx_buffer.read_index++;
            if (usr_rx_buffer.read_index == 6){
                usr_rx_buffer.read_index = 0;
            }
        }
        usr_rx_buffer.new = false;
    }

}


static void rx_callback(__attribute__ ((unused)) int   pans,
                     __attribute__ ((unused)) int   dst_addr,
                     __attribute__ ((unused)) int   src_addr,
                     __attribute__ ((unused)) void* _ud) {   

    char* rx_buf = swap_shared_kernel_buf(&otTockInstance)[0];

    char* head_index = &rx_buf[0];
    char* tail_index = &rx_buf[1];
    
    int offset = 2 + *head_index * IEEE802154_FRAME_LEN;    

    // | head active # | tail active # | frame 0 | frame 1 | ... | frame n |
    while (*head_index != *tail_index) {
        int payload_offset = rx_buf[offset];
        int payload_length = rx_buf[offset+1];
        int mic_len = rx_buf[offset+2];


        int receive_frame_length = payload_length+payload_offset+mic_len;

        int ring_buffer_offset = usr_rx_buffer.write_index * IEEE802154_FRAME_LEN;

        for (int i = 0; i < (receive_frame_length+3); i++) {
            usr_rx_buffer.buffer[ring_buffer_offset+i] = rx_buf[i+offset];
            rx_buf[i+offset] = 0;
            // if (i % 8 == 0) printf("\n");
            // printf("%x ", usr_rx_buffer.buffer[ring_buffer_offset+i]);
            
        }

        usr_rx_buffer.new = true;
        usr_rx_buffer.write_index++;
        if (usr_rx_buffer.write_index == 6) {
            usr_rx_buffer.write_index = 0;
        } 

        *head_index = (*head_index + 1) % MAX_FRAME_COUNT;
        offset+= IEEE802154_FRAME_LEN;

        if (*head_index == 0) {
            offset = 2;
        }
    }
    assert(*head_index == *tail_index);

}


otError otTockStartReceive(uint8_t aChannel) {
    if (aChannel != 26) {
        return OT_ERROR_NONE;
    }
    int res = ieee802154_receive(rx_callback, otTockInstance.kernel_rx_buf, (2+IEEE802154_FRAME_LEN*3), NULL);

    otError result = OT_ERROR_NONE;

    if (res != 0) {
        result = OT_ERROR_FAILED;
    }

    return result;

}