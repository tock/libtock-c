.PHONY: clean

LIBOPENTHREAD-MTD_DIR:=$(TOCK_USERLAND_BASE_DIR)/libopenthread/libopenthread-mtd

clean::
	rm -rf build

libopenthread-mtd: 
	make -C $(LIBOPENTHREAD-MTD_DIR)

# Make sure the library is built for every architecture
define MTD_RULES
$(TOCK_USERLAND_BASE_DIR)/libopenthread/libopenthread-mtd/build/$(1)/libopenthread-mtd.a: libopenthread-mtd 
endef
$(foreach arch,$(TOCK_ARCHS),$(eval $(call MTD_RULES,$(arch))))
