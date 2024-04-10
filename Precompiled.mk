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
# Supported architectures in gcc format.
#
# These are used to generate rules for each library in the precompiled
# libraries.
################################################################################

ARM_ARCHS := thumb/v6-m/nofp thumb/v7-m/nofp thumb/v7e-m/nofp
RISCV_ARCHS := rv32i/ilp32 rv32im/ilp32 rv32imac/ilp32

################################################################################
# Newlib Rules
#
# These pre-compiled archives were created using the libtock-c/newlib folder.
################################################################################

# Rule to ensure that the newlib libraries for an architecture exist.
#
# Arguments:
# - $(1): Version
# - $(2): Family
# - $(3): Toolchain
# - $(4): Arch
define PRECOMPILED_NEWLIB_RULES

$$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-$(1)/$(2)/$(3)/lib/$(4)/libc.a: | $$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-$(1)
$$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-$(1)/$(2)/$(3)/lib/$(4)/libm.a: | $$(TOCK_USERLAND_BASE_DIR)/lib/libtock-newlib-$(1)

endef

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

$(foreach arch,$(ARM_ARCHS),$(eval $(call PRECOMPILED_NEWLIB_RULES,4.2.0.20211231,arm,arm-none-eabi,$(arch))))
$(foreach arch,$(RISCV_ARCHS),$(eval $(call PRECOMPILED_NEWLIB_RULES,4.2.0.20211231,riscv,riscv64-unknown-elf,$(arch))))

# newlib 4.3.0.20230120

$(foreach arch,$(ARM_ARCHS),$(eval $(call PRECOMPILED_NEWLIB_RULES,4.3.0.20230120,arm,arm-none-eabi,$(arch))))
$(foreach arch,$(RISCV_ARCHS),$(eval $(call PRECOMPILED_NEWLIB_RULES,4.3.0.20230120,riscv,riscv64-unknown-elf,$(arch))))

################################################################################
# LIBC++ Rules
#
# These pre-compiled archives were created using the libtock-c/libc++ folder.
################################################################################

# Rule to ensure that the libc++ libraries for an architecture exist.
#
# Arguments:
# - $(1): Version
# - $(2): Family
# - $(3): Toolchain
# - $(4): Arch
define PRECOMPILED_CPPLIB_RULES

$$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-$(1)/$(2)/$(3)/lib/$(4)/libstdc++.a: | $$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-$(1)
$$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-$(1)/$(2)/$(3)/lib/$(4)/libsupc++.a: | $$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-$(1)
$$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-$(1)/$(2)/lib/gcc/$(3)/$(1)/$(4)/libgcc.a: | $$(TOCK_USERLAND_BASE_DIR)/lib/libtock-libc++-$(1)

endef

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

$(foreach arch,$(ARM_ARCHS),$(eval $(call PRECOMPILED_CPPLIB_RULES,10.5.0,arm,arm-none-eabi,$(arch))))
$(foreach arch,$(RISCV_ARCHS),$(eval $(call PRECOMPILED_CPPLIB_RULES,10.5.0,riscv,riscv64-unknown-elf,$(arch))))

# LIBC++ 12.3.0

$(foreach arch,$(ARM_ARCHS),$(eval $(call PRECOMPILED_CPPLIB_RULES,12.3.0,arm,arm-none-eabi,$(arch))))
$(foreach arch,$(RISCV_ARCHS),$(eval $(call PRECOMPILED_CPPLIB_RULES,12.3.0,riscv,riscv64-unknown-elf,$(arch))))

# LIBC++ 13.2.0

$(foreach arch,$(ARM_ARCHS),$(eval $(call PRECOMPILED_CPPLIB_RULES,13.2.0,arm,arm-none-eabi,$(arch))))
$(foreach arch,$(RISCV_ARCHS),$(eval $(call PRECOMPILED_CPPLIB_RULES,13.2.0,riscv,riscv64-unknown-elf,$(arch))))


endif
