################################################################################
##
## libtock-c build system rules for pre-compiled libraries.
##
## This includes rules to require that pre-compiled libraries be present in the
## `/lib` folder.
##
## Supported pre-compiled libraries:
##
## - newlib
## - libc++
##
################################################################################

# Ensure that this file is only included once.
ifndef PRECOMPILED_MAKEFILE
PRECOMPILED_MAKEFILE = 1

################################################################################
# Newlib Rules
#
# These pre-compiled archives were created using the libtock-c/newlib folder.
################################################################################

# Target to download and extract newlib.
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231:
	cd $(TOCK_USERLAND_BASE_DIR)/lib; ./fetch-newlib.sh 4.2.0.20211231

# Target to download and extract newlib.
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120:
	cd $(TOCK_USERLAND_BASE_DIR)/lib; ./fetch-newlib.sh 4.3.0.20230120

# Helper rule to specify newlib as a dependency in the libtock-c build system.
newlib-4.2.0.20211231: | $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231

newlib-4.3.0.20230120: | $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120

# newlib 4.2.0.20211231

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.2.0.20211231

# newlib 4.3.0.20230120

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libm.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-4.3.0.20230120


################################################################################
# LIBC++ Rules
#
# These pre-compiled archives were created using the libtock-c/libc++ folder.
################################################################################

# Target to download and extract the C++ libraries.
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0:
	cd $(TOCK_USERLAND_BASE_DIR)/lib; ./fetch-libc++.sh 10.5.0

# Target to download and extract the C++ libraries.
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0:
	cd $(TOCK_USERLAND_BASE_DIR)/lib; ./fetch-libc++.sh 12.3.0

# Target to download and extract the C++ libraries.
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0:
	cd $(TOCK_USERLAND_BASE_DIR)/lib; ./fetch-libc++.sh 13.2.0

# Helper rule to specify libc++ as a dependency in the libtock-c build system.
libc++-10.5.0: | $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
libc++-12.3.0: | $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
libc++-13.2.0: | $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0

# LIBC++ 10.5.0

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/lib/gcc/arm-none-eabi/10.5.0/thumb/v6-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/lib/gcc/arm-none-eabi/10.5.0/thumb/v7-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/arm/lib/gcc/arm-none-eabi/10.5.0/thumb/v7e-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/lib/gcc/riscv64-unknown-elf/10.5.0/rv32i/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/lib/gcc/riscv64-unknown-elf/10.5.0/rv32im/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0/riscv/lib/gcc/riscv64-unknown-elf/10.5.0/rv32imac/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-10.5.0

# LIBC++ 12.3.0

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/lib/gcc/arm-none-eabi/12.3.0/thumb/v6-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/lib/gcc/arm-none-eabi/12.3.0/thumb/v7-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/arm/lib/gcc/arm-none-eabi/12.3.0/thumb/v7e-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/lib/gcc/riscv64-unknown-elf/12.3.0/rv32i/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/lib/gcc/riscv64-unknown-elf/12.3.0/rv32im/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0/riscv/lib/gcc/riscv64-unknown-elf/12.3.0/rv32imac/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-12.3.0

# LIBC++ 13.2.0

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/arm-none-eabi/lib/thumb/v6-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/lib/gcc/arm-none-eabi/13.2.0/thumb/v6-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/arm-none-eabi/lib/thumb/v7-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/lib/gcc/arm-none-eabi/13.2.0/thumb/v7-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/arm-none-eabi/lib/thumb/v7e-m/nofp/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/arm/lib/gcc/arm-none-eabi/13.2.0/thumb/v7e-m/nofp/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0

$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/riscv64-unknown-elf/lib/rv32i/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/lib/gcc/riscv64-unknown-elf/13.2.0/rv32i/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/lib/gcc/riscv64-unknown-elf/13.2.0/rv32im/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libstdc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/riscv64-unknown-elf/lib/rv32imac/ilp32/libsupc++.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0
$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0/riscv/lib/gcc/riscv64-unknown-elf/13.2.0/rv32imac/ilp32/libgcc.a: $(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-13.2.0


endif
