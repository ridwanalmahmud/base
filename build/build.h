#ifndef BUILD_H
#define BUILD_H

#include "nob.h"
#include "../base.h"

// === set compiler ===
// never used windows or msvc, so i dont have a f**king(freaking without 'a') idea
#if defined(COMPILER_CLANG)
#    define CC "clang"
#elif defined(COMPILER_GCC)
#    define CC "gcc"
#elif defined(COMPILER_CL)
#    define CC "cl"
#else
#    error "ONLY clang, gcc and msvc are supported"
#endif

// === set archiver ===
#if defined(PLATFORM_LINUX)
#    define AR "ar"
#elif defined(PLATFORM_WIN)
#    define AR "lib"
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
#        define COMPILE_FLAGS                                     \
            "-O3", "-Os", "-march=native", "-D_FORTIFY_SOURCE=2", \
                "-fstack-protector-strong", "-fPIC"
#        define LINK_FLAGS "-flto=auto", "-Wl,-z,relro", "-pie"
#    endif
#    define LIB_COMPILE_FLAGS
#    define SO_LINK_FLAGS "-shared"
#    define AR_LINK_FLAGS "rcs"

#elif defined(COMPILER_CL)
#    define COMMON_FLAGS "/W4", "/permissive-"
#    if definedBUILD_DEBUG
#        define COMPILE_FLAGS "/Zi", "/Od", "/RTC1", "/MTd"
#        define LINK_FLAGS "/DEBUG"
#    endif
#    if defined(BUILD_RELEASE)
#        define COMPILE_FLAGS "/O2", "/Ob2", "/Ot", "/GL", "/MT"
#        define LINK_FLAGS "/LTCG", "/DYNAMICBASE"
#    endif
#    define SO_LINK_FLAGS "/DLL"
#endif

typedef struct {
    const char **srcs;
    size_t src_count;
    const char **objs; // objs returned by compiling srcs
    size_t obj_count; // to ensure src_count == obj_count
    const char *obj_dir;
    bool is_compiled; // to ensure srcs are compiled only once
} Build_Data;

bool build_exec(Nob_Cmd *cmd, Build_Data *items, const char *exec_name);
bool build_shared(Nob_Cmd *cmd, Build_Data *items, const char *so_name);
bool build_static(Nob_Cmd *cmd, Build_Data *items, const char *ar_name);

// USAGE:
/*
#define NOB_IMPLEMENTATION

#include "build/build.h"

#define BUILD_DIR "build"
#define OBJ_DIR "obj"
#define EXEC "build/main"

// define source files
static const char *srcs[] = {
    "src/main.c",
    "ui/colors.c"
};

Build_Data items = {
    .srcs = srcs,
    .src_count = arr_len(srcs),
    .objs = nullptr,
    .obj_count = 0,
    .obj_dir = OBJ_DIR,
    .is_compiled = false,
};

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};

    // OBJ_DIR is made internally
    // exec dir has to be build separately
    if (!nob_mkdir_if_not_exists(BUILD_DIR))
        return false;

    if (!build_exec(&cmd, &items, EXEC)) {
        nob_log(NOB_ERROR, "Linking failed");
    }

    return 0;
}
*/
// COMPILE:
// clang build/build.c nob.c -o nob

#endif
