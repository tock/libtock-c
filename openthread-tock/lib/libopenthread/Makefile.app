
$(info $(LIB_DIR))
$(LIB_DIR)/libopenthread.a:
	echo libopen build
	cd $(TOCK_USERLAND_BASE_DIR)/openthread-tock && ./create_openthread-tock.sh

