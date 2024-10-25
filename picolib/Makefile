all: rebuild-picolib

ifeq ($(PICOLIB_VERSION),)
  $(error Need to set the PICOLIB_VERSION variable to choose which to build.)
endif

picolib-$(PICOLIB_VERSION).tar.xz:
	@echo "Downloading picolib source $(@F)"
	@wget -q -O $@ https://github.com/picolibc/picolibc/releases/download/$(PICOLIB_VERSION)/picolibc-$(PICOLIB_VERSION).tar.xz

picolibc-$(PICOLIB_VERSION): picolib-$(PICOLIB_VERSION).tar.xz
	@echo "Extracting $(<F)"
	@tar -xzf $<
	@touch $@ # Touch so directory appears newer than tarball

rebuild-picolib: picolibc-$(PICOLIB_VERSION)
	@rm -rf picolib-arm-$(PICOLIB_VERSION)-out
	@rm -rf picolib-arm-$(PICOLIB_VERSION)-install
	@rm -rf picolib-riscv-$(PICOLIB_VERSION)-out
	@rm -rf picolib-riscv-$(PICOLIB_VERSION)-install
	@rm -rf libtock-picolib-$(PICOLIB_VERSION).zip
	@rm -rf libtock-picolib-$(PICOLIB_VERSION)
	@echo ""
	@echo "=== BEGINNING ARM BUILD =========================="
	@echo ""
	@mkdir -p picolib-arm-$(PICOLIB_VERSION)-out
	@mkdir -p picolib-arm-$(PICOLIB_VERSION)-install
	@echo "Entering directory picolib-arm-$(PICOLIB_VERSION)-out"
	cd picolib-arm-$(PICOLIB_VERSION)-out; ../build-arm.sh ../$< ../picolib-arm-$(PICOLIB_VERSION)-install | tee build-arm.log
	@echo ""
	@echo "=== BEGINNING RISC-V BUILD ======================="
	@echo ""
	@mkdir -p picolib-riscv-$(PICOLIB_VERSION)-out
	@mkdir -p picolib-riscv-$(PICOLIB_VERSION)-install
	@echo "Entering directory picolib-riscv-$(PICOLIB_VERSION)-out"
	cd picolib-riscv-$(PICOLIB_VERSION)-out; ../build-riscv.sh ../$< ../picolib-riscv-$(PICOLIB_VERSION)-install | tee build-riscv.log
	@echo ""
	@echo "=== PACKAGING PICOLIB ARTIFACTS ==================="
	@echo ""
	@mkdir -p libtock-picolib-$(PICOLIB_VERSION)/arm
	@mkdir -p libtock-picolib-$(PICOLIB_VERSION)/riscv
	@cp -r picolib-arm-$(PICOLIB_VERSION)-install/picolibc/arm-none-eabi libtock-picolib-$(PICOLIB_VERSION)/arm
	@cp -r picolib-riscv-$(PICOLIB_VERSION)-install/picolibc/riscv64-unknown-elf libtock-picolib-$(PICOLIB_VERSION)/riscv
	@mv picolib-arm-$(PICOLIB_VERSION)-out/build-arm.log picolib-riscv-$(PICOLIB_VERSION)-out/build-riscv.log libtock-picolib-$(PICOLIB_VERSION)
	@zip -r libtock-picolib-$(PICOLIB_VERSION).zip libtock-picolib-$(PICOLIB_VERSION)
	@echo ""
	@echo "=== FINISHED PICOLIB ZIP =========================="
	@echo ""
	@sha256sum libtock-picolib-$(PICOLIB_VERSION).zip
