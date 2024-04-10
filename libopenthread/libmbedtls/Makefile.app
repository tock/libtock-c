.PHONY: clean

LIBMBEDTLS_DIR:=$(TOCK_USERLAND_BASE_DIR)/libopenthread/libmbedtls

clean::
	rm -rf build

libmbedtls:
	make -C $(LIBMBEDTLS_DIR)

# Make sure the library is built for every architecture
define MBEDTLS_RULES
$(TOCK_USERLAND_BASE_DIR)/libopenthread/libmbedtls/build/$(1)/libmbedtls.a: libmbedtls 
endef
$(foreach arch,$(TOCK_ARCHS),$(eval $(call MBEDTLS_RULES,$(arch))))
