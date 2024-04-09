#pragma once

// We use the default mbedtls-config.h file with one change.

#include "mbedtls-config.h"

// Check for cortex-m0.
#if defined(__thumb__) && !defined(__thumb2__)
// Only set `MBEDTLS_HAVE_ASM` when using thumb2, aka not on cortex-m0. The
// assembly implementation uses register r9 which we use for PIC. So we cannot
// use the assembly implementation on cortex-m0.
#undef MBEDTLS_HAVE_ASM
#endif
