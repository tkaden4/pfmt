#include<stdio.h>
#include<stdarg.h>

#include<pfmt/pfmt.h>

/* prepare for codes */

static inline void start_attrs(pfmt_builder_t out)
{
    out("\x1b[");
}

/* color manipulation */

static void color_seq(const pfmt_color_t *color, int mode, pfmt_builder_t out)
{
    const char *fmt = "48;2;%d;%d;%dm";
    if(mode == FG){
        fmt = "38;2;%d;%d;%dm";
    }
    out(fmt, color->r, color->g, color->b);
}

static void color_reset(pfmt_builder_t out)
{
    out("0m");
}

void pfmt_set_color(const pfmt_color_t *color, int mode, pfmt_builder_t out)
{
    start_attrs(out);
    color_seq(color, mode, out);
}

void pfmt_reset_color(pfmt_builder_t out)
{
    start_attrs(out);
    color_reset(out);
}

/* position manipulation */

static void move(const pfmt_pos_t *to, pfmt_builder_t out)
{
    out("%lu;%luH", to->y + 1, to->x + 1);
}

void pfmt_set_position(const pfmt_pos_t *to, pfmt_builder_t out)
{
    start_attrs(out);
    move(to, out);
}

/* misc. utilities */

void pfmt_clear(pfmt_builder_t out)
{
    start_attrs(out);
    out("2J");
}

void pfmt_reset(pfmt_builder_t out)
{
    pfmt_reset_color(out);
    pfmt_set_position(&(pfmt_pos_t){ 0, 0 }, out);
    pfmt_clear(out);
}

void pfmt_set_attrs(const pfmt_attr_t *attrs, pfmt_builder_t out)
{
    /* fg color */
    pfmt_set_color(&attrs->fg, FG, out);
    /* bg color */
    pfmt_set_color(&attrs->bg, BG, out);
    /* move to position */
    pfmt_set_position(&attrs->pos, out);
}

void fpprintf(FILE *file, const pfmt_attr_t *attrs, const char *fmt, ...)
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
    pfmt_set_attrs(attrs, out);
    vfprintf(file, fmt, args);

    va_end(args);
}
