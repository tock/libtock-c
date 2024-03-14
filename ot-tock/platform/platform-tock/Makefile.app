.PHONY: clean

clean::
	cd $(TOCK_USERLAND_BASE_DIR)/ot-tock/platform/platform-tock && rm -rf build
	make -C $(TOCK_USERLAND_BASE_DIR)/ot-tock clean

../../ot-tock/platform/platform-tock/build/cortex-m4/platform-tock.a:
	make -C $(TOCK_USERLAND_BASE_DIR)/ot-tock/platform/platform-tock

$(TOCK_USERLAND_BASE_DIR)/ot-tock/lib/libtcplp/build/cortex-m4/libtcplp.a: 
	cd $(TOCK_USERLAND_BASE_DIR)/ot-tock && ./create_ot-tock

$(TOCK_USERLAND_BASE_DIR)/ot-tock/lib/libopenthread/build/cortex-m4/libopenthread.a:
	cd $(TOCK_USERLAND_BASE_DIR)/ot-tock && ./create_ot-tock

$(TOCK_USERLAND_BASE_DIR)/ot-tock/lib/libmbedx509/build/cortex-m4/libmbedx509.a:
	cd $(TOCK_USERLAND_BASE_DIR)/ot-tock && ./create_ot-tock

$(TOCK_USERLAND_BASE_DIR)/ot-tock/lib/libmbedtls/build/cortex-m4/libmbedtls.a:
	cd $(TOCK_USERLAND_BASE_DIR)/ot-tock && ./create_ot-tock

$(TOCK_USERLAND_BASE_DIR)/ot-tock/lib/libmbedcrypto/build/cortex-m4/libmbedcrypto.a:
	cd $(TOCK_USERLAND_BASE_DIR)/ot-tock && ./create_ot-tock

