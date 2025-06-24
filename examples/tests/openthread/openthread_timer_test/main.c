#include <assert.h>

#include <libopenthread/platform/openthread-system.h>
#include <openthread/dataset_ftd.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>

#include <libtock-sync/services/alarm.h>

#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////
// OPENTHREAD TIMER TEST //
// Openthread implements logic to provide a global time
// counter. OpenThread must handle wrapping logic to
// do this. This test verifies that the timer is
// able to accurately count upwards for 10 minutes.
// To provide a realistic test, the standard openthread
// app setup/configuration is used. However, the timer
// logic is tested rather than launching the thread network
// and executing the thread main loop.
////////////////////////////////////////////////////////////

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

  /* Start the Thread stack (CLI cmd -> thread start) */
  // otThreadSetEnabled(instance, true);

// TIMER TEST LOGIC
  printf("Beginning OpenThread timer test.\n");

  uint32_t passed_test      = 0; // Counter for progress along the way.
  uint32_t now              = 0;
  uint32_t prev             = otPlatAlarmMilliGetNow();
  const uint32_t END_TIME   = 1030 * 1000; // 10 minutes and 30 seconds.
  const uint32_t DELAY_TIME = 1000; // 1 second delay.
  libtocksync_alarm_delay_ms(DELAY_TIME);
  while (now < END_TIME) {
    now = otPlatAlarmMilliGetNow();
    // Confirm that obtained time is greater than last (checking for overflow) and
    // that the difference is within 3 ms of the expected 1000 ms (some latency
    // due to syscalls/kernel work is expected).
    if (now >= prev && (now - prev - DELAY_TIME) > 3) {
      printf("[FAIL] Now Value: %ld, Prev Value: %ld, Diff: %ld.\n", now, prev, now - prev - 1000);
      return -1;
    }
    ;
    prev = now;
    libtocksync_alarm_delay_ms(DELAY_TIME);

    // Print progress every minute.
    if (now > (30000 * (passed_test + 1))) {
      passed_test++;
      printf("[TEST UPDATE] %ld sec.\n", now / 1000);
    }
  }

  // Completed test.
  printf("[PASS] OpenThread Timer Test Passed.\n");
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
