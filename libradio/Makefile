LIBNAME := RadioLib

$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/libradio/$(LIBNAME)

$(LIBNAME)_SRCS  :=                                     \
    $(wildcard $($(LIBNAME)_DIR)/src/*.cpp)             \
    $(wildcard $($(LIBNAME)_DIR)/src/*/*.cpp)           \
    $(wildcard $($(LIBNAME)_DIR)/src/*/*/*.cpp)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
