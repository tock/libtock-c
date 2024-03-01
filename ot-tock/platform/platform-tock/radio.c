// TODO COPYRIGHT

#include <ieee802154.h>
#include <openthread/platform/radio.h>
#include <stdio.h>
// #include <openthread/platform/alarm-milli.h>

#define ACK_SIZE 3
static uint8_t tx_mPsdu[OT_RADIO_FRAME_MAX_SIZE];
static uint8_t rx_mPsdu[OT_RADIO_FRAME_MAX_SIZE];
static uint8_t ack_mPSdu[ACK_SIZE] = {0x02, 0x00, 0x00};
static ieee802154_rxbuf rx_buf;


char frame_a[IEEE802154_FRAME_LEN];
char frame_b[IEEE802154_FRAME_LEN];
char frame_c[IEEE802154_FRAME_LEN];

static otRadioFrame transmitFrame = {
    .mPsdu = tx_mPsdu,
    .mLength = OT_RADIO_FRAME_MAX_SIZE
};

static otRadioFrame receiveFrame = {
    .mPsdu = rx_mPsdu,
    .mLength = OT_RADIO_FRAME_MAX_SIZE
};

static otRadioFrame ackFrame = {
    .mPsdu = ack_mPSdu,
    .mLength = 3
};

int rx_count=0;
int trans_count=0;
static void rx_callback(__attribute__ ((unused)) int   pans,
                     __attribute__ ((unused)) int   dst_addr,
                     __attribute__ ((unused)) int   src_addr,
                     __attribute__ ((unused)) void* aInstance) {   
    printf("rx_callback\n");
      ieee802154_unallow_rx_buf();
    
    int offset = 2;    
    // | head active # | tail active # | frame 0 | frame 1 | ... | frame n |

    char* head_index = &rx_buf[0];
    char* tail_index = &rx_buf[1];
    printf("start head_index: %d\n", *head_index);
    printf("start tail_index: %d\n", *tail_index);
    offset += *head_index * IEEE802154_FRAME_LEN;

    while (*head_index != *tail_index) {

    // int payload_offset = ieee802154_frame_get_payload_offset(packet_rx);
    // int payload_length = ieee802154_frame_get_payload_length(packet_rx);
    printf("offset val %d\n", offset);
    int payload_offset = rx_buf[offset];
    int payload_length = rx_buf[offset+1];
    int mic_len = rx_buf[offset+2];

    printf("payload_offset: %d\n", payload_offset);
    printf("payload_length: %d\n", payload_length);
    printf("mic_len: %d\n", mic_len);

    // MIC len is sometimes 2, sometimes 4 bytes
    receiveFrame.mInfo.mRxInfo.mTimestamp = otPlatAlarmMilliGetNow(aInstance) * 1000;
    receiveFrame.mInfo.mRxInfo.mRssi = 50;
    receiveFrame.mLength = payload_length+payload_offset+mic_len;
    receiveFrame.mInfo.mRxInfo.mLqi = 0x7f;
    // printf("THE RECEIVE LENGTH IS %d\n", receiveFrame.mLength);
    // copy packet_rx into receiveFrame.Psdu
    for (int i = 0; i < (receiveFrame.mLength); i++) {
        receiveFrame.mPsdu[i] = rx_buf[i+3+offset];
        rx_buf[i+3+offset] = 0;
    }
        printf("psdu: ");
    for (int i = 0; i < receiveFrame.mLength; i++) {
        if (i % 8 == 0) printf("\n");
        printf("%x ", receiveFrame.mPsdu[i]);
        }
    printf("\n");

    *head_index = (*head_index + 1) % MAX_FRAME_COUNT;
    offset+= IEEE802154_FRAME_LEN;

    if (*head_index == 0) {
        offset = 2;
    }
        printf("end head_index: %d\n", *head_index);
        printf("end tail_index: %d\n", *tail_index);

    otPlatRadioReceiveDone(aInstance, &receiveFrame, OT_ERROR_NONE);

    /*
    
    state of world as of 2/27 -- parent req / resp work because they are not encrypted
    parsing encrypted is different (due to mic being included). We need to somehow 
    convey this data from / across the capsule to here. This should resolve the issue 
    with joining. 
    
    */
  
    // // print all fields in received frame struct
    // printf("received frame: \n");
    // printf("length: %d\n", receiveFrame.mLength);

                            // assert(0);


    // if (mic_len == 4) {
    //     assert(0);
    // }
    }


    assert(*head_index == *tail_index);
    rx_count++;


}

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64) {
    // TODO HARDCODED FOR NOW... think about this
    OT_UNUSED_VARIABLE(aInstance);
    aIeeeEui64[0] = 0xf4;
    aIeeeEui64[1] = 0xce;
    aIeeeEui64[2] = 0x36;
    aIeeeEui64[3] = 0x67;
    aIeeeEui64[4] = 0x13;
    aIeeeEui64[5] = 0x12;
    aIeeeEui64[6] = 0x3f;
    aIeeeEui64[7] = 0xa6;
}

void otPlatRadioSetPanId(otInstance *aInstance, uint16_t aPanid) {
    OT_UNUSED_VARIABLE(aInstance);
    ieee802154_set_pan(aPanid);
    ieee802154_config_commit();
}

void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress) {
    OT_UNUSED_VARIABLE(aInstance);
    // printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    // convert aExtAddress to be big endian in temp variable
    otExtAddress temp;
    for (int i = 0; i < 8; i++) {
        temp.m8[i] = aExtAddress->m8[7-i];
    }


    int retCode = ieee802154_set_address_long((unsigned char*) &temp);
    assert(retCode==0);
    // print aExtAddress in hex values 
    for (int i = 8; i > 0; i--) {
        printf("[addr] %x ", aExtAddress->m8[i]);
    }
    if (retCode != 0) printf("Error setting long address");
}

// [addr] ee [addr] 68 [addr] bf [addr] 21 [addr] 2b [addr] c7 [addr] e0 [addr] 8a [D] SubMac--------: RadioExtAddress: 8ae0c72b21bf68ee

void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress) {
    OT_UNUSED_VARIABLE(aInstance);
    ieee802154_set_address(aShortAddress);
        ieee802154_config_commit();

}

bool otPlatRadioIsEnabled(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    return ieee802154_is_up();
}

otError otPlatRadioEnable(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);

    int retCode = ieee802154_up();
    if (retCode == 0)
        return OT_ERROR_NONE;
    else {
        printf("Initializing Radio Failed!\n");
        return OT_ERROR_FAILED;
    }
}

otError otPlatRadioDisable(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);

    int retCode = ieee802154_down();

    if (retCode == 0)
        return OT_ERROR_NONE;
    else {
        printf("Disable Radio Failed!\n");
        return OT_ERROR_FAILED;
    }
}

otError otPlatRadioSleep(otInstance *aInstance) {
    // TODO: There is no sleep function.
    OT_UNUSED_VARIABLE(aInstance);
    return OT_ERROR_NONE;
}

otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel) {

    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    
    // switch to channel TODO

    if (aChannel != 26) {
        return OT_ERROR_NONE;
    }

    receiveFrame.mChannel = 26;

    // Start receiving
    unsigned int packet_len = 2+(IEEE802154_FRAME_LEN*3);
    int retCode = ieee802154_receive(rx_callback, &rx_buf, packet_len, aInstance);


    // otPlatRadioReceiveDone()
    return OT_ERROR_NONE;
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    // For some reason, the frame length is 2 bytes longer than the actual frame length
    // this is corrected here. I have looked within openthread as to why this is 
    // happening, but I cannot find the source of the problem. This "magic number"
    // fix is not ideal, but fixes the issue.
    printf("send trans buf\n");

    aFrame->mLength = aFrame->mLength-2;

    if (aFrame->mChannel != 26) {
            otPlatRadioTxDone(aInstance, aFrame, NULL, OT_ERROR_NONE);

        return OT_ERROR_NONE;
    }

    // print the mPsdu in byte increments 
    printf("psdu: ");
    for (int i = 0; i < aFrame->mLength; i++) {
        if (i % 8 == 0) printf("\n");
        printf("%x ", aFrame->mPsdu[i]);
    }

    printf("\n*/*/*/*/*/*/*/*/*/\n");

    // Send the frame, and check if it was successful
    int send_result =  ieee802154_send_direct(aFrame->mPsdu, aFrame->mLength);
    
    // Send direct does support ACK so no ACK is also considered a successful transmission
    if (send_result != RETURNCODE_SUCCESS && send_result != RETURNCODE_ENOACK){
        return OT_ERROR_FAILED;
    }

    
    otPlatRadioTxDone(aInstance, aFrame, &ackFrame, OT_ERROR_NONE);

    return OT_ERROR_NONE;
}

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance) {
    printf("get trans buf\n");
    OT_UNUSED_VARIABLE(aInstance);
    return &transmitFrame;
}

int8_t otPlatRadioGetRssi(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return 0;
}

otRadioCaps otPlatRadioGetCaps(otInstance *aInstance) {
    // TODO: This is temporarily saying there are no capabilities.
    OT_UNUSED_VARIABLE(aInstance);
    return (otRadioCaps)(OT_RADIO_CAPS_CSMA_BACKOFF);
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance) {
    // Tock sets its radio to promiscuous mode by default. OT will not
    // operate when the radio is in promiscuous mode. We return false 
    // here, but this is not true.
    OT_UNUSED_VARIABLE(aInstance);
    return false;
}

void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

void otPlatRadioClearSrcMatchShortEntries(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otPlatRadioClearSrcMatchExtEntries(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return 50;
}