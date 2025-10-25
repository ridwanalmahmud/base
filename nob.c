#define NOB_IMPLEMENTATION

#include "build/build.h"

#define BUILD_DIR "obj"
#if defined(PLATFORM_LINUX)
#    define SO_NAME "libbase.so"
#    define AR_NAME "libbase.a"
#elif defined(PLATFORM_WIN)
#    define SO_NAME "/Felibbase.dll"
#    define AR_NAME "/OUT:libbase.lib"
#else
#    error "Only linux and windows are supported"
#endif

static const char *srcs[] = {
    "ui/colors.c",
    "build/build.c",
    "mem/pool.c",
};

Build_Data items = {
    .srcs = srcs,
    .src_count = arr_len(srcs),
    .objs = nullptr,
    .obj_count = 0,
};

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!nob_mkdir_if_not_exists(BUILD_DIR))
        return 1;

    Nob_Cmd cmd = {0};

    if (!compile(&cmd, &items, BUILD_DIR)) {
        nob_log(NOB_ERROR, "Compilation failed");
    }

    if (!build_shared(&cmd, &items, SO_NAME)) {
        nob_log(NOB_ERROR, "Failed building shared object");
    }

    if (!build_static(&cmd, &items, AR_NAME)) {
        nob_log(NOB_ERROR, "Failed building static lib");
    }

    return 0;
}
