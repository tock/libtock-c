.PHONY: default
default: all ;

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := u8g2
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)
VERSION_HASH := c4f9cd9f8717661c46be16bfbcb0017d785db3c1
LIB_SRC_DIR := u8g2-$(VERSION_HASH)

# List all C and Assembly files
$(LIBNAME)_SRCS_ALL  += $(wildcard $(LIB_SRC_DIR)/csrc/u8g2*.c)
$(LIBNAME)_SRCS_ALL  += $(wildcard $(LIB_SRC_DIR)/csrc/u8x8*.c)

$(LIBNAME)_SRCS_ALL  += u8g2-tock.c

# Remove the buffer file, we need to create our own tock-specific version.
$(LIBNAME)_SRCS := $(filter-out $(LIB_SRC_DIR)/csrc/u8g2_buffer.c,$($(LIBNAME)_SRCS_ALL))

# Need libtock headers
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock
override CPPFLAGS += -I$(LIB_SRC_DIR)/csrc

# Rules to download the source repository if needed.
$(LIB_SRC_DIR).zip:
	curl -L --output $(LIB_SRC_DIR).zip https://codeload.github.com/olikraus/u8g2/zip/$(VERSION_HASH)

$(LIB_SRC_DIR)/csrc/%.c: | $(LIB_SRC_DIR).zip
	unzip $(LIB_SRC_DIR).zip

u8g2-tock.c: $(LIB_SRC_DIR)/csrc/u8g2_setup.c

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
