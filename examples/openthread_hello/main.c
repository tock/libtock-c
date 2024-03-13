#include <assert.h>
#include <openthread-system.h>
#include <openthread/dataset_ftd.h>
#include <openthread/instance.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <plat.h>
#include <stdio.h>
#include <string.h>
#include <timer.h>

static void setNetworkConfiguration(otInstance *aInstance);

int main( __attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
  otInstance *instance;
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

  // set child timeout to 10 seconds(helpful for testing attachment issues)
  otThreadSetChildTimeout(instance, 10);

  /* Start the Thread network interface (CLI cmd > ifconfig up) */
  otIp6SetEnabled(instance, true);

  // Initialize UDP socket TODO (see guide: https://openthread.io/codelabs/openthread-apis#7)

  /* Start the Thread stack (CLI cmd > thread start) */
  otThreadSetEnabled(instance, true);

  for ( ;;) {
    otTaskletsProcess(instance);
    otSysProcessDrivers(instance);
    yield();
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