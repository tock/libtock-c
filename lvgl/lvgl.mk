
override CFLAGS += -Wno-error=unused-macros -Wno-error=sign-compare -Wno-error=float-equal -Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=shadow

include $(LVGL_DIR)/lvgl/lvgl.mk
