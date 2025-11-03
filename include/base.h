#ifndef BASE_H
#define BASE_H

#include <stdio.h>

// == platform detection ==
#if defined(__linux__) || defined(__gnu_linux__)
#    define PLATFORM_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#    define PLATFORM_WIN
#endif

// == compiler detection ==
#if defined(__clang__)
#    define COMPILER_CLANG
#elif defined(_MSC_VER)
#    define COMPILER_CL
#elif defined(__GNUC__)
#    define COMPILER_GCC
#endif

// == arch detection ==
// x86/x64: SSE2
#if defined(__x86_64__) || defined(__SSE2__) || defined(_M_X64) || defined(_M_IX86) || \
    defined(_M_AMD64) || defined(__i386__)
#    define ARCH_X86

// ARM: NEON (AArch64 or ARMv7+NEON)
#elif defined(__aarch64__) || defined(__ARM_NEON) || defined(__ARM_NEON__) || defined(_M_ARM64) || \
    (defined(_M_ARM) && (defined(__ARM_NEON) || defined(__ARM_NEON__)))
#    define ARCH_ARM
#endif

// == simd setup ==
#if defined(ENABLE_SIMD)
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
#elif defined(COMPILER_CLANG) || defined(COMPILER_GCC)
#    define align_16 __attribute__((aligned(16)))
#endif

// == logging macros ==
#if defined(ENABLE_DEBUG)
#    define log_info(...)                                                             \
        do {                                                                          \
            fprintf(stdout, "\033[1;34mINFO\033[0m [%s:%d] => ", __FILE__, __LINE__); \
            fprintf(stdout, __VA_ARGS__);                                             \
            fprintf(stdout, "\n");                                                    \
        } while (0)

#else
#    define log_info(str, ...)
#endif

// == error handling ==
#define log_err(...)                                                               \
    do {                                                                           \
        fprintf(stderr, "\033[1;31mERROR\033[0m [%s:%d] => ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                                              \
        fprintf(stderr, "\n");                                                     \
    } while (0)

// #define nullptr ((void *)0)

#endif
