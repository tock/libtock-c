#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <libopenthread/platform/openthread-system.h>
#include <libopenthread/platform/plat.h>
#include <openthread/dataset_ftd.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <openthread/udp.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/ipc.h>
#include <libtock/services/alarm.h>
#include <libtock/tock.h>

#include "oracle.h"

static uint8_t encrypted_data[64];
static uint8_t decrypted_data[64];

static otUdpSocket sUdpSocket;

void initUdp(otInstance* instance);

void handleUdpRecv(void* aContext, otMessage* aMessage,
                   const otMessageInfo* aMessageInfo);

// helper utility demonstrating network config setup
static void setNetworkConfiguration(otInstance* aInstance);

// callback for Thread state change events
static void stateChangeCallback(uint32_t flags, void* context);

// helper utility to print ip address
static void print_ip_addr(otInstance* instance);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[]) {
  // Initialize OpenThread instance.
  otSysInit(argc, argv);
  otInstance* instance;
  instance = otInstanceInitSingle();
  assert(instance);

  // set child timeout to 60 seconds.
  otThreadSetChildTimeout(instance, 60);

  // Set callback to be notified when thread state changes.
  otSetStateChangedCallback(instance, stateChangeCallback, instance);

  ///////////////////////////////////////////////////
  // THREAD NETWORK SETUP HERE

  // Configure network.
  setNetworkConfiguration(instance);

  // Enable network interface.
  while (otIp6SetEnabled(instance, true) != OT_ERROR_NONE) {
    printf("Failed to start Thread network interface!\n");
    libtocksync_alarm_delay_ms(100);
  }

  // initUDP
  initUdp(instance);

  // Print IPv6 address.
  print_ip_addr(instance);

  // Start Thread network.
  while (otThreadSetEnabled(instance, true) != OT_ERROR_NONE) {
    printf("Failed to start Thread stack!\n");
    libtocksync_alarm_delay_ms(100);
  }

  //
  ////////////////////////////////////////////////////

  // OpenThread main loop.
  for ( ;;) {
    // Execute any pending OpenThread related work.
    otTaskletsProcess(instance);

    // Execute any platform related work (e.g. check
    // radio buffer for new packets).
    otSysProcessDrivers(instance);

    // If there is not pending platform or OpenThread
    // related work -- yield.
    if (!otTaskletsArePending(instance) &&
        !openthread_platform_pending_work()) {
      yield();
    }
  }

  return 0;
}

// Helper method that configures the OpenThread network dataset
// for the desired tutorial configuration.
// We set the following dataset parameters:
//  -- Channel:    26
//  -- PanId:      0xabcd
//  -- Networkkey: 00112233445566778899aabbccddeeff
void setNetworkConfiguration(otInstance* aInstance) {
  otOperationalDataset aDataset;

  memset(&aDataset, 0, sizeof(otOperationalDataset));

  /* Set Channel to 26 */
  aDataset.mChannel = 26;
  aDataset.mComponents.mIsChannelPresent = true;

  /* Set Pan ID to abcd */
  aDataset.mPanId = (otPanId)0xabcd;
  aDataset.mComponents.mIsPanIdPresent = true;

  /* Set network key to 00112233445566778899aabbccddeeff */
  uint8_t key[OT_NETWORK_KEY_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  memcpy(aDataset.mNetworkKey.m8, key, sizeof(aDataset.mNetworkKey));
  aDataset.mComponents.mIsNetworkKeyPresent = true;

  otError error = otDatasetSetActive(aInstance, &aDataset);
  assert(error == 0);
}

// Helper method that registers a stateChangeCallback to print
// when state changes occur (useful for debugging).
static void stateChangeCallback(uint32_t flags, void* context) {
  otInstance* instance = (otInstance*)context;
  if (!(flags & OT_CHANGED_THREAD_ROLE)) {
    return;
  }

  switch (otThreadGetDeviceRole(instance)) {
    case OT_DEVICE_ROLE_DISABLED:
      printf("[State Change] - Disabled.\n");
      break;
    case OT_DEVICE_ROLE_DETACHED:
      printf("[State Change] - Detached.\n");
      break;
    case OT_DEVICE_ROLE_CHILD:
      printf("[State Change] - Child.\n");
      printf("Successfully attached to Thread network as a child.\n");
      break;
    case OT_DEVICE_ROLE_ROUTER:
      printf("[State Change] - Router.\n");
      break;
    case OT_DEVICE_ROLE_LEADER:
      printf("[State Change] - Leader.\n");
      break;
    default:
      break;
  }
}

// Helper method to print the given Thread node's registered
// ipv6 address.
static void print_ip_addr(otInstance* instance) {
  char addr_string[64];
  const otNetifAddress* unicastAddrs = otIp6GetUnicastAddresses(instance);

  printf("[THREAD] Device IPv6 Addresses: ");
  for (const otNetifAddress* addr = unicastAddrs; addr; addr = addr->mNext) {
    const otIp6Address ip6_addr = addr->mAddress;
    otIp6AddressToString(&ip6_addr, addr_string, sizeof(addr_string));
    printf("%s\n", addr_string);
  }
}

void handleUdpRecv(void* aContext, otMessage* aMessage,
                   const otMessageInfo* aMessageInfo) {
  OT_UNUSED_VARIABLE(aContext);
  OT_UNUSED_VARIABLE(aMessageInfo);
  char buf[150];
  int length;

  printf("Received UDP packet [%d bytes] from ", otMessageGetLength(aMessage) - otMessageGetOffset(aMessage));
  const otIp6Address sender_addr = aMessageInfo->mPeerAddr;
  otIp6AddressToString(&sender_addr, buf, sizeof(buf));
  printf(" %s ", buf);

  length = otMessageRead(aMessage, otMessageGetOffset(aMessage), buf, sizeof(buf));

  // Packet is of form | 3 byte header | 16 byte IV | encrypted data |
  const uint8_t HEADER_LENGTH         = 3;
  const uint8_t IV_OFFSET             = HEADER_LENGTH;
  const uint8_t IV_LENGTH             = 16;
  const uint8_t ENCRYPTED_DATA_OFFSET = IV_OFFSET + IV_LENGTH;

  // Determine if this is an encrypted packet
  // payload using header "Rot" -- Root of Trust.
  if (memcmp(buf, "RoT", HEADER_LENGTH) == 0) {

    // Must be at least 20 bytes long (3 for "RoT" header,
    // 16 for IV, and at least 1 byte of encrypted data).
    if (length <= (HEADER_LENGTH + IV_LENGTH + 1)) {
      return;
    }

    uint8_t iv[IV_LENGTH];
    memcpy(iv, buf + IV_OFFSET, IV_LENGTH);

    uint8_t encrypted_data_length = length - ENCRYPTED_DATA_OFFSET;
    memcpy(encrypted_data, buf + ENCRYPTED_DATA_OFFSET, encrypted_data_length);

    // Decrypt the data using the oracle_decrypt function.
    length = oracle_decrypt(iv, encrypted_data, encrypted_data_length, decrypted_data, sizeof(decrypted_data));
    memcpy(buf, decrypted_data, length);
  }

  // Print the received data.
  for (int i = 0; i < length; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");
}

void initUdp(otInstance* aInstance) {
  otSockAddr listenSockAddr;

  memset(&sUdpSocket, 0, sizeof(sUdpSocket));
  memset(&listenSockAddr, 0, sizeof(listenSockAddr));

  listenSockAddr.mPort = 1212;

  otUdpOpen(aInstance, &sUdpSocket, handleUdpRecv, aInstance);
  otUdpBind(aInstance, &sUdpSocket, &listenSockAddr, OT_NETIF_THREAD);
}
