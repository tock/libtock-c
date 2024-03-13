// TODO Copyright

#include <openthread-core-config.h>
#include <openthread/config.h>
#include <openthread/platform/logging.h>
#include <openthread/platform/toolchain.h>
#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
// The compiler gives the following warning:
//
// ot-tock/platform/platform-tock/logging.c:18:5: warning: function 'otPlatLog'
// might be a candidate for 'gnu_printf' format attribute [-Wsuggest-attribute=format]
// 18 |     vprintf(aFormat, args);
//
// I am surpressing this warning here because I cannot annotate the otPlatLog function as
// it is defined in the openthread repo (which we do not want to modify). There may
// be another solution I am unaware of so others input would be welcome.

#if (OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED)
OT_TOOL_WEAK void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
  OT_UNUSED_VARIABLE(aLogLevel);
  OT_UNUSED_VARIABLE(aLogRegion);

  va_list args;

  va_start(args, aFormat);
  vprintf(aFormat, args);
  printf("\n");
  va_end(args);
}
#endif

#pragma GCC diagnostic pop