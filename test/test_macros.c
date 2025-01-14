#include "../src/base/ail_base.h"
#include "../src/base/ail_base_math.h"
#include "./assert.h"
#include <stdlib.h> // For atoi
#include <string.h> // For strcmp
#include <stdio.h>  // For printf

bool test_arrlen(void)
{
    u32 a[77] = { 1, 2, 0 };
    ASSERT(ail_arrlen(a) == 77);
    u8 b[24];
    ASSERT(ail_arrlen(b) == 24);
    struct { b32 b; f64 f; } c[AIL_KB(2) - 63] = { 0 };
    ASSERT(ail_arrlen(c) == AIL_KB(2) - 63);
    return true;
}

bool test_typeof(void)
{
#ifndef ail_typeof
    printf("\033[33mCannot test ail_typeof, as the current compiler does not support a typeof intrinsic\033[0m\n");
#else
    u32 a = UINT32_MAX;
    u32 b = 420;
    ail_typeof(a) c = a;
    a = b;
    b = c;
    ASSERT(a == 420);
    ASSERT(b == UINT32_MAX);
#endif
    return true;
}

bool test_offsetof(void)
{
    struct T { f64 f; b32 b; pchar s; };
    struct T t;
    ASSERT(sizeof(t) == 24);
    ASSERT(ail_offset_of(&t, f) == 0);
    ASSERT(ail_offset_of(&t, b) == 8);
    ASSERT(ail_offset_of(&t, s) == 16);
    struct S { u8 a; struct T t; i64 *p; };
    struct S s;
    ASSERT(sizeof(s) == 40);
    ASSERT(ail_offset_of(&s, a)   == 0);
    ASSERT(ail_offset_of(&s, t)   == 8);
    ASSERT(ail_offset_of(&s, t.f) == 8);
    ASSERT(ail_offset_of(&s, t.b) == 16);
    ASSERT(ail_offset_of(&s, t.s) == 24);
    ASSERT(ail_offset_of(&s, p)   == 32);
    return true;
}

bool test_swap(void)
{
    struct T { i16 a; pchar b; };
    f64 fa_og = 0.69;
    f64 fb_og = 4.20;
    f64 fa = fa_og;
    f64 fb = fb_og;
    struct T ta_og = { .a = -69,       .b = "Foo" };
    struct T tb_og = { .a = INT16_MAX, .b = "bar" };
    struct T ta = ta_og;
    struct T tb = tb_og;
    ail_swap(f64, fa, fb);
    ASSERT(fa == fb_og);
    ASSERT(fb == fa_og);
    ail_swap(struct T, ta, tb);
    ASSERT(ta.a == tb_og.a && ta.b == tb_og.b);
    ASSERT(tb.a == ta_og.a && tb.b == ta_og.b);
#ifdef ail_typeof
    ail_swap(fa, fb);
    ASSERT(fa == fa_og);
    ASSERT(fb == fb_og);
    ail_swap(ta, tb);
    ASSERT(ta.a == ta_og.a && ta.b == ta_og.b);
    ASSERT(tb.a == tb_og.a && tb.b == tb_og.b);
#endif
    return true;
}

bool test_minmax(void)
{
    u16 a = 420;
    f32 b = 203.781;
    i64 c = 69;
    b32 d = true;
    i8  e = 0;
    i32 f = -17;
    ASSERT(ail_max(a, b) == a);
    ASSERT(ail_min(a, b) == b);
    ASSERT(ail_max(a, c) == a);
    ASSERT(ail_min(a, c) == c);
    ASSERT(ail_max(a, d) == a);
    ASSERT(ail_min(a, d) == d);
    ASSERT(ail_max(a, e) == a);
    ASSERT(ail_min(a, e) == e);
    ASSERT(ail_max(a, f) == a);
    ASSERT(ail_min(a, f) == f);
    ASSERT(ail_max(b, c) == b);
    ASSERT(ail_min(b, c) == c);
    ASSERT(ail_max(b, d) == b);
    ASSERT(ail_min(b, d) == d);
    ASSERT(ail_max(b, e) == b);
    ASSERT(ail_min(b, e) == e);
    ASSERT(ail_max(b, f) == b);
    ASSERT(ail_min(b, f) == f);
    ASSERT(ail_max(c, d) == c);
    ASSERT(ail_min(c, d) == d);
    ASSERT(ail_max(c, e) == c);
    ASSERT(ail_min(c, e) == e);
    ASSERT(ail_max(c, f) == c);
    ASSERT(ail_min(c, f) == f);
    ASSERT(ail_max((i8)d, e)  == (i32)d);
    ASSERT(ail_min((i8)d, e)  == e);
    ASSERT(ail_max((i32)d, f) == (i32)d);
    ASSERT(ail_min((i32)d, f) == f);
    ASSERT(ail_max(e, f) == e);
    ASSERT(ail_min(e, f) == f);
    return true;
}

bool test_clamp(void)
{
    ASSERT(ail_clamp(3, 0, 6) == 3);
    ASSERT(ail_clamp(-3, 0, 6) == 0);
    ASSERT(ail_clamp(9, 0, 6) == 6);
    ASSERT(ail_clamp(3, -1, 6) == 3);
    ASSERT(ail_clamp(UINT64_MAX, INT64_MIN, INT8_MIN) == INT8_MIN);
    ASSERT(ail_clamp(6.9f, 69, 420) == 69);
    ASSERT(ail_clamp(6.9f, 0, 69) == 6.9f);
    ASSERT(ail_clamp(-6.9f, -7.8f, -6.99f) == -6.99f);
    ASSERT(ail_clamp(6.9f, -1e+10, -0.0f) == -0.0f);
    return true;
}

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
    u64 u;
    ail_next_2power_pos(0x7BA42ACD251B7129, u);
    ASSERT(u == 0x8000000000000000);
    ail_next_2power_pos(0xf2, u);
    ASSERT(u == 0x100);
    ail_next_2power_pos(0, u);
    ASSERT(u == 1);
    ail_next_2power_pos(1, u);
    ASSERT(u == 2);
    ail_next_2power_pos(12, u);
    ASSERT(u == 16);
    ail_next_2power_pos(10, u);
    ASSERT(u == 16);
    i64 i;
    ail_next_2power(-1, i);
    ASSERT(i == -2);
    ail_next_2power(-12, i);
    ASSERT(i == -16);
    ail_next_2power(-15, i);
    ASSERT(i == -16);
    ail_next_2power(-2000, i);
    ASSERT(i == -2048);
    return true;
}

#define float_eq(a, b, delta) ((a) <= (b) + (delta) || (a) >= (b) - (delta))

bool test_lerp(void)
{
    ASSERT(ail_lerp(0, 33, 69) == 33);
    ASSERT(ail_inv_lerp(33, 33, 69) == 0);
    ASSERT(ail_lerp(1, 33, 69) == 69);
    ASSERT(ail_inv_lerp(69, 33, 69) == 1);
    ASSERT(ail_lerp(0.5f, 4, 20) == 12);
    ASSERT(ail_inv_lerp(12, 4, 20) == 0.5f);
    ASSERT(ail_lerp(0.66f, 4, 104) == 70);
    ASSERT(float_eq(ail_inv_lerp(70, 4, 104), 0.66f, 0.001f));
    ASSERT(ail_lerp(0.2f, -77, 23) == -57);
    ASSERT(float_eq(ail_inv_lerp(-57, -77, 23), 0.2f, 0.001f));
    return true;
}

bool test_sizes(void)
{
    ASSERT(AIL_KB(1)  ==  1*1024ull);
    ASSERT(AIL_KB(17) == 17*1024ull);
    ASSERT(AIL_MB(1)  ==  1*1048576ull);
    ASSERT(AIL_MB(17) == 17*1048576ull);
    ASSERT(AIL_GB(1)  ==  1*1073741824ull);
    ASSERT(AIL_GB(17) == 17*1073741824ull);
    ASSERT(AIL_TB(1)  ==  1*1099511627776ull);
    ASSERT(AIL_TB(17) == 17*1099511627776ull);
    return true;
}

bool test_warns(void)
{
    // @TODO
    return true;
}

bool test_strs(void)
{
    ASSERT(!strcmp(AIL_STRINGIFY(test), "test"));
    ASSERT(!strcmp(AIL_STRINGIFY(69x420), "69x420"));
    ASSERT(atoi(AIL_STR_LINE) == __LINE__);
    u32 foo69 = 69;
    ASSERT(AIL_CONCAT(foo, 69) == 69);
    ASSERT(!strcmp(AIL_STRINGIFY(AIL_CONCAT(foo, 69)), "foo69"));
    ASSERT(!strcmp(AIL_STRINGIFY(AIL_CONCAT(  foo  , 69 )), "foo69"));
    ASSERT(!strcmp(AIL_STRINGIFY(AIL_CONCAT(  f o     o  , 6 9 )), "f o o6 9"));
    return true;
}

// @Note: Cannot test that the attributes exactly work as expected, but instead just whether they lead to compilation errors
bool test_misc(void)
{
    ASSERT(AIL_LIKELY(true));
    ASSERT(AIL_UNLIKELY(true));
    u64 x = 69;
    AIL_UNUSED(x);
    switch (true) {
        case 1 == 2:
            x = 67;
            fallthrough;
        case 1 == 1:
            ASSERT(true);
            break;
        case -3:
        default:
            ASSERT(false);
            break;
    }
    return true;
}

#define TESTS          \
    X(test_arrlen)     \
    X(test_typeof)     \
    X(test_typeof)     \
    X(test_swap)       \
    X(test_minmax)     \
    X(test_is2power)   \
    X(test_next2Power) \
    X(test_clamp)      \
    X(test_lerp)       \
    X(test_sizes)      \
    X(test_warns)      \
    X(test_strs)       \
    X(test_misc)

int main(void)
{
#if AIL_COMP_GCC
    #define X(func) if (func()) printf("\033[32m" AIL_STRINGIFY(func) " succesful :)\033[0m\n"); else printf("\033[31m" AIL_STRINGIFY(func) " failed :(\033[0m\n");
        TESTS
    #undef X
#endif
    return 0;
}
