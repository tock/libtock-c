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

# Add include paths expected by lwip.
# The first adds the include folder inside the lwip submodule
override CPPFLAGS += -I$(LWIPDIR)/include
# The second adds our local include folder with platform-specifc config info
override CPPFLAGS += -I$($(LIBNAME)_DIR)/include

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
