#include <stdio.h>
#include <string.h>

#include <assert.h>

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

// helper utility demonstrating network config setup
static void setNetworkConfiguration(otInstance* aInstance);

static otUdpSocket sUdpSocket;

void initUdp(otInstance* aInstance);

void handleUdpRecvTemperature(void* aContext, otMessage* aMessage,
                              const otMessageInfo* aMessageInfo);

void sendUdpTemperature(otInstance* aInstance, uint8_t temperature);

// callback for Thread state change events
static void stateChangeCallback(uint32_t flags, void* context);

// helper utility to print ip address
static void print_ip_addr(otInstance* instance);

uint8_t local_temperature_setpoint        = 22;
uint8_t global_temperature_setpoint       = 255;
uint8_t prior_global_temperature_setpoint = 255;
bool network_up      = false;
bool send_local_temp = false;

static void openthread_ipc_callback(int pid, int len, int buf,
                                    __attribute__((unused)) void* ud) {
  // A client has requested us to provide them the current temperature value.
  // We must make sure that it provides us with a buffer sufficiently large to
  // store a single integer:
  if (len < ((int) sizeof(prior_global_temperature_setpoint))) {
    // We do not inform the caller and simply return. We do print a log message:
    puts("[thread] ERROR: sensor IPC invoked with too small buffer.\r\n");
  }

  // copy value in buffer to local_temperature_setpoint
  uint8_t passed_local_setpoint = *((uint8_t*) buf);
  if (passed_local_setpoint != local_temperature_setpoint) {
    // The local setpoint has changed, update it.
    local_temperature_setpoint = passed_local_setpoint;
    send_local_temp = true;
  }

  if (network_up) {
    if (prior_global_temperature_setpoint != global_temperature_setpoint) {
      prior_global_temperature_setpoint = global_temperature_setpoint;

      // The buffer is large enough, copy the current temperature into it.
      memcpy((void*) buf, &global_temperature_setpoint, sizeof(global_temperature_setpoint));

      // Notify the client that the temperature has changed.
      ipc_notify_client(pid);
    }
  }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[]) {
  // Initialize OpenThread instance.
  otSysInit(argc, argv);
  otInstance* instance;
  instance = otInstanceInitSingle();
  assert(instance);

  // Register this application as an IPC service under its name:
  ipc_register_service_callback("org.tockos.thread-tutorial.openthread",
                                openthread_ipc_callback,
                                NULL);

  // set child timeout to 60 seconds.
  otThreadSetChildTimeout(instance, 60);

  // Set callback to be notified when thread state changes.
  otSetStateChangedCallback(instance, stateChangeCallback, instance);

  ///////////////////////////////////////////////////
  // THREAD NETWORK SETUP HERE

  // Configure network.
  setNetworkConfiguration(instance);

  // Init UDP interface.
  initUdp(instance);

  // Enable network interface.
  while (otIp6SetEnabled(instance, true) != OT_ERROR_NONE) {
    printf("Failed to start Thread network interface!\n");
    libtocksync_alarm_delay_ms(100);
  }

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

    // Send udp packet if pending temperature update.
    if (send_local_temp) {
      sendUdpTemperature(instance, local_temperature_setpoint);
      send_local_temp = false;
    }

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
      sendUdpTemperature(instance, local_temperature_setpoint);
      network_up = true;
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


void handleUdpRecvTemperature(void* aContext, otMessage* aMessage,
                              const otMessageInfo* aMessageInfo) {
  OT_UNUSED_VARIABLE(aContext);
  OT_UNUSED_VARIABLE(aMessageInfo);
  char buf[2];

  const otIp6Address sender_addr = aMessageInfo->mPeerAddr;
  otIp6AddressToString(&sender_addr, buf, sizeof(buf));

  otMessageRead(aMessage, otMessageGetOffset(aMessage), buf, sizeof(buf) - 1);
  global_temperature_setpoint = buf[0];
}

void initUdp(otInstance* aInstance) {
  otSockAddr listenSockAddr;

  memset(&sUdpSocket, 0, sizeof(sUdpSocket));
  memset(&listenSockAddr, 0, sizeof(listenSockAddr));

  listenSockAddr.mPort = 1212;

  otUdpOpen(aInstance, &sUdpSocket, handleUdpRecvTemperature, aInstance);
  otUdpBind(aInstance, &sUdpSocket, &listenSockAddr, OT_NETIF_THREAD);
}

void sendUdpTemperature(otInstance* aInstance, uint8_t temperature) {

  otError error = OT_ERROR_NONE;
  otMessage*   message;
  otMessageInfo messageInfo;
  otIp6Address destinationAddr;

  memset(&messageInfo, 0, sizeof(messageInfo));

  otIp6AddressFromString("ff02::02", &destinationAddr);
  messageInfo.mPeerAddr = destinationAddr;
  messageInfo.mPeerPort = 1212;

  message = otUdpNewMessage(aInstance, NULL);
  if (message == NULL) {
    printf("Error creating udp message\n");
    return;
  }

  error = otMessageAppend(message, &temperature, 1);
  if (error != OT_ERROR_NONE && message != NULL) {
    printf("Error appending to udp message\n");
    otMessageFree(message);
    return;
  }

  error = otUdpSend(aInstance, &sUdpSocket, message, &messageInfo);
  if (error != OT_ERROR_NONE && message != NULL) {
    printf("Error sending udp packet\n");
    otMessageFree(message);
  }
}
