#include "build.h"

bool compile(Nob_Cmd *cmd, Build_Data *items, const char *out_dir) {
    if (!nob_mkdir_if_not_exists(out_dir))
        return false;

    const char **objs = malloc(sizeof(const char *) * items->src_count);

    for (size_t i = 0; i < items->src_count; i++) {
        const char *src = items->srcs[i];
        const char *basename = nob_path_name(src);
        const char *obj_name = nob_temp_sprintf("%s.o", basename);
        const char *out_path = nob_temp_sprintf("%s/%s", out_dir, obj_name);

        objs[i] = out_path;

        nob_cmd_append(cmd, CC, COMMON_FLAGS, COMPILE_FLAGS);

        nob_cmd_append(cmd, "-c", src);
        nob_cmd_append(cmd, "-o", out_path);

        if (!nob_cmd_run_sync_and_reset(cmd)) {
            free(objs);
            return false;
        }
    }

    items->objs = objs;
    items->obj_count = items->src_count;
    return true;
}

bool build_exec(Nob_Cmd *cmd, Build_Data *items, const char *exec_name) {
    nob_cmd_append(cmd, CC);

    // nob_cmd_append(cmd, LINK_FLAGS);

    for (size_t i = 0; i < items->obj_count; i++) {
        nob_cmd_append(cmd, items->objs[i]);
    }

    nob_cmd_append(cmd, "-o", exec_name);
    return nob_cmd_run_sync_and_reset(cmd);
}

bool build_shared(Nob_Cmd *cmd, Build_Data *items, const char *so_name) {
    nob_cmd_append(cmd, CC);

    nob_cmd_append(cmd, SO_LINK_FLAGS);

    for (size_t i = 0; i < items->obj_count; i++) {
        nob_cmd_append(cmd, items->objs[i]);
    }

    nob_cmd_append(cmd, "-o", so_name);
    return nob_cmd_run_sync_and_reset(cmd);
}

bool build_static(Nob_Cmd *cmd, Build_Data *items, const char *ar_name) {
    nob_cmd_append(cmd, AR);

    nob_cmd_append(cmd, AR_LINK_FLAGS);
    nob_cmd_append(cmd, ar_name);

    for (size_t i = 0; i < items->obj_count; i++) {
        nob_cmd_append(cmd, items->objs[i]);
    }

    return nob_cmd_run_sync_and_reset(cmd);
}
