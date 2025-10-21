#ifndef BASE_H
#define BASE_H

#include <stdio.h>

// == utility macros ==
#define arr_len(a) (sizeof(a) / sizeof(a[0]))

// nullptr type
#define nullptr ((void *)0)

// math utilities
#define min(a, b) ((a) < (b) ? a : b)
#define max(a, b) ((a) > (b) ? a : b)
#define is_odd(a) ((a) & 1)
#define is_even(a) ((a) & 0)
#define sign(a) ((a) >= 0.0 ? 1 : -1)

// == compiler detection ==
#if defined(__clang__)
#    define COMPILER_CLANG
#elif defined(_MSC_VER)
#    define COMPILER_CL
#elif defined(__GNUC__)
#    define COMPILER_GCC
#endif

// == platform detection ==
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#    define PLATFORM_WIN
#elif defined(__linux__) || defined(__gnu_linux__)
#    define PLATFORM_LINUX
#endif

// == arch detection ==
// x86/x64: SSE2
#if defined(__x86_64__) || defined(__SSE2__) || defined(_M_X64) || \
    defined(_M_IX86) || defined(_M_AMD64) || defined(__i386__)
#    define ARCH_X86

// ARM: NEON (AArch64 or ARMv7+NEON)
#elif defined(__aarch64__) || defined(__ARM_NEON) || defined(__ARM_NEON__) || \
    defined(_M_ARM64) ||                                                      \
    (defined(_M_ARM) && (defined(__ARM_NEON) || defined(__ARM_NEON__)))
#    define ARCH_ARM
#endif

// == simd setup ==
#if defined(DEFINE_SIMD)
#    if defined(ARCH_X86)
#        define DEFINE_SIMD__SSE
#        include <emmintrin.h>
#    elif defined(ARCH_ARM)
#        define DEFINE_SIMD__NEON
#        include <arm_neon.h>
#    endif
#endif

// 16 bit memory alignment
#if defined(COMPILER_CL)
#    define align_16 __declspec(aligned(16))
#else
#    define align_16 __attribute__((aligned(16)))
#endif

// == logging macros ==
#if defined(DEFINE_DEBUG)
#    define log_info(str, ...)                     \
        do {                                  \
            fprintf(stdout,                   \
                    "INFO [%s:%d] " str "\n", \
                    __FILE__,                 \
                    __LINE__,                 \
                    ##__VA_ARGS__);           \
        } while (0);

#else
#    define log_info(str, ...)
#endif

// == error handling ==
#if defined(DEFINE_ERR_HANDLING)
#    define log_err(str, ...)                    \
        do {                                 \
            fprintf(stderr,                  \
                    "ERR [%s:%d] " str "\n", \
                    __FILE__,                \
                    __LINE__,                \
                    ##__VA_ARGS__);          \
        } while (0);

#else
#    define log_err(str, ...)
#endif

// == base types ==
// not much useful tbh
#if defined(DEFINE_BASE_TYPES)
// unsigned int types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// signed int types
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// floating point types
typedef float f32;
typedef double f64;

// boolean types
typedef u8 b8;
typedef u32 b32;
#endif

#endif
