#include "./test_assert.h"
#include "../ail_sv.h"
#include <stdbool.h>
#include <string.h>

bool test_ail_sv_inits(void)
{
    char  *a = "abcdefg";
    AIL_SV b = ail_sv_from_cstr(a);
    AIL_SV c = ail_sv_from_cstr("abcdefg");
    AIL_SV d = ail_sv_from_parts("abcdefg", strlen("abcdefg"));

    AIL_DA(char) da = ail_da_new(char);
    ail_da_pushn(&da, a, 7);
    AIL_SV e = ail_sv_from_da(da);

    ASSERT(memcmp(a, b.str, b.len) == 0);
    ASSERT(memcmp(a, c.str, c.len) == 0);
    ASSERT(memcmp(a, d.str, d.len) == 0);
    ASSERT(memcmp(a, e.str, e.len) == 0);
    return true;
}

bool test_ail_sv_num_conversions(void)
{
    // @TODO: Test from/to float conversion too
    u32 len;
    AIL_SV nonum1  = ail_sv_from_cstr("not a number");
    AIL_SV nonum2  = ail_sv_from_cstr("+not a number");
    AIL_SV nonum3  = ail_sv_from_cstr("-not a number");
    ASSERT(ail_sv_parse_unsigned(nonum1, &len) == 0);
    ASSERT(len == 0);
    ASSERT(ail_sv_parse_unsigned(nonum2, &len) == 0);
    ASSERT(len == 0);
    ASSERT(ail_sv_parse_signed(nonum1, &len) == 0);
    ASSERT(len == 0);
    ASSERT(ail_sv_parse_signed(nonum3, &len) == 0);
    ASSERT(len == 0);

    i64 zero = 0;
    i64 one  = 1;
    i64 big  = 19420;
    i64 neg  = -329;
    AIL_SV numu0   = ail_sv_new_unsigned(zero);
    AIL_SV numu1   = ail_sv_new_unsigned(one);
    AIL_SV numu2   = ail_sv_new_unsigned(big);
    AIL_SV numi0   = ail_sv_new_signed(zero);
    AIL_SV numi1   = ail_sv_new_signed(one);
    AIL_SV numi2   = ail_sv_new_signed(big);
    AIL_SV numineg = ail_sv_new_signed(neg);
    ASSERT(ail_sv_eq(ail_sv_from_cstr("0"),     numu0));
    ASSERT(ail_sv_eq(ail_sv_from_cstr("1"),     numu1));
    ASSERT(ail_sv_eq(ail_sv_from_cstr("19420"), numu2));
    ASSERT(ail_sv_eq(ail_sv_from_cstr("0"),     numi0));
    ASSERT(ail_sv_eq(ail_sv_from_cstr("1"),     numi1));
    ASSERT(ail_sv_eq(ail_sv_from_cstr("19420"), numi2));
    ASSERT(ail_sv_eq(ail_sv_from_cstr("-329"),  numineg));
    ASSERT(ail_sv_parse_unsigned(numu0,   &len) == (u64)zero);
    ASSERT(len == 1);
    ASSERT(ail_sv_parse_unsigned(numu1,   &len) == (u64)one);
    ASSERT(len == 1);
    ASSERT(ail_sv_parse_unsigned(numu2,   &len) == (u64)big);
    ASSERT(len == 5);
    ASSERT(ail_sv_parse_signed(numi0,   &len) == zero);
    ASSERT(len == 1);
    ASSERT(ail_sv_parse_signed(numi1,   &len) == one);
    ASSERT(len == 1);
    ASSERT(ail_sv_parse_signed(numi2,   &len) == big);
    ASSERT(len == 5);
    ASSERT(ail_sv_parse_signed(numineg, &len) == neg);
    ASSERT(len == 4);

    AIL_SV add = ail_sv_from_cstr("13+5");
    AIL_SV sub = ail_sv_from_cstr("-13-5");
    ASSERT(ail_sv_parse_unsigned(add, &len) == 13);
    ASSERT(len == 2);
    ASSERT(ail_sv_parse_unsigned(sub, &len) == 0);
    ASSERT(len == 0);
    ASSERT(ail_sv_parse_signed(sub, &len) == -13);
    ASSERT(len == 3);
    return true;
}

bool test_ail_sv_cmps(void)
{
    char  *s = "abc defg";
    char  c1 = 'a';
    char  c2 = 'g';
    AIL_SV a = ail_sv_from_parts(s, strlen(s));
    AIL_SV b = ail_sv_from_parts("abc defg", 8);
    AIL_SV c = ail_sv_from_parts(s, 4);
    AIL_SV d = ail_sv_from_cstr("defg");

    ASSERT(ail_sv_eq(a, b));
    ASSERT(!ail_sv_eq(a, c));
    ASSERT(!ail_sv_eq(a, d));
    ASSERT(!ail_sv_eq(c, d));
    ASSERT(ail_sv_starts_with(a, c));
    ASSERT(ail_sv_starts_with(a, b));
    ASSERT(!ail_sv_starts_with(c, a));
    ASSERT(!ail_sv_starts_with(a, d));
    ASSERT(ail_sv_starts_with_char(a, c1));
    ASSERT(ail_sv_ends_with_char(a, c2));
    ASSERT(ail_sv_ends_with(a, d));
    ASSERT(!ail_sv_ends_with(a, c));
    ASSERT(!ail_sv_ends_with(c, d));
    return true;
}

bool test_ail_sv_split(void)
{
    // @TODO: Also test the following
    // - ail_sv_split_next_char
    // - ail_sv_split_next
    // - ail_sv_split_char
    AIL_SV a = ail_sv_from_cstr("_:__:_abc_:_def_:_\nghi_:_jkl_:_");
    AIL_DA(AIL_SV) lines = ail_sv_split_lines(a, true);
    ASSERT(lines.len == 2);
    AIL_SV space = ail_sv_from_cstr("_:_");
    AIL_DA(AIL_SV) words = ail_sv_split(lines.data[0], space, true);
    AIL_DA(AIL_SV) splitted = ail_sv_split(lines.data[0], space, false);
    ASSERT(words.len == 2);
    ASSERT(splitted.len == 4);
    ASSERT(ail_sv_eq(words.data[0], ail_sv_from_cstr("abc")));
    ASSERT(ail_sv_eq(words.data[1], ail_sv_from_cstr("def")));
    ASSERT(ail_sv_eq(splitted.data[0], ail_sv_from_parts(NULL, 0)));
    AIL_Str joined_words    = ail_sv_join(words.data, words.len, ail_sv_from_cstr(" "));
    ASSERT(joined_words.str[joined_words.len] == 0);
    AIL_Str joined_splitted = ail_sv_join_da(splitted, ail_sv_from_cstr(" "));
    ASSERT(joined_splitted.str[joined_splitted.len] == 0);
    ASSERT(joined_words.len + 2 == joined_splitted.len);
    ASSERT(ail_sv_eq(joined_words, ail_sv_trim(ail_sv_from_str(joined_splitted))));
    ASSERT(ail_sv_eq(joined_words, ail_sv_offset(ail_sv_from_str(joined_splitted), 2)));
    return true;
}

bool test_ail_sv_others(void)
{
    // @TODO: Also test the following
    AIL_SV empty  = {0};
    AIL_SV hello1 = ail_sv_from_cstr("   Hello World! ");
    AIL_SV hello2 = ail_sv_from_cstr("Hello World!  ");

    ASSERT(ail_sv_eq(ail_sv_trim(hello1), ail_sv_rtrim(hello2)));
    ASSERT(ail_sv_eq(ail_sv_trim(hello1), ail_sv_ltrim(ail_sv_rtrim(hello1))));
    ASSERT(ail_sv_eq(empty, ail_sv_trim(empty)));
    ASSERT(ail_sv_eq(empty, ail_sv_ltrim(empty)));
    ASSERT(ail_sv_eq(empty, ail_sv_rtrim(empty)));

    ASSERT(ail_sv_eq(ail_sv_from_cstr("def"), ail_sv_offset(ail_sv_from_cstr("abcdef"), 3)));
    ASSERT(ail_sv_eq(empty, ail_sv_offset(empty, 3)));

    char *hello_copy = ail_sv_to_cstr(hello1);
    ASSERT(hello_copy != hello1.str); // checks that it isn't the same pointer
    ASSERT(memcmp(hello_copy, hello1.str, hello1.len) == 0);
    ASSERT(hello_copy[hello1.len] == 0);
    free(hello_copy);

    char *empty_copy = ail_sv_to_cstr(empty);
    ASSERT(empty_copy != empty.str); // checks that it isn't the same pointer
    ASSERT(memcmp(empty_copy, empty.str, empty.len) == 0);
    ASSERT(empty_copy[empty.len] == 0);
    free(empty_copy);

    AIL_SV  hello = ail_sv_from_cstr("Hello");
    AIL_SV  hi    = ail_sv_from_cstr("Hi");
    AIL_SV  a     = ail_sv_from_cstr("Hello World, Hello!");
    AIL_Str b     = ail_sv_replace(a, hello, hi);
    ASSERT(b.str[b.len] == 0);
    ASSERT(ail_sv_eq(ail_sv_from_cstr("Hi World, Hi!"), b));
    free(b.str);

    AIL_Str s = ail_sv_replace(empty, a, ail_sv_from_str(b));
    ASSERT(ail_sv_eq(s, empty));
    ASSERT(s.str[0] == 0);

    AIL_SV  xhi  = ail_sv_from_cstr("hi");
    AIL_SV  xhey = ail_sv_from_cstr("hey");
    AIL_SV  xin  = ail_sv_from_cstr("hello, hello, hello!");
    AIL_Str xout = ail_sv_replace(xin, xhi, xhey);
    ASSERT(ail_sv_eq(xin, xout));
    ASSERT(xin.str != xout.str);
    free(xout.str);
    return true;
}

int main(void)
{
    if (test_ail_sv_inits()) printf("\033[032mAIL SV initialization works :)\033[0m\n");
    else                     printf("\033[031mAIL SV initialization fails :(\033[0m\n");
    if (test_ail_sv_cmps()) printf("\033[032mAIL SV comparisons work :)\033[0m\n");
    else                    printf("\033[031mAIL SV comparisons fail :(\033[0m\n");
    if (test_ail_sv_num_conversions()) printf("\033[032mAIL SV number conversions work :)\033[0m\n");
    else                               printf("\033[031mAIL SV number conversions fail :(\033[0m\n");
    if (test_ail_sv_split()) printf("\033[032mAIL SV split funcs work :)\033[0m\n");
    else                     printf("\033[031mAIL SV split funcs fail :(\033[0m\n");
    if (test_ail_sv_others()) printf("\033[032mAIL SV other funcs work :)\033[0m\n");
    else                      printf("\033[031mAIL SV other funcs fail :(\033[0m\n");
}
