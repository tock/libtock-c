TOCK_USERLAND_BASE_DIR = ../../..

# The probes need Thumb-2, so no Cortex-M0.
TOCK_TARGETS     = cortex-m4 $(QEMU_RV32_VIRT_TOCK_TARGETS)
C_SRCS          := $(wildcard *.c)
STACK_SIZE       = 2048
APP_HEAP_SIZE    = 8192
KERNEL_HEAP_SIZE = 2048

include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
