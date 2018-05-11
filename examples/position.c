#include<pfmt/pfmt.h>

#include<stdio.h>

int main()
{
    pfmt_pos_t pos = { .x = 10, .y = 20 };
    pfmt_reset(pfmt_stdout_builder);
    pfmt_set_position(&pos, pfmt_stdout_builder);
    puts("Im at x = 10, y = 20!");
    return 0;
}
