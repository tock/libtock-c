# Include the libtock folder for includes to find header files. This is
# temporary until the libtock-c rewrite is merged.
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/simple-ble
