u8g2 - Monochrome Graphics for Tock
===================================

This library adds support for [u8g2](https://github.com/olikraus/u8g2) in Tock.

The library uses the screen driver to write the screen in the kernel.

Using u8g2 in libtock-c
-----------------------

To use this library in an app, include this line in the app Makefile:

```make
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/u8g2
```

A basic example of an app using u8g2 looks like:

```c
#include <u8g2-tock.h>
#include <u8g2.h>

u8g2_t u8g2;

int main(void) {
  u8g2_tock_init(&u8g2);

  // Call u8g2 functions.

  return 0;
}
```


Compile the library manually
----------------------------

When building an app that uses u8g2 this library will automatically be compiled.

To compile manually (perhaps for testing), run `make` in this folder.

```
cd libtock-c/u8g2
make
```
