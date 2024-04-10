.PHONY: clean

PLATFORM_DIR:=$(TOCK_USERLAND_BASE_DIR)/libopenthread/platform

clean::
	rm -rf build

platform:
	make -C $(PLATFORM_DIR)

# Make sure the library is built for every architecture
define PLATFORM_RULES
$(TOCK_USERLAND_BASE_DIR)/libopenthread/platform/build/$(1)/platform.a: platform 
endef
$(foreach arch,$(TOCK_ARCHS),$(eval $(call PLATFORM_RULES,$(arch))))


