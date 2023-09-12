NEWLIB_VERSION ?= 4.3.0.20230120

all: rebuild-newlib

newlib-$(NEWLIB_VERSION).tar.gz:
	@echo "Downloading newlib source $(@F)"
	@wget -q -O $@ ftp://sourceware.org/pub/newlib/newlib-$(NEWLIB_VERSION).tar.gz

newlib-$(NEWLIB_VERSION): newlib-$(NEWLIB_VERSION).tar.gz
	@echo "Extracting $(<F)"
	@tar -xzf $<
	@touch $@ # Touch so directory appears newer than tarball

newlib-headers: newlib-$(NEWLIB_VERSION)
	@rm -rf include
	@cp -r $</newlib/libc/include include

rebuild-newlib: newlib-$(NEWLIB_VERSION) newlib-headers
	@echo ""
	@echo "=== BEGINNING ARM BUILD =========================="
	@echo ""
	@rm -rf newlib-arm-$(NEWLIB_VERSION)-out
	@mkdir -p newlib-arm-$(NEWLIB_VERSION)-out
	@echo "Entering directory newlib-arm-$(NEWLIB_VERSION)-out"
	cd newlib-arm-$(NEWLIB_VERSION)-out; sh ../build-arm.sh ../$<
	cd newlib-arm-$(NEWLIB_VERSION)-out/arm-none-eabi/thumb/v6-m/nofp/newlib; cp libc.a libm.a ../../../../../../cortex-m/v6-m
	cd newlib-arm-$(NEWLIB_VERSION)-out/arm-none-eabi/thumb/v7-m/nofp/newlib; cp libc.a libm.a ../../../../../../cortex-m/v7-m
	cd newlib-arm-$(NEWLIB_VERSION)-out/arm-none-eabi/thumb/v7e-m/nofp/newlib; cp libc.a libm.a ../../../../../../cortex-m/v7e-m
	@echo ""
	@echo "=== BEGINNING RISC-V BUILD =========================="
	@echo ""
	@rm -rf newlib-riscv-$(NEWLIB_VERSION)-out
	@mkdir -p newlib-riscv-$(NEWLIB_VERSION)-out
	@echo "Entering directory newlib-riscv-$(NEWLIB_VERSION)-out"
	cd newlib-riscv-$(NEWLIB_VERSION)-out; sh ../build-riscv.sh ../$<
	cd newlib-riscv-$(NEWLIB_VERSION)-out/riscv64-elf/rv32i/ilp32/newlib; cp libc.a libm.a ../../../../../rv32/rv32i/
	cd newlib-riscv-$(NEWLIB_VERSION)-out/riscv64-elf/rv32im/ilp32/newlib; cp libc.a libm.a ../../../../../rv32/rv32im/
	cd newlib-riscv-$(NEWLIB_VERSION)-out/riscv64-elf/rv32imac/ilp32/newlib; cp libc.a libm.a ../../../../../rv32/rv32imac/