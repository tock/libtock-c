# Include the core RadioLib headers
override CPPFLAGS += -isystem $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/src

# Include the Tock specific headers
override CPPFLAGS += -isystem $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock
