NEWLIB_VERSION ?= 4.1.0

all: rebuild-newlib

newlib-$(NEWLIB_VERSION).tar.gz:
	@echo "Downloading newlib source $(@F)"
	@wget -q -O $@ ftp://sourceware.org/pub/newlib/newlib-$(NEWLIB_VERSION).tar.gz

newlib-$(NEWLIB_VERSION): newlib-$(NEWLIB_VERSION).tar.gz
	@echo "Extracting $(<F)"
	@tar -xzf $<
	@touch $@ # Touch so directory appears newer than tarball

rebuild-newlib: newlib-$(NEWLIB_VERSION)
	@rm -rf newlib-$(NEWLIB_VERSION)-out
	@mkdir -p newlib-$(NEWLIB_VERSION)-out
	@echo "Entering directory newlib-$(NEWLIB_VERSION)-out"
	cd newlib-$(NEWLIB_VERSION)-out; sh ../build.sh ../$<
	cd newlib-$(NEWLIB_VERSION)-out; sh ../build-riscv.sh ../$<
