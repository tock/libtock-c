// TODO COPYRIGHT

#include<openthread/platform/radio.h>

// Some functions require returning pointer to an otRadioFrame (TODO)
static otRadioFrame tempFrame;
static otRadioCaps tempRadioCaps;

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64){
    // TODO
}

void otPlatRadioSetPanId(otInstance *aInstance, uint16_t aPanid){
    // TODO
}

void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress){
    // TODO
}

void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress) {
    // TODO
}

bool otPlatRadioIsEnabled(otInstance *aInstance){
    // TODO
    return true;
}

otError otPlatRadioEnable(otInstance *aInstance){
    // TODO 
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioDisable(otInstance *aInstance){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioSleep(otInstance *aInstance){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance){
    // TODO
    return &tempFrame;
}

int8_t otPlatRadioGetRssi(otInstance *aInstance){
    // TODO
    return 0;
}

otRadioCaps otPlatRadioGetCaps(otInstance *aInstance)
{
    // TODO
    return tempRadioCaps;
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance){
    // TODO
    return true;
}

void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable){
    // TODO
}

void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable){
    // TODO
}

otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

void otPlatRadioClearSrcMatchShortEntries(otInstance *aInstance){
    // TODO
}

void otPlatRadioClearSrcMatchExtEntries(otInstance *aInstance){
    // TODO
}

otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold){
    // TODO
    return OT_ERROR_NOT_IMPLEMENTED;
}

int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance){
    // TODO
    return 0;
}