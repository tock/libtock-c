DEPPATH += --dep-path $(LVGL_DIR)
VPATH += :$(LVGL_DIR)
CSRCS += lvgl_driver.c

override CFLAGS += -Wno-error=unused-macros -Wno-error=sign-compare -Wno-error=float-equal\
  -Wno-error=unused-parameter -Wno-error=unused-variable\
  -Wno-error=shadow -Wno-error=type-limits -Wno-error=overflow

include $(LVGL_DIR)/lvgl/lvgl.mk
