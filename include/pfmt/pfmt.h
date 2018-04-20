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
} color_t;

/* screen position */
typedef struct {
    uint32_t x;
    uint32_t y;
} pos_t;

/* screen attributes */
typedef struct {
    pos_t pos;
    color_t fg;
    color_t bg;
} attr_t;

/* color mode selectors */
enum {
    FG,
    BG
};

/* callback for writing output */
typedef void(*output_builder_t)(const char *, ...);

/* print a formatted string with attributes */
void fpprintf(FILE *file, const attr_t *attrs, const char *fmt, ...);
/* change all the output attributes at once */
void set_attrs(const attr_t *attrs, output_builder_t out);
/* set the color */
void set_color(const color_t *, int, output_builder_t);
/* revert to terminal colors */
void reset_color(output_builder_t);
/* set the cursor position */
void set_position(const pos_t *, output_builder_t);
/* reset the terminal to its initial state */
void reset(output_builder_t);
/* clear the screen */
void clear(output_builder_t out);
