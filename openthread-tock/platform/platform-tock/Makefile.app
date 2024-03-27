.PHONY: clean build_lib

lib_dir:=$(TOCK_USERLAND_BASE_DIR)/openthread-tock/lib

clean::
	cd $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock && rm -rf build
	make -C $(TOCK_USERLAND_BASE_DIR)/openthread-tock clean

platform-tock:
	make -C $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock

$(lib_dir)/libtcplp/build/cortex-m4/libtcplp.a: build_lib

$(lib_dir)/libopenthread/build/cortex-m4/libopenthread.a: build_lib

$(lib_dir)/libmbedx509/build/cortex-m4/libmbedx509.a: build_lib

$(lib_dir)/libmbedtls/build/cortex-m4/libmbedtls.a: build_lib

$(lib_dir)/libmbedcrypto/build/cortex-m4/libmbedcrypto.a: build_lib

$(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock/build/cortex-m4/platform-tock.a: | platform-tock
	make -C $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock

build_lib:
	cd $(TOCK_USERLAND_BASE_DIR)/openthread-tock && ./create_openthread-tock.sh

