# Support `#include <lua/lua.h>` in apps.
U8G2_LIB_DIR := $(TOCK_USERLAND_BASE_DIR)/lua53
override CFLAGS += -I$(U8G2_LIB_DIR)
