LIBNAME := RadioLib

TOCK_USERLAND_BASE_DIR ?= ../

$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/libradio/$(LIBNAME)

# We ideally want this:
#     $(LIBNAME)_SRCS  :=                                     \
#     $($(LIBNAME)_DIR)/src/Module.cpp                    \
#     $(wildcard $($(LIBNAME)_DIR)/src/*.cpp)             \
#     $(wildcard $($(LIBNAME)_DIR)/src/*/*.cpp)           \
#     $(wildcard $($(LIBNAME)_DIR)/src/*/*/*.cpp)
#
# But due to this in the GCC arm binary in the CI:
#
# ```
# (insn 179 178 46 8 (set (reg:SI 3 r3 [168])
#         (mem/u/c:SI (plus:SI (reg:SI 12 ip [169])
#                 (unspec:SI [
#                         (symbol_ref/u:SI ("*.LC1") [flags 0x2])
#                     ] UNSPEC_PIC_SYM)) [0  S4 A32])) "../../../RadioLib/src/modules/LR11x0/LR11x0.cpp":638:16 929 {*thumb1_movsi_insn}
#      (expr_list:REG_EQUAL (symbol_ref/u:SI ("*.LC1") [flags 0x2])
#         (nil)))
# during RTL pass: postreload
# ../../../RadioLib/src/modules/LR11x0/LR11x0.cpp:650:1: internal compiler error: in extract_constrain_insn, at recog.c:2195
# ```
#
# we need to drop LR11x0
$(LIBNAME)_SRCS  :=                                     \
    $(wildcard $($(LIBNAME)_DIR)/src/*.cpp)             \
    $(wildcard $($(LIBNAME)_DIR)/src/utils/*.cpp)       \
    $(wildcard $($(LIBNAME)_DIR)/src/modules/C*/.cpp)   \
    $(wildcard $($(LIBNAME)_DIR)/src/modules/LL*/*.cpp) \
    $(wildcard $($(LIBNAME)_DIR)/src/modules/RF*/*.cpp) \
    $(wildcard $($(LIBNAME)_DIR)/src/modules/S*/*.cpp)   \
    $(wildcard $($(LIBNAME)_DIR)/src/modules/n*/*.cpp)   \
    $(wildcard $($(LIBNAME)_DIR)/src/protocols/*/*.cpp) \

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
