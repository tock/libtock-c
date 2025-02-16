# Base library definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lwip

# This Makefile's directory, library is actually located in the `lwip`
# submodule under this directory
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# Include the filelist makefile of lwip. The filelist requires a variable named
# `LWIPDIR` to be in scope, which points to the `src` directory inside the git
# submodule. This is not to be confused with `$(LIBNAME)_DIR` (expands to
# `lwip_DIR`), which is the top-level Tock directory name:
LWIPDIR := $($(LIBNAME)_DIR)/$(LIBNAME)/src
include $($(LIBNAME)_DIR)/lwip/src/Filelists.mk

# Include the filelist's C and assembly library files in the Tock
# library sources. These are properly prefixed with the `src` path
# through `$(LWIPDIR)`:
$(LIBNAME)_SRCS := $(LWIPNOAPPSFILES) $(LWIPAPPFILES)

# Hack: we don't want to modify the LWIP submodule, but LWIP expects
# us to place certain headers containing platform definitions
# there. Add these as additional headers in the compiler flags here:
override CPPFLAGS += -I$(lwip_DIR)/include -I$(lwip_DIR)/include/lwip

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
