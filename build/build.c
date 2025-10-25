#include "build.h"

// only available in current file as the functions call this internally
static bool compile(Nob_Cmd *cmd, Build_Data *items, bool is_lib) {
    if (!cmd || !items || !items->obj_dir) {
        log_err("Uninitialized parameters");
    } else if (!items->srcs || items->src_count == 0) {
        log_err("No available src files");
    }

    if (items->is_compiled) {
        nob_log(NOB_INFO, "Already compiled, reusing...");
        return true;
    }

    if (!nob_mkdir_if_not_exists(items->obj_dir)) {
        return false;
    }

    const char **objs = calloc(items->src_count, sizeof(const char *));

    // compile wach src files to its corresponding obj files
    for (size_t i = 0; i < items->src_count; i++) {
        const char *src = items->srcs[i];
        const char *basename =
            nob_path_name(src); // get the basename for src files
        const char *obj_name =
            nob_temp_sprintf("%s.o", basename); // add .o extention
        const char *out_path = nob_temp_sprintf(
            "%s/%s", items->obj_dir, obj_name); // append to items->obj_dir

        objs[i] = out_path;

        nob_cmd_append(cmd, CC, COMMON_FLAGS, COMPILE_FLAGS);
        if (is_lib) {
            nob_cmd_append(cmd, "-fPIC");
        }

        // compilation
        nob_cmd_append(cmd, "-c", src);
        nob_cmd_append(cmd, "-o", out_path);

        if (!nob_cmd_run_async_and_reset(cmd)) {
            free(objs);
            return false;
        }

        items->obj_count++;
    }

    if (items->obj_count != items->src_count) {
        nob_log(NOB_ERROR, "Some src files were not compiled");
    }

    items->objs = objs;
    items->is_compiled = true;
    return true;
}

bool build_exec(Nob_Cmd *cmd, Build_Data *items, const char *exec_name) {
    if (!cmd || !items || !exec_name) {
        log_err("Uninitialized parameters");
    }

    if (!compile(cmd, items, false)) {
        nob_log(NOB_ERROR, "Compilation failed");
    }

    nob_cmd_append(cmd, CC);

    // nob_cmd_append(cmd, LINK_FLAGS);

    for (size_t i = 0; i < items->obj_count; i++) {
        nob_cmd_append(cmd, items->objs[i]);
    }

    nob_cmd_append(cmd, "-o", exec_name);
    return nob_cmd_run_async_and_reset(cmd);
}

bool build_shared(Nob_Cmd *cmd, Build_Data *items, const char *so_name) {
    if (!cmd || !items || !so_name) {
        log_err("Uninitialized parameters");
    }

    if (!compile(cmd, items, true)) {
        nob_log(NOB_ERROR, "Compilation failed");
    }

    nob_cmd_append(cmd, CC);

    nob_cmd_append(cmd, SO_LINK_FLAGS);

    for (size_t i = 0; i < items->obj_count; i++) {
        nob_cmd_append(cmd, items->objs[i]);
    }

    nob_cmd_append(cmd, "-o", so_name);
    return nob_cmd_run_async_and_reset(cmd);
}

bool build_static(Nob_Cmd *cmd, Build_Data *items, const char *ar_name) {
    if (!cmd || !items || !ar_name) {
        log_err("Uninitialized parameters");
    }

    if (!compile(cmd, items, true)) {
        nob_log(NOB_ERROR, "Compilation failed");
    }

    nob_cmd_append(cmd, AR);

    nob_cmd_append(cmd, AR_LINK_FLAGS);
    nob_cmd_append(cmd, ar_name);

    for (size_t i = 0; i < items->obj_count; i++) {
        nob_cmd_append(cmd, items->objs[i]);
    }

    return nob_cmd_run_async_and_reset(cmd);
}
