#include "../../src/base/ail_base.h"
#include "../../src/base/ail_base_math.h"
#include "../test.h"

bool test_minmax(void)
{
    u16 a = 420;
    f32 b = 203.781;
    i64 c = 69;
    b32 d = true;
    i8  e = 0;
    i32 f = -17;
    TEST_ASSERT(ail_max(a, b) == a);
    TEST_ASSERT(ail_min(a, b) == b);
    TEST_ASSERT(ail_max(a, c) == a);
    TEST_ASSERT(ail_min(a, c) == c);
    TEST_ASSERT(ail_max(a, d) == a);
    TEST_ASSERT(ail_min(a, d) == d);
    TEST_ASSERT(ail_max(a, e) == a);
    TEST_ASSERT(ail_min(a, e) == e);
    TEST_ASSERT(ail_max(a, f) == a);
    TEST_ASSERT(ail_min(a, f) == f);
    TEST_ASSERT(ail_max(b, c) == b);
    TEST_ASSERT(ail_min(b, c) == c);
    TEST_ASSERT(ail_max(b, d) == b);
    TEST_ASSERT(ail_min(b, d) == d);
    TEST_ASSERT(ail_max(b, e) == b);
    TEST_ASSERT(ail_min(b, e) == e);
    TEST_ASSERT(ail_max(b, f) == b);
    TEST_ASSERT(ail_min(b, f) == f);
    TEST_ASSERT(ail_max(c, d) == c);
    TEST_ASSERT(ail_min(c, d) == d);
    TEST_ASSERT(ail_max(c, e) == c);
    TEST_ASSERT(ail_min(c, e) == e);
    TEST_ASSERT(ail_max(c, f) == c);
    TEST_ASSERT(ail_min(c, f) == f);
    TEST_ASSERT(ail_max((i8)d, e)  == (i32)d);
    TEST_ASSERT(ail_min((i8)d, e)  == e);
    TEST_ASSERT(ail_max((i32)d, f) == (i32)d);
    TEST_ASSERT(ail_min((i32)d, f) == f);
    TEST_ASSERT(ail_max(e, f) == e);
    TEST_ASSERT(ail_min(e, f) == f);
    return true;
}

bool test_clamp(void)
{
    TEST_ASSERT(ail_clamp(3, 0, 6) == 3);
    TEST_ASSERT(ail_clamp(-3, 0, 6) == 0);
    TEST_ASSERT(ail_clamp(9, 0, 6) == 6);
    TEST_ASSERT(ail_clamp(3, -1, 6) == 3);
    TEST_ASSERT(ail_clamp(UINT64_MAX, INT64_MIN, INT8_MIN) == INT8_MIN);
    TEST_ASSERT(ail_clamp(6.9f, 69, 420) == 69);
    TEST_ASSERT(ail_clamp(6.9f, 0, 69) == 6.9f);
    TEST_ASSERT(ail_clamp(-6.9f, -7.8f, -6.99f) == -6.99f);
    TEST_ASSERT(ail_clamp(6.9f, -1e+10, -0.0f) == -0.0f);
    return true;
}

bool test_is2power(void)
{
    TEST_ASSERT(!ail_is_2power(0));
    TEST_ASSERT(ail_is_2power(1));
    TEST_ASSERT(ail_is_2power(2));
    TEST_ASSERT(ail_is_2power(-2));
    TEST_ASSERT(ail_is_2power(-256));
    TEST_ASSERT(!ail_is_2power(3));
    TEST_ASSERT(ail_is_2power(-1));
    TEST_ASSERT(!ail_is_2power(-3));
    TEST_ASSERT(!ail_is_2power(255));
    TEST_ASSERT(ail_is_2power(0x800000));
    TEST_ASSERT(ail_is_2power(-0x800000));
    TEST_ASSERT(!ail_is_2power(0x900000));
    TEST_ASSERT(!ail_is_2power(-0x900000));
    TEST_ASSERT(!ail_is_2power(0x800200));
    TEST_ASSERT(!ail_is_2power(0x2222));
    return true;
}

bool test_next2Power(void)
{
    u64 u;
    ail_next_2power_pos(0x7BA42ACD251B7129, u);
    TEST_ASSERT(u == 0x8000000000000000);
    ail_next_2power_pos(0xf2, u);
    TEST_ASSERT(u == 0x100);
    ail_next_2power_pos(0, u);
    TEST_ASSERT(u == 1);
    ail_next_2power_pos(1, u);
    TEST_ASSERT(u == 2);
    ail_next_2power_pos(12, u);
    TEST_ASSERT(u == 16);
    ail_next_2power_pos(10, u);
    TEST_ASSERT(u == 16);
    i64 i;
    ail_next_2power(-1, i);
    TEST_ASSERT(i == -2);
    ail_next_2power(-12, i);
    TEST_ASSERT(i == -16);
    ail_next_2power(-15, i);
    TEST_ASSERT(i == -16);
    ail_next_2power(-2000, i);
    TEST_ASSERT(i == -2048);
    return true;
}

#define float_eq(a, b, delta) ((a) <= (b) + (delta) || (a) >= (b) - (delta))

bool test_lerp(void)
{
    TEST_ASSERT(ail_lerp(0, 33, 69) == 33);
    TEST_ASSERT(ail_inv_lerp(33, 33, 69) == 0);
    TEST_ASSERT(ail_lerp(1, 33, 69) == 69);
    TEST_ASSERT(ail_inv_lerp(69, 33, 69) == 1);
    TEST_ASSERT(ail_lerp(0.5f, 4, 20) == 12);
    TEST_ASSERT(ail_inv_lerp(12, 4, 20) == 0.5f);
    TEST_ASSERT(ail_lerp(0.66f, 4, 104) == 70);
    TEST_ASSERT(float_eq(ail_inv_lerp(70, 4, 104), 0.66f, 0.001f));
    TEST_ASSERT(ail_lerp(0.2f, -77, 23) == -57);
    TEST_ASSERT(float_eq(ail_inv_lerp(-57, -77, 23), 0.2f, 0.001f));
    return true;
}

int main(void)
{
    TEST(test_minmax);
    TEST(test_clamp);
    TEST(test_is2power);
    TEST(test_next2Power);
    TEST(test_lerp);
    return 0;
}
