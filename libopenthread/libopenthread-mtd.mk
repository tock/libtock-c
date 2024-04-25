# Include this Makefile in the app's makefile to use the OpenThread Minimal
# Thread Device (MTD) library.

EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libopenthread-mtd
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libtcplp
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libmbedtls
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/platform
