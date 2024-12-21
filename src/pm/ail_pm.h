/*
*** Pattern Matching ***
*
* This implementation was lightly inspired by the tiny-regex-c library here: https://github.com/kokke/tiny-regex-c
*
* Currently, regex and glob patterns are supported (see list of supported syntax below)
*
* To match a pattern, you first need to compile it with `ail_pm_compile`
* Then you can match strings against the pattern with `ail_pm_match_greedy` or `ail_pm_match_lazy`
* @TODO: For now, lazy matching is not implemented yet
*
*  *** REGEX Support ***
*  * '.'        Dot, matches any character
*  * '^'        Start anchor, matches beginning of string
*  * '$'        End anchor, matches end of string
*  * '*'        Asterisk, match zero or more
*  * '+'        Plus, match one or more
*  * '?'        Question, match zero or one
*  * '[abc]'    Character class, match if one of {'a', 'b', 'c'}
*  * '[^abc]'   Inverted class, match if NOT one of {'a', 'b', 'c'}
*  * '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
*  * '\s'       Whitespace, \t \f \r \n \v and spaces
*  * '\S'       Non-whitespace
*  * '\w'       Alphanumeric, [a-zA-Z0-9_]
*  * '\W'       Non-alphanumeric
*  * '\d'       Digits, [0-9]
*  * '\D'       Non-digits
*
*  *** GLOB Support ***
*  * '*'        Asterisk, matches any character zero or more times
*  * '?'        Question, matches one or zero characters
*  * '[abc]'    Character class, match if one of {'a', 'b', 'c'}
*  * '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
*
* LICENSE
*/

#ifndef _AIL_PM_H_
#define _AIL_PM_H_

#include "../base/ail_base.h"
#include "../base/ail_str.h"
#include "../base/ail_arr.h"

typedef enum AIL_PM_Exp_Type {
    AIL_PM_EXP_REGEX,
    AIL_PM_EXP_GLOB,
    AIL_PM_EXP_COUNT,
} AIL_PM_Exp_Type;
global const char *ail_pm_exp_type_strs[] = {
    [AIL_PM_EXP_REGEX] = "Regular Expression",
    [AIL_PM_EXP_GLOB]  = "Glob",
    [AIL_PM_EXP_COUNT] = "",
};

typedef enum AIL_PM_Count_Type {
    AIL_PM_COUNT_ONCE,
    AIL_PM_COUNT_ZERO_PLUS,
    AIL_PM_COUNT_ONE_PLUS,
    AIL_PM_COUNT_ONE_OR_NONE,
    AIL_PM_COUNT_COUNT,
} AIL_PM_Count_Type;
global const char *ail_pm_count_type_strs[] = {
    [AIL_PM_COUNT_ONCE]        = "ONCE",
    [AIL_PM_COUNT_ZERO_PLUS]   = "ZERO_PLUS",
    [AIL_PM_COUNT_ONE_PLUS]    = "ONE_PLUS",
    [AIL_PM_COUNT_ONE_OR_NONE] = "ONE_OR_NONE",
    [AIL_PM_COUNT_COUNT]       = "COUNT",
};

typedef enum AIL_PM_El_Type {
    AIL_PM_EL_CHAR,
    AIL_PM_EL_RANGE,
    AIL_PM_EL_ONE_OF_CHAR,
    AIL_PM_EL_ONE_OF_RANGE,
    AIL_PM_EL_GROUP,
    AIL_PM_EL_ANY,
    AIL_PM_EL_DIGIT,
    AIL_PM_EL_ALPHA,
    AIL_PM_EL_ALPHANUM,
    AIL_PM_EL_WHITESPACE,
    AIL_PM_EL_COUNT,
} AIL_PM_El_Type;
global const char *ail_pm_el_type_strs[] = {
    [AIL_PM_EL_CHAR]         = "CHAR",
    [AIL_PM_EL_RANGE]        = "RANGE",
    [AIL_PM_EL_ONE_OF_CHAR]  = "ONE_OF_CHAR",
    [AIL_PM_EL_ONE_OF_RANGE] = "ONE_OF_RANGE",
    [AIL_PM_EL_GROUP]        = "GROUP",
    [AIL_PM_EL_ANY]          = "ANY",
    [AIL_PM_EL_DIGIT]        = "DIGIT",
    [AIL_PM_EL_ALPHA]        = "ALPHA",
    [AIL_PM_EL_ALPHANUM]     = "ALPHANUM",
    [AIL_PM_EL_WHITESPACE]   = "WHITESPACE",
    [AIL_PM_EL_COUNT]        = "",
};

typedef struct AIL_PM_Range {
    char start;
    char end;
} AIL_PM_Range;
AIL_DA_INIT(AIL_PM_Range);
AIL_SA_INIT(AIL_PM_Range);

// @Memory: This struct takes up much more space than neccessary rn (pack attributes together to improve this)
// @Note: The implementation uses the assumption that the 0-value for AIL_PM_El means that exactly one non-inverted character with c=='\0'
// @TODO: anonymous unions require C11, the rest of the ail libraries usually only require C99... is this a good enough reason to switch to C11?
typedef struct AIL_PM_El {
    AIL_PM_El_Type type;
    AIL_PM_Count_Type count;
    b32 inverted;
    union {
        char c;
        AIL_PM_Range r;
        AIL_SA(char) cs;
        AIL_SA(AIL_PM_Range) rs;
    };
} AIL_PM_El;
AIL_DA_INIT(AIL_PM_El);
AIL_SA_INIT(AIL_PM_El);

// Used as bitmasks
typedef enum AIL_PM_Pattern_Attr {
    AIL_PM_ATTR_START = 1,
    AIL_PM_ATTR_END   = 2,
} AIL_PM_Pattern_Attr;
global const char *ail_pm_attr_strs[] = {
    [AIL_PM_ATTR_START] = "START",
    [AIL_PM_ATTR_END]   = "END",
};

typedef struct AIL_PM_Pattern {
    AIL_PM_El *els;
    u32 len;
    AIL_PM_Pattern_Attr attrs;
} AIL_PM_Pattern;

typedef enum AIL_PM_Err_Type {
    AIL_PM_ERR_NONE = 0,
    AIL_PM_ERR_UNKNOWN_EXP_TYPE,
    AIL_PM_ERR_LATE_START_MARKER,
    AIL_PM_ERR_EARLY_END_MARKER,
    AIL_PM_ERR_INCOMPLETE_ESCAPE,
    AIL_PM_ERR_INVALID_COUNT_QUALIFIER,
    AIL_PM_ERR_MISSING_BRACKET,
    AIL_PM_ERR_INVALID_BRACKET,
    AIL_PM_ERR_INVALID_RANGE,
    AIL_PM_ERR_INVALID_RANGE_SYNTAX,
    AIL_PM_ERR_EMPTY_GROUP,
    AIL_PM_ERR_INCOMPLETE_RANGE,
    AIL_PM_ERR_INVALID_SPECIAL_CHAR,
    AIL_PM_ERR_COUNT,
} AIL_PM_Err_Type;
global const char *ail_pm_err_type_strs[] = {
    [AIL_PM_ERR_NONE]                    = "No Error",
    [AIL_PM_ERR_UNKNOWN_EXP_TYPE]        = "UNKNOWN_EXP_TYPE",
    [AIL_PM_ERR_LATE_START_MARKER]       = "LATE_START_MARKER",
    [AIL_PM_ERR_EARLY_END_MARKER]        = "EARLY_END_MARKER",
    [AIL_PM_ERR_INCOMPLETE_ESCAPE]       = "INCOMPLETE_ESCAPE",
    [AIL_PM_ERR_INVALID_COUNT_QUALIFIER] = "INVALID_COUNT_QUALIFIER",
    [AIL_PM_ERR_MISSING_BRACKET]         = "MISSING_BRACKET",
    [AIL_PM_ERR_INVALID_BRACKET]         = "INVALID_BRACKET",
    [AIL_PM_ERR_INVALID_RANGE]           = "INVALID_RANGE",
    [AIL_PM_ERR_INVALID_RANGE_SYNTAX]    = "INVALID_RANGE_SYNTAX",
    [AIL_PM_ERR_EMPTY_GROUP]             = "EMPTY_GROUP",
    [AIL_PM_ERR_INCOMPLETE_RANGE]        = "INCOMPLETE_RANGE",
    [AIL_PM_ERR_INVALID_SPECIAL_CHAR]    = "INVALID_SPECIAL_CHAR",
    [AIL_PM_ERR_COUNT]                   = "COUNT",
};

typedef struct AIL_PM_Err {
    AIL_PM_Err_Type type;
    u32         idx;
} AIL_PM_Err;

typedef struct AIL_PM_Comp_Char_Res {
    char c;
    AIL_PM_Err_Type e; // No error occured, if this is 0 (== AIL_PM_ERR_NONE)
} AIL_PM_Comp_Char_Res;

typedef struct AIL_PM_Comp_El_Res {
    b32 failed;
    union {
        AIL_PM_El  el;
        AIL_PM_Err err;
    };
} AIL_PM_Comp_El_Res;

typedef struct AIL_PM_Comp_Res {
    b32 failed;
    union {
        AIL_PM_Pattern pattern;
        AIL_PM_Err     err;
    };
} AIL_PM_Comp_Res;

typedef struct AIL_PM_Match {
    u32 idx;
    u32 len;
} AIL_PM_Match;


global AIL_Allocator ail_pm_tmp_allocator;


inline_func void ail_pm_init(u64 tmp_allocater_default_cap);
inline_func void ail_pm_deinit(void);

internal const char* ail_pm_exp_to_str(AIL_PM_Exp_Type type);
internal u32   ail_pm_err_to_str(AIL_PM_Err err, char *buf, u32 buflen);
internal u32   ail_pm_el_to_str (AIL_PM_El  el,  char *buf, u32 buflen);
internal u32   ail_pm_pattern_to_str(AIL_PM_Pattern pattern, char *buf, u32 buflen);

internal AIL_PM_Comp_Res ail_pm_compile_a(const char *p, u32 plen, AIL_PM_Exp_Type exp_type, AIL_Allocator allocator);
internal void         ail_pm_free_a(AIL_PM_Pattern pattern, AIL_Allocator allocator);
internal AIL_PM_Match ail_pm_match (AIL_PM_Pattern pattern, const char *s, u32 slen);
internal AIL_PM_Match ail_pm_match_lazy  (AIL_PM_Pattern pattern, const char *s, u32 len);
internal AIL_PM_Match ail_pm_match_greedy(AIL_PM_Pattern pattern, const char *s, u32 len);
internal b32          ail_pm_matches(AIL_PM_Pattern pattern, const char *s, u32 slen);
inline_func b32   ail_pm_match_eq(AIL_PM_Match a, AIL_PM_Match b);
#define ail_pm_compile(p, plen, exp_type) ail_pm_compile_a(p, plen, exp_type, ail_default_allocator)
#define ail_pm_free(pattern)              ail_pm_free_a(pattern, ail_default_allocator)

internal AIL_PM_Comp_Res ail_pm_compile_str_a(AIL_Str pattern, AIL_PM_Exp_Type type, AIL_Allocator allocator);
internal AIL_PM_Match ail_pm_match_greedy_str(AIL_PM_Pattern pattern, AIL_Str str);
internal AIL_PM_Match ail_pm_match_lazy_str  (AIL_PM_Pattern pattern, AIL_Str str);
internal AIL_PM_Match ail_pm_match_str  (AIL_PM_Pattern pattern, AIL_Str str);
internal b32          ail_pm_matches_str(AIL_PM_Pattern pattern, AIL_Str str);
#define ail_pm_compile_str(pattern, type) ail_pm_compile_str_a(pattern, type, ail_default_allocator)

internal AIL_PM_Comp_Char_Res _ail_pm_comp_range_char(const char *p, u32 plen, u32 *idx);
internal AIL_PM_Comp_El_Res   _ail_pm_comp_range(const char *p, u32 plen, u32 *idx, AIL_Allocator allocator);
internal b32 _ail_pm_match_el(AIL_PM_El el, char c);
internal u32 _ail_pm_match_immediate_greedy(AIL_PM_El *els, u32 ellen, const char *s, u32 slen);

#endif // _AIL_PM_H_


////////////////////
// IMPLEMENTATION //
////////////////////

#if !defined(AIL_NO_PM_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_PM_IMPL_GUARD_
#define _AIL_PM_IMPL_GUARD_
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

void ail_pm_init(u64 tmp_allocater_default_cap)
{
    ail_pm_tmp_allocator = ail_alloc_arena_new(tmp_allocater_default_cap, &ail_alloc_pager);
}

void ail_pm_deinit(void)
{
    ail_call_free_all(ail_pm_tmp_allocator);
    ail_pm_tmp_allocator = (AIL_Allocator){ 0 };
}

const char* ail_pm_exp_to_str(AIL_PM_Exp_Type type)
{
    if (type < AIL_PM_EXP_COUNT) {
        return ail_pm_exp_type_strs[type];
    } else {
        AIL_UNREACHABLE();
        return "<Unknown PM-Expression Type>";
    }
}

u32 ail_pm_err_to_str(AIL_PM_Err err, char *buf, u32 buflen)
{
    return snprintf(buf, buflen, "{%s idx: %u}", ail_pm_err_type_strs[err.type], err.idx);
}

u32 ail_pm_el_to_str(AIL_PM_El el, char *buf, u32 buflen)
{
    u32 n = snprintf(buf, buflen, "{%s %s", ail_pm_el_type_strs[el.type], ail_pm_count_type_strs[el.count]);
    if (el.inverted) n += snprintf(buf + n, buflen - n, " NOT");
    switch (el.type) {
        case AIL_PM_EL_CHAR:
            n += snprintf(buf + n, buflen - n, " %c", el.c);
            break;
        case AIL_PM_EL_RANGE:
            n += snprintf(buf + n, buflen - n, " %c-%c", el.r.start, el.r.end);
            break;
        case AIL_PM_EL_ONE_OF_CHAR:
            ail_assert(el.cs.len > 0);
            n += snprintf(buf + n, buflen - n, " %c", el.cs.data[0]);
            for (u32 i = 1; i < el.cs.len; i++) {
                n += snprintf(buf + n, buflen - n, " %c", el.cs.data[i]);
            }
            break;
        case AIL_PM_EL_GROUP:
            if (el.cs.len) {
                n += snprintf(buf + n, buflen - n, " %c", el.cs.data[0]);
                for (u32 i = 1; i < el.cs.len; i++) {
                    n += snprintf(buf + n, buflen - n, "%c", el.cs.data[i]);
                }
            }
            break;
        case AIL_PM_EL_ONE_OF_RANGE:
            n += snprintf(buf + n, buflen - n, " %c-%c", el.rs.data[0].start, el.rs.data[0].end);
            for (u32 i = 1; i < el.cs.len; i++) {
                n += snprintf(buf + n, buflen - n, " %c-%c", el.rs.data[i].start, el.rs.data[i].end);
            }
            break;
        case AIL_PM_EL_ANY:
        case AIL_PM_EL_DIGIT:
        case AIL_PM_EL_ALPHA:
        case AIL_PM_EL_ALPHANUM:
        case AIL_PM_EL_WHITESPACE:
            break;
        case AIL_PM_EL_COUNT:
            AIL_UNREACHABLE();
    }
    n += snprintf(buf + n, buflen - n, "}");
    return n;
}

u32 ail_pm_pattern_to_str(AIL_PM_Pattern pattern, char *buf, u32 buflen)
{
    u32 n = snprintf(buf, buflen, "{ Attrs: [ ");
    for (u32 i = 0; i < ail_arrlen(ail_pm_attr_strs); i++) {
        if (ail_pm_attr_strs[i] && (pattern.attrs & i)) n += snprintf(buf + n, buflen - n, "%s ", ail_pm_attr_strs[i]);
    }
    n += snprintf(buf + n, buflen - n, "] Els: [ ");
    for (u32 i = 0; i < pattern.len; i++) {
        n += ail_pm_el_to_str(pattern.els[i], buf + n, buflen - n);
        n += snprintf(buf + n, buflen - n, " ");
    }
    n += snprintf(buf + n, buflen - n, "] }");
    return n;
}

AIL_PM_Comp_Char_Res _ail_pm_comp_range_char(const char *p, u32 plen, u32 *idx)
{
    AIL_PM_Comp_Char_Res res = {0};
    u32 i = *idx;
    if (p[i] == '\\') {
        if (++i >= plen) res.e = AIL_PM_ERR_INCOMPLETE_ESCAPE;
    } else if (p[i] == '^' || p[i] == '-' || p[i] == '[') {
         res.e = AIL_PM_ERR_INVALID_SPECIAL_CHAR;
    } else {
        res.c = p[i];
    }
    *idx = i;
    return res;
}

AIL_PM_Comp_El_Res _ail_pm_comp_range(const char *p, u32 plen, u32 *idx, AIL_Allocator allocator)
{
    u32 i = *idx;
    AIL_PM_El el = {0};
    if (++i >= plen || i+1 >= plen) goto missing_bracket;
    if (p[i] == '^') {
        el.inverted = 1;
        i++;
    }

    if (p[i+1] == '-') {
        AIL_DA(AIL_PM_Range) ranges = ail_da_new_with_alloc(AIL_PM_Range, 4, allocator);
        for (; i < plen && p[i] != ']'; i += 3) {
            AIL_PM_Err_Type err_type = AIL_PM_ERR_NONE;
            AIL_PM_Range r;
            AIL_PM_Comp_Char_Res x = _ail_pm_comp_range_char(p, plen, &i);
            if (x.e) { err_type = x.e; goto report_err; }
            r.start = x.c;

            if (i+2 >= plen)   { err_type = AIL_PM_ERR_INCOMPLETE_RANGE;     goto report_err; }
            if (p[++i] != '-') { err_type = AIL_PM_ERR_INVALID_RANGE_SYNTAX; goto report_err; }
            ++i;

            x = _ail_pm_comp_range_char(p, plen, &i);
            if (x.e) { err_type = x.e; goto report_err; }
            r.end = x.c;
            if (r.end >= r.start) err_type = AIL_PM_ERR_INVALID_RANGE;

report_err:
            if (err_type) return (AIL_PM_Comp_El_Res){.failed=1, .err={.type=err_type, .idx=i}};
            ail_da_push(&ranges, r);
        }

        if (ranges.len == 0) {
            return (AIL_PM_Comp_El_Res){.failed=1, .err={.type=AIL_PM_ERR_EMPTY_GROUP, .idx=i-1}};
        } else if (ranges.len == 1) {
            el.type = AIL_PM_EL_RANGE;
            el.r    = ranges.data[i];
            ail_da_free(&ranges);
        } else {
            el.type = AIL_PM_EL_ONE_OF_RANGE;
            el.rs   = ail_sa_from_struct_t(AIL_PM_Range, ranges);
        }
    }
    else {
        AIL_DA(char) chars = ail_da_new_with_alloc(char, 16, allocator);
        for (; i < plen && p[i] != ']'; i++) {
            AIL_PM_Comp_Char_Res x = _ail_pm_comp_range_char(p, plen, &i);
            if (x.e) return (AIL_PM_Comp_El_Res) {.failed=1, .err={.type=x.e, .idx=i}};
            ail_da_push(&chars, x.c);
        }

        if (chars.len == 0) {
            return (AIL_PM_Comp_El_Res){.failed=1, .err={.type=AIL_PM_ERR_EMPTY_GROUP, .idx=i-1}};
        } else if (chars.len == 1) {
            el.type = AIL_PM_EL_CHAR;
            el.c    = chars.data[i];
            ail_da_free(&chars);
        } else {
            el.type = AIL_PM_EL_ONE_OF_CHAR;
            el.cs   = ail_sa_from_struct_t(char, chars);
        }
    }

    *idx = i;
    if (i < plen) { // No missing brackets
        ail_assert(p[*idx++] == ']');
        return (AIL_PM_Comp_El_Res){ .el = el };
    }
missing_bracket:
    return (AIL_PM_Comp_El_Res){.failed=1, .err={.type=AIL_PM_ERR_MISSING_BRACKET, .idx=i}};

}

AIL_PM_Comp_Res ail_pm_compile_a(const char *p, u32 plen, AIL_PM_Exp_Type exp_type, AIL_Allocator allocator)
{
    AIL_DA(AIL_PM_El)   els   = ail_da_new_with_alloc(AIL_PM_El, 32, allocator);
    AIL_PM_Pattern_Attr attrs = 0;
    if (exp_type >= AIL_PM_EXP_COUNT) return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_UNKNOWN_EXP_TYPE}};
    for (u32 i = 0; i < plen; i++) {
        char c = p[i];
        switch (exp_type) {
            case AIL_PM_EXP_REGEX: switch (c) {
                case '.':
                    ail_da_push(&els, (AIL_PM_El){.type=AIL_PM_EL_ANY});
                    break;
                case '^':
                    if (i > 0) return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_LATE_START_MARKER, .idx=i}};
                    attrs |= AIL_PM_ATTR_START;
                    break;
                case '$':
                    if (i+1 < plen) return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_EARLY_END_MARKER, .idx=i}};
                    attrs |= AIL_PM_ATTR_END;
                    break;
                case '*':
                    if (els.len == 0 || els.data[els.len-1].count) return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_INVALID_COUNT_QUALIFIER, .idx=i}};
                    els.data[els.len-1].count = AIL_PM_COUNT_ZERO_PLUS;
                    break;
                case '+':
                    if (els.len == 0 || els.data[els.len-1].count) return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_INVALID_COUNT_QUALIFIER, .idx=i}};
                    els.data[els.len-1].count = AIL_PM_COUNT_ONE_PLUS;
                    break;
                case '?':
                    if (els.len == 0 || els.data[els.len-1].count) return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_INVALID_COUNT_QUALIFIER, .idx=i}};
                    els.data[els.len-1].count = AIL_PM_COUNT_ONE_OR_NONE;
                    break;
                // case ']':
                //     return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_INVALID_BRACKET, .idx=1}};
                case '[': {
                    AIL_PM_Comp_El_Res x = _ail_pm_comp_range(p, plen, &i, allocator);
                    if (x.failed) return (AIL_PM_Comp_Res){.failed=1, .err=x.err};
                    else ail_da_push(&els, x.el);
                } break;
                case '\\':
                    if (i+1 == plen) return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_INCOMPLETE_ESCAPE, .idx=i}};
                    AIL_PM_El el = {0};
                    switch (p[++i]) {
                        case 's': el.type=AIL_PM_EL_WHITESPACE; el.inverted=0; break;
                        case 'S': el.type=AIL_PM_EL_WHITESPACE; el.inverted=1; break;
                        case 'w': el.type=AIL_PM_EL_ALPHANUM;   el.inverted=0; break;
                        case 'W': el.type=AIL_PM_EL_ALPHANUM;   el.inverted=1; break;
                        case 'd': el.type=AIL_PM_EL_DIGIT;      el.inverted=0; break;
                        case 'D': el.type=AIL_PM_EL_DIGIT;      el.inverted=1; break;
                        default:  el.c = p[i];                             break;
                    }
                    ail_da_push(&els, el);
                    break;
                default:
                    ail_da_push(&els, (AIL_PM_El){.c=c});
                    break;
            } break;
            case AIL_PM_EXP_GLOB: switch (c) {
                case '*': {
                    AIL_PM_El el = {
                        .type  = AIL_PM_EL_ANY,
                        .count = AIL_PM_COUNT_ZERO_PLUS
                    };
                    ail_da_push(&els, el);
                } break;
                case '?': {
                    AIL_PM_El el = {
                        .type  = AIL_PM_EL_ANY,
                        .count = AIL_PM_COUNT_ONE_OR_NONE
                    };
                    ail_da_push(&els, el);
                } break;
                case ']':
                    return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_INVALID_BRACKET, .idx=1}};
                case '[': {
                    AIL_PM_Comp_El_Res x = _ail_pm_comp_range(p, plen, &i, allocator);
                    if (x.failed) return (AIL_PM_Comp_Res){.failed=1, .err=x.err};
                    else ail_da_push(&els, x.el);
                } break;
                case '\\':
                    if (i+1 == plen) return (AIL_PM_Comp_Res){.failed=1, .err={.type=AIL_PM_ERR_INCOMPLETE_ESCAPE, .idx=i}};
                    ail_da_push(&els, (AIL_PM_El){.c=p[++i]});
                    break;
                default:
                    ail_da_push(&els, (AIL_PM_El){.c=c});
                    break;
            } break;
            case AIL_PM_EXP_COUNT: AIL_UNREACHABLE();
        }
    }

    return (AIL_PM_Comp_Res) { .pattern = {
        .els    = els.data,
        .len    = els.len,
        .attrs  = attrs,
    }};
}

AIL_PM_Comp_Res ail_pm_compile_str_a(AIL_Str pattern, AIL_PM_Exp_Type type, AIL_Allocator allocator)
{
    return ail_pm_compile_a((char*)pattern.data, pattern.len, type, allocator);
}

void ail_pm_free_a(AIL_PM_Pattern pattern, AIL_Allocator allocator)
{
    ail_call_free(allocator, pattern.els);
}

b32 _ail_pm_match_el(AIL_PM_El el, char c)
{
    u8 u = *(u8*)&c;
    switch (el.type) {
        case AIL_PM_EL_CHAR:         return el.c == u;
        case AIL_PM_EL_RANGE:        return el.r.start <= u && u <= el.r.end;
        case AIL_PM_EL_ONE_OF_CHAR:  return ail_str_find_char(ail_str_from_parts((u8*)el.cs.data, el.cs.len), u) >= 0;
        case AIL_PM_EL_ONE_OF_RANGE:
            for (u32 i = 0; i < el.rs.len; i++) {
                AIL_PM_Range r = el.rs.data[i];
                if (r.start <= u && u <= r.end) return true;
            }
            return false;
        case AIL_PM_EL_ANY:          return c != '\n' && c != '\r';
        case AIL_PM_EL_DIGIT:        return ail_str_is_digit(c);
        case AIL_PM_EL_ALPHA:        return ail_str_is_alpha(c);
        case AIL_PM_EL_ALPHANUM:     return ail_str_is_alpha(c) || ail_str_is_digit(c);
        case AIL_PM_EL_WHITESPACE:   return ail_str_is_space(c);
        case AIL_PM_EL_GROUP:        AIL_TODO(); break;
        case AIL_PM_EL_COUNT:        AIL_UNREACHABLE();
    }
    return false;
}

u32 _ail_pm_match_immediate_greedy(AIL_PM_El *els, u32 ellen, const char *s, u32 slen)
{
    u32 *stack = ail_call_alloc(ail_pm_tmp_allocator, ellen*sizeof(u32));
    u32 n = 0;
    u32 el_idx = 0;
match:
    while (el_idx < ellen) {
        if (n >= slen) goto backtrack;
        AIL_PM_El el = els[el_idx];
        switch (el.count) {
            case AIL_PM_COUNT_ONCE:
                if (!_ail_pm_match_el(el, s[n++])) goto backtrack;
                break;
            case AIL_PM_COUNT_ZERO_PLUS:
                while (n < slen && _ail_pm_match_el(el, s[n])) n++;
                break;
            case AIL_PM_COUNT_ONE_PLUS:
                if (!_ail_pm_match_el(el, s[n++])) goto backtrack;
                while (n < slen && _ail_pm_match_el(el, s[n])) n++;
                break;
            case AIL_PM_COUNT_ONE_OR_NONE:
                if (_ail_pm_match_el(el, s[n])) n++;
                break;
            case AIL_PM_COUNT_COUNT: AIL_UNREACHABLE();
        }
        ail_assert(el_idx < ellen);
        stack[el_idx++] = n;
    }
done:
    ail_call_free(ail_pm_tmp_allocator, stack);
    return n;
backtrack:
    for (i32 i = el_idx; i >= 0; i--) {
        if (els[i].count == AIL_PM_COUNT_ONCE) continue;
        if (i == 0 && !stack[i])               continue;
        if (i > 0  && stack[i] == stack[i-1])  continue;
        n = --stack[i];
        el_idx = i + 1;
        goto match;
    }
    n = 0;
    goto done;
}

AIL_PM_Match ail_pm_match_greedy(AIL_PM_Pattern pattern, const char *s, u32 len)
{
    if ((pattern.attrs & (AIL_PM_ATTR_START | AIL_PM_ATTR_END)) == (AIL_PM_ATTR_START | AIL_PM_ATTR_END)) {
        u32 n = _ail_pm_match_immediate_greedy(pattern.els, pattern.len, s, len);
        return (AIL_PM_Match) {
            .idx = 0,
            .len = (n == len) ? n : 0,
        };
    } else if (pattern.attrs & AIL_PM_ATTR_START) {
        return (AIL_PM_Match) {
            .idx = 0,
            .len = _ail_pm_match_immediate_greedy(pattern.els, pattern.len, s, len),
        };
    } else if (pattern.attrs & AIL_PM_ATTR_END) {
        u32 n = 0;
        for (i32 i = len - 1; !n && i >= 0; i--) {
            n = _ail_pm_match_immediate_greedy(pattern.els, pattern.len, s + i, len - i);
            if (n && i + n == len) return (AIL_PM_Match) {
                .idx = i,
                .len = n
            };
        }
    } else {
        for (u32 i = 0; i < len; i++) {
            u32 n = _ail_pm_match_immediate_greedy(pattern.els, pattern.len, s + i, len - i);
            if (n) return (AIL_PM_Match) {
                .idx = i,
                .len = n,
            };
        }
    }
    return (AIL_PM_Match) {0};
}

AIL_PM_Match ail_pm_match_lazy(AIL_PM_Pattern pattern, const char *s, u32 len)
{
    AIL_UNUSED(pattern);
    AIL_UNUSED(s);
    AIL_UNUSED(len);
    return ail_pm_match_greedy(pattern, s, len); // @TODO: Actually implement lazy pattern matching
    // AIL_TODO();
    // return (AIL_PM_Match) {0};
}

AIL_PM_Match ail_pm_match(AIL_PM_Pattern pattern, const char *s, u32 len)
{
    return ail_pm_match_greedy(pattern, s, len);
}

AIL_PM_Match ail_pm_match_str(AIL_PM_Pattern pattern, AIL_Str str)
{
    return ail_pm_match(pattern, (char*)str.data, str.len);
}

AIL_PM_Match ail_pm_match_greedy_str(AIL_PM_Pattern pattern, AIL_Str str)
{
    return ail_pm_match_greedy(pattern, (char*)str.data, str.len);
}

AIL_PM_Match ail_pm_match_lazy_str(AIL_PM_Pattern pattern, AIL_Str str)
{
    return ail_pm_match_lazy(pattern, (char*)str.data, str.len);
}

b32 ail_pm_matches(AIL_PM_Pattern pattern, const char *s, u32 slen)
{
    return ail_pm_match(pattern, s, slen).len > 0;
}

b32 ail_pm_matches_str(AIL_PM_Pattern pattern, AIL_Str str)
{
    return ail_pm_match(pattern, (char*)str.data, str.len).len > 0;
}

b32 ail_pm_match_eq(AIL_PM_Match a, AIL_PM_Match b)
{
    ail_static_assert(sizeof(AIL_PM_Match) == 8);
    return a.len == b.len && (!a.len || a.idx == b.idx);
}

AIL_WARN_POP
#endif // _AIL_PM_IMPL_GUARD_
#endif // AIL_NO_PM_IMPL
