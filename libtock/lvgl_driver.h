#ifdef LVGL

#pragma once

#include "tock.h"
#include "framebuffer.h"
#include "lv_conf.h"
#include <lvgl/lvgl.h>

int lvgl_driver_init (int buffer_size);
void lvgl_driver_event (int mills);

#else

#error "Please enable lvgl by defining LVGL"

#endif
