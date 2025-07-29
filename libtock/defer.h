#pragma once

// Implementation of the C `defer {}` feature.
//
// As of July 2025, upcoming versions of C will support `defer` as a way to run
// code when a value goes out of scope. See:
// https://thephd.dev/_vendor/future_cxx/technical%20specification/C%20-%20defer/C%20-%20defer%20Technical%20Specification.pdf
//
// This implements the same feature with a macro. This implementation used from
// https://gustedt.wordpress.com/2025/01/06/simple-defer-ready-to-use/.

#ifdef __cplusplus
extern "C" {
#endif

#define __DEFER__(F, V)                \
        auto void F(int* V##SENTINEL); \
        [[gnu::cleanup(F)]] int V;     \
        auto void F(__attribute__ ((unused)) int* V##SENTINEL)

#define defer __DEFER(__COUNTER__)
#define __DEFER(N) __DEFER_(N)
#define __DEFER_(N) __DEFER__(__DEFER_FUNCTION_##N, __DEFER_VARIABLE_##N)

#ifdef __cplusplus
}
#endif
