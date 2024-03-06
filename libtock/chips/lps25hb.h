#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Callback type when a pressure reading is ready.
typedef void (*libtock_lps25hb_callback)(returncode_t, int);

// Request a pressure reading delivered through the callback.
returncode_t libtock_lps25hb_get_pressure(libtock_lps25hb_callback cb);

#ifdef __cplusplus
}
#endif
