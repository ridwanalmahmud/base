#define NOB_IMPLEMENTATION

#include "nob.h"
#include "../base.h"

// === set compiler ===
#if defined(COMPILER_CLANG)
#    define CC "clang"
#elif defined(COMPILER_GCC)
#    define CC "gcc"
#elif defined(COMPILER_CL)
#    define CC "cl"
#else
#    error "ONLY clang, gcc and msvc is supported"
#endif

// === build types ===
#if !defined(BUILD_DEBUG) && !defined(BUILD_RELEASE)
#    define BUILD_DEBUG
#endif
#if defined(BUILD_DEBUG) && defined(BUILD_RELEASE)
#    error "Cannot enable both DEBUG && OPT"
#endif

// === compiler and build type specific flags ===
#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
#    define COMMON_FLAGS "-Wall", "-Wextra", "-pedantic"
#    if defined(BUILD_DEBUG)
#        define COMPILE_FLAGS "-g3", "-fno-omit-frame-pointer"
#        define LINK_FLAGS "-fsanitize=address,undefined,leak"
#    endif
#    if defined(BUILD_RELEASE)
#        define COMPILE_FLAGS       \
            "-O3",                 \
             "-Os",                 \
             "-march=native",       \
             "-D_FORTIFY_SOURCE=2", \
             "-fstack-protector-strong"
#        define LINK_FLAGS "-flto=auto", "-Wl,-z,relro"
#    endif
#    if defined(BUILD_LIB)
#        define LIB_COMPILE_FLAGS "-fPIE", "-fPIC"
#        define LIB_LINK_FLAGS "-pie"
#    endif

#elif defined(COMPILER_CL)
#    define COMMON_FLAGS "/W4", "/permissive-"
#    if definedBUILD_DEBUG
#        define COMPILE_FLAGS "/Zi", "/Od", "/RTC1", "/MTd"
#        define LINK_FLAGS "/DEBUG"
#    endif
#    if definedBUILD_RELEASE
#        define COMPILE_FLAGS "/O2", "/Ob2", "/Ot", "/GL", "/MT"
#        define LINK_FLAGS "/LTCG"
#    endif
#    if definedBUILD_LIB
#        define LIB_COMPILE_FLAGS "/LD"
#        define LIB_LINK_FLAGS "/DLL"
#    endif
#endif
