#pragma once

#include<stdint.h>

/* 24-bit color struct */
typedef struct {
    union {
        struct {
            uint8_t b;
            uint8_t g;
            uint8_t r;
        };
        uint32_t rgb;
    };
} pfmt_color_t;

/* screen position */
typedef struct {
    uint32_t x;
    uint32_t y;
} pfmt_pos_t;

/* screen attributes */
typedef struct {
    pfmt_pos_t pos;
    pfmt_color_t fg;
    pfmt_color_t bg;
} pfmt_attr_t;

/* color mode selectors */
enum {
    FG,
    BG
};

/* callback for writing output */
typedef void(*pfmt_builder_t)(const char *, ...);

#define pfmt_stdout_builder ((pfmt_builder_t)printf)

/* print a formatted string with attributes */
void fpprintf(FILE *, const pfmt_attr_t *, const char *, ...);
/* change all the output attributes at once */
void pfmt_set_attrs(const pfmt_attr_t *, pfmt_builder_t);
/* set the color */
void pfmt_set_color(const pfmt_color_t *, int, pfmt_builder_t);
/* revert to terminal colors */
void pfmt_reset_color(pfmt_builder_t);
/* set the cursor position */
void pfmt_set_position(const pfmt_pos_t *, pfmt_builder_t);
/* reset the terminal to its initial state */
void pfmt_reset(pfmt_builder_t);
/* clear the screen */
void pfmt_clear(pfmt_builder_t);
