#define AIL_SV_IMPL
#include "../ail_sv.h"
#include <stdbool.h>
#include <string.h>

bool test_ail_sv_inits()
{
	char  *a = "abcdefg";
	AIL_SV b = ail_sv_from_cstr(a);
	AIL_SV c = ail_sv_from_cstr("abcdefg");
	AIL_SV d = ail_sv_from_parts("abcdefg", strlen("abcdefg"));

	AIL_DA(char) da = ail_da_new(char);
	ail_da_pushn(&da, a, 7);
	AIL_SV e = ail_sv_from_da(da);

	bool res = true;
	res &= memcmp(a, b.str, b.len) == 0;
	res &= memcmp(a, c.str, c.len) == 0;
	res &= memcmp(a, d.str, d.len) == 0;
	res &= memcmp(a, e.str, e.len) == 0;
	return res;
}

bool test_ail_sv_cmps()
{
	char  *s = "abc defg";
	char  c1 = 'a';
	char  c2 = 'g';
	AIL_SV a = ail_sv_from_parts(s, strlen(s));
	AIL_SV b = ail_sv_from_parts("abc defg", 8);
	AIL_SV c = ail_sv_from_parts(s, 4);
	AIL_SV d = ail_sv_from_cstr("defg");

	bool res = true;
	res &= ail_sv_eq(a, b);
	res &= !ail_sv_eq(a, c);
	res &= !ail_sv_eq(a, d);
	res &= !ail_sv_eq(c, d);
	res &= ail_sv_starts_with(a, c);
	res &= ail_sv_starts_with(a, b);
	res &= !ail_sv_starts_with(c, a);
	res &= !ail_sv_starts_with(a, d);
	res &= ail_sv_starts_with_char(a, c1);
	res &= ail_sv_ends_with_char(a, c2);
	res &= ail_sv_ends_with(a, d);
	res &= !ail_sv_ends_with(a, c);
	res &= !ail_sv_ends_with(c, d);
	return res;
}

bool test_ail_sv_others()
{
	bool res = true;
	AIL_SV a = ail_sv_from_cstr("_:__:_abc_:_def_:_\nghi_:_jkl_:_");
	AIL_DA(AIL_SV) lines = ail_sv_split_lines(a, true);
	res &= lines.len == 2;
	AIL_SV space = ail_sv_from_cstr("_:_");
	AIL_DA(AIL_SV) words = ail_sv_split(lines.data[0], space, true);
	AIL_DA(AIL_SV) splitted = ail_sv_split(lines.data[0], space, false);
	res &= words.len == 2;
	res &= splitted.len == 4;
	res &= ail_sv_eq(words.data[0], ail_sv_from_cstr("abc"));
	res &= ail_sv_eq(words.data[1], ail_sv_from_cstr("def"));
	res &= ail_sv_eq(splitted.data[0], ail_sv_from_parts(NULL, 0));
	AIL_SV joined_words = ail_sv_join(words.data, words.len, ail_sv_from_cstr(" "));
	AIL_SV joined_splitted = ail_sv_join_da(splitted, ail_sv_from_cstr(" "));
	res &= joined_words.len + 2 == joined_splitted.len;
	res &= ail_sv_eq(joined_words, ail_sv_trim(joined_splitted));
	res &= ail_sv_eq(joined_words, ail_sv_offset(joined_splitted, 2));
	return res;
}

int main(void)
{
	if (test_ail_sv_inits()) printf("\033[032mAIL SV initialization works :)\033[0m\n");
	else                     printf("\033[031mAIL SV initialization fails :(\033[0m\n");
	if (test_ail_sv_cmps()) printf("\033[032mAIL SV comparisons work :)\033[0m\n");
	else                    printf("\033[031mAIL SV comparisons fail :(\033[0m\n");
	if (test_ail_sv_others()) printf("\033[032mAIL SV other funcs work :)\033[0m\n");
	else                      printf("\033[031mAIL SV other funcs fail :(\033[0m\n");
}