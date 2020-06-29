lvGL 6.1.2 for Tock OS
===================================

LittlevGL is a free and open-source graphics library providing everything you need to create embedded GUI with easy-to-use graphical elements, beautiful visual effects and low memory footprint.

See https://docs.littlevgl.com/en/html/index.html for documentation.

Using `lvgl` in Tock
--------------------

To use `lvgl`, add the following include to the application's
Makefile:

    EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/lvgl



Re-compiling `lvgl`
-----------------

Checkout the lvgl submodule in this directory

    $ git submodule init -- lvgl
    $ git submodule update

then run `make`

    $ cd lvgl
    $ make

Configure lvGL
--------------
Edit the ``lv_conf.h`` in this folder to set up some parameters and optionals
