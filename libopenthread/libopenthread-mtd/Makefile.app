TOCK_USERLAND_BASE_DIR ?= ../..

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libopenthread/openthread/include
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libopenthread/openthread/src/core
