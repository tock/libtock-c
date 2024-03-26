.PHONY: clean 


clean::
	cd $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock && rm -rf build
	make -C $(TOCK_USERLAND_BASE_DIR)/openthread-tock clean

../../openthread-tock/platform/platform-tock/build/cortex-m4/platform-tock.a:
	make -C $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock

platform-tock: $(wildcard $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock/*.c)
	make -C $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock

$(TOCK_USERLAND_BASE_DIR)/openthread-tock/lib/libtcplp/build/cortex-m4/libtcplp.a: build_lib 

$(TOCK_USERLAND_BASE_DIR)/openthread-tock/lib/libopenthread/build/cortex-m4/libopenthread.a: build_lib

$(TOCK_USERLAND_BASE_DIR)/openthread-tock/lib/libmbedx509/build/cortex-m4/libmbedx509.a: build_lib

$(TOCK_USERLAND_BASE_DIR)/openthread-tock/lib/libmbedtls/build/cortex-m4/libmbedtls.a: build_lib

$(TOCK_USERLAND_BASE_DIR)/openthread-tock/lib/libmbedcrypto/build/cortex-m4/libmbedcrypto.a: build_lib

build_lib:
	cd $(TOCK_USERLAND_BASE_DIR)/openthread-tock && ./create_openthread-tock.sh
	
