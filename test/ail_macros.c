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
    if (next2PowerTest()) printf("next2PowerTest successful :)\n");
    else                  printf("next2PowerTest failed     :(\n");
    return 0;
}