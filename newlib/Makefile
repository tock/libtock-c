NEWLIB_VERSION ?= 2.5.0.20170421

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


riscv-newlib-3.1.0.tar.gz:
	@echo "Downloading riscv-newlib source $(@F)"
	@wget -q -O $@ https://github.com/riscv/riscv-newlib/archive/riscv-newlib-3.1.0.tar.gz

riscv-newlib-3.1.0: riscv-newlib-3.1.0.tar.gz
	@echo "Extracting $(<F)"
	@tar -xzf $<
	@mv riscv-newlib-riscv-newlib-3.1.0 $@
	@touch $@ # Touch so directory appears newer than tarball

riscv-rebuild-newlib: riscv-newlib-3.1.0
	@rm -rf riscv-newlib-3.1.0-out
	@mkdir -p riscv-newlib-3.1.0-out
	@echo "Entering directory riscv-newlib-3.1.0-out"
	cd riscv-newlib-3.1.0-out; sh ../build-riscv.sh ../$<

