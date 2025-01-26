################################################################################
##
## Makefile for loading applications onto a Tockloader compatible board.
##
################################################################################

$(call check_defined, BUILDDIR)
$(call check_defined, PACKAGE_NAME)

TOCKLOADER ?= tockloader

# Upload programs over UART with tockloader.
ifdef PORT
  TOCKLOADER_GENERAL_FLAGS += --port $(PORT)
endif

.PHONY: program
program: $(BUILDDIR)/$(PACKAGE_NAME).tab
	$(TOCKLOADER) $(TOCKLOADER_GENERAL_FLAGS) install $<

.PHONY: flash
flash: $(BUILDDIR)/$(PACKAGE_NAME).tab
	$(TOCKLOADER) install $<

.PHONY: install
install: $(BUILDDIR)/$(PACKAGE_NAME).tab
	$(TOCKLOADER) install $<
