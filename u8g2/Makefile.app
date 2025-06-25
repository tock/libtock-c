# Git hash of the library to use. Must match the hash in Makefile.version.
U8G2_VERSION_HASH := bde09fbf787892c79a184e88b124aa5c79393aed

# Base folder definitions
U8G2_LIB_DIR := $(TOCK_USERLAND_BASE_DIR)/u8g2
U8G2_SRC_DIR := $(U8G2_LIB_DIR)/u8g2-$(U8G2_VERSION_HASH)

override CFLAGS += -I$(U8G2_SRC_DIR)/csrc
override CFLAGS += -I$(U8G2_LIB_DIR)
