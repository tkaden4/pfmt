#pragma once

#include<stdint.h>

/* 24-bit color */
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

/* foreground or background */
enum {
    FG,
    BG
};

/* callback for writing output */
typedef void(*output_builder_t)(const char *, ...);

void start_attrs(output_builder_t);

void set_color(const color_t *, int, output_builder_t);
void color_reset(output_builder_t);

void set_position(const pos_t *, output_builder_t);

void reset(output_builder_t);
