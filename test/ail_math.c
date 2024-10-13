#define AIL_MATH_IMPL
#include "../ail_math.h"
#include "./test_assert.h"
#include <stdio.h>

bool test_is2power(void)
{
    ASSERT(!AIL_IS_2POWER(0));
    ASSERT(AIL_IS_2POWER(1));
    ASSERT(AIL_IS_2POWER(2));
    ASSERT(AIL_IS_2POWER(-2));
    ASSERT(AIL_IS_2POWER(-256));
    ASSERT(!AIL_IS_2POWER(3));
    ASSERT(AIL_IS_2POWER(-1));
    ASSERT(!AIL_IS_2POWER(-3));
    ASSERT(!AIL_IS_2POWER(255));
    ASSERT(AIL_IS_2POWER(0x800000));
    ASSERT(AIL_IS_2POWER(-0x800000));
    ASSERT(!AIL_IS_2POWER(0x900000));
    ASSERT(!AIL_IS_2POWER(-0x900000));
    ASSERT(!AIL_IS_2POWER(0x800200));
    ASSERT(!AIL_IS_2POWER(0x2222));
    return true;
}

bool test_next2Power(void)
{
    ASSERT(AIL_NEXT_2POWER(0)     == 1);
    ASSERT(AIL_NEXT_2POWER(1)     == 2);
    ASSERT(AIL_NEXT_2POWER(-1)    == -2);
    ASSERT(AIL_NEXT_2POWER(8)     == 16);
    ASSERT(AIL_NEXT_2POWER(-15)   == -16);
    ASSERT(AIL_NEXT_2POWER(0xf2)  == 0x100);
    ASSERT(AIL_NEXT_2POWER(-2000) == -2048);
    ASSERT(AIL_NEXT_2POWER(0x7BA42ACD251B7129) == 0x8000000000000000);
    return true;
}

#define TESTS          \
    X(test_is2power)   \
    X(test_next2Power)

int main(void)
{
    #define X(func) if (func()) printf("\033[32m" AIL_STRINGIFY(func) " succesful :)\033[0m\n"); else printf("\033[31m" AIL_STRINGIFY(func) " failed :(\033[0m\n");
        TESTS
    #undef X
    return 0;
}
