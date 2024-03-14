
$(info $(LIB_DIR))
$(LIB_DIR)/libopenthread.a:
	echo libopen build
	cd $(TOCK_USERLAND_BASE_DIR)/ot-tock && ./create_ot-tock

