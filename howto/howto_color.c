#include "ui.h"

int main(void) {
    // define {r, g, b};
    Colors fg = {170, 170, 170};
    Colors bg = COLOR_RED;

    // define {fg, bg, bold, italic, underline}
    Style style = {fg, bg, false, false, true};

    // print styled text
    char *content = "Hello! gracious\n";
    styled_text(style, content);

    return 0;
}
