#include<pfmt/pfmt.h>

#include<stdio.h>

int main()
{
    pfmt_color_t fg = { .r = 0xff, .g = 0x00, .b = 0xff };
    pfmt_set_color(&fg, FG, pfmt_stdout_builder);
    puts("Im Purple!");
    pfmt_reset_color(pfmt_stdout_builder);
    return 0;
}
