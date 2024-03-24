.PHONY: default
default: all ;

# Git hash of the library to use. Must match the hash in Makefile.
U8G2_VERSION_HASH := c4f9cd9f8717661c46be16bfbcb0017d785db3c1

# Base folder definitions
U8G2_LIB_DIR := $(TOCK_USERLAND_BASE_DIR)/u8g2
U8G2_SRC_DIR := $(U8G2_LIB_DIR)/u8g2-$(U8G2_VERSION_HASH)

override CFLAGS += -I$(U8G2_SRC_DIR)/csrc
override CFLAGS += -I$(U8G2_LIB_DIR)

# Rules to download the source repository if needed. These are here so that the
# expanded library is available before calling into the library Makefile.
$(U8G2_SRC_DIR).zip:
	curl -L --output $(U8G2_SRC_DIR).zip https://codeload.github.com/olikraus/u8g2/zip/$(U8G2_VERSION_HASH)

# The .h file will exist when the library is unzipped.
$(U8G2_SRC_DIR)/csrc/u8g2.h: | $(U8G2_SRC_DIR).zip
	unzip -q -d $(U8G2_LIB_DIR) $(U8G2_SRC_DIR).zip

# Ensure the library exists before compiling main.c
main.c: $(U8G2_SRC_DIR)/csrc/u8g2.h

# Rule to build the library
u8g2lib: $(U8G2_SRC_DIR)/csrc/u8g2.h
	make -C $(U8G2_LIB_DIR) all

# Make sure the library is built for every architecture
define U8G2_RULES
$(U8G2_LIB_DIR)/build/$(1)/u8g2.a: | u8g2lib
endef
$(foreach arch,$(TOCK_ARCHS),$(eval $(call U8G2_RULES,$(arch))))
