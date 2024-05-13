#include <net/ieee802154.h>
#include <libtock-sync/net/ieee802154.h>
#include <libtock/net/eui64.h>

#include <openthread-system.h>
#include <openthread/platform/radio.h>
#include <plat.h>

#include <stdio.h>

#define ACK_SIZE 3


static uint8_t tx_mPsdu[OT_RADIO_FRAME_MAX_SIZE];
static otRadioFrame transmitFrame = {
  .mPsdu   = tx_mPsdu,
  .mLength = OT_RADIO_FRAME_MAX_SIZE
};

// nrf52840 ACK on transmit is currently unimplemented. We fake this
// for now by always saying the sent packet was ACKed by the receiver.
static uint8_t ack_mPSdu[ACK_SIZE] = {0x02, 0x00, 0x00};
static otRadioFrame ackFrame = {
  .mPsdu   = ack_mPSdu,
  .mLength = 3
};

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64) {
  OT_UNUSED_VARIABLE(aInstance);
  uint64_t eui64;
  libtock_eui64_get(&eui64);
  for (int i = 0; i < 8; i++) {
	  aIeeeEui64[i] = (eui64 >> (8 * i)) & 0xFF;
  }

}

void otPlatRadioSetPanId(otInstance *aInstance, uint16_t aPanid) {
  OT_UNUSED_VARIABLE(aInstance);
  libtock_ieee802154_set_pan(aPanid);
  libtock_ieee802154_config_commit();
}

void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress) {
  OT_UNUSED_VARIABLE(aInstance);

  // convert aExtAddress to be big endian in temp variable
  otExtAddress temp;
  for (int i = 0; i < 8; i++) {
    temp.m8[i] = aExtAddress->m8[7 - i];
  }

  int retCode = libtock_ieee802154_set_address_long((unsigned char*) &temp);

  if (retCode != RETURNCODE_SUCCESS) printf("Error setting long address.\n");
}

void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress) {
  OT_UNUSED_VARIABLE(aInstance);
  libtock_ieee802154_set_address_short(aShortAddress);
  libtock_ieee802154_config_commit();
}

bool otPlatRadioIsEnabled(otInstance *aInstance) {
  OT_UNUSED_VARIABLE(aInstance);
  bool res;
  libtock_ieee802154_is_up(&res);
  return res;
}

otError otPlatRadioEnable(otInstance *aInstance) {
  OT_UNUSED_VARIABLE(aInstance);

  int retCode = libtocksync_ieee802154_up();
  if (retCode == RETURNCODE_SUCCESS) {
    return OT_ERROR_NONE;
  } else {
    printf("Initializing Radio Failed!\n");
    return OT_ERROR_FAILED;
  }
}

otError otPlatRadioDisable(otInstance *aInstance) {
  OT_UNUSED_VARIABLE(aInstance);

  int retCode = libtock_ieee802154_down();

  if (retCode == RETURNCODE_SUCCESS) {
    return OT_ERROR_NONE;
  } else {
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
    return OT_ERROR_NOT_IMPLEMENTED;
  }

  otError result = otTockStartReceive(aChannel, aInstance);
  return result;
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame) {
  OT_UNUSED_VARIABLE(aInstance);
  // For some reason, the frame length is 2 bytes longer than the actual frame length
  // this is corrected here. I have looked within openthread as to why this is
  // happening, but I cannot find the source of the problem. This "magic number"
  // fix is not ideal, but fixes the issue.
  aFrame->mLength = aFrame->mLength - 2;

  // since we do not support channel switching, if the channel is not 26, we fake
  // the result and tell openthread that we "sent" the packet successfully when
  // in actuality we do not.
  if (aFrame->mChannel != 26) {
    otPlatRadioTxDone(aInstance, aFrame, NULL, OT_ERROR_CHANNEL_ACCESS_FAILURE);
    return OT_ERROR_NONE;
  }

  // send raw will yield_for until the transmission completes
  int send_result =  libtocksync_ieee802154_send_raw((uint8_t*) aFrame->mPsdu, aFrame->mLength);

  // nrf52840 does not currently support ACK so no ACK is also considered a successful transmission
  if (send_result != RETURNCODE_SUCCESS && send_result != RETURNCODE_ENOACK) {
    otPlatRadioTxDone(aInstance, aFrame, &ackFrame, OT_ERROR_ABORT);
    return OT_ERROR_NONE;
  }

  // notify openthread that transmission is completed, faking the ACK value
  // with the ackFrame (see comment at the top of this file for more information)
  otPlatRadioTxDone(aInstance, aFrame, &ackFrame, OT_ERROR_NONE);
  return OT_ERROR_NONE;
}

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance) {
  OT_UNUSED_VARIABLE(aInstance);
  return &transmitFrame;
}

int8_t otPlatRadioGetRssi(otInstance *aInstance) {
  // TODO this is hardcoded to be -50 dbm currently. This will
  // result in the link quality indicator (LQI) to be excellent
  OT_UNUSED_VARIABLE(aInstance);
  // printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return -50;
}

otRadioCaps otPlatRadioGetCaps(otInstance *aInstance) {
  // TODO: Keeping this here as a placeholder to think
  // more about what capabilites we want to define for openthread.
  // Currently, we implement CSMA-CA backoff in the radio driver,
  // but we may add the security capability.
  OT_UNUSED_VARIABLE(aInstance);
  return (otRadioCaps)(OT_RADIO_CAPS_CSMA_BACKOFF | OT_RADIO_CAPS_SLEEP_TO_TX);
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance) {
  // TODO Tock sets its radio to promiscuous mode by default. OT will not
  // operate when the radio is in promiscuous mode. We return false
  // here, but this is not true.
  OT_UNUSED_VARIABLE(aInstance);
  return false;
}

void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aEnable);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aEnable);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aShortAddress);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return OT_ERROR_NONE;
}

otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aExtAddress);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return OT_ERROR_NONE;
}

otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aShortAddress);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return OT_ERROR_NONE;
}

otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aExtAddress);
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
  OT_UNUSED_VARIABLE(aScanChannel);
  OT_UNUSED_VARIABLE(aScanDuration);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return OT_ERROR_NONE;
}

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aPower);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return OT_ERROR_NONE;
}

otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aPower);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return OT_ERROR_NONE;
}

otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aThreshold);

  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return OT_ERROR_NONE;
}

otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold) {
  // TODO
  OT_UNUSED_VARIABLE(aInstance);
  OT_UNUSED_VARIABLE(aThreshold);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return OT_ERROR_NONE;
}

int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance) {
  // TODO this is hardcoded to be -50 dbm currently. This will
  // result in the link quality indicator (LQI) to be excellent
  OT_UNUSED_VARIABLE(aInstance);
  // printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  return -50;
}
