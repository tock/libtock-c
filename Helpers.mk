################################################################################
##
## libtock-c helper functions and definitions for use by Tock makefiles.
##  Included by AppMakefile.mk and libtock's Makefile
##
################################################################################

# Ensure that this file is only included once.
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

# Check that the variable has no leading or trailing spaces, or spaces within
# the variable.
#
# Params:
#   1. Variable name to test.
check_no_spaces = \
    $(if $(findstring $(strip $($(strip $1))),$($(strip $1))),,$(error Error: Space in variable $(strip $1))) \
    $(if $(word 2,$($(strip $1))),$(error Error: Multiple entries in variable $(strip $1)),)

# Check for a ~/ at the beginning of a path variable (TOCK_USERLAND_BASE_DIR).
# Make will not properly expand this.
ifdef TOCK_USERLAND_BASE_DIR
    ifneq (,$(findstring BEGINNINGOFVARIABLE~/,BEGINNINGOFVARIABLE$(TOCK_USERLAND_BASE_DIR)))
        $(error Hi! Using "~" in Makefile variables is not supported. Use "$$(HOME)" instead)
    endif
endif


# Format check rule
.PHONY: _format_check_unstaged
_format_check_unstaged:
	$(Q)$(TOCK_USERLAND_BASE_DIR)/tools/check_unstaged.sh

#########################################################################################
## Pretty-printing rules

# If environment variable V is non-empty, be verbose.
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
TRACE_E2T =
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
TRACE_E2T = @echo " E2T       " $@
endif

endif
