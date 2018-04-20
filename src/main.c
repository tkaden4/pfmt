#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<getopt.h>

#include<pfmt/pfmt.h>

// TODO options
#define usage() \
    puts("usage: pfmt options...")

#define fatal(fmt, ...) \
    ({ \
        fprintf(stderr, "pfmt: " fmt "\n", ##__VA_ARGS__); \
        exit(1); \
     })

/* safely parse a number from a string */

int parseul(const char *str, uint32_t *res, int base)
{
    char *end = NULL;
    uint32_t ul = strtoul(str, &end, base);
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
        .fg = { .rgb = 0x00ffffff },
        .bg = { .rgb = 0x00000000 },
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
            if(parseul(optarg, &attrs.fg.rgb, 16)){
                fatal("invalid foreground color \"%s\"", optarg);
            }
            break;
        case 'b':
            flags.bg = 1;
            if(parseul(optarg, &attrs.bg.rgb, 16)){
                fatal("invalid background color \"%s\"", optarg);
            }
            break;
        case 'y':
            flags.pos = 1;
            if(parseul(optarg, &attrs.pos.y, 10)){
                fatal("invalid y position \"%s\"", optarg);
            }
            break;
        case 'x':
            flags.pos = 1;
            if(parseul(optarg, &attrs.pos.x, 10)){
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
        reset_color(out);
    }
    return 0;
}
