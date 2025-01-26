#include <assert.h>

#include <libopenthread/platform/openthread-system.h>
#include <libopenthread/platform/plat.h>
#include <openthread/dataset_ftd.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/message.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <openthread/udp.h>

#include <libtock/tock.h>

#include <stdio.h>
#include <string.h>

#define UDP_PORT 1212

static const char UDP_DEST_ADDR[] = "ff03::1";
static const char UDP_PAYLOAD[]   = "Hello OpenThread World from Tock!";
static void sendUdp(otInstance* aInstance);

static otUdpSocket sUdpSocket;
static void initUdp(otInstance* aInstance);

// helper utility demonstrating network config setup
static void setNetworkConfiguration(otInstance* aInstance);

// callback for Thread state change events
static void stateChangeCallback(uint32_t flags, void* context);

// helper utility to print ip address
static void print_ip_addr(otInstance* instance);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[]) {
  otSysInit(argc, argv);
  otInstance* instance;
  instance = otInstanceInitSingle();
  assert(instance);

  /* As part of the initialization, we will:
      - Init dataset with the following properties:
          - channel: 26
          - network key: 0x00112233445566778899aabbccddeeff
          - PAN ID: 0xabcd
      - configure ip addr (ifconfig up)
      - start thread network (thread start)
   */

  setNetworkConfiguration(instance);

  // set child timeout to 60 seconds
  otThreadSetChildTimeout(instance, 60);

  /* Start the Thread network interface (CLI cmd -> ifconfig up) */
  otIp6SetEnabled(instance, true);

  otSetStateChangedCallback(instance, stateChangeCallback, instance);

  print_ip_addr(instance);

  // Initialize UDP socket (see guide: https://openthread.io/codelabs/openthread-apis#7)
  initUdp(instance);

  /* Start the Thread stack (CLI cmd -> thread start) */
  otThreadSetEnabled(instance, true);

  uint32_t prev_time = 0;
  uint32_t curr_time = 0;

  for ( ;;) {
    // Send UDP packet every 2.5 seconds
    curr_time = otPlatAlarmMilliGetNow();
    if (curr_time - prev_time > 2500) {
      sendUdp(instance);
      prev_time = curr_time;
    }

    // main loop work
    otTaskletsProcess(instance);
    otSysProcessDrivers(instance);

    if (!otTaskletsArePending(instance) && !openthread_platform_pending_work()) {
      yield();
    }

  }

  return 0;
}

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

void initUdp(otInstance* aInstance) {
  otSockAddr listenSockAddr;

  memset(&sUdpSocket, 0, sizeof(sUdpSocket));
  memset(&listenSockAddr, 0, sizeof(listenSockAddr));

  listenSockAddr.mPort = UDP_PORT;

  otUdpOpen(aInstance, &sUdpSocket, NULL, aInstance);
  otUdpBind(aInstance, &sUdpSocket, &listenSockAddr, OT_NETIF_THREAD);
}

void sendUdp(otInstance* aInstance) {
  otError error = OT_ERROR_NONE;
  otMessage*   message;
  otMessageInfo messageInfo;
  otIp6Address destinationAddr;

  memset(&messageInfo, 0, sizeof(messageInfo));

  otIp6AddressFromString(UDP_DEST_ADDR, &destinationAddr);
  messageInfo.mPeerAddr = destinationAddr;
  messageInfo.mPeerPort = UDP_PORT;

  message = otUdpNewMessage(aInstance, NULL);
  if (message == NULL) {
    return;
  }

  error = otMessageAppend(message, UDP_PAYLOAD, sizeof(UDP_PAYLOAD));

  error = otUdpSend(aInstance, &sUdpSocket, message, &messageInfo);

  if (error != OT_ERROR_NONE && message != NULL) {
    otMessageFree(message);
  }
}

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

static void print_ip_addr(otInstance* instance) {
  char addr_string[64];
  const otNetifAddress* unicastAddrs = otIp6GetUnicastAddresses(instance);

  for (const otNetifAddress* addr = unicastAddrs; addr; addr = addr->mNext) {
    const otIp6Address ip6_addr = addr->mAddress;
    otIp6AddressToString(&ip6_addr, addr_string, sizeof(addr_string));
    printf("%s\n", addr_string);
  }

}
