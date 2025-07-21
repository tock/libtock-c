#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define __DEFER__(F, V) \
  auto void F(int* V ## SENTINEL); \
  [[gnu::cleanup(F)]] int V; \
  auto void F(__attribute__ ((unused)) int* V ## SENTINEL)

#define defer __DEFER(__COUNTER__)
#define __DEFER(N) __DEFER_(N)
#define __DEFER_(N) __DEFER__(__DEFER_FUNCTION_ ## N, __DEFER_VARIABLE_ ## N)

#ifdef __cplusplus
}
#endif
