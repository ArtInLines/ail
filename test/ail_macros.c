#define AIL_TYPES_IMPL
#include "../ail.h"
#include "./test_assert.h"
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
    struct T { f64 f; b32 b; str s; };
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
    struct T { i16 a; str b; };
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
#ifdef AIL_SWAP
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
    i64 b = 69;
    ASSERT(AIL_MAX(a, b) == a);
    ASSERT(AIL_MIN(a, b) == b);
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
    ASSERT(!AIL_IS_2POWER(-1));
    ASSERT(!AIL_IS_2POWER(255));
    ASSERT(AIL_IS_2POWER(0x800000));
    ASSERT(!AIL_IS_2POWER(0x900000));
    ASSERT(!AIL_IS_2POWER(0x800200));
    ASSERT(!AIL_IS_2POWER(0x2222));
    return true;
}

bool test_next2Power(void)
{
    u64 x        = 0x7BA42ACD251B7129;
    u64 expected = 0x8000000000000000;
    u64 out;
    AIL_NEXT_2POWER(x, out);
    return out == expected;
}

#define TESTS          \
    X(test_arrlen)     \
    X(test_typeof)     \
    X(test_typeof)     \
    X(test_swap)       \
    X(test_minmax)     \
    X(test_is2power)   \
    X(test_next2Power)

int main(void)
{
    #define X(func) if (func()) printf("\033[32m" AIL_STRINGIFY(func) " succesful :)\033[0m\n"); else printf("\033[31m" AIL_STRINGIFY(func) " failed :(\033[0m\n");
        TESTS
    #undef X
    return 0;
}
