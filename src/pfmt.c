#include<stdio.h>
#include<stdarg.h>

#include<pfmt/pfmt.h>

/* prepare for codes */

static inline void start_attrs(output_builder_t out)
{
    out("\x1b[");
}

/* color manipulation */

static void color_seq(const color_t *color, int mode, output_builder_t out)
{
    const char *fmt = "48;2;%d;%d;%dm";
    if(mode == FG){
        fmt = "38;2;%d;%d;%dm";
    }
    out(fmt, color->r, color->g, color->b);
}

static void color_reset(output_builder_t out)
{
    out("0m");
}

void set_color(const color_t *color, int mode, output_builder_t out)
{
    start_attrs(out);
    color_seq(color, mode, out);
}

void reset_color(output_builder_t out)
{
    start_attrs(out);
    color_reset(out);
}

/* position manipulation */

static void move(const pos_t *to, output_builder_t out)
{
    out("%lu;%luH", to->y + 1, to->x + 1);
}

void set_position(const pos_t *to, output_builder_t out)
{
    start_attrs(out);
    move(to, out);
}

/* misc. utilities */

void clear(output_builder_t out)
{
    start_attrs(out);
    out("2J");
}

void reset(output_builder_t out)
{
    start_attrs(out);
    color_reset(out);
    start_attrs(out);
    move(&(pos_t){ 0, 0 }, out);
    clear(out);
}

void set_attrs(const attr_t *attrs, output_builder_t out)
{
    /* fg color */
    set_color(&attrs->fg, FG, out);
    /* bg color */
    set_color(&attrs->bg, BG, out);
    /* move to position */
    set_position(&attrs->pos, out);
}

void fpprintf(FILE *file, const attr_t *attrs, const char *fmt, ...)
{
    /* the printer for the attributes */
    void out(const char *str, ...)
    {
        va_list args;
        va_start(args, str);
        vfprintf(file, str, args);
        va_end(args);
    }
    va_list args;
    va_start(args, fmt);

    /* print attributes and formatted text */
    set_attrs(attrs, out);
    vfprintf(file, fmt, args);

    va_end(args);
}
