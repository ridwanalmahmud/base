#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <stdbool.h>

// Defines rgb value for foreground or background
typedef struct {
    uint8_t r, g, b;
} Colors;

// Defined core styling
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

// Initializes styling
void apply_styling(Style style);
// Use predefined styling for printing styled text
void styled_text(Style style, const char *text);

#endif
