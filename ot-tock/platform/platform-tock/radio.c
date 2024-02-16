// TODO COPYRIGHT

#include <ieee802154.h>
#include <openthread/platform/radio.h>
#include <stdio.h>

// Some functions require returning pointer to an otRadioFrame (TODO)
static otRadioFrame tempTransmitBuffer;
uint8_t mPsdu[OT_RADIO_FRAME_MAX_SIZE];
static otRadioCaps tempRadioCaps = OT_RADIO_CAPS_NONE;

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    aIeeeEui64[0] = 0x18;
    aIeeeEui64[1] = 0xb4;
    aIeeeEui64[2] = 0x30;
    aIeeeEui64[3] = 0x00;
    aIeeeEui64[4] = 0xff;
    aIeeeEui64[5] = 0xff;
    aIeeeEui64[6] = 0xff;
    aIeeeEui64[7] = 0xff;
}

void otPlatRadioSetPanId(otInstance *aInstance, uint16_t aPanid) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    ieee802154_set_pan(aPanid);
}

void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    int retCode = ieee802154_set_address_long((unsigned char*) aExtAddress);
    assert(retCode==0);
    // print aExtAddress in hex values 
    for (int i = 0; i < 8; i++) {
        printf("[addr] %x ", aExtAddress->m8[i]);
    }
    if (retCode != 0) printf("Error setting long address");
}

void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    int retCode = ieee802154_set_address(aShortAddress);
    if (retCode != 0) printf("Error setting address");
}

bool otPlatRadioIsEnabled(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    return ieee802154_is_up();
}

otError otPlatRadioEnable(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

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
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

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
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    // otPlatRadioReceiveDone()
    return OT_ERROR_NONE;
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    // // print transmitted psdu in aFrame
    // for (int i = 0; i < aFrame->mLength; i++) {
    //     printf("[psdu]: %x \n", aFrame->mPsdu[i]);
    // }
    aFrame->mLength = aFrame->mLength-2;
    ieee802154_send_direct(aFrame->mPsdu, aFrame->mLength);
    otPlatRadioTxDone(aInstance, aFrame, NULL, OT_ERROR_NONE);
    printf("**********************frame len %d\n", aFrame->mLength);
    return OT_ERROR_NONE;
}

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    tempTransmitBuffer.mPsdu = mPsdu;
    return &tempTransmitBuffer;
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
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return tempRadioCaps;
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
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
    return 0;
}