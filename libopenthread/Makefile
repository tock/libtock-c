.PHONY: clean default

TOCK_USERLAND_BASE_DIR ?= ..

default: ftd mtd

ftd: dependencies
	make -C libopenthread-ftd

mtd: dependencies
	make -C libopenthread-mtd

# confirm openthread git submodule is checked out.
$(TOCK_USERLAND_BASE_DIR)/libopenthread/openthread/README.md:
	git submodule update --init -- openthread

dependencies: $(TOCK_USERLAND_BASE_DIR)/libopenthread/openthread/README.md
	make -C libmbedtls
	make -C libtcplp
	make -C platform

clean::
	make -C libopenthread-ftd clean
	make -C libopenthread-mtd clean
	make -C libmbedtls clean
	make -C libtcplp clean
	make -C platform clean
