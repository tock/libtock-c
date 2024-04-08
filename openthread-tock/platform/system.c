#include <ieee802154.h>
#include <openthread-system.h>
#include <openthread/instance.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/radio.h>
#include <plat.h>
#include <stdio.h>

static ieee802154_rxbuf rx_buf_a;
static ieee802154_rxbuf rx_buf_b;

#define USER_RX_BUF_FRAME_COUNT 3
static char buf[IEEE802154_FRAME_LEN * USER_RX_BUF_FRAME_COUNT];
ring_buffer usr_rx_buffer = {
  .buffer      = buf,
  .write_index = 0,
  .read_index  = 0,
  .new         = false
};

static uint8_t rx_mPsdu[OT_RADIO_FRAME_MAX_SIZE];
static otRadioFrame receiveFrame = {
  .mPsdu   = rx_mPsdu,
  .mLength = OT_RADIO_FRAME_MAX_SIZE
};

typedef struct otTock {
  ring_buffer* usr_rx_buffer;
  ieee802154_rxbuf* kernel_rx_buf;
  otInstance* instance;
} otTock;

typedef struct otTock otTock;
otTock otTockInstance = {
  .usr_rx_buffer = &usr_rx_buffer,
  .kernel_rx_buf = &rx_buf_a,
  .instance      = NULL,
};

// Helper utility to provide a new ring buffer to the kernel and 
// return the ring buffer previously held by the kernel
static ieee802154_rxbuf* swap_shared_kernel_buf(otTock* instance);

static void rx_callback(__attribute__ ((unused)) int   pans,
                        __attribute__ ((unused)) int   dst_addr,
                        __attribute__ ((unused)) int   src_addr,
                        __attribute__ ((unused)) void* _ud);

static ieee802154_rxbuf* swap_shared_kernel_buf(otTock* instance) {
  ieee802154_rxbuf* rx_buf;

  if (instance->kernel_rx_buf == &rx_buf_a) {
    instance->kernel_rx_buf = &rx_buf_b;
    rx_buf = &rx_buf_a;
  } else {
    instance->kernel_rx_buf = &rx_buf_a;
    rx_buf = &rx_buf_b;
  }

  reset_ring_buf(instance->kernel_rx_buf, rx_callback, NULL);
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
  // If new data exists, we copy the data from the returned kernel
  // ring buffer into the user space ring buffer
  if (usr_rx_buffer.new) {
    int offset;

    // loop through data until all new data is read
    while (usr_rx_buffer.read_index != usr_rx_buffer.write_index) {
      offset = usr_rx_buffer.read_index * IEEE802154_FRAME_LEN;
      char* rx_buf       = usr_rx_buffer.buffer;
      int header_len  = rx_buf[offset];
      int payload_len = rx_buf[offset + 1];
      int mic_len     = rx_buf[offset + 2];

      // this does not seem necessary since we implement the csma backoff in the radio driver
      // receiveFrame.mInfo.mRxInfo.mTimestamp = otPlatAlarmMilliGetNow() * 1000;
      receiveFrame.mInfo.mRxInfo.mRssi      = 50;
      receiveFrame.mLength = payload_len + header_len + mic_len + 2;
      receiveFrame.mInfo.mRxInfo.mTimestamp = 0;
      receiveFrame.mInfo.mRxInfo.mLqi       = 0x7f;

      // copy data
      for (int i = 0; i < (receiveFrame.mLength + IEEE802154_FRAME_META_LEN); i++) {
        receiveFrame.mPsdu[i] = rx_buf[i + IEEE802154_FRAME_META_LEN + offset];
      }

      // notify openthread instance that a frame has been received
      otPlatRadioReceiveDone(aInstance, &receiveFrame, OT_ERROR_NONE);
      usr_rx_buffer.read_index++;
      if (usr_rx_buffer.read_index == USER_RX_BUF_FRAME_COUNT) {
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

  /* It is important to avoid sync operations that yield (i.e. printf)
     as this may cause a new upcall to be handled and data to be received 
     out of order and/or lost. */

  char* rx_buf = swap_shared_kernel_buf(&otTockInstance)[0];
  
  char* head_index = &rx_buf[0];
  char* tail_index = &rx_buf[1];

  int offset = 2 + *head_index * IEEE802154_FRAME_LEN;

  // | head active # | tail active # | frame 0 | frame 1 | ... | frame n |
  while (*head_index != *tail_index) {
    int header_len = rx_buf[offset];
    int payload_len = rx_buf[offset + 1];
    int mic_len        = rx_buf[offset + 2];

    int receive_frame_length = payload_len + header_len + mic_len;
    int ring_buffer_offset = usr_rx_buffer.write_index * IEEE802154_FRAME_LEN;

    for (int i = 0; i < (IEEE802154_FRAME_META_LEN + receive_frame_length); i++) {
      usr_rx_buffer.buffer[ring_buffer_offset + i] = rx_buf[i + offset];
      rx_buf[i + offset] = 0;
    }

    usr_rx_buffer.new = true;
    usr_rx_buffer.write_index++;
    if (usr_rx_buffer.write_index == USER_RX_BUF_FRAME_COUNT) {
      usr_rx_buffer.write_index = 0;
    }

    *head_index = (*head_index + 1) % IEEE802154_MAX_RING_BUF_FRAMES;
    offset     += IEEE802154_FRAME_LEN;

    if (*head_index == 0) {
      offset = 2;
    }
  }

  assert(*head_index == *tail_index);

}


otError otTockStartReceive(uint8_t aChannel, otInstance *aInstance) {
  if (otTockInstance.instance == NULL) {
    otTockInstance.instance = aInstance;
  }
  if (aChannel != 26) {
    return OT_ERROR_NONE;
  }

  int res = ieee802154_receive(rx_callback, otTockInstance.kernel_rx_buf, NULL);

  otError result = OT_ERROR_NONE;

  if (res != 0) {
    result = OT_ERROR_FAILED;
  }

  return result;
}