# Base definitions
TOCK_USERLAND_BASE_DIR ?= ../../..
LIBNAME := platform-tock

CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/include
CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/src/core
CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

# Careful! Must be a path that resolves correctly **from where make is invoked**
#
# If you are only ever compiling a standalone library, then it's fine to simply set
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/ot-tock/platform/platform-tock
#
# If you will be asking applications to rebuild this library (see the development
# section below), then you'll need to ensure that this directory is still correct
# when invoked from inside the application folder.
#
# Tock accomplishes this for in-tree libraries by having all makefiles
# conditionally set the TOCK_USERLAND_BASE_DIR variable, so that there
# is a common relative path everywhere.
#$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# Grab all relevant source files. You can list them directly:
#$(LIBNAME)_SRCS :=                                      \
#    $($LIBNAME)_DIR)\libexample.c                       \
#    $($LIBNAME)_DIR)\libexample_helper.c                \
#    $($LIBNAME)_DIR)\subfolders_are_fine\otherfile.c

# Or let make find them automatically:
$(LIBNAME)_SRCS  :=                                     \
    $(wildcard $($(LIBNAME)_DIR)/*.c)                   \
    $(wildcard $($(LIBNAME)_DIR)/*.cxx)                 \ # or .cpp or .cc
    $(wildcard $($(LIBNAME)_DIR)/*.s)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
