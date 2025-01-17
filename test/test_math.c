#include "../src/base/ail_base_math.h"
#include "../src/math/ail_math_all.h"
#include "./assert.h"
#include <stdio.h>

bool test_is2power(void)
{
    ASSERT(!ail_is_2power(0));
    ASSERT(ail_is_2power(1));
    ASSERT(ail_is_2power(2));
    ASSERT(ail_is_2power(-2));
    ASSERT(ail_is_2power(-256));
    ASSERT(!ail_is_2power(3));
    ASSERT(ail_is_2power(-1));
    ASSERT(!ail_is_2power(-3));
    ASSERT(!ail_is_2power(255));
    ASSERT(ail_is_2power(0x800000));
    ASSERT(ail_is_2power(-0x800000));
    ASSERT(!ail_is_2power(0x900000));
    ASSERT(!ail_is_2power(-0x900000));
    ASSERT(!ail_is_2power(0x800200));
    ASSERT(!ail_is_2power(0x2222));
    return true;
}

bool test_next2Power(void)
{
    // ASSERT(AIL_NEXT_2POWER(0)     == 1);
    // ASSERT(AIL_NEXT_2POWER(1)     == 2);
    // ASSERT(AIL_NEXT_2POWER(-1)    == -2);
    // ASSERT(AIL_NEXT_2POWER(8)     == 16);
    // ASSERT(AIL_NEXT_2POWER(-15)   == -16);
    // ASSERT(AIL_NEXT_2POWER(0xf2)  == 0x100);
    // ASSERT(AIL_NEXT_2POWER(-2000) == -2048);
    // ASSERT(AIL_NEXT_2POWER(0x7BA42ACD251B7129) == 0x8000000000000000);
    return true;
}

bool test_vectors(void)
{
    i16 x = -13, y = 6;
    unsigned long z = 3;
    AIL_Vec2f64 v1 = ail_vec2f64(3, 4.14);
    AIL_Vec2i16 v2 = ail_vec2(x, y);
    AIL_Vec3u32 v3 = ail_vec3(22u, 0xff23532d, x);
    AIL_Vec2u64 v4 = ail_vec2(z, 7ull);
    AIL_Vec2u64 v5 = ail_vec2u64(z, 9);
    AIL_UNUSED(v1);
    AIL_UNUSED(v2);
    AIL_UNUSED(v3);
    ASSERT(ail_vec2_eq(ail_vec2_add(v4, v5), ail_vec2_add(v5, v4)));
    ASSERT(ail_vec2_eq(ail_vec2_add(v4, v5), ail_vec2(2*z, 7+9)));
    return true;
}

#define TESTS          \
    X(test_is2power)   \
    X(test_next2Power) \
    X(test_vectors)

int main(void)
{
    #define X(func) if (func()) printf("\033[32m" AIL_STRINGIFY(func) " succesful :)\033[0m\n"); else printf("\033[31m" AIL_STRINGIFY(func) " failed :(\033[0m\n");
        TESTS
    #undef X
    return 0;
}
