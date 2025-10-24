#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t r, g, b;
} Colors;

typedef struct {
    Colors fg;
    Colors bg;
    bool bold;
    bool italic;
    bool underline;
} Style;

static const Colors COLOR_BLACK = {0, 0, 0};
static const Colors COLOR_WHITE = {255, 255, 255};
static const Colors COLOR_RED = {255, 0, 0};
static const Colors COLOR_GREEN = {0, 255, 0};
static const Colors COLOR_BLUE = {0, 0, 255};

void apply_styling(Style style);
void styled_text(Style style, const char *text);

#endif
