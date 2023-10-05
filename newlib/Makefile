NEWLIB_VERSION ?= 4.3.0.20230120

all: rebuild-newlib

newlib-$(NEWLIB_VERSION).tar.gz:
	@echo "Downloading newlib source $(@F)"
	@wget -q -O $@ ftp://sourceware.org/pub/newlib/newlib-$(NEWLIB_VERSION).tar.gz

newlib-$(NEWLIB_VERSION): newlib-$(NEWLIB_VERSION).tar.gz
	@echo "Extracting $(<F)"
	@tar -xzf $<
	@touch $@ # Touch so directory appears newer than tarball

rebuild-newlib: newlib-$(NEWLIB_VERSION)
	@echo ""
	@echo "=== BEGINNING ARM BUILD =========================="
	@echo ""
	@rm -rf newlib-arm-$(NEWLIB_VERSION)-out
	@mkdir -p newlib-arm-$(NEWLIB_VERSION)-out
	@echo "Entering directory newlib-arm-$(NEWLIB_VERSION)-out"
	cd newlib-arm-$(NEWLIB_VERSION)-out; sh ../build-arm.sh ../$<
	@echo ""
	@echo "=== BEGINNING RISC-V BUILD ======================="
	@echo ""
	@rm -rf newlib-riscv-$(NEWLIB_VERSION)-out
	@mkdir -p newlib-riscv-$(NEWLIB_VERSION)-out
	@echo "Entering directory newlib-riscv-$(NEWLIB_VERSION)-out"
	cd newlib-riscv-$(NEWLIB_VERSION)-out; sh ../build-riscv.sh ../$<
	@echo ""
	@echo "=== PACKAGING NEWLIB ARTIFACTS ==================="
	@echo ""
	@mkdir -p libtock-newlib-$(NEWLIB_VERSION)/thumb
	@mkdir -p libtock-newlib-$(NEWLIB_VERSION)/riscv
	@cp -r newlib-$(NEWLIB_VERSION)/newlib/libc/include libtock-newlib-$(NEWLIB_VERSION)/headers
	@rsync -ram --include='libm.a' --include='libc.a' --include='*/' --exclude='*' newlib-arm-$(NEWLIB_VERSION)-out/arm-none-eabi/thumb/ libtock-newlib-$(NEWLIB_VERSION)/thumb/
	@rsync -ram --exclude='/*/*/*/*/' --include='libm.a' --include='libc.a' --include='*/' --exclude='*' newlib-riscv-$(NEWLIB_VERSION)-out/riscv64-elf/ libtock-newlib-$(NEWLIB_VERSION)/riscv/
	@patch -p1 < newlib-$(NEWLIB_VERSION).patch
	@zip -r libtock-newlib-$(NEWLIB_VERSION).zip libtock-newlib-$(NEWLIB_VERSION)
