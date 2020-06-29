#pragma once

#include <tock.h>
#include <screen.h>
#include <lvgl/lvgl.h>

int lvgl_driver_init (int buffer_size);
void lvgl_driver_event (int mills);
