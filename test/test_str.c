#include "./assert.h"
#include "../src/base/ail_alloc.h"
#include "../src/base/ail_str.h"
#include <stdbool.h>
#include <string.h>

bool test_ail_str_inits(void)
{
    char  *a = "abcdefg";
    AIL_Str b = ail_str_from_cstr(a);
    AIL_Str c = ail_str_from_cstr("abcdefg");
    AIL_Str d = ail_str_from_parts((u8*)"abcdefg", strlen("abcdefg"));

    AIL_DA(u8) da = ail_da_new(u8);
    ail_da_pushn(&da, a, 7);
    AIL_Str e = ail_str_from_arr(da);

    ASSERT(memcmp(a, b.data, b.len) == 0);
    ASSERT(memcmp(a, c.data, c.len) == 0);
    ASSERT(memcmp(a, d.data, d.len) == 0);
    ASSERT(memcmp(a, e.data, e.len) == 0);
    return true;
}

bool test_ail_str_num_conversions(void)
{
    // @TODO: Test from/to float conversion too
    AIL_Str nonum1  = ail_str_from_cstr("not a number");
    AIL_Str nonum2  = ail_str_from_cstr("+not a number");
    AIL_Str nonum3  = ail_str_from_cstr("-not a number");
    ASSERT(ail_str_parse_unsigned(nonum1).val == 0);
    ASSERT(ail_str_parse_unsigned(nonum1).len == 0);
    ASSERT(ail_str_parse_unsigned(nonum2).val == 0);
    ASSERT(ail_str_parse_unsigned(nonum2).len == 0);
    ASSERT(ail_str_parse_signed(nonum1).val == 0);
    ASSERT(ail_str_parse_signed(nonum1).len == 0);
    ASSERT(ail_str_parse_signed(nonum3).val == 0);
    ASSERT(ail_str_parse_signed(nonum3).len == 0);

    i64 zero = 0;
    i64 one  = 1;
    i64 big  = 19420;
    i64 neg  = -329;
    AIL_Str numu0   = ail_str_new_unsigned(zero);
    AIL_Str numu1   = ail_str_new_unsigned(one);
    AIL_Str numu2   = ail_str_new_unsigned(big);
    AIL_Str numi0   = ail_str_new_signed(zero);
    AIL_Str numi1   = ail_str_new_signed(one);
    AIL_Str numi2   = ail_str_new_signed(big);
    AIL_Str numineg = ail_str_new_signed(neg);
    ASSERT(ail_str_eq(ail_str_from_cstr("0"),     numu0));
    ASSERT(ail_str_eq(ail_str_from_cstr("1"),     numu1));
    ASSERT(ail_str_eq(ail_str_from_cstr("19420"), numu2));
    ASSERT(ail_str_eq(ail_str_from_cstr("0"),     numi0));
    ASSERT(ail_str_eq(ail_str_from_cstr("1"),     numi1));
    ASSERT(ail_str_eq(ail_str_from_cstr("19420"), numi2));
    ASSERT(ail_str_eq(ail_str_from_cstr("-329"),  numineg));
    ASSERT(ail_str_parse_unsigned(numu0).val == (u64)zero);
    ASSERT(ail_str_parse_unsigned(numu0).len == 1);
    ASSERT(ail_str_parse_unsigned(numu1).val == (u64)one);
    ASSERT(ail_str_parse_unsigned(numu1).len == 1);
    ASSERT(ail_str_parse_unsigned(numu2).val == (u64)big);
    ASSERT(ail_str_parse_unsigned(numu2).len == 5);
    ASSERT(ail_str_parse_signed(numi0).val == zero);
    ASSERT(ail_str_parse_signed(numi0).len == 1);
    ASSERT(ail_str_parse_signed(numi1).val == one);
    ASSERT(ail_str_parse_signed(numi1).len == 1);
    ASSERT(ail_str_parse_signed(numi2).val == big);
    ASSERT(ail_str_parse_signed(numi2).len == 5);
    ASSERT(ail_str_parse_signed(numineg).val == neg);
    ASSERT(ail_str_parse_signed(numineg).len == 4);

    AIL_Str add = ail_str_from_cstr("13+5");
    AIL_Str sub = ail_str_from_cstr("-13-5");
    ASSERT(ail_str_parse_unsigned(add).val == 13);
    ASSERT(ail_str_parse_unsigned(add).len == 2);
    ASSERT(ail_str_parse_unsigned(sub).val == 0);
    ASSERT(ail_str_parse_unsigned(sub).len == 0);
    ASSERT(ail_str_parse_signed(sub).val == -13);
    ASSERT(ail_str_parse_signed(sub).len == 3);
    return true;
}

bool test_ail_str_cmps(void)
{
    char  *s = "abc defg";
    char  c1 = 'a';
    char  c2 = 'g';
    AIL_Str a = ail_str_from_parts((u8*)s, strlen(s));
    AIL_Str b = ail_str_from_parts((u8*)"abc defg", 8);
    AIL_Str c = ail_str_from_parts((u8*)s, 4);
    AIL_Str d = ail_str_from_cstr("defg");

    ASSERT(ail_str_eq(a, b));
    ASSERT(!ail_str_eq(a, c));
    ASSERT(!ail_str_eq(a, d));
    ASSERT(!ail_str_eq(c, d));
    ASSERT(ail_str_starts_with(a, c));
    ASSERT(ail_str_starts_with(a, b));
    ASSERT(!ail_str_starts_with(c, a));
    ASSERT(!ail_str_starts_with(a, d));
    ASSERT(ail_str_starts_with_char(a, c1));
    ASSERT(ail_str_ends_with_char(a, c2));
    ASSERT(ail_str_ends_with(a, d));
    ASSERT(!ail_str_ends_with(a, c));
    ASSERT(!ail_str_ends_with(c, d));
    return true;
}

bool test_ail_str_split(void)
{
    // @TODO: Also test the following
    // - ail_str_split_next_char
    // - ail_str_split_next
    // - ail_str_split_char
    AIL_Str a = ail_str_from_cstr("_:__:_abc_:_def_:_\nghi_:_jkl_:_");
    AIL_DA(AIL_Str) lines = ail_str_split_lines(a, true);
    ASSERT(lines.len == 2);
    AIL_Str space = ail_str_from_cstr("_:_");
    AIL_DA(AIL_Str) words = ail_str_split(lines.data[0], space, true);
    AIL_DA(AIL_Str) splitted = ail_str_split(lines.data[0], space, false);
    ASSERT(words.len == 2);
    ASSERT(splitted.len == 4);
    ASSERT(ail_str_eq(words.data[0], ail_str_from_cstr("abc")));
    ASSERT(ail_str_eq(words.data[1], ail_str_from_cstr("def")));
    ASSERT(ail_str_eq(splitted.data[0], ail_str_from_parts(NULL, 0)));
    AIL_Str joined_words    = ail_str_join(words.data, words.len, ail_str_from_cstr(" "));
    ASSERT(joined_words.data[joined_words.len] == 0);
    AIL_Str joined_splitted = ail_str_join_da(splitted, ail_str_from_cstr(" "));
    ASSERT(joined_splitted.data[joined_splitted.len] == 0);
    ASSERT(joined_words.len + 2 == joined_splitted.len);
    ASSERT(ail_str_eq(joined_words, ail_str_trim(joined_splitted)));
    ASSERT(ail_str_eq(joined_words, ail_str_offset(joined_splitted, 2)));
    return true;
}

bool test_ail_str_others(void)
{
    // @TODO: Also test the following
    AIL_Str empty  = {0};
    AIL_Str hello1 = ail_str_from_cstr("   Hello World! ");
    AIL_Str hello2 = ail_str_from_cstr("Hello World!  ");

    ASSERT(ail_str_eq(ail_str_trim(hello1), ail_str_rtrim(hello2)));
    ASSERT(ail_str_eq(ail_str_trim(hello1), ail_str_ltrim(ail_str_rtrim(hello1))));
    ASSERT(ail_str_eq(empty, ail_str_trim(empty)));
    ASSERT(ail_str_eq(empty, ail_str_ltrim(empty)));
    ASSERT(ail_str_eq(empty, ail_str_rtrim(empty)));

    ASSERT(ail_str_eq(ail_str_from_cstr("def"), ail_str_offset(ail_str_from_cstr("abcdef"), 3)));
    ASSERT(ail_str_eq(empty, ail_str_offset(empty, 3)));

    char *hello_copy = ail_str8_to_cstr(hello1);
    ASSERT(hello_copy != (char*)hello1.data); // checks that it isn't the same pointer
    ASSERT(memcmp(hello_copy, hello1.data, hello1.len) == 0);
    ASSERT(hello_copy[hello1.len] == 0);
    ail_call_free(ail_default_allocator, hello_copy);

    char *empty_copy = ail_str8_to_cstr(empty);
    ASSERT(empty_copy != (char*)empty.data); // checks that it isn't the same pointer
    ASSERT(memcmp(empty_copy, empty.data, empty.len) == 0);
    ASSERT(empty_copy[empty.len] == 0);
    ail_call_free(ail_default_allocator, empty_copy);

    AIL_Str  hello = ail_str_from_cstr("Hello");
    AIL_Str  hi    = ail_str_from_cstr("Hi");
    AIL_Str  a     = ail_str_from_cstr("Hello World, Hello!");
    AIL_Str b     = ail_str_replace(a, hello, hi);
    ASSERT(b.data[b.len] == 0);
    ASSERT(ail_str_eq(ail_str_from_cstr("Hi World, Hi!"), b));
    ail_call_free(ail_default_allocator, b.data);

    AIL_Str s = ail_str_replace(empty, a, b);
    ASSERT(ail_str_eq(s, empty));

    AIL_Str  xhi  = ail_str_from_cstr("hi");
    AIL_Str  xhey = ail_str_from_cstr("hey");
    AIL_Str  xin  = ail_str_from_cstr("hello, hello, hello!");
    AIL_Str xout = ail_str_replace(xin, xhi, xhey);
    ASSERT(ail_str_eq(xin, xout));
    ASSERT(xin.data != xout.data);
    ail_call_free(ail_default_allocator, xout.data);
    return true;
}

int main(void)
{
    ail_default_allocator = ail_alloc_std;
    if (test_ail_str_inits()) printf("\033[032mAIL_Str initialization works :)\033[0m\n");
    else                      printf("\033[031mAIL_Str initialization fails :(\033[0m\n");
    if (test_ail_str_cmps()) printf("\033[032mAIL_Str comparisons work :)\033[0m\n");
    else                     printf("\033[031mAIL_Str comparisons fail :(\033[0m\n");
    if (test_ail_str_num_conversions()) printf("\033[032mAIL_Str number conversions work :)\033[0m\n");
    else                                printf("\033[031mAIL_Str number conversions fail :(\033[0m\n");
    if (test_ail_str_split()) printf("\033[032mAIL_Str split funcs work :)\033[0m\n");
    else                      printf("\033[031mAIL_Str split funcs fail :(\033[0m\n");
    if (test_ail_str_others()) printf("\033[032mAIL_Str other funcs work :)\033[0m\n");
    else                       printf("\033[031mAIL_Str other funcs fail :(\033[0m\n");
}
