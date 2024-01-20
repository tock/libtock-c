#ifndef DUMMY_PLATFORM_H
#define DUMMY_PLATFORM_H

#include <string.h>

#include <openthread/config.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/dns.h>
#include <openthread/platform/dso_transport.h>
#include <openthread/platform/entropy.h>
#include <openthread/platform/logging.h>
#include <openthread/platform/misc.h>
#include <openthread/platform/multipan.h>
#include <openthread/platform/radio.h>
#include <openthread/platform/trel.h>

#include "common/code_utils.hpp"
#include "instance/instance.hpp"

//#include "test_util.h"

ot::Instance *testInitInstance(void);
#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE && OPENTHREAD_CONFIG_MULTIPLE_STATIC_INSTANCE_ENABLE
ot::Instance *testInitAdditionalInstance(uint8_t id);
#endif
void testFreeInstance(otInstance *aInstance);

#endif // DUMMY_PLATFORM_H
