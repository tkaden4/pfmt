#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<stdarg.h>

#include<unistd.h>
#include<getopt.h>

// TODO options
#define usage() \
    puts("usage: pfmt options...")

#define fatal(fmt, ...) \
    ({ \
        fprintf(stderr, "pfmt: " fmt "\n", ##__VA_ARGS__); \
        exit(1); \
     })

typedef struct {
    union {
        struct {
            uint8_t __pad;
            uint8_t g;
            uint8_t b;
            uint8_t r;
        };
        uint32_t rgb;
    };
} color_t;

typedef struct {
    uint32_t x;
    uint32_t y;
} pos_t;

typedef struct {
    pos_t pos;
    color_t fg;
    color_t bg;
} attr_t;

typedef void(*output_builder_t)(const char *, ...);

enum {
    FG = true,
    BG = false
};

/* prepare for codes */

void start_attrs(output_builder_t out)
{
    out("\x1b[");
}

/* color manipulation */

void color_seq(const color_t *color, int mode, output_builder_t out)
{
    const char *fmt = "48;2;%d;%d;%dm";
    if(mode == FG){
        fmt = "38;2;%d;%d;%dm";
    }
    out(fmt, color->r, color->g, color->b);
}

void color_reset(output_builder_t out)
{
    out("0m");
}

void set_color(const color_t *color, int mode, output_builder_t out)
{
    start_attrs(out);
    color_seq(color, mode, out);
}

/* position manipulation */

void move(const pos_t *to, output_builder_t out)
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
    /* set color attributes */
    /* fg color */
    start_attrs(out);
    color_seq(&attrs->fg, FG, out);
    /* bg color */
    start_attrs(out);
    color_seq(&attrs->bg, BG, out);
    /* move to position */
    start_attrs(out);
    move(&attrs->pos, out);
}

/* print a formatted string with attributes to a FILE * */
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

/* safely parse a number from a string */

int parseul(const char *str, uint32_t *res)
{
    char *end = NULL;
    uint32_t ul = strtoul(str, &end, 10);
    if(*end){
        return 1;
    }
    *res = ul;
    return 0;
}

int main(int argc, char **argv)
{
    // TODO also format stdin
    if(!isatty(0)){
        fatal("input is not a tty");
    }
    /* default attributes */
    attr_t attrs = {
        .fg = { .rgb = 0xffffff },
        .bg = { .rgb = 0x000000 },
        .pos = { .x = 10, .y = 0 }
    };
    /* flags for each attribute */
    struct {
        int clr: 1;
        int pos: 1;
        int fg: 1;
        int bg: 1;
    } flags = {
        0,
        0,
        0,
        0
    };
    /* TODO move to sto* class of conversion functions */
    int opt = EOF;
    while((opt = getopt(argc, argv, "cx:y:f:b:")) != EOF){
        switch(opt){
        case 'c':
            flags.clr = 1;
            break;
        case 'f':
            flags.fg = 1;
            if(parseul(optarg, &attrs.fg.rgb)){
                fatal("invalid foreground color \"%s\"", optarg);
            }
            break;
        case 'b':
            flags.bg = 1;
            if(parseul(optarg, &attrs.bg.rgb)){
                fatal("invalid background color \"%s\"", optarg);
            }
            break;
        case 'y':
            flags.pos = 1;
            if(parseul(optarg, &attrs.pos.y)){
                fatal("invalid y position \"%s\"", optarg);
            }
            break;
        case 'x':
            flags.pos = 1;
            if(parseul(optarg, &attrs.pos.x)){
                fatal("invalid x position \"%s\"", optarg);
            }
            break;
        default:
            usage();
            exit(0);
            break;
        }
    }
    output_builder_t out = (output_builder_t)printf;
    /* clear the screen, if necessary */
    if(flags.clr){
        reset(out);
    }
    /* make sure both parts of position are specified */
    if(flags.pos){
        set_position(&attrs.pos, out);
    }
    /* set foreground color */
    if(flags.fg){
        set_color(&attrs.fg, FG, out);
    }
    /* set background color */
    if(flags.bg){
        set_color(&attrs.bg, BG, out);
    }
    /* print the arguments */
    for(; optind < argc; ++optind){
        puts(argv[optind]);
    }
    /* if we set colors, reset them now */
    if(flags.bg || flags.fg){
        start_attrs(out);
        color_reset(out);
    }
    return 0;
}
