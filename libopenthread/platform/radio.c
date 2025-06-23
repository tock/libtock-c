#include <stdio.h>

#include <libtock-sync/net/ieee802154.h>
#include <libtock/net/eui64.h>
#include <libtock/net/ieee802154.h>

#include <openthread/platform/radio.h>

#include "openthread-system.h"
#include "plat.h"

#define ACK_SIZE 3

returncode_t ieee802154_set_channel_and_commit(uint8_t channel);

static uint8_t tx_mPsdu[OT_RADIO_FRAME_MAX_SIZE];
static otRadioFrame transmitFrame = {
  .mPsdu   = tx_mPsdu,
  .mLength = OT_RADIO_FRAME_MAX_SIZE
};

// nrf52840 ACK on transmit is currently unimplemented. We fake this
// for now by always saying the sent packet was ACKed by the receiver.
static uint8_t ack_mPSdu[ACK_SIZE] = {0x02, 0x00, 0x00};
static otRadioFrame ackFrame_radio = {
  .mPsdu   = ack_mPSdu,
  .mLength = 3
};

static struct pending_tx_done_callback {
  bool flag;
  bool acked;
  statuscode_t status;
} pending_tx_done_callback = {false, false, TOCK_STATUSCODE_FAIL};

static void tx_done_callback(statuscode_t status, bool acked) {
	pending_tx_done_callback.flag = true;
  pending_tx_done_callback.acked = acked;
  pending_tx_done_callback.status = status;
}

bool pending_tx_done_callback_status(otRadioFrame *ackFrame, returncode_t *status, otRadioFrame *txFrame) {
  if (pending_tx_done_callback.flag) {
    *ackFrame = ackFrame_radio;
    *status = tock_status_to_returncode(pending_tx_done_callback.status);
    *txFrame = transmitFrame;
  } 
	
  return pending_tx_done_callback.flag;
}

void reset_pending_tx_done_callback(void) {
	pending_tx_done_callback.flag = false;
}

// Helper method for setting radio channel and calling config commit.
returncode_t ieee802154_set_channel_and_commit(uint8_t channel) {
  returncode_t retCode = libtock_ieee802154_set_channel(channel);
  if (retCode != RETURNCODE_SUCCESS) {
    return retCode;
  }
  return libtock_ieee802154_config_commit();
}

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
  OT_UNUSED_VARIABLE(aInstance);

  int retCode = libtock_ieee802154_radio_off();

  if (retCode == RETURNCODE_SUCCESS) {
    return OT_ERROR_NONE;
  } else {
    printf("Sleep Radio Failed!\n");
    return OT_ERROR_FAILED;
  }
  return OT_ERROR_NONE;
}

otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel) {
  if (!otPlatRadioIsEnabled(aInstance)){
    otPlatRadioEnable(aInstance);
  }

  int retCode = ieee802154_set_channel_and_commit(aChannel);
  if (retCode != RETURNCODE_SUCCESS) {
    return OT_ERROR_FAILED;
  }

  otError result = otTockStartReceive(aChannel, aInstance);
  return result;
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame) {
  if (!otPlatRadioIsEnabled(aInstance)){
    otPlatRadioEnable(aInstance);
  }

  // The Tock raw 15.4 driver expects frames that do not include the MFR (aka
  // the CRC bytes). OpenThread gives us the full frame, so we just drop the
  // final two bytes.
  aFrame->mLength = aFrame->mLength - 2;

  int retCode = ieee802154_set_channel_and_commit(aFrame->mChannel);
  if (retCode != RETURNCODE_SUCCESS) {
    return OT_ERROR_FAILED;
  }

  returncode_t send_result =  libtock_ieee802154_send_raw((uint8_t*) aFrame->mPsdu, aFrame->mLength, tx_done_callback);
  if (send_result != RETURNCODE_SUCCESS) {
    return OT_ERROR_FAILED;
  }

  // Notify openthread that transmission is successfully scheduled to be transmitted.
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
