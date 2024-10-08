#define AIL_PM_IMPL
#define AIL_SV_IMPL
#include "../ail_sv.h"
#include "../ail_pm.h"
#include "./test_assert.h"

typedef struct SuccTest {
    AIL_SV pattern;
    AIL_SV greedy[3];
    AIL_SV lazy[3];
} SuccTest;

typedef struct FailTest {
    AIL_SV pattern;
    AIL_PM_Err err;
} FailTest;

#define SV AIL_SV_FROM_LITERAL
#define _RES_3(pre, matched, post) {SV(pre), SV(matched), SV(post)}
#define _RES_2(pre, matched)       _RES_3(pre, matched, "")
#define _RES_1(pre)                _RES_2(pre, "")
#define _RES_0()                   _RES_1("")
#define RES(...) AIL_VFUNC(_RES_, __VA_ARGS__)
#define TWICE_RES(...) RES(__VA_ARGS__), RES(__VA_ARGS__)
#define FAIL(pattern, err, i) { SV(pattern), { .type = err, .idx = i } }

global SuccTest regex_succ_tests[] = {
    { SV("\\.c"),  TWICE_RES("test", ".c", "pp") },
    { SV("\\.c"),  TWICE_RES("", "", "testc.hc") },
    { SV("\\.c"),  TWICE_RES("test", ".c", "pp.cpp") },
    { SV("\\.c$"), TWICE_RES("test.cpp", ".c") },
    { SV("\\.c$"), TWICE_RES("", "", "test.cpp") },
};

global SuccTest glob_succ_tests[] = {
    { SV("*.c"), TWICE_RES("", "test.c"), },
    { SV("*.c"), TWICE_RES("", "test.c", "pp") },
    { SV("*.c"), TWICE_RES("", "test.cpp.c") },
};

global FailTest regex_fail_tests[] = {
    FAIL("*.c", AIL_PM_ERR_INVALID_COUNT_QUALIFIER, 0),
};

global FailTest glob_fail_tests[] = {
    FAIL(".c\\", AIL_PM_ERR_INCOMPLETE_ESCAPE, 2),
};


b32 pm_err_eq(AIL_PM_Err a, AIL_PM_Err b)
{
    return a.idx == b.idx && a.type == b.type;
}

b32 fail_tests(FailTest tests[], u32 count, AIL_PM_Exp_Type type, b32 verbose)
{
    char buffer[2048] = {0};
    for (u32 i = 0; i < count; i++) {
        AIL_SV pattern = tests[i].pattern;
        AIL_PM_Err err     = tests[i].err;
        AIL_PM_Comp_Res cr = ail_pm_compile_sv_a(pattern, type, ail_default_allocator);
        if (!cr.failed) {
            printf("\033[31mNo error thrown when compiling bad pattern: '%s'\033[0m\n", pattern.str);
            return false;
        }
        if (!pm_err_eq(err, cr.err)) {
            ail_pm_err_to_str(cr.err, buffer, sizeof(buffer));
            printf("\033[31m'%s': Expected %s", pattern.str, buffer);
            ail_pm_err_to_str(err, buffer, sizeof(buffer));
            printf(" - Received %s\033[0m\n", buffer);
            return false;
        }
        if (verbose) {
            ail_pm_err_to_str(cr.err, buffer, sizeof(buffer));
            printf("\033[32m'%s' -> %s\033[0m\n", pattern.str, buffer);
        }
    }
    return true;
}

b32 succ_tests(SuccTest tests[], u32 count, AIL_PM_Exp_Type type, b32 verbose)
{
    char buffer[2048] = {0};
    for (u32 i = 0; i < count; i++) {
        AIL_SV pattern  = tests[i].pattern;
        AIL_SV gpre     = tests[i].greedy[0];
        AIL_SV gmatched = tests[i].greedy[1];
        AIL_SV gpost    = tests[i].greedy[2];
        AIL_SV lpre     = tests[i].lazy[0];
        AIL_SV lmatched = tests[i].lazy[1];
        AIL_SV lpost    = tests[i].lazy[2];
        AIL_SV sv       = ail_sv_from_str(ail_sv_concat(ail_sv_from_str(ail_sv_concat(gpre, gmatched)), gpost));
        ASSERT(ail_sv_eq(sv, ail_sv_from_str(ail_sv_concat(ail_sv_from_str(ail_sv_concat(lpre, lmatched)), lpost))));

        AIL_PM_Comp_Res cr = ail_pm_compile_sv_a(pattern, type, ail_default_allocator);
        if (cr.failed) {
            printf("\033[31mFailed to compile '%s'\033[0m\n", pattern.str);
            return false;
        }

        AIL_PM_Match gm = ail_pm_match_greedy_sv(cr.pattern, sv);
        AIL_Str      g1 = ail_str_new_sv(ail_sv_from_parts(sv.str, gm.idx));
        AIL_Str      g2 = ail_str_new_sv(ail_sv_from_parts(sv.str + gm.idx, gm.len));
        AIL_Str      g3 = ail_str_new_sv(ail_sv_offset(sv, gm.idx + gm.len));
        AIL_PM_Match lm = ail_pm_match_lazy_sv(cr.pattern, sv);
        AIL_Str      l1 = ail_str_new_sv(ail_sv_from_parts(sv.str, lm.idx));
        AIL_Str      l2 = ail_str_new_sv(ail_sv_from_parts(sv.str + lm.idx, lm.len));
        AIL_Str      l3 = ail_str_new_sv(ail_sv_offset(sv, lm.idx + lm.len));

        if (!ail_sv_eq(gpre, g1) || !ail_sv_eq(gmatched, g2) || !ail_sv_eq(gpost, g3) ||
            !ail_sv_eq(lpre, l1) || !ail_sv_eq(lmatched, l2) || !ail_sv_eq(lpost, l3)) {
            ail_pm_pattern_to_str(cr.pattern, buffer, sizeof(buffer));
            printf("\033[31mPattern: '%s' -> '%s':\033[0m\n", pattern.str, buffer);
            printf("  Greedy: Expected: '%s\033[36m%s\033[0m%s' - Received: '%s\033[36m%s\033[0m%s' \n", ail_sv_to_cstr(gpre), ail_sv_to_cstr(gmatched), ail_sv_to_cstr(gpost), g1.str, g2.str, g3.str);
            printf("  Lazy:   Expected: '%s\033[36m%s\033[0m%s' - Received: '%s\033[36m%s\033[0m%s' \n", ail_sv_to_cstr(lpre), ail_sv_to_cstr(lmatched), ail_sv_to_cstr(lpost), l1.str, l2.str, l3.str);
            return false;
        }
        if (verbose) {
            printf("\033[32m'%s': Greedy: '%s\033[36m%s\033[32m%s' - Lazy: '%s\033[36m%s\033[32m%s'\033[0m\n", pattern.str, g1.str, g2.str, g3.str, l1.str, l2.str, l3.str);
        }
    }
    return true;
}

int main(void)
{
    b32 verbose = false;
    if (succ_tests(regex_succ_tests, AIL_ARRLEN(regex_succ_tests), AIL_PM_EXP_REGEX, verbose)) printf("\033[32mSuccessfully matched regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all regex tests :(\033[0m\n");

    if (succ_tests(glob_succ_tests, AIL_ARRLEN(glob_succ_tests), AIL_PM_EXP_GLOB, verbose)) printf("\033[32mSuccessfully matched glob tests :)\033[0m\n");
    else printf("\033[31mFailed to match all glob tests :(\033[0m\n");

    if (fail_tests(regex_fail_tests, AIL_ARRLEN(regex_fail_tests), AIL_PM_EXP_REGEX, false)) printf("\033[32mInvalid Regexs fail as expected :)\033[0m\n");
    else printf("\033[31mNot all invalid regexs failed to compile as expected :(\033[0m\n");

    if (fail_tests(glob_fail_tests, AIL_ARRLEN(glob_fail_tests), AIL_PM_EXP_GLOB, false)) printf("\033[32mInvalid Globs fail as expected :)\033[0m\n");
    else printf("\033[31mNot all invalid globs failed to compile as expected :(\033[0m\n");

    return 0;
}
