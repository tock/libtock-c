# Include the core RadioLib headers
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/RadioLib/RadioLib/src

# Include the Tock specific headers
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/RadioLib/RadioLib/examples/NonArduino/Tock
