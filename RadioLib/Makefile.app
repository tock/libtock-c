# Include the core RadioLib headers
override CPPFLAGS += -isystem$(TOCK_USERLAND_BASE_DIR)/RadioLib/RadioLib/src

# Include the Tock specific headers
override CPPFLAGS += -isystem$(TOCK_USERLAND_BASE_DIR)/RadioLib

# Exceptions
#override CPPFLAGS_$(LIBNAME) += -fno-rtti
#override CPPFLAGS_$(LIBNAME) += -fno-exceptions

# Regions
#override CPPFLAGS_$(LIBNAME) += -DRADIOLIB_LORAWAN_REGION_US915_ENABLE
