#include "../ail.h"
#include "./assert.h"
#include <string.h>
#include <stdio.h>

bool test_arrlen(void)
{
    u32 a[77] = { 1, 2, 0 };
    ASSERT(AIL_ARRLEN(a) == 77);
    u8 b[24];
    ASSERT(AIL_ARRLEN(b) == 24);
    struct { b32 b; f64 f; } c[AIL_KB(2) - 63] = { 0 };
    ASSERT(AIL_ARRLEN(c) == AIL_KB(2) - 63);
    return true;
}

bool test_typeof(void)
{
#ifndef AIL_TYPEOF
    printf("\033[33mCannot test AIL_TYPEOF, as the current compiler does not support a typeof intrinsic\033[0m\n");
#else
    u32 a = UINT32_MAX;
    u32 b = 420;
    AIL_TYPEOF(a) c = a;
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
    ASSERT(AIL_OFFSETOF(&t, f) == 0);
    ASSERT(AIL_OFFSETOF(&t, b) == 8);
    ASSERT(AIL_OFFSETOF(&t, s) == 16);
    struct S { u8 a; struct T t; i64 *p; };
    struct S s;
    ASSERT(sizeof(s) == 40);
    ASSERT(AIL_OFFSETOF(&s, a)   == 0);
    ASSERT(AIL_OFFSETOF(&s, t)   == 8);
    ASSERT(AIL_OFFSETOF(&s, t.f) == 8);
    ASSERT(AIL_OFFSETOF(&s, t.b) == 16);
    ASSERT(AIL_OFFSETOF(&s, t.s) == 24);
    ASSERT(AIL_OFFSETOF(&s, p)   == 32);
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
    AIL_SWAP_PORTABLE(f64, fa, fb);
    ASSERT(fa == fb_og);
    ASSERT(fb == fa_og);
    AIL_SWAP_PORTABLE(struct T, ta, tb);
    ASSERT(ta.a == tb_og.a && ta.b == tb_og.b);
    ASSERT(tb.a == ta_og.a && tb.b == ta_og.b);
#ifdef AIL_TYPEOF
    AIL_SWAP(fa, fb);
    ASSERT(fa == fa_og);
    ASSERT(fb == fb_og);
    AIL_SWAP(ta, tb);
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
    ASSERT(AIL_MAX(a, b) == a);
    ASSERT(AIL_MIN(a, b) == b);
    ASSERT(AIL_MAX(a, c) == a);
    ASSERT(AIL_MIN(a, c) == c);
    ASSERT(AIL_MAX(a, d) == a);
    ASSERT(AIL_MIN(a, d) == d);
    ASSERT(AIL_MAX(a, e) == a);
    ASSERT(AIL_MIN(a, e) == e);
    ASSERT(AIL_MAX(a, f) == a);
    ASSERT(AIL_MIN(a, f) == f);
    ASSERT(AIL_MAX(b, c) == b);
    ASSERT(AIL_MIN(b, c) == c);
    ASSERT(AIL_MAX(b, d) == b);
    ASSERT(AIL_MIN(b, d) == d);
    ASSERT(AIL_MAX(b, e) == b);
    ASSERT(AIL_MIN(b, e) == e);
    ASSERT(AIL_MAX(b, f) == b);
    ASSERT(AIL_MIN(b, f) == f);
    ASSERT(AIL_MAX(c, d) == c);
    ASSERT(AIL_MIN(c, d) == d);
    ASSERT(AIL_MAX(c, e) == c);
    ASSERT(AIL_MIN(c, e) == e);
    ASSERT(AIL_MAX(c, f) == c);
    ASSERT(AIL_MIN(c, f) == f);
    ASSERT(AIL_MAX(d, e) == d);
    ASSERT(AIL_MIN(d, e) == e);
    ASSERT(AIL_MAX((i32)d, f) == d);
    ASSERT(AIL_MIN((i32)d, f) == f);
    ASSERT(AIL_MAX(e, f) == e);
    ASSERT(AIL_MIN(e, f) == f);
    return true;
}

bool test_clamp(void)
{
    ASSERT(AIL_CLAMP(3, 0, 6) == 3);
    ASSERT(AIL_CLAMP(-3, 0, 6) == 0);
    ASSERT(AIL_CLAMP(9, 0, 6) == 6);
    ASSERT(AIL_CLAMP(3, -1, 6) == 3);
    ASSERT(AIL_CLAMP(UINT64_MAX, INT64_MIN, INT8_MIN) == INT8_MIN);
    ASSERT(AIL_CLAMP(6.9f, 69, 420) == 69);
    ASSERT(AIL_CLAMP(6.9f, 0, 69) == 6.9f);
    ASSERT(AIL_CLAMP(-6.9f, -7.8f, -6.99f) == -6.99f);
    ASSERT(AIL_CLAMP(6.9f, -1e+10, -0.0f) == -0.0f);
    return true;
}

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
    u64 u;
    AIL_NEXT_2POWER_POS(0x7BA42ACD251B7129, u);
    ASSERT(u == 0x8000000000000000);
    AIL_NEXT_2POWER_POS(0xf2, u);
    ASSERT(u == 0x100);
    AIL_NEXT_2POWER_POS(0, u);
    ASSERT(u == 1);
    AIL_NEXT_2POWER_POS(1, u);
    ASSERT(u == 2);
    AIL_NEXT_2POWER_POS(12, u);
    ASSERT(u == 16);
    AIL_NEXT_2POWER_POS(10, u);
    ASSERT(u == 16);
    i64 i;
    AIL_NEXT_2POWER(-1, i);
    ASSERT(i == -2);
    AIL_NEXT_2POWER(-12, i);
    ASSERT(i == -16);
    AIL_NEXT_2POWER(-15, i);
    ASSERT(i == -16);
    AIL_NEXT_2POWER(-2000, i);
    ASSERT(i == -2048);
    return true;
}

#define float_eq(a, b, delta) ((a) <= (b) + (delta) || (a) >= (b) - (delta))

bool test_lerp(void)
{
    ASSERT(AIL_LERP(0, 33, 69) == 33);
    ASSERT(AIL_INV_LERP(33, 33, 69) == 0);
    ASSERT(AIL_LERP(1, 33, 69) == 69);
    ASSERT(AIL_INV_LERP(69, 33, 69) == 1);
    ASSERT(AIL_LERP(0.5f, 4, 20) == 12);
    ASSERT(AIL_INV_LERP(12, 4, 20) == 0.5f);
    ASSERT(AIL_LERP(0.66f, 4, 104) == 70);
    ASSERT(float_eq(AIL_INV_LERP(70, 4, 104), 0.66f, 0.001f));
    ASSERT(AIL_LERP(0.2f, -77, 23) == -57);
    ASSERT(float_eq(AIL_INV_LERP(-57, -77, 23), 0.2f, 0.001f));
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
            AIL_FALLTHROUGH;
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
    #define X(func) if (func()) printf("\033[32m" AIL_STRINGIFY(func) " succesful :)\033[0m\n"); else printf("\033[31m" AIL_STRINGIFY(func) " failed :(\033[0m\n");
        TESTS
    #undef X
    return 0;
}
