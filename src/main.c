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
    if(end && *end){
        return 1;
    }
    *res = ul;
    return 0;
}

/* getopt.h argument flags */
enum {
    NO_ARG = 0,
    REQ_ARG = 1,
    OPT_ARG = 2
};

static struct option long_opts[] = {
    { "clear",      NO_ARG,  NULL, 'c' },
    { "kill-line",  NO_ARG,  NULL, 'k' },
    { "xpos",       REQ_ARG, NULL, 'x' },
    { "ypos",       REQ_ARG, NULL, 'y' },
    { "fg",         REQ_ARG, NULL, 'f' },
    { "bg",         REQ_ARG, NULL, 'b' },
    /* sentinel value */
    { NULL,     0, NULL, 0 }
};

int main(int argc, char **argv)
{
    /* default attributes */
    pfmt_attr_t attrs = {
        .fg = { .rgb = 0x00ffffff },
        .bg = { .rgb = 0x00000000 },
        .pos = { .x = 10, .y = 0 }
    };
    /* flags for each attribute */
    struct {
        int clr: 1;
        int kill: 1;
        int pos: 1;
        int fg: 1;
        int bg: 1;
    } flags = {
        0,
        0,
        0,
        0,
        0
    };

    int opt = EOF;
    while((opt = getopt_long(argc, argv, "kcx:y:f:b:", long_opts, NULL)) != EOF){
        switch(opt){
        case 'c':
            flags.clr = 1;
            break;
        case 'k':
            flags.kill = 1;
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
    pfmt_builder_t out = pfmt_stdout_builder;
    /* clear the screen, if necessary */
    if(flags.clr){
        pfmt_reset(out);
    }
    /* make sure both parts of position are specified */
    if(flags.pos){
        pfmt_set_position(&attrs.pos, out);
    }
    /* kill the line, if necessary */
    if(flags.kill){
        pfmt_clear_line(out);
    }
    /* set foreground color */
    if(flags.fg){
        pfmt_set_color(&attrs.fg, FG, out);
    }
    /* set background color */
    if(flags.bg){
        pfmt_set_color(&attrs.bg, BG, out);
    }
    /* print the arguments */
    if(optind < argc){
        for(; optind < argc; ++optind){
            puts(argv[optind]);
        }
    }else{
        size_t page_size = getpagesize();
        char buf[page_size];
        setbuf(stdin, NULL);
        setbuf(stdout, NULL);
        while(fgets(buf, page_size, stdin)){
            fputs(buf, stdout);
        }
    }
    /* if we set colors, reset them now */
    if(flags.bg || flags.fg){
        pfmt_reset_color(out);
    }
    return 0;
}
