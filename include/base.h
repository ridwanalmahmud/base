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

// == attribute macros ==
// visibility macros
#define use extern __attribute__((visibility("default")))
#define pub __attribute__((visibility("default")))
#define local __attribute__((visibility("hidden")))
#define internal __attribute__((visibility("internal")))
#define locked __attribute__((visibility("protected")))

#define constfunc __attribute__((const)) // no side-effects
#define purefunc __attribute__((pure))   // no side-effects, no global state
#define unused __attribute__((unused))
#define deprecated __attribute__((deprecated))
#define weak __attribute__((weak))
#define noreturn __attribute__((noreturn)) void
#define alwaysinline __attribute__((always_inline))
#define noinline __attribute__((noinline))

// lifecycle
#define oninit __attribute__((constructor))
#define onexit __attribute__((destructor))
#define cleanup(func) __attribute__((cleanup(func)))
#define section(name) __attribute__((section(name)))

// branch optimizations
#define hot __attribute__((hot))
#define cold __attribute__((cold))
#define likely(cond) __builtin_expect(!!(cond), 1)
#define unlikely(cond) __builtin_expect(!!(cond), 0)
#define assume(cond)                 \
    do {                             \
        if (!(cond))                 \
            __builtin_unreachable(); \
    } while (0)

// memory macros
#define packed __attribute__((packed))
#define align(x) __attribute__((aligned(x)))
#define nil ((void *)0)

// prefetch
#define prefetch(addr) __builtin_prefetch(addr)

// == number types ==
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

#endif
