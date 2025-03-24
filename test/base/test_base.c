#include "../../src/base/ail_base.h"
#include "../test.h"
#include <string.h>
#include <stdlib.h>

// Make sure keywords & types don't cause compilation errors
internal bool test_internal_func(void)
{
    return true;
}

inline_func b32 test_inline_func(void)
{
    return !false;
}

bool test_persist(void)
{
    persist bool result = 1;
    return result;
}

global u8  global_variable;
export u16 export_variable = 16;
import u32 import_variable;
u64 num_u64 = 0;
i8  num_i8;
i16 num_i16 = -64;
i32 num_i32;
i64 num_i64;
f32 num_f32 = 2.7f;
f64 num_f64 = 3.14;
b32  var_true  = 1;
bool var_false = false;
pchar c_str = "Test";


// Test Allocator interface
bool test_empty_allocator(void)
{
    AIL_Allocator al = ail_default_allocator;
    TEST_ASSERT(!ail_call_alloc(al, 32));
    TEST_ASSERT(!ail_call_calloc(al, 32));
    TEST_ASSERT(!ail_call_calloc(al, 8, 4));
    TEST_ASSERT(!ail_call_realloc(al, NULL, 64));
    TEST_ASSERT(!ail_call_shrink(al, NULL, 32));
    TEST_ASSERT(!ail_call_free(al, NULL));
    TEST_ASSERT(!ail_call_clear_all(al));
    TEST_ASSERT(!ail_call_free_all(al));
    return true;
}


// Test Utility Macros
bool test_arrlen(void)
{
    u32 a[77] = { 1, 2, 0 };
    TEST_ASSERT(ail_arrlen(a) == 77);
    u8 b[24];
    TEST_ASSERT(ail_arrlen(b) == 24);
    struct { b32 b; f64 f; } c[AIL_KB(2) - 63] = { 0 };
    TEST_ASSERT(ail_arrlen(c) == AIL_KB(2) - 63);
    return true;
}

bool test_int_ptr_conversion(void)
{
    pchar s  = "Hello World\n";
    void *p  = (void*)0xff78;
    i64   n  = 3293002137;
    char *np = (void*)3293002137;
    TEST_ASSERT(ail_ptr_from_int(ail_int_from_ptr(s)) == s);
    TEST_ASSERT(ail_ptr_from_int(ail_int_from_ptr(p)) == p);
    TEST_ASSERT(ail_int_from_ptr(p) == 0xff78);
    TEST_ASSERT(ail_ptr_from_int(n) == np);
    TEST_ASSERT(ail_int_from_ptr(ail_ptr_from_int(n)) == n);
    return true;
}

bool test_typeof(void)
{
#ifndef ail_typeof
    printf("  \033[33mCannot test ail_typeof, as the current compiler does not support a typeof intrinsic\033[0m\n");
#else
    u32 a = UINT32_MAX;
    u32 b = 420;
    ail_typeof(a) c = a;
    a = b;
    b = c;
    TEST_ASSERT(a == 420);
    TEST_ASSERT(b == UINT32_MAX);
#endif
    return true;
}

bool test_offset_of(void)
{
    struct T { f64 f; b32 b; pchar s; };
    struct T t;
    TEST_ASSERT(sizeof(t) == 24);
    TEST_ASSERT(ail_offset_of(&t, f) == 0);
    TEST_ASSERT(ail_offset_of(&t, b) == 8);
    TEST_ASSERT(ail_offset_of(&t, s) == 16);
    // TEST_ASSERT(ail_offset_of_type(struct T, f) == 0);
    // TEST_ASSERT(ail_offset_of_type(struct T, b) == 8);
    // TEST_ASSERT(ail_offset_of_type(struct T, s) == 16);
    struct S { u8 a; struct T t; i64 *p; };
    struct S s;
    TEST_ASSERT(sizeof(s) == 40);
    TEST_ASSERT(ail_offset_of(&s, a)   == 0);
    TEST_ASSERT(ail_offset_of(&s, t)   == 8);
    TEST_ASSERT(ail_offset_of(&s, t.f) == 8);
    TEST_ASSERT(ail_offset_of(&s, t.b) == 16);
    TEST_ASSERT(ail_offset_of(&s, t.s) == 24);
    TEST_ASSERT(ail_offset_of(&s, p)   == 32);
    // TEST_ASSERT(ail_offset_of_type(struct S, a)   == 0);
    // TEST_ASSERT(ail_offset_of_type(struct S, t)   == 8);
    // TEST_ASSERT(ail_offset_of_type(struct S, t.f) == 8);
    // TEST_ASSERT(ail_offset_of_type(struct S, t.b) == 16);
    // TEST_ASSERT(ail_offset_of_type(struct S, t.s) == 24);
    // TEST_ASSERT(ail_offset_of_type(struct S, p)   == 32);
    return true;
}

bool test_field_ptr(void)
{
    return true;
}

bool test_base_from_field(void)
{
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
    TEST_ASSERT(fa == fb_og);
    TEST_ASSERT(fb == fa_og);
    ail_swap(struct T, ta, tb);
    TEST_ASSERT(ta.a == tb_og.a && ta.b == tb_og.b);
    TEST_ASSERT(tb.a == ta_og.a && tb.b == ta_og.b);
#ifdef ail_typeof
    ail_swap(fa, fb);
    TEST_ASSERT(fa == fa_og);
    TEST_ASSERT(fb == fb_og);
    ail_swap(ta, tb);
    TEST_ASSERT(ta.a == ta_og.a && ta.b == ta_og.b);
    TEST_ASSERT(tb.a == tb_og.a && tb.b == tb_og.b);
#endif
    return true;
}

bool test_sizes(void)
{
    TEST_ASSERT(AIL_KB(1)  ==  1*1024ull);
    TEST_ASSERT(AIL_KB(17) == 17*1024ull);
    TEST_ASSERT(AIL_MB(1)  ==  1*1048576ull);
    TEST_ASSERT(AIL_MB(17) == 17*1048576ull);
    TEST_ASSERT(AIL_GB(1)  ==  1*1073741824ull);
    TEST_ASSERT(AIL_GB(17) == 17*1073741824ull);
    TEST_ASSERT(AIL_TB(1)  ==  1*1099511627776ull);
    TEST_ASSERT(AIL_TB(17) == 17*1099511627776ull);
    return true;
}

bool test_strs(void)
{
    TEST_ASSERT(!strcmp(AIL_STRINGIFY(test), "test"));
    TEST_ASSERT(!strcmp(AIL_STRINGIFY(69x420), "69x420"));
    TEST_ASSERT(atoi(AIL_STR_LINE) == __LINE__);
    u32 foo69 = 69;
    TEST_ASSERT(AIL_CONCAT(foo, 69) == 69);
    TEST_ASSERT(!strcmp(AIL_STRINGIFY(AIL_CONCAT(foo, 69)), "foo69"));
    TEST_ASSERT(!strcmp(AIL_STRINGIFY(AIL_CONCAT(  foo  , 69 )), "foo69"));
    TEST_ASSERT(!strcmp(AIL_STRINGIFY(AIL_CONCAT(  f o     o  , 6 9 )), "f o o6 9"));
    return true;
}

// @Note: Cannot test that the attributes exactly work as expected, but instead just whether they lead to compilation errors
bool test_misc(void)
{
    TEST_ASSERT(AIL_LIKELY(true));
    TEST_ASSERT(AIL_UNLIKELY(true));
    u64 x = 69;
    AIL_UNUSED(x);
    switch (true) {
        case 1 == 2:
            x = 67;
            fallthrough;
        case 1 == 1:
            TEST_ASSERT(true);
            break;
        case -3:
        default:
            TEST_ASSERT(false);
            break;
    }
    return true;
}


int main(void)
{
    TEST(test_internal_func);
    TEST(test_inline_func);
    TEST(test_persist);
    TEST(test_empty_allocator);
    
    TEST(test_arrlen);
    TEST(test_typeof);
    TEST(test_int_ptr_conversion);
    TEST(test_offset_of);
    TEST(test_field_ptr);
    TEST(test_base_from_field);
    TEST(test_swap);
    TEST(test_sizes);

    TEST(test_strs);
    TEST(test_misc);
    return 0;
}
