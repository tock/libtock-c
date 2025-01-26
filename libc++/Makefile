all: rebuild-gcc

ifeq ($(GCC_VERSION),)
  $(error Need to set the GCC_VERSION variable to choose which to build.)
endif

ifeq ($(NEWLIB_VERSION),)
  $(error Need to set the NEWLIB_VERSION variable to choose which to use.)
endif

# Determine which RISC-V toolchain is installed.
ifneq (,$(shell which riscv64-none-elf-gcc 2>/dev/null))
  TOOLCHAIN_rv32i := riscv64-none-elf
else ifneq (,$(shell which riscv32-none-elf-gcc 2>/dev/null))
  TOOLCHAIN_rv32i := riscv32-none-elf
else ifneq (,$(shell which riscv64-elf-gcc 2>/dev/null))
  TOOLCHAIN_rv32i := riscv64-elf
else ifneq (,$(shell which riscv64-unknown-elf-clang 2>/dev/null))
  TOOLCHAIN_rv32i := riscv64-unknown-elf
else ifneq (,$(shell which riscv32-unknown-elf-clang 2>/dev/null))
  TOOLCHAIN_rv32i := riscv32-unknown-elf
else
  TOOLCHAIN_rv32i := riscv64-unknown-elf
endif

# We assume we are using newlib when creating libc++.
NEWLIB_INCLUDE_PATH ?= ../newlib-$(NEWLIB_VERSION)/newlib/libc/include

gcc-$(GCC_VERSION).tar.xz:
	@echo "Downloading gcc source $(@F)"
	@wget -q -O $@ 'https://mirrors.sarata.com/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.xz'

gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.xz | newlib-$(NEWLIB_VERSION)
	@echo "Extracting $(<F)"
	@tar -xf $<
	@touch $@ # Touch so directory appears newer than tarball

newlib-$(NEWLIB_VERSION).tar.gz:
	@echo "Downloading newlib source $(@F)"
	@wget -q -O $@ https://sourceware.mirror.garr.it/newlib/newlib-$(NEWLIB_VERSION).tar.gz

newlib-$(NEWLIB_VERSION): newlib-$(NEWLIB_VERSION).tar.gz
	@echo "Extracting $(<F)"
	@tar -xzf $<
	@touch $@ # Touch so directory appears newer than tarball

rebuild-gcc: gcc-$(GCC_VERSION)
	@echo ""
	@echo "=== BEGINNING ARM BUILD =========================="
	@echo ""
	@mkdir -p gcc-arm-$(GCC_VERSION)-out
	@mkdir -p gcc-arm-$(GCC_VERSION)-install
	@echo "Entering directory gcc-arm-$(GCC_VERSION)-out"
	cd gcc-arm-$(GCC_VERSION)-out; ../build-arm.sh ../$< ../gcc-arm-$(GCC_VERSION)-install $(NEWLIB_INCLUDE_PATH) | tee build-arm.log
	@echo ""
	@echo "=== BEGINNING RISC-V BUILD ======================="
	@echo ""
	@mkdir -p gcc-riscv-$(GCC_VERSION)-out
	@mkdir -p gcc-riscv-$(GCC_VERSION)-install
	@echo "Entering directory gcc-riscv-$(GCC_VERSION)-out"
	cd gcc-riscv-$(GCC_VERSION)-out; ../build-riscv.sh ../$< ../gcc-riscv-$(GCC_VERSION)-install $(NEWLIB_INCLUDE_PATH) $(TOOLCHAIN_rv32i) | tee build-riscv.log
	@echo ""
	@echo "=== PACKAGING LIBC++ ARTIFACTS ==================="
	@echo ""
	@mkdir -p libtock-libc++-$(GCC_VERSION)/arm
	@cp -r gcc-arm-$(GCC_VERSION)-install/lib libtock-libc++-$(GCC_VERSION)/arm
	@cp -r gcc-arm-$(GCC_VERSION)-install/arm-none-eabi libtock-libc++-$(GCC_VERSION)/arm
	@mkdir -p libtock-libc++-$(GCC_VERSION)/riscv
	@cp -r gcc-riscv-$(GCC_VERSION)-install/lib libtock-libc++-$(GCC_VERSION)/riscv
	@cp -r gcc-riscv-$(GCC_VERSION)-install/$(TOOLCHAIN_rv32i) libtock-libc++-$(GCC_VERSION)/riscv
	@mv gcc-arm-$(GCC_VERSION)-out/build-arm.log gcc-riscv-$(GCC_VERSION)-out/build-riscv.log libtock-libc++-$(GCC_VERSION)
	@zip -r libtock-libc++-$(GCC_VERSION).zip libtock-libc++-$(GCC_VERSION)


clean:
	rm -f gcc-$(GCC_VERSION).tar.xz
	rm -f newlib-$(NEWLIB_VERSION).tar.gz
	rm -rf gcc-$(GCC_VERSION)
	rm -rf gcc-$(GCC_VERSION)-out
	rm -rf gcc-$(GCC_VERSION)-install

.PHONY: clean rebuild-gcc

