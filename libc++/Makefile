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


gcc-arm-$(GCC_VERSION)-install/lib/libcc1.la: gcc-$(GCC_VERSION)
	@echo ""
	@echo "=== BEGINNING ARM BUILD =========================="
	@echo ""
	@mkdir -p gcc-arm-$(GCC_VERSION)-out
	@mkdir -p gcc-arm-$(GCC_VERSION)-install
	@echo "Entering directory gcc-arm-$(GCC_VERSION)-out"
	cd gcc-arm-$(GCC_VERSION)-out; ../build-arm.sh ../$< ../gcc-arm-$(GCC_VERSION)-install $(NEWLIB_INCLUDE_PATH) | tee build-arm.log

gcc-riscv-$(GCC_VERSION)-medlow-install/lib/libcc1.la: gcc-$(GCC_VERSION)
	@echo ""
	@echo "=== BEGINNING RISC-V MEDLOW BUILD ======================="
	@echo ""
	@mkdir -p gcc-riscv-$(GCC_VERSION)-medlow-out
	@mkdir -p gcc-riscv-$(GCC_VERSION)-medlow-install
	@echo "Entering directory gcc-riscv-$(GCC_VERSION)-medlow-out"
	cd gcc-riscv-$(GCC_VERSION)-medlow-out; ../build-riscv.sh ../$< ../gcc-riscv-$(GCC_VERSION)-medlow-install $(NEWLIB_INCLUDE_PATH) $(TOOLCHAIN_rv32i) medlow | tee build-riscv-medlow.log

gcc-riscv-$(GCC_VERSION)-medany-install/lib/libcc1.la: gcc-$(GCC_VERSION)
	@echo ""
	@echo "=== BEGINNING RISC-V MEDANY BUILD ======================="
	@echo ""
	@mkdir -p gcc-riscv-$(GCC_VERSION)-medany-out
	@mkdir -p gcc-riscv-$(GCC_VERSION)-medany-install
	@echo "Entering directory gcc-riscv-$(GCC_VERSION)-medany-out"
	cd gcc-riscv-$(GCC_VERSION)-medany-out; ../build-riscv.sh ../$< ../gcc-riscv-$(GCC_VERSION)-medany-install $(NEWLIB_INCLUDE_PATH) $(TOOLCHAIN_rv32i) medany | tee build-riscv-medany.log

# Helper targets to run this from the docker script incrementally
.PHONY: arm riscv-medlow riscv-medany
arm: gcc-arm-$(GCC_VERSION)-install/lib/libcc1.la
riscv-medlow: gcc-riscv-$(GCC_VERSION)-medlow-install/lib/libcc1.la
riscv-medany: gcc-riscv-$(GCC_VERSION)-medany-install/lib/libcc1.la

rebuild-gcc: gcc-arm-$(GCC_VERSION)-install/lib/libcc1.la gcc-riscv-$(GCC_VERSION)-medlow-install/lib/libcc1.la gcc-riscv-$(GCC_VERSION)-medany-install/lib/libcc1.la
	@echo ""
	@echo "=== PACKAGING LIBC++ ARTIFACTS ==================="
	@echo ""
	# Copy the arm built output into our libtock-libc++ folder.
	@mkdir -p libtock-libc++-$(GCC_VERSION)/arm
	@cp -r gcc-arm-$(GCC_VERSION)-install/lib           libtock-libc++-$(GCC_VERSION)/arm
	@cp -r gcc-arm-$(GCC_VERSION)-install/arm-none-eabi libtock-libc++-$(GCC_VERSION)/arm
	# Create space for the riscv output in our libtock-libc++ folder.
	@mkdir -p libtock-libc++-$(GCC_VERSION)/riscv
	# Copy the medlow built output into our riscv libtock-libc++ folder. This is the base.
	@cp -r gcc-riscv-$(GCC_VERSION)-medlow-install/lib                libtock-libc++-$(GCC_VERSION)/riscv
	@cp -r gcc-riscv-$(GCC_VERSION)-medlow-install/$(TOOLCHAIN_rv32i) libtock-libc++-$(GCC_VERSION)/riscv
	# Remove the 64-bit medlow. We won't use it.
	@rm -rf libtock-libc++-$(GCC_VERSION)/riscv/lib/gcc/$(TOOLCHAIN_rv32i)/$(GCC_VERSION)/rv64*
	@rm -rf libtock-libc++-$(GCC_VERSION)/riscv/$(TOOLCHAIN_rv32i)/lib/rv64*
	# Copy the 64-bit medany libraries into our folder.
	@cp -r gcc-riscv-$(GCC_VERSION)-medany-install/lib/gcc/$(TOOLCHAIN_rv32i)/$(GCC_VERSION)/rv64* libtock-libc++-$(GCC_VERSION)/riscv/lib/gcc/$(TOOLCHAIN_rv32i)/$(GCC_VERSION)/
	@cp -r gcc-riscv-$(GCC_VERSION)-medany-install/$(TOOLCHAIN_rv32i)/lib/rv64*                    libtock-libc++-$(GCC_VERSION)/riscv/$(TOOLCHAIN_rv32i)/lib/
	# Save the build logs in case anyone ever cares.
	@cp gcc-arm-$(GCC_VERSION)-out/build-arm.log gcc-riscv-$(GCC_VERSION)-medlow-out/build-riscv-medlow.log gcc-riscv-$(GCC_VERSION)-medany-out/build-riscv-medany.log libtock-libc++-$(GCC_VERSION)
	# Package the output into a .zip file.
	@zip -r libtock-libc++-$(GCC_VERSION).zip libtock-libc++-$(GCC_VERSION)


clean:
	rm -f gcc-$(GCC_VERSION).tar.xz
	rm -f newlib-$(NEWLIB_VERSION).tar.gz
	rm -rf gcc-$(GCC_VERSION)
	rm -rf gcc-$(GCC_VERSION)-out
	rm -rf gcc-$(GCC_VERSION)-install

.PHONY: clean rebuild-gcc

