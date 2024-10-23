all: rebuild-newlib

ifeq ($(NEWLIB_VERSION),)
  $(error Need to set the NEWLIB_VERSION variable to choose which to build.)
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

newlib-$(NEWLIB_VERSION).tar.gz:
	@echo "Downloading newlib source $(@F)"
	@wget -q -O $@ https://sourceware.mirror.garr.it/newlib/newlib-$(NEWLIB_VERSION).tar.gz

newlib-$(NEWLIB_VERSION): newlib-$(NEWLIB_VERSION).tar.gz
	@echo "Extracting $(<F)"
	@tar -xzf $<
	@touch $@ # Touch so directory appears newer than tarball

rebuild-newlib: newlib-$(NEWLIB_VERSION)
	@echo ""
	@echo "=== BEGINNING ARM BUILD =========================="
	@echo ""
	@mkdir -p newlib-arm-$(NEWLIB_VERSION)-out
	@mkdir -p newlib-arm-$(NEWLIB_VERSION)-install
	@echo "Entering directory newlib-arm-$(NEWLIB_VERSION)-out"
	cd newlib-arm-$(NEWLIB_VERSION)-out; ../build-arm.sh ../$< ../newlib-arm-$(NEWLIB_VERSION)-install | tee build-arm.log
	@echo ""
	@echo "=== BEGINNING RISC-V BUILD ======================="
	@echo ""
	@mkdir -p newlib-riscv-$(NEWLIB_VERSION)-out
	@mkdir -p newlib-riscv-$(NEWLIB_VERSION)-install
	@echo "Entering directory newlib-riscv-$(NEWLIB_VERSION)-out"
	cd newlib-riscv-$(NEWLIB_VERSION)-out; ../build-riscv.sh ../$< ../newlib-riscv-$(NEWLIB_VERSION)-install $(TOOLCHAIN_rv32i) | tee build-riscv.log
	@echo ""
	@echo "=== PACKAGING NEWLIB ARTIFACTS ==================="
	@echo ""
	@mkdir -p libtock-newlib-$(NEWLIB_VERSION)/arm
	@mkdir -p libtock-newlib-$(NEWLIB_VERSION)/riscv
	@cp -r newlib-arm-$(NEWLIB_VERSION)-install/arm-none-eabi libtock-newlib-$(NEWLIB_VERSION)/arm
	@cp -r newlib-riscv-$(NEWLIB_VERSION)-install/$(TOOLCHAIN_rv32i) libtock-newlib-$(NEWLIB_VERSION)/riscv
	@cd libtock-newlib-$(NEWLIB_VERSION)/arm/arm-none-eabi; [ -f ../../../newlib-$(NEWLIB_VERSION).patch ] && patch -p1 < ../../../newlib-$(NEWLIB_VERSION).patch || true
	@cd libtock-newlib-$(NEWLIB_VERSION)/riscv/$(TOOLCHAIN_rv32i); [ -f ../../../newlib-$(NEWLIB_VERSION).patch ] && patch -p1 < ../../../newlib-$(NEWLIB_VERSION).patch || true
	@mv newlib-arm-$(NEWLIB_VERSION)-out/build-arm.log newlib-riscv-$(NEWLIB_VERSION)-out/build-riscv.log libtock-newlib-$(NEWLIB_VERSION)
	@zip -r libtock-newlib-$(NEWLIB_VERSION).zip libtock-newlib-$(NEWLIB_VERSION)
	@echo ""
	@echo "=== FINISHED NEWLIB ZIP =========================="
	@echo ""
	@sha256sum libtock-newlib-$(NEWLIB_VERSION).zip
