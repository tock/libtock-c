GCC_VERSION ?= 12.2.0

all: rebuild-gcc

gcc-$(GCC_VERSION).tar.xz:
	@echo "Downloading gcc source $(@F)"
	@wget -q -O $@ 'https://bigsearcher.com/mirrors/gcc/releases/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.xz'

gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.xz
	@echo "Extracting $(<F)"
	@tar -xf $<
	@touch $@ # Touch so directory appears newer than tarball

rebuild-gcc: gcc-$(GCC_VERSION)
	@echo ""
	@echo "=== BEGINNING ARM BUILD =========================="
	@echo ""
	@mkdir -p gcc-arm-$(GCC_VERSION)-out
	@echo "Entering directory gcc-arm-$(GCC_VERSION)-out"
	mkdir -p cortex-m/v6-m
	mkdir -p cortex-m/v7-m
	mkdir -p cortex-m/v7e-m
	cd gcc-arm-$(GCC_VERSION)-out; sh ../build-arm.sh ../$<
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v6-m/nofp/libgcc/libgcc.a ./cortex-m/v6-m/
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v6-m/nofp/libstdc++-v3/src/.libs/libstdc++.a ./cortex-m/v6-m/
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v6-m/nofp/libstdc++-v3/libsupc++/.libs/libsupc++.a ./cortex-m/v6-m/
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v7-m/nofp/libgcc/libgcc.a ./cortex-m/v7-m/
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v7-m/nofp/libstdc++-v3/src/.libs/libstdc++.a ./cortex-m/v7-m/
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v7-m/nofp/libstdc++-v3/libsupc++/.libs/libsupc++.a ./cortex-m/v7-m/
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v7e-m/nofp/libgcc/libgcc.a ./cortex-m/v7e-m/
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v7e-m/nofp/libstdc++-v3/src/.libs/libstdc++.a ./cortex-m/v7e-m/
	cp gcc-arm-$(GCC_VERSION)-out/arm-none-eabi/thumb/v7e-m/nofp/libstdc++-v3/libsupc++/.libs/libsupc++.a ./cortex-m/v7e-m/
	@echo ""
	@echo "=== BEGINNING RISC-V BUILD =========================="
	@echo ""
	@mkdir -p gcc-rv32i-$(GCC_VERSION)-out
	@mkdir -p rv32i
	@mkdir -p rv32im
	@mkdir -p rv32imac
	@echo "Entering directory gcc-rv32i-$(GCC_VERSION)-out"
	cd gcc-rv32i-$(GCC_VERSION)-out; sh ../build-riscv.sh ../$<
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32i/ilp32/libgcc/libgcc.a ./rv32i/
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32i/ilp32/libstdc++-v3/src/.libs/libstdc++.a ./rv32i/
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32i/ilp32/libstdc++-v3/libsupc++/.libs/libsupc++.a ./rv32i/
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32im/ilp32/libgcc/libgcc.a ./rv32im/
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32im/ilp32/libstdc++-v3/src/.libs/libstdc++.a ./rv32im/
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32im/ilp32/libstdc++-v3/libsupc++/.libs/libsupc++.a ./rv32im/
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32imac/ilp32/libgcc/libgcc.a ./rv32imac/
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32imac/ilp32/libstdc++-v3/src/.libs/libstdc++.a ./rv32imac/
	cp gcc-rv32i-$(GCC_VERSION)-out/riscv64-elf/rv32imac/ilp32/libstdc++-v3/libsupc++/.libs/libsupc++.a ./rv32imac/

clean:
	rm -f gcc-$(GCC_VERSION).tar.xz
	rm -rf gcc-$(GCC_VERSION)
	rm -rf gcc-$(GCC_VERSION)-out

.PHONY: clean rebuild-gcc

