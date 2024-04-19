#define AIL_TYPES_IMPL
#include "../ail.h"
#include <stdbool.h>
#include <stdio.h>

bool next2PowerTest(void)
{
    u64 x        = 0x7BA42ACD251B7129;
    u64 expected = 0x8000000000000000;
    u64 out;
    AIL_NEXT_2POWER(x, out);
    return out == expected;
}

int main(void)
{
    if (next2PowerTest()) printf("\033[32mnext2PowerTest successful :)\033[0m\n");
    else                  printf("\033[31mnext2PowerTest failed     :(\033[0m\n");
    return 0;
}
