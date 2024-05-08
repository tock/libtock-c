#include <stdio.h>
#include <string.h>

#include <assert.h>

#include <openthread-system.h>
#include <openthread/dataset_ftd.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <plat.h>

#include <openthread/udp.h>

#include <ipc.h>
#include <tock.h>
#include <temperature.h>
#include <timer.h>

#define UDP_PORT 1212
static const char UDP_ROUTER_MULTICAST[] = "ff02::2";
static char UDP_CHAR;

static otUdpSocket sUdpSocket;
static void initUdp(otInstance *aInstance);
static void sendUdp(otInstance *aInstance);

// Callback method for received udp packets.
static void handleUdpReceive(void* aContext, otMessage *aMessage,
                             const otMessageInfo *aMessageInfo);

// TEMP
#include <button.h>
uint8_t temperature = 22;
uint8_t global_avg_openthread = 22;
static void button_callback(int                            btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1) {
    if (btn_num == 0 && temperature < 35) {
      temperature++;
    } else if (btn_num == 1 && temperature > 5) {
      temperature--;
    } else if (btn_num == 2) {
      temperature = 22;
    }
  
    UDP_CHAR = temperature;
    sendUdp((otInstance *)ud);
  }
}

static void print_temp(void){
      puts("\n");
      puts("====================================\n");
      printf("Preferred setpoint: %d\r\n", temperature);
      printf("Global setpoint: %d\r\n", global_avg_openthread);
      puts("====================================\r\n");
}
///

// Global variable storing the current temperature. This is written to in the
// main loop, and read from in the IPC handler. Because the app is single
// threaded and has no yield point when writing the value, we do not need to
// worry about synchronization -- reads never happen during a write.
static int current_global_avg = 0;


static void openthread_ipc_callback(int pid, int len, int buf,
		                __attribute__((unused)) void *ud)
{
  // A client has requested us to provide them the current temperature value.
  // We must make sure that it provides us with a buffer sufficiently large to
  // store a single integer:
  if (len < ((int) sizeof(current_global_avg))) {
    // We do not inform the caller and simply return. We do print a log message:
    puts("[thread] ERROR: sensor IPC invoked with too small buffer.\r\n");
  }

  // The buffer is large enough, copy the current temperature into it:
  memcpy((void*) buf, &current_global_avg, sizeof(current_global_avg));

  // Let the client know:
  ipc_notify_client(pid);
}


 
// helper utility demonstrating network config setup
static void setNetworkConfiguration(otInstance *aInstance);

// callback for Thread state change events
static void stateChangeCallback(uint32_t flags, void *context);

// helper utility to print ip address
static void print_ip_addr(otInstance *instance);

int main( __attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
 // Register this application as an IPC service under its name:
  ipc_register_service_callback(
    "org.tockos.thread-tutorial.openthread",
    openthread_ipc_callback,
    NULL);



  otSysInit(argc, argv);

  otInstance *instance;
  instance = otInstanceInitSingle();
  assert(instance);


  /// TEMP
  int err = button_subscribe(button_callback, instance);
  if (err < 0) return err;

button_enable_interrupt(0);
button_enable_interrupt(1);
button_enable_interrupt(2);
  /// END TEMP

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
  while(otIp6SetEnabled(instance, true) != OT_ERROR_NONE) {
    printf("Failed to start Thread network interface!\n");
    delay_ms(100);
  }

  otSetStateChangedCallback(instance, stateChangeCallback, instance);

  print_ip_addr(instance);

  // Initialize UDP socket (see guide: https://openthread.io/codelabs/openthread-apis#7)
  initUdp(instance);

  /* Start the Thread stack (CLI cmd -> thread start) */
  while(otThreadSetEnabled(instance, true) != OT_ERROR_NONE) {
    printf("Failed to start Thread stack!\n");
    delay_ms(100);
  }

  for (;;) {
      otTaskletsProcess(instance);
      otSysProcessDrivers(instance);

    if (!otTaskletsArePending(instance)) {
      	    yield();
    }

  }

  return 0;
}

void setNetworkConfiguration(otInstance *aInstance)
{
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

static void stateChangeCallback(uint32_t flags, void *context)
{
  otInstance *instance = (otInstance *)context;
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

static void print_ip_addr(otInstance *instance){
  char addr_string[64];
  const otNetifAddress *unicastAddrs = otIp6GetUnicastAddresses(instance);

  for (const otNetifAddress *addr = unicastAddrs; addr; addr = addr->mNext) {
    const otIp6Address ip6_addr = addr->mAddress;
    otIp6AddressToString(&ip6_addr, addr_string, sizeof(addr_string));
    printf("%s\n", addr_string);
  }

}

void handleUdpReceive(void *aContext, otMessage *aMessage,
                      const otMessageInfo *aMessageInfo)
{
  OT_UNUSED_VARIABLE(aContext);
  OT_UNUSED_VARIABLE(aMessageInfo);
  char buf[2];

  const otIp6Address sender_addr = aMessageInfo->mPeerAddr;
  otIp6AddressToString(&sender_addr, buf, sizeof(buf));

  uint16_t length = otMessageRead(aMessage, otMessageGetOffset(aMessage), buf, sizeof(buf) - 1);
  
  global_avg_openthread = buf[0];
  print_temp();
}

void initUdp(otInstance *aInstance)
{
  otSockAddr listenSockAddr;

  memset(&sUdpSocket, 0, sizeof(sUdpSocket));
  memset(&listenSockAddr, 0, sizeof(listenSockAddr));

  listenSockAddr.mPort = UDP_PORT;

  otUdpOpen(aInstance, &sUdpSocket, handleUdpReceive, aInstance);
  otUdpBind(aInstance, &sUdpSocket, &listenSockAddr, OT_NETIF_THREAD);
}

void sendUdp(otInstance *aInstance)
{
  otError error = OT_ERROR_NONE;
  otMessage *   message;
  otMessageInfo messageInfo;
  otIp6Address destinationAddr;

  memset(&messageInfo, 0, sizeof(messageInfo));

  otIp6AddressFromString(UDP_ROUTER_MULTICAST, &destinationAddr);
  messageInfo.mPeerAddr = destinationAddr;
  messageInfo.mPeerPort = UDP_PORT;

  message = otUdpNewMessage(aInstance, NULL);
  if(message == NULL){
    return;
  } 

  error = otMessageAppend(message, &UDP_CHAR, sizeof(UDP_CHAR));

  error = otUdpSend(aInstance, &sUdpSocket, message, &messageInfo);

    if (error != OT_ERROR_NONE && message != NULL)
    {
        otMessageFree(message);
    }
}
