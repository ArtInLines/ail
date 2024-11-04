// Pattern Matching Tests
// Testcases were partially created by Lily Val Richter, but were mostly taken from the sources listed below
// The copied testcases were copied in a different syntax and partially modified
// Licenses of these testcase-sources are reprinted below
//
// Sources for Testcases:
// 1. DotNet Runtime's AT&T Regex Tests: https://github.com/dotnet/runtime/blob/main/src/libraries/System.Text.RegularExpressions/tests/FunctionalTests/AttRegexTests.cs
//
// Licenses:
// 1. MIT License via .NET Foundation Licenses
/*
 *	Glenn Fowler <gsf@research.att.com>
 *	AT&T Research
 *
 * PLEASE: publish your tests so everyone can benefit
 *
 * The following license covers testregex.c and all associated test data.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of THIS SOFTWARE FILE (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following disclaimer:
 *
 * THIS SOFTWARE IS PROVIDED BY AT&T ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AT&T BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// Tests sourced from .dat files at http://gsf.cococlyde.org/download.
// Then some inputs were deleted / tweaked based on expected differences in behavior.

#define AIL_PM_IMPL
#define AIL_SV_IMPL
#include "../ail_sv.h"
#include "../ail_pm.h"
#include "./test_assert.h"

typedef struct SuccTest {
    AIL_SV pattern;
    AIL_SV testStr;
    AIL_PM_Match greedyRes;
    AIL_PM_Match lazyRes;
} SuccTest;

typedef struct FailTest {
    AIL_SV pattern;
    AIL_PM_Err err;
} FailTest;

#define SV AIL_SV_FROM_LITERAL
#define SVS_TO_MATCH(pre, match) { .idx = (pre), .len = (match) }
#define _RES_3(pre, matched, post) SVS_TO_MATCH(sizeof(AIL_STRINGIFY(pre))-1, sizeof(AIL_STRINGIFY(matched))-1)
#define _RES_2(pre, matched)       _RES_3(pre, matched, "")
#define _RES_1(pre)                _RES_2(pre, "")
#define _RES_0()                   _RES_1("")
#define RES(...) AIL_VFUNC(_RES_, __VA_ARGS__)
#define TWICE_RES(...) SV(AIL_STRINGIFY(AIL_CONCAT(__VA_ARGS__))), RES(__VA_ARGS__), RES(__VA_ARGS__)
#define FAIL(pattern, err, i) { SV(pattern), { .type = err, .idx = i } }

#define pm_match(i, l) { .idx = (i), .len = (l) }
#define pm_match_2idxs(i1, i2) pm_match(i1, (i2) - (i1))
#define pm_match_twice(i, l) pm_match(i, l), pm_match(i, l)
#define pm_match_twice_2idxs(i1, i2) pm_match_twice(i1, (i2) - (i1))

global SuccTest glob_succ_tests[] = {
    { SV("*.c"), SV("test.c"),   pm_match_twice(0, 6) },
    { SV("*.c"), SV("test.cpp"), pm_match_twice(0, 6) },
    { SV("*.c"), SV("test.cpp"), pm_match_twice(0, 6) },
};

global FailTest glob_fail_tests[] = {
    FAIL(".c\\", AIL_PM_ERR_INCOMPLETE_ESCAPE, 2),
};

global SuccTest regex_succ_basic_tests[] = {
    { SV("\\.c"),  SV("test.cpp"), pm_match_twice(4, 2) },
    { SV("\\.c"),  SV("testc.hc"), pm_match_twice(0, 0) },
    { SV("\\.c"),  SV("test.cpp.cpp"), pm_match_twice(4, 2) },
    { SV("\\.c$"), SV("test.cpp.c"), pm_match_twice(8, 2) },
    { SV("\\.c$"), SV("test.cpp"), pm_match_twice(0, 0) },
    { SV("abracadabra$"), SV("abracadabracadabra"), pm_match_twice_2idxs(7,18) },
    { SV("a...b"), SV("abababbb"), pm_match_twice_2idxs(2,7) },
    { SV("XXXXXX"), SV("..XXXXXX"), pm_match_twice_2idxs(2,8) },
    { SV("\\)"), SV("()"), pm_match_twice_2idxs(1,2) },
    { SV("a]"), SV("a]a"), pm_match_twice_2idxs(0,2) },
    { SV("}"), SV("}"), pm_match_twice_2idxs(0,1) },
    { SV("\\}"), SV("}"), pm_match_twice_2idxs(0,1) },
    { SV("\\]"), SV("]"), pm_match_twice_2idxs(0,1) },
    { SV("]"), SV("]"), pm_match_twice_2idxs(0,1) },
    { SV("{"), SV("{"), pm_match_twice_2idxs(0,1) },
    { SV("^a"), SV("ax"), pm_match_twice_2idxs(0,1) },
    { SV("\\^a"), SV("a^a"), pm_match_twice_2idxs(1,3) },
    { SV("a\\^"), SV("a^"), pm_match_twice_2idxs(0,2) },
    { SV("a$"), SV("aa"), pm_match_twice_2idxs(1,2) },
    { SV("a\\$"), SV("a$"), pm_match_twice_2idxs(0,2) },
    { SV("^$"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("\n"), SV("\n"), pm_match_twice_2idxs(0,1) },
    { SV("\na"), SV("\na"), pm_match_twice_2idxs(0,2) },
    { SV("xxx"), SV("xxx"), pm_match_twice_2idxs(0,3) },
    { SV(".*"), SV("\x0001\x00ff"), pm_match_twice_2idxs(0,2) },
    { SV("aaaa\nbbbb\ncccc\nddddd\neeeeee\nfffffff\ngggg\nhhhh\niiiii\njjjjj\nkkkkk\nllll"), SV("XaaaXbbbXcccXdddXeeeXfffXgggXhhhXiiiXjjjXkkkXlllXcbaXaaaa"), pm_match_twice_2idxs(0,0) },
    { SV("a*a*a*a*a*b"), SV("aaaaaaaaab"), pm_match_twice_2idxs(0,10) },
    { SV("^"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("$"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("^a$"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("abc"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("abc"), SV("xabcy"), pm_match_twice_2idxs(1,4) },
    { SV("abc"), SV("ababc"), pm_match_twice_2idxs(2,5) },
    { SV("ab*c"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("ab*bc"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("ab*bc"), SV("abbc"), pm_match_twice_2idxs(0,4) },
    { SV("ab*bc"), SV("abbbbc"), pm_match_twice_2idxs(0,6) },
    { SV("ab+bc"), SV("abbc"), pm_match_twice_2idxs(0,4) },
    { SV("ab+bc"), SV("abbbbc"), pm_match_twice_2idxs(0,6) },
    { SV("^abc$"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("^abc"), SV("abcc"), pm_match_twice_2idxs(0,3) },
    { SV("abc$"), SV("aabc"), pm_match_twice_2idxs(1,4) },
    { SV("^"), SV("abc"), pm_match_twice_2idxs(0,0) },
    { SV("$"), SV("abc"), pm_match_twice_2idxs(3,3) },
    { SV("a.c"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("a.c"), SV("axc"), pm_match_twice_2idxs(0,3) },
    { SV("a.*c"), SV("axyzc"), pm_match_twice_2idxs(0,5) },
    { SV("a]"), SV("a]"), pm_match_twice_2idxs(0,2) },
    { SV("a\\(b"), SV("a(b"), pm_match_twice_2idxs(0,3) },
    { SV("a\\(*b"), SV("ab"), pm_match_twice_2idxs(0,2) },
    { SV("a\\(*b"), SV("a((b"), pm_match_twice_2idxs(0,4) },
    { SV("a*"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("abcd*efg"), SV("abcdefg"), pm_match_twice_2idxs(0,7) },
    { SV("ab*"), SV("xabyabbbz"), pm_match_twice_2idxs(1,3) },
    { SV("ab*"), SV("xayabbbz"), pm_match_twice_2idxs(1,2) },
    { SV("multiple words"), SV("multiple words yeah"), pm_match_twice_2idxs(0,14) },
    { SV("abcd"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("^.+$"), SV("vivi"), pm_match_twice_2idxs(0,4) },
    { SV("\\\\XXX"), SV("\\XXX"), pm_match_twice_2idxs(0,4) },
    { SV("\\\\000"), SV("\\000"), pm_match_twice_2idxs(0,4) },
};

global SuccTest regex_succ_question_tests[] = {
    { SV("ab?bc"), SV("abbc"), pm_match_twice_2idxs(0,4) },
    { SV("ab?bc"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("ab?c"), SV("abc"), pm_match_twice_2idxs(0,3) },
};

global SuccTest regex_succ_group_tests[] = {
    // { SV("[a-m-]*"), SV("--amoma--"), pm_match_twice_2idxs(0,4) },
    { SV("[^a]"), SV("\n"), pm_match_twice_2idxs(0,1) },
    { SV("a[bc]d"), SV("abd"), pm_match_twice_2idxs(0,3) },
    { SV("a[b-d]e"), SV("ace"), pm_match_twice_2idxs(0,3) },
    { SV("a[b-d]"), SV("aac"), pm_match_twice_2idxs(1,3) },
    // { SV("a[-b]"), SV("a-"), pm_match_twice_2idxs(0,2) },
    // { SV("a[b-]"), SV("a-"), pm_match_twice_2idxs(0,2) },
    { SV("a[]]b"), SV("a]b"), pm_match_twice_2idxs(0,3) },
    // { SV("a[^bc]d"), SV("aed"), pm_match_twice_2idxs(0,3) },
    // { SV("a[^-b]c"), SV("adc"), pm_match_twice_2idxs(0,3) },
    // { SV("a[^]b]c"), SV("adc"), pm_match_twice_2idxs(0,3) },
    // { SV("[^ab]*"), SV("cde"), pm_match_twice_2idxs(0,3) },
    { SV("[abhgefdc]ij"), SV("hij"), pm_match_twice_2idxs(0,3) },
    { SV("a[bcd]*dcdcde"), SV("adcdcde"), pm_match_twice_2idxs(0,7) },
    { SV("[A-Za-z_][A-Za-z0-9_]*"), SV("alpha"), pm_match_twice_2idxs(0,5) },
    // { SV("[a-]*"), SV("--a"), pm_match_twice_2idxs(0,3) },
    // { SV("[^-]"), SV("--a"), pm_match_twice_2idxs(2,3) },
    // { SV("a[-]?c"), SV("ac"), pm_match_twice_2idxs(0,3) },
};

global SuccTest regex_succ_or_tests[] = {
    { SV("ab|abab"), SV("abbabab"), pm_match_twice_2idxs(0,2) },
    { SV("aba|bab|bba"), SV("baaabbbaba"), pm_match_twice_2idxs(5,8) },
    { SV("aba|bab"), SV("baaabbbaba"), pm_match_twice_2idxs(6,9) },
    { SV("ab|a"), SV("xabc"), pm_match_twice_2idxs(1,3) },
    { SV("ab|a"), SV("xxabc"), pm_match_twice_2idxs(2,4) },
    { SV(":::1:::0:|:::1:1:0:"), SV(":::0:::1:::1:::0:"), pm_match_twice_2idxs(8,17) },
    { SV(":::1:::0:|:::1:1:1:"), SV(":::0:::1:::1:::0:"), pm_match_twice_2idxs(8,17) },
    { SV("abaa|abbaa|abbbaa|abbbbaa"), SV("ababbabbbabbbabbbbabbbbaa"), pm_match_twice_2idxs(18,25) },
    { SV("abaa|abbaa|abbbaa|abbbbaa"), SV("ababbabbbabbbabbbbabaa"), pm_match_twice_2idxs(18,22) },
    { SV("aaac|aabc|abac|abbc|baac|babc|bbac|bbbc"), SV("baaabbbabac"), pm_match_twice_2idxs(7,11) },
    { SV("aaaa|bbbb|cccc|ddddd|eeeeee|fffffff|gggg|hhhh|iiiii|jjjjj|kkkkk|llll"), SV("XaaaXbbbXcccXdddXeeeXfffXgggXhhhXiiiXjjjXkkkXlllXcbaXaaaa"), pm_match_twice_2idxs(53,57) },
    { SV("ab|cd"), SV("abc"), pm_match_twice_2idxs(0,2) },
    { SV("ab|cd"), SV("abcd"), pm_match_twice_2idxs(0,2) },
    { SV("a|b|c|d|e"), SV("e"), pm_match_twice_2idxs(0,1) },
};

global SuccTest regex_succ_subexpr_tests[] = {
    { SV("a($)"), SV("aa"), pm_match_twice_2idxs(1,2) },
    { SV("(a.|.a.)*|(a|.a...)"), SV("aa"), pm_match_twice_2idxs(0,2) },
    { SV("a*(^a)"), SV("aa"), pm_match_twice_2idxs(0,1) },
    { SV("(..)*(...)*"), SV("a"), pm_match_twice_2idxs(0,0) },
    { SV("(..)*(...)*"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("(aa|aaa)*|(a|aaaaa)"), SV("aa"), pm_match_twice_2idxs(0,2) },
    { SV("(ab|a)(bc|c)"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("(ab)c|abc"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV(".*(/000).*"), SV("/000"), pm_match_twice_2idxs(0,4) },
    { SV(".*(\\\\000).*"), SV("\\000"), pm_match_twice_2idxs(0,4) },
    { SV("a{0}b"), SV("ab"), pm_match_twice_2idxs(1,2) },
    { SV("(a*)(b?)(b+)b{3}"), SV("aaabbbbbbb"), pm_match_2idxs(0,10), pm_match_2idxs(4,7) },
    { SV("(a*)(b{0,1})(b{1,})b{3}"), SV("aaabbbbbbb"), pm_match_2idxs(0,10), pm_match_2idxs(4,7) },
    { SV("((a|a)|a)"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("(a*)(a|aa)"), SV("aaaa"), pm_match_2idxs(0,4), pm_match_2idxs(3,4) },
    { SV("a*(a.|aa)"), SV("aaaa"), pm_match_2idxs(0,4), pm_match_2idxs(2,4) },
    { SV("(a|b)?.*"), SV("b"), pm_match_twice_2idxs(0,1) },
    { SV("(a|b)c|a(b|c)"), SV("ac"), pm_match_twice_2idxs(0,2) },
    { SV("(a|b)*c|(a|ab)*c"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("(a|b)*c|(a|ab)*c"), SV("xc"), pm_match_twice_2idxs(1,2) },
    { SV("(.a|.b).*|.*(.a|.b)"), SV("xa"), pm_match_twice_2idxs(0,2) },
    { SV("a?(ab|ba)ab"), SV("abab"), pm_match_twice_2idxs(0,4) },
    { SV("a?(ac{0}b|ba)ab"), SV("abab"), pm_match_twice_2idxs(0,4) },
    { SV("(?i)(Ab|cD)*"), SV("aBcD"), pm_match_twice_2idxs(0,4) },
    { SV("(a)(b)(c)"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("(^|[ (,;])((([Ff]eb[^ ]* *|0*2/|\\* */?)0*[6-7]))([^0-9]|$)"), SV("feb 6,"), pm_match_twice_2idxs(0,6) },
    { SV("(^|[ (,;])((([Ff]eb[^ ]* *|0*2/|\\* */?)0*[6-7]))([^0-9]|$)"), SV("2/7"), pm_match_twice_2idxs(0,3) },
    { SV("(^|[ (,;])((([Ff]eb[^ ]* *|0*2/|\\* */?)0*[6-7]))([^0-9]|$)"), SV("feb 1,Feb 6"), pm_match_twice_2idxs(5,11) },
    { SV("((((((((((((((((((((((((((((((x))))))))))))))))))))))))))))))"), SV("x"), pm_match_twice_2idxs(0,1) },
    { SV("((((((((((((((((((((((((((((((x))))))))))))))))))))))))))))))*"), SV("xx"), pm_match_twice_2idxs(0,2) },
    { SV("a?(ab|ba)*"), SV("ababababababababababababababababababababababababababababababababababababababababa"), pm_match_2idxs(0,81), pm_match_2idxs(79,81) },
    { SV("((a))"), SV("abc"), pm_match_twice_2idxs(0,1) },
    { SV("(a)b(c)"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("a+b+c"), SV("aabbabc"), pm_match_twice_2idxs(4,7) },
    { SV("a*"), SV("aaa"), pm_match_twice_2idxs(0,3) },
    { SV("(a*)*"), SV("-"), pm_match_twice_2idxs(0,0) },
    { SV("(a*)+"), SV("-"), pm_match_twice_2idxs(0,0) },
    { SV("(a*|b)*"), SV("-"), pm_match_twice_2idxs(0,0) },
    { SV("(a+|b)*"), SV("ab"), pm_match_2idxs(0,2), pm_match_2idxs(1,2) },
    { SV("(a+|b)+"), SV("ab"), pm_match_2idxs(0,2), pm_match_2idxs(1,2) },
    { SV("(a+|b)?"), SV("ab"), pm_match_2idxs(0,1), pm_match_2idxs(0,1) },
    { SV("(^)*"), SV("-"), pm_match_twice_2idxs(0,0) },
    { SV("([abc])*d"), SV("abbbcd"), pm_match_2idxs(0,6), pm_match_2idxs(4,5) },
    { SV("([abc])*bcd"), SV("abcd"), pm_match_2idxs(0,4), pm_match_2idxs(0,1) },
    { SV("(a|b|c|d|e)f"), SV("ef"), pm_match_2idxs(0,2), pm_match_2idxs(0,1) },
    { SV("((a*|b))*"), SV("-"), pm_match_twice_2idxs(0,0) },
    { SV("(ab|cd)e"), SV("abcde"), pm_match_2idxs(2,5), pm_match_2idxs(2,4) },
    { SV("(a|b)c*d"), SV("abcd"), pm_match_2idxs(1,4), pm_match_2idxs(1,2) },
    { SV("(ab|ab*)bc"), SV("abc"), pm_match_2idxs(0,3), pm_match_2idxs(0,1) },
    { SV("a([bc]*)c*"), SV("abc"), pm_match_2idxs(0,3), pm_match_2idxs(1,3) },
    { SV("a([bc]*)(c*d)"), SV("abcd"), pm_match_2idxs(0,4), pm_match_2idxs(3,4) },
    { SV("a([bc]+)(c*d)"), SV("abcd"), pm_match_2idxs(0,4), pm_match_2idxs(3,4) },
    { SV("a([bc]*)(c+d)"), SV("abcd"), pm_match_2idxs(0,4), pm_match_2idxs(2,4) },
    { SV("(ab|a)b*c"), SV("abc"), pm_match_2idxs(0,3), pm_match_2idxs(0,2) },
    { SV("((a)(b)c)(d)"), SV("abcd"), pm_match_2idxs(0,4), pm_match_2idxs(3,4) },
    { SV("^a(bc+|b[eh])g|.h$"), SV("abh"), pm_match_twice_2idxs(1,3) },
    { SV("(bc+d$|ef*g.|h?i(j|k))"), SV("effgz"), pm_match_twice_2idxs(0,5) },
    { SV("(bc+d$|ef*g.|h?i(j|k))"), SV("ij"), pm_match_2idxs(0,2), pm_match_2idxs(1,2) },
    { SV("(bc+d$|ef*g.|h?i(j|k))"), SV("reffgz"), pm_match_twice_2idxs(1,6) },
    { SV("(((((((((a)))))))))"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("(.*)c(.*)"), SV("abcde"), pm_match_2idxs(0,5), pm_match_2idxs(3,5) },
    { SV("a(bc)d"), SV("abcd"), pm_match_2idxs(0,4), pm_match_2idxs(1,3) },
    { SV("a+(b|c)*d+"), SV("aabcdd"), pm_match_2idxs(0,6), pm_match_2idxs(3,4) },
    { SV("^(.+)$"), SV("vivi"), pm_match_twice_2idxs(0,4) },
    { SV("^([^!.]+).att.com!(.+)$"), SV("gryphon.att.com!eby"), pm_match_2idxs(0,19), pm_match_2idxs(16,19) },
    { SV("^([^!]+!)?([^!]+)$"), SV("bar!bas"), pm_match_2idxs(0,7), pm_match_2idxs(4,7) },
    { SV("^([^!]+!)?([^!]+)$"), SV("foo!bas"), pm_match_2idxs(0,7), pm_match_2idxs(4,7) },
    { SV("^.+!([^!]+!)([^!]+)$"), SV("foo!bar!bas"), pm_match_2idxs(0,11), pm_match_2idxs(8,11) },
    { SV("((foo)|(bar))!bas"), SV("foo!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,3) },
    { SV("((foo)|bar)!bas"), SV("bar!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,3) },
    { SV("((foo)|bar)!bas"), SV("foo!bar!bas"), pm_match_2idxs(4,11), pm_match_2idxs(4,7) },
    { SV("((foo)|bar)!bas"), SV("foo!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,3) },
    { SV("(foo|(bar))!bas"), SV("bar!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,3) },
    { SV("(foo|(bar))!bas"), SV("foo!bar!bas"), pm_match_2idxs(4,11), pm_match_2idxs(4,7) },
    { SV("(foo|(bar))!bas"), SV("foo!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,3) },
    { SV("(foo|bar)!bas"), SV("bar!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,3) },
    { SV("(foo|bar)!bas"), SV("foo!bar!bas"), pm_match_2idxs(4,11), pm_match_2idxs(4,7) },
    { SV("(foo|bar)!bas"), SV("foo!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,3) },
    { SV("^([^!]+!)?([^!]+)$|^.+!([^!]+!)([^!]+)$"), SV("bar!bas"), pm_match_2idxs(0,7), pm_match_2idxs(4,7) },
    { SV("^([^!]+!)?([^!]+)$|^.+!([^!]+!)([^!]+)$"), SV("foo!bas"), pm_match_2idxs(0,7), pm_match_2idxs(4,7) },
    { SV("^(([^!]+!)?([^!]+)|.+!([^!]+!)([^!]+))$"), SV("bar!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,4) },
    { SV("^(([^!]+!)?([^!]+)|.+!([^!]+!)([^!]+))$"), SV("foo!bas"), pm_match_2idxs(0,7), pm_match_2idxs(0,4) },
    { SV(".*(/XXX).*"), SV("/XXX"), pm_match_twice_2idxs(0,4) },
    { SV(".*(\\\\XXX).*"), SV("\\XXX"), pm_match_twice_2idxs(0,4) },
};

global SuccTest regex_succ_subexpr_reps_tests[] = {
    { SV("((..)|(.))"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.))((..)|(.))"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.))((..)|(.))((..)|(.))"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.)){1}"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.)){2}"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.)){3}"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.))*"), SV(""), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.))((..)|(.))"), SV("a"), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.))((..)|(.))((..)|(.))"), SV("a"), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.)){2}"), SV("a"), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.)){3}"), SV("a"), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.))((..)|(.))((..)|(.))"), SV("aa"), pm_match_twice_2idxs(0,0) },
    { SV("((..)|(.)){3}"), SV("aa"), pm_match_twice_2idxs(0,0) },
    { SV("X(.?){0,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){1,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){2,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){3,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){4,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){5,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){6,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){7,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){8,}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){0,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){1,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){2,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){3,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){4,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){5,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){6,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){7,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("X(.?){8,8}Y"), SV("X1234567Y"), pm_match_twice_2idxs(0,9) },
    { SV("(a|ab|c|bcd){0,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,1) },
    { SV("(a|ab|c|bcd){1,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,1) },
    { SV("(a|ab|c|bcd){2,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(a|ab|c|bcd){3,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(a|ab|c|bcd){4,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,0) },
    { SV("(a|ab|c|bcd){0,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,1) },
    { SV("(a|ab|c|bcd){1,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,1) },
    { SV("(a|ab|c|bcd){2,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(a|ab|c|bcd){3,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(a|ab|c|bcd){4,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,0) },
    { SV("(a|ab|c|bcd)*(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,1) },
    { SV("(a|ab|c|bcd)+(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,1) },
    { SV("(ab|a|c|bcd){0,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd){1,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd){2,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd){3,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd){4,}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,0) },
    { SV("(ab|a|c|bcd){0,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd){1,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd){2,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd){3,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd){4,10}(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,0) },
    { SV("(ab|a|c|bcd)*(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
    { SV("(ab|a|c|bcd)+(d*)"), SV("ababcd"), pm_match_twice_2idxs(0,6) },
};

SuccTest regex_succ_unknown_assoc_tests[] = {
    { SV("(a|ab)(c|bcd)(d*)"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("(a|ab)(bcd|c)(d*)"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("(ab|a)(c|bcd)(d*)"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("(ab|a)(bcd|c)(d*)"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("(a*)(b|abc)(c*)"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("(a*)(abc|b)(c*)"), SV("abc"), pm_match_twice_2idxs(0,3) },
    { SV("(a|ab)(c|bcd)(d|.*)"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("(a|ab)(bcd|c)(d|.*)"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("(ab|a)(c|bcd)(d|.*)"), SV("abcd"), pm_match_twice_2idxs(0,4) },
    { SV("(ab|a)(bcd|c)(d|.*)"), SV("abcd"), pm_match_twice_2idxs(0,4) },
};

// nullsubexpr.dat
SuccTest regex_succ_null_subexpr[] = {
    { SV("(a*)*"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("(a*)*"), SV("x"), pm_match_twice_2idxs(0,0) },
    { SV("(a*)*"), SV("aaaaaa"), pm_match_twice_2idxs(0,6) },
    { SV("(a*)*"), SV("aaaaaax"), pm_match_twice_2idxs(0,6) },
    { SV("(a*)+"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("(a+)*"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("(a+)*"), SV("x"), pm_match_twice_2idxs(0,0) },
    { SV("(a+)+"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("(a+)+"), SV("x"), pm_match_twice_2idxs(0,0) },
    { SV("([a]*)*"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("([a]*)+"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("([^b]*)*"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("([^b]*)*"), SV("b"), pm_match_twice_2idxs(0,0) },
    { SV("([^b]*)*"), SV("aaaaaab"), pm_match_twice_2idxs(0,6) },
    { SV("([ab]*)*"), SV("a"), pm_match_twice_2idxs(0,1) },
    { SV("([ab]*)*"), SV("ababab"), pm_match_twice_2idxs(0,6) },
    { SV("([ab]*)*"), SV("bababa"), pm_match_twice_2idxs(0,6) },
    { SV("([ab]*)*"), SV("b"), pm_match_twice_2idxs(0,1) },
    { SV("([ab]*)*"), SV("bbbbbb"), pm_match_twice_2idxs(0,6) },
    { SV("([ab]*)*"), SV("aaaabcde"), pm_match_twice_2idxs(0,5) },
    { SV("([^a]*)*"), SV("b"), pm_match_twice_2idxs(0,1) },
    { SV("([^a]*)*"), SV("aaaaaa"), pm_match_twice_2idxs(0,0) },
    { SV("([^ab]*)*"), SV("ccccxx"), pm_match_twice_2idxs(0,6) },
    { SV("([^ab]*)*"), SV("ababab"), pm_match_twice_2idxs(0,0) },
    { SV("((z)+|a)*"), SV("zabcde"), pm_match_twice_2idxs(0,2) },
    { SV("a+?"), SV("aaaaaa"), pm_match_twice_2idxs(0,1) },
    { SV("(a)"), SV("aaa"), pm_match_twice_2idxs(0,1) },
    { SV("(a*?)"), SV("aaa"), pm_match_twice_2idxs(0,0) },
    { SV("(a)*?"), SV("aaa"), pm_match_twice_2idxs(0,0) },
    { SV("(a*?)*?"), SV("aaa"), pm_match_twice_2idxs(0,0) },
    { SV("(a*)*(x)"), SV("x"), pm_match_twice_2idxs(0,1) },
    { SV("(a*)*(x)"), SV("ax"), pm_match_twice_2idxs(0,2) },
    { SV("(a*)*(x)"), SV("axa"), pm_match_twice_2idxs(0,2) },
    { SV("(a*)+(x)"), SV("x"), pm_match_twice_2idxs(0,1) },
    { SV("(a*)+(x)"), SV("ax"), pm_match_twice_2idxs(0,2) },
    { SV("(a*)+(x)"), SV("axa"), pm_match_twice_2idxs(0,2) },
    { SV("(a*){2}(x)"), SV("x"), pm_match_twice_2idxs(0,1) },
    { SV("(a*){2}(x)"), SV("ax"), pm_match_twice_2idxs(0,2) },
    { SV("(a*){2}(x)"), SV("axa"), pm_match_twice_2idxs(0,2) },
};

global FailTest regex_fail_tests[] = {
    FAIL("*.c", AIL_PM_ERR_INVALID_COUNT_QUALIFIER, 0),
    FAIL("$^", AIL_PM_ERR_EARLY_END_MARKER, 0),
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

b32 succ_tests(SuccTest tests[], u32 count, AIL_PM_Exp_Type type)
{
    char buffer[2048] = {0};
    for (u32 i = 0; i < count; i++) {
        SuccTest t = tests[i];
        AIL_PM_Comp_Res cr = ail_pm_compile_sv_a(t.pattern, type, ail_default_allocator);
        if (cr.failed) {
            ail_pm_err_to_str(cr.err, buffer, AIL_ARRLEN(buffer));
            printf("\033[31mFailed to compile '%s' (%s)\033[0m\n", t.pattern.str, buffer);
            return false;
        }

        AIL_PM_Match gm = ail_pm_match_greedy_sv(cr.pattern, t.testStr);
        AIL_PM_Match lm = ail_pm_match_lazy_sv(cr.pattern, t.testStr);
        if (!ail_pm_match_eq(gm, t.greedyRes) || !ail_pm_match_eq(lm, t.lazyRes)) {
            ail_pm_pattern_to_str(cr.pattern, buffer, sizeof(buffer));
            printf("\033[31mPattern: '%s' -> '%s':\033[0m\n", t.pattern.str, buffer);
            printf("\033[31mTest-String: '%s'\033[0m\n", t.testStr.str);
            printf("  Greedy: Expected: (%d, %d) - Received: (%d, %d)\n", t.greedyRes.idx, t.greedyRes.len, gm.idx, gm.len);
            printf("  Lazy:   Expected: (%d, %d) - Received: (%d, %d)\n", t.lazyRes.idx, t.lazyRes.len, lm.idx, lm.len);
            return false;
        }
    }
    return true;
}

int main(void)
{
    if (succ_tests(regex_succ_basic_tests, AIL_ARRLEN(regex_succ_basic_tests), AIL_PM_EXP_REGEX)) printf("\033[32mSuccessfully matched basic regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all basic regex tests :(\033[0m\n");
    if (succ_tests(regex_succ_question_tests, AIL_ARRLEN(regex_succ_question_tests), AIL_PM_EXP_REGEX)) printf("\033[32mSuccessfully matched question regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all question regex tests :(\033[0m\n");
    if (succ_tests(regex_succ_group_tests, AIL_ARRLEN(regex_succ_group_tests), AIL_PM_EXP_REGEX)) printf("\033[32mSuccessfully matched group regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all group regex tests :(\033[0m\n");
    if (succ_tests(regex_succ_or_tests, AIL_ARRLEN(regex_succ_or_tests), AIL_PM_EXP_REGEX)) printf("\033[32mSuccessfully matched or regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all or regex tests :(\033[0m\n");
    if (succ_tests(regex_succ_subexpr_tests, AIL_ARRLEN(regex_succ_subexpr_tests), AIL_PM_EXP_REGEX)) printf("\033[32mSuccessfully matched subexpr regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all subexpr regex tests :(\033[0m\n");
    if (succ_tests(regex_succ_null_subexpr, AIL_ARRLEN(regex_succ_null_subexpr), AIL_PM_EXP_REGEX)) printf("\033[32mSuccessfully matched null-subexpr regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all null-subexpr regex tests :(\033[0m\n");
    if (succ_tests(regex_succ_subexpr_reps_tests, AIL_ARRLEN(regex_succ_subexpr_reps_tests), AIL_PM_EXP_REGEX)) printf("\033[32mSuccessfully matched repition regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all subexpr repition regex tests :(\033[0m\n");
    if (succ_tests(regex_succ_unknown_assoc_tests, AIL_ARRLEN(regex_succ_unknown_assoc_tests), AIL_PM_EXP_REGEX)) printf("\033[32mSuccessfully matched unknown-association regex tests :)\033[0m\n");
    else printf("\033[31mFailed to match all unknown-association regex tests :(\033[0m\n");

    if (succ_tests(glob_succ_tests, AIL_ARRLEN(glob_succ_tests), AIL_PM_EXP_GLOB)) printf("\033[32mSuccessfully matched glob tests :)\033[0m\n");
    else printf("\033[31mFailed to match all glob tests :(\033[0m\n");

    if (fail_tests(regex_fail_tests, AIL_ARRLEN(regex_fail_tests), AIL_PM_EXP_REGEX, false)) printf("\033[32mInvalid Regexs fail as expected :)\033[0m\n");
    else printf("\033[31mNot all invalid regexs failed to compile as expected :(\033[0m\n");

    if (fail_tests(glob_fail_tests, AIL_ARRLEN(glob_fail_tests), AIL_PM_EXP_GLOB, false)) printf("\033[32mInvalid Globs fail as expected :)\033[0m\n");
    else printf("\033[31mNot all invalid globs failed to compile as expected :(\033[0m\n");

    return 0;
}
