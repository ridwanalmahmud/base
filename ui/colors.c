#include "colors.h"
#include <stdio.h>

static inline void color_reset(void) {
    printf("\033[0m");
}

static inline void fg_color(Colors color) {
    printf("\033[38;2;%d;%d;%dm", color.r, color.g, color.b);
}

static inline void bg_color(Colors color) {
    printf("\033[48;2;%d;%d;%dm", color.r, color.g, color.b);
}

void apply_styling(Style style) {
    color_reset();

    fg_color(style.fg);
    if (style.bg.r != 0 || style.bg.g != 0 || style.bg.b != 0) {
        bg_color(style.bg);
    }

    if (style.bold)
        printf("\033[1m");
    if (style.italic)
        printf("\033[3m");
    if (style.underline)
        printf("\033[4m");
}

void styled_text(Style style, const char *text) {
    apply_styling(style);
    printf("%s", text);
    color_reset();
}
