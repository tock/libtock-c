.PHONY: clean

TCPLP_DIR:=$(TOCK_USERLAND_BASE_DIR)/libopenthread/libtcplp

clean:: 
	rm -rf build

libtcplp:
	make -C $(TCPLP_DIR)

# Make sure the library is built for every architecture
define TCPLP_RULES
$(TOCK_USERLAND_BASE_DIR)/libopenthread/libtcplp/build/$(1)/libtcplp.a: libtcplp
endef
$(foreach arch,$(TOCK_ARCHS),$(eval $(call TCPLP_RULES,$(arch))))

