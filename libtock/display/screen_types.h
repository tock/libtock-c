#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Supported pixel formats.
typedef enum {
  MONO     =0,
  RGB_233  =1,
  RGB_565  =2,
  RGB_888  =3,
  ARGB_8888=4,
} libtock_screen_format_t;

// Supported screen rotations.
typedef enum {
  ROTATION_NORMAL=0,
  ROTATION_90    =1,
  ROTATION_180   =2,
  ROTATION_270   =3,
} libtock_screen_rotation_t;

#ifdef __cplusplus
}
#endif
