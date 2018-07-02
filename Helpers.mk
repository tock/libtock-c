# Various helper functions and definitions for use by Tock makefiles. Included
# by AppMakefile.mk and libtock's Makefile

# ensure that this file is only included once
ifndef HELPERS_MAKEFILE
HELPERS_MAKEFILE = 1

# http://stackoverflow.com/questions/10858261/abort-makefile-if-variable-not-set
# Check that given variables are set and all have non-empty values,
# die with an error otherwise.
#
# Params:
#   1. Variable name(s) to test.
#   2. (optional) Error message to print.
check_defined = \
    $(strip $(foreach 1,$1, \
        $(call __check_defined,$1,$(strip $(value 2)))))
__check_defined = \
    $(if $(value $1),, \
      $(error Undefined $1$(if $2, ($2))))

# Check for a ~/ at the beginning of a path variable (TOCK_USERLAND_BASE_DIR).
# Make will not properly expand this.
ifdef TOCK_USERLAND_BASE_DIR
    ifneq (,$(findstring BEGINNINGOFVARIABLE~/,BEGINNINGOFVARIABLE$(TOCK_USERLAND_BASE_DIR)))
        $(error Hi! Using "~" in Makefile variables is not supported. Use "$$(HOME)" instead)
    endif
endif



# Here we define some specialized rules for each compiler
define HELPERS_RULES

ifeq ($$(CC_$(1)),arm-none-eabi-gcc)

# Validate the the toolchain is new enough (known not to work for gcc <= 5.1)
CC_VERSION_MAJOR_$(1) := $$(shell $$(CC_$(1)) -dumpversion | cut -d '.' -f1)
ifeq (1,$$(shell expr $$(CC_VERSION_MAJOR_$(1)) \>= 6))
  # Opportunistically turn on gcc 6.0+ warnings since we're already version checking:
  override CPPFLAGS_$(1) += -Wduplicated-cond #  if (p->q != NULL) { ... } else if (p->q != NULL) { ... }
  override CPPFLAGS_$(1) += -Wnull-dereference # deref of NULL (thought default if -fdelete-null-pointer-checks, in -Os, but no?)
else
  ifneq (5,$$(CC_VERSION_MAJOR_$(1)))
    $$(info CC=$$(CC_$(1)))
    $$(info $$$$(CC_$(1)) -dumpversion: $$(shell $$(CC_$(1)) -dumpversion))
    $$(error Your compiler is too old. Need gcc version > 5.1)
  endif
    CC_VERSION_MINOR_$(1) := $$(shell $$(CC_$(1)) -dumpversion | cut -d '.' -f2)
  ifneq (1,$$(shell expr $$(CC_VERSION_MINOR_$(1)) \> 1))
    $$(info CC=$$(CC_$(1)))
    $$(info $$$$(CC_$(1)) -dumpversion: $$(shell $$(CC_$(1)) -dumpversion))
    $$(error Your compiler is too old. Need gcc version > 5.1)
  endif
endif

endif

endef

$(foreach arch,$(TOCK_ARCHS),$(eval $(call HELPERS_RULES,$(arch))))




# Format check rule
.PHONY: _format_check_unstaged
_format_check_unstaged:
	$(Q)$(TOCK_USERLAND_BASE_DIR)/tools/check_unstaged.sh

#########################################################################################
## Pretty-printing rules

# If environment variable V is non-empty, be verbose
ifneq ($(V),)
Q=
TRACE_DIR =
TRACE_BIN =
TRACE_DEP =
TRACE_CC  =
TRACE_CXX =
TRACE_LD  =
TRACE_AR  =
TRACE_AS  =
TRACE_LST =
ELF2TAB_ARGS += -v
else
Q=@
TRACE_DIR = @echo " DIR       " $@
TRACE_BIN = @echo " BIN       " $@
TRACE_DEP = @echo " DEP       " $<
TRACE_CC  = @echo "  CC       " $<
TRACE_CXX = @echo " CXX       " $<
TRACE_LD  = @echo "  LD       " $@
TRACE_AR  = @echo "  AR       " $@
TRACE_AS  = @echo "  AS       " $<
TRACE_LST = @echo " LST       " $<
endif

endif

