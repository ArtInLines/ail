// String View
//
// A String View (sometimes also called string-slice, here AIL_SV) is a lightweight structure
// containing a pointer to a string and its length.
// None of the functions here change the underlying string.
// In other words: A String View does not own the memory it points at.
//
// This library also contains a String structure (here AIL_Str), which does own its memory.
// The AIL_Str is guarantueed to be null-terminated, but stores its length as well.
//
// Any String View function that would require changes to the underlying string (e.g. ail_sv_replace)
// instead allocates a new string, on which to produce the desired changes.
// Usually this newly produced string is then returned as an AIL_Str
// Any function that does allocate a new string, is annotated with '@Important'
// and a reminder to free said memory.
//
// To prevent you from needing to copy the string back to a null-terminated string
// (see ail_sv_copy_to_cstr) as much as possible, this library implements most standard
// C functions that work with strings to allow working with String-Views instead.
// This includes among others also formatting and printing functions.
//
// This library was inspired by tsoding's SV library (https://github.com/tsoding/sv)
//
// @Note: As this library is not finished, some of the above mentioned functions
// might not yet be Implemented. This should be fixed soon
//
// LICENSE
/*
Copyright (c) 2024 Lily Val Richter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef AIL_SV_H_
#define AIL_SV_H_

#define AIL_ALL_IMPL
#include "ail.h"

#ifndef AIL_SV_DEF
#ifdef  AIL_DEF
#define AIL_SV_DEF AIL_DEF
#else
#define AIL_SV_DEF
#endif // AIL_DEF
#endif // AIL_SV_DEF
#ifndef AIL_SV_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_SV_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_SV_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_SV_DEF_INLINE

#if !defined(AIL_SV_MALLOC) && !defined(AIL_SV_FREE)
#if  defined(AIL_MALLOC)    &&  defined(AIL_FREE)
#define AIL_SV_MALLOC AIL_MALLOC
#define AIL_SV_FREE   AIL_FREE
#else
#include <stdlib.h>
#define AIL_SV_MALLOC(size) malloc(size)
#define AIL_SV_FREE(ptr)    free(ptr)
#endif
#elif !defined(AIL_SV_MALLOC) || !defined(AIL_SV_FREE)
#error "You must define both AIL_SV_MALLOC and AIL_SV_FREE, or neither."
#endif

#ifndef AIL_SV_MEMCPY
#ifdef  AIL_MEMCMPY
#define AIL_SV_MEMCPY AIL_MEMCMPY
#else
#include <string.h>
#define AIL_SV_MEMCPY(dst, src, n) memcpy(dst, src, n)
#endif
#endif

typedef struct AIL_SV {
    const char *str;
    u64 len;
} AIL_SV;
AIL_DA_INIT(AIL_SV);

typedef struct AIL_Str {
    char *str;
    u64   len;
} AIL_Str;
AIL_DA_INIT(AIL_Str);

typedef struct AIL_SV_Find_Of_Res {
    i64 sv_idx;
    i32 needle_idx;
} AIL_SV_Find_Of_Res;

// @TODO: Implement the following functions:
// - printf (whole family of printf functions)
// - split by list of splitters
// - parse float
// - convert to float


////////////////////
// Creating a Str //
////////////////////

#define AIL_STR_FROM_LITERAL(clit) { .str = (clit), .len = sizeof(clit)-1 }
#define AIL_STR_FROM_LITERAL_T(clit) (AIL_Str)AIL_STR_FROM_LITERAL(clit)
AIL_SV_DEF_INLINE AIL_Str ail_str_from_parts(char *s, u64 len);
AIL_SV_DEF_INLINE AIL_Str ail_str_from_cstr (char *s);
AIL_SV_DEF_INLINE AIL_Str ail_str_from_da(AIL_DA(char) str);
AIL_SV_DEF AIL_Str ail_str_from_unsigned(u64 num);
AIL_SV_DEF AIL_Str ail_str_from_signed  (i64 num);
AIL_SV_DEF AIL_Str ail_str_from_float   (f64 num);

// @Important: Copies the underlying string to a new memory region. Remember to free the new AIL_Str
AIL_SV_DEF AIL_Str ail_str_from_sv(AIL_SV sv);

AIL_SV_DEF void ail_str_free(AIL_Str str);

// @Note: Same as ail_str_from_sv(sv).str
// @Important: Remmber to free the string you receive from ail_sv_copy_to_cstr
AIL_SV_DEF char* ail_sv_copy_to_cstr(AIL_SV sv);


///////////////////
// Creating a SV //
///////////////////

#define AIL_SV_FROM_LITERAL(clit) { .str = (clit), .len = sizeof(clit)-1 }
#define AIL_SV_FROM_LITERAL_T(clit) (AIL_SV)AIL_SV_FROM_LITERAL(clit)
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_parts(const char *str, u64 len);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_cstr (const char *str);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_str(AIL_Str str);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_da(AIL_DA(char) str);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_unsigned(u64 num);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_signed  (i64 num);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_float   (f64 num);


/////////////////////
// Parsing numbers //
/////////////////////

// When parsing, `len` will be set to he number of bytes that the number takes in the string
// @Important: If `sv` doesn't start witha digit, '+', '-' or '.', 0 will be returned with len set to 0
// To check for errors, you thus need to check whether `len` is 0
AIL_SV_DEF u64 ail_sv_parse_unsigned(AIL_SV sv, u32 *len);
AIL_SV_DEF i64 ail_sv_parse_signed  (AIL_SV sv, u32 *len);
AIL_SV_DEF f64 ail_sv_parse_float   (AIL_SV sv, u32 *len);


////////////////////////////////
// Comparison, Prefix, Suffix //
////////////////////////////////

#define ail_sv_eq(a, b)  ail_sv_full_eq ((a).str, (a).len, (b).str, (b).len)
#define ail_sv_cmp(a, b) ail_sv_full_cmp((a).str, (a).len, (b).str, (b).len)
AIL_SV_DEF bool ail_sv_full_eq (const char *astr, u64 alen, const char *bstr, u64 blen);
AIL_SV_DEF i32  ail_sv_full_cmp(const char *astr, u64 alen, const char *bstr, u64 blen);
AIL_SV_DEF bool ail_sv_starts_with     (AIL_SV str, AIL_SV prefix);
AIL_SV_DEF bool ail_sv_starts_with_char(AIL_SV str, char   prefix);
AIL_SV_DEF bool ail_sv_ends_with     (AIL_SV str, AIL_SV suffix);
AIL_SV_DEF bool ail_sv_ends_with_char(AIL_SV str, char   suffix);


//////////////
// Index of //
//////////////

// Get the first index in `str` that the substring `neddle` appears at
AIL_SV_DEF i64 ail_sv_find(AIL_SV str, AIL_SV needle);
// Get the last index in `str` that the substring `neddle` appears at
AIL_SV_DEF i64 ail_sv_find_last(AIL_SV str, AIL_SV needle);
// Get the first index in `str` that any of the substrings in `neddles` appears at
AIL_SV_DEF AIL_SV_Find_Of_Res ail_sv_find_of(AIL_SV str, AIL_SV *needles, i32 needles_count);
// Get the last index in `str` that any of the substrings in `neddles` appears at
AIL_SV_DEF AIL_SV_Find_Of_Res ail_sv_find_last_of(AIL_SV str, AIL_SV *needles, i32 needles_count);

// Get the first index in `str` that `needle` appears at
AIL_SV_DEF i64 ail_sv_find_char(AIL_SV str, char needle);
// Get the last index in `str` that `needle` appears at
AIL_SV_DEF i64 ail_sv_find_last_char(AIL_SV str, char needle);
// Get the first index in `str` that any of the `neddles` appears at
AIL_SV_DEF AIL_SV_Find_Of_Res ail_sv_find_of_chars(AIL_SV str, char *needles, i32 needles_count);
// Get the last index in `str` that any of the `neddles` appears at
AIL_SV_DEF AIL_SV_Find_Of_Res ail_sv_find_last_of_chars(AIL_SV str, char *needles, i32 needles_count);

////////////////////////
// Splitting, Joining //
////////////////////////

// @Note: There are both lazy and greedy functions for splitting a string
// The lazy splitting functions all have 'next' in their name and change the inputted original SV
// After calling a split_next function, the original SV is changed to point at the start of the string
// afer the first splitting result. Take the following example:
/*
    bool ignore_empty = true;
    AIL_SV sv     = ail_sv_from_cstr(",,Hello,World,");
    AIL_SV first  = ail_sv_split_next_char(&sv, ',', ignore_empty);
    // sv points at ",World," now
    AIL_SV second = ail_sv_split_next_char(&sv, ',', ignore_empty);
    assert(ail_sv_eq(ail_sv_from_cstr("Hello"), first));
    assert(ail_sv_eq(ail_sv_from_cstr("World"), first));
*/

// @Note: Every splitting function further takes a boolean parameter determining whether empty values should be ignored
// If `ignore_empty` was false in the above example, first would be an empty string and second would be "Hello"

AIL_SV_DEF AIL_SV ail_sv_split_next_char(AIL_SV *sv, char   split_by, bool ignore_empty);
AIL_SV_DEF AIL_SV ail_sv_split_next     (AIL_SV *sv, AIL_SV split_by, bool ignore_empty);
AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_char(AIL_SV sv, char   split_by, bool ignore_empty);
AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split     (AIL_SV sv, AIL_SV split_by, bool ignore_empty);
AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_lines(AIL_SV sv, bool ignore_empty);
AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_whitespace(AIL_SV sv, bool ignore_empty);

// @Note: rev_join joins the splitted substrings in reverse order
// @Important: To avoid memory leaks, make sure to free the underlying string
AIL_SV_DEF AIL_Str ail_sv_join    (AIL_SV *list, u64 n, AIL_SV joiner);
AIL_SV_DEF AIL_Str ail_sv_rev_join(AIL_SV *list, u64 n, AIL_SV joiner);
AIL_SV_DEF AIL_Str ail_sv_join_da    (AIL_DA(AIL_SV) list, AIL_SV joiner);
AIL_SV_DEF AIL_Str ail_sv_rev_join_da(AIL_DA(AIL_SV) list, AIL_SV joiner);


//////////////////
// Miscellanous //
//////////////////

AIL_SV_DEF bool ail_sv_is_space(char c);

// Receive a new SV, that points at the same underlying string as `sv` but offset by `offset` bytes.
// If `offset` is greater than the length of `sv`, then a SV with zero length is returned
AIL_SV_DEF AIL_SV ail_sv_offset(AIL_SV sv, u64 offset);

// Receive a new SV, that has the beginning and ending whitespace from `sv` removed
AIL_SV_DEF AIL_SV ail_sv_trim(AIL_SV sv);
AIL_SV_DEF AIL_SV ail_sv_ltrim(AIL_SV sv);
AIL_SV_DEF AIL_SV ail_sv_rtrim(AIL_SV sv);

// Concatenate two String-Views to a single String
// @Important: To avoid memory leaks, make sure to free the underlying string
AIL_SV_DEF AIL_Str ail_sv_concat(AIL_SV a, AIL_SV b);

// Receive a new SV, that has all appearances of `to_replace` replaced with `replace_with`
// @Note: Since this only works by changing the underlying string, an allocation and copy of the original string is required
// This operation can thus potentially be relatively expensive.
// @Important: To avoid memory leaks, make sure to free the underlying string
// Make sure that you don't lose the pointer to the original start of the string. See the following for example:
/*
    AIL_SV hello = ail_sv_from_cstr("Hello");
    AIL_SV hi    = ail_sv_from_cstr("Hi");
    AIL_SV a     = ail_sv_from_cstr("  Hello World! ");

    // This would lose the original pointer:
    // AIL_SV b = ail_sv_trim(ail_sv_from_str(ail_sv_replace(a, hello, hi)));

    // This keeps the original pointer
    AIL_Str b         = ail_sv_replace(a, hello, hi);
    AIL_SV trimmed_b = ail_sv_trim(ail_sv_from_str(b));
    uses_the_sv_somehow(trimmed_b);
    free(b.str);
    // @Note: if AIL_SV_MALLOC is defined as something other than malloc, you probably need to use a different function for freeing too
*/
AIL_SV_DEF AIL_Str ail_sv_replace(AIL_SV sv, AIL_SV to_replace, AIL_SV replace_with);

#endif // AIL_SV_H_


////////////////////
// IMPLEMENTATION //
////////////////////

#ifdef AIL_SV_IMPL
#ifndef _AIL_SV_IMPL_GUARD_
#define _AIL_SV_IMPL_GUARD_

AIL_SV_DEF_INLINE AIL_SV ail_sv_from_parts(const char *s, u64 len)
{
    return (AIL_SV) {
        .str = s,
        .len = len,
    };
}

AIL_SV_DEF_INLINE AIL_SV ail_sv_from_da(AIL_DA(char) str)
{
    return ail_sv_from_parts(str.data, str.len);
}

AIL_SV_DEF_INLINE AIL_SV ail_sv_from_str(AIL_Str str)
{
    return ail_sv_from_parts(str.str, str.len);
}

AIL_SV_DEF_INLINE AIL_SV ail_sv_from_cstr(const char *str)
{
    u64 len = 0;
    while (str[len]) len++;
    return ail_sv_from_parts(str, len);
}

AIL_SV_DEF_INLINE AIL_Str ail_str_from_parts(char *s, u64 len)
{
    return (AIL_Str) {
        .str = s,
        .len = len,
    };
}

AIL_SV_DEF_INLINE AIL_Str ail_str_from_cstr(char *s)
{
    u64 len = 0;
    while (s[len]) len++;
    return ail_str_from_parts(s, len);
}

AIL_SV_DEF_INLINE AIL_Str ail_str_from_da(AIL_DA(char) str)
{
    return ail_str_from_parts(str.data, str.len);
}

AIL_SV_DEF AIL_Str ail_str_from_sv(AIL_SV sv)
{
    char *out = AIL_SV_MALLOC(sv.len + 1);
    AIL_SV_MEMCPY(out, sv.str, sv.len);
    out[sv.len] = 0;
    return ail_str_from_parts(out, sv.len);
}

AIL_SV_DEF void ail_str_free(AIL_Str str)
{
    AIL_SV_FREE(str.str);
}

AIL_SV_DEF char* ail_sv_copy_to_cstr(AIL_SV sv)
{
    return ail_str_from_sv(sv).str;
}

AIL_SV_DEF AIL_Str ail_str_from_unsigned(u64 num)
{
    // @Decide: max u64 is 20 chars long - should the default capacity be 24 then?
    if (num == 0) return (AIL_Str) { .str = "0", .len = 1 };
    AIL_DA(char) da = ail_da_new_with_cap(char, 16);
    while (num > 0) {
        ail_da_push(&da, '0' + (num % 10));
        num /= 10;
    }
    for (u64 i = 0, j = da.len - 1; i < da.len/2; i++, j--) {
        char tmp   = da.data[i];
        da.data[i] = da.data[j];
        da.data[j] = tmp;
    }
    return (AIL_Str) {
        .str = da.data,
        .len = da.len,
    };
}

AIL_SV_DEF AIL_Str ail_str_from_signed(i64 num)
{
    if (num == 0) return (AIL_Str) { .str = "0", .len = 1 };
    AIL_DA(char) da = ail_da_new_with_cap(char, 16);
    bool is_neg = num < 0;
    if (is_neg) {
        ail_da_push(&da, '-');
        num *= -1;
    }
    while (num > 0) {
        ail_da_push(&da, '0' + (num % 10));
        num /= 10;
    }
    for (u64 i = (u64)is_neg, j = da.len - 1; i < da.len/2; i++, j--) {
        char tmp   = da.data[i];
        da.data[i] = da.data[j];
        da.data[j] = tmp;
    }
    return (AIL_Str) {
        .str = da.data,
        .len = da.len,
    };
}

AIL_SV_DEF AIL_Str ail_str_from_float(f64 num)
{
    (void)num;
    AIL_TODO();
    return (AIL_Str) {0};
}

AIL_SV_DEF_INLINE AIL_SV ail_sv_from_unsigned(u64 num)
{
    return ail_sv_from_str(ail_str_from_unsigned(num));
}

AIL_SV_DEF_INLINE AIL_SV ail_sv_from_signed(i64 num)
{
    return ail_sv_from_str(ail_str_from_signed(num));
}

AIL_SV_DEF_INLINE AIL_SV ail_sv_from_float(f64 num)
{
    return ail_sv_from_str(ail_str_from_float(num));
}


AIL_SV_DEF u64 ail_sv_parse_unsigned(AIL_SV sv, u32 *len)
{
    u64 out = 0;
    u64 i   = 0;
    if (sv.len && sv.str[0] == '+') i++;
    while (i < sv.len && sv.str[i] >= '0' && sv.str[i] <= '9') {
        out *= 10;
        out += sv.str[i] - '0';
        i++;
    }
    if (AIL_UNLIKELY(i == 1 && sv.str[0] == '+')) *len = 0;
    else *len = (u32)i;
    return out;
}

AIL_SV_DEF i64 ail_sv_parse_signed(AIL_SV sv, u32 *len)
{
    bool is_neg = false;
    u64 out = 0;
    u64 i   = 0;
    if (sv.len) {
        if      (sv.str[0] == '+') i++;
        else if (sv.str[0] == '-') {
            is_neg = true;
            i++;
        }
    }
    while (i < sv.len && sv.str[i] >= '0' && sv.str[i] <= '9') {
        out *= 10;
        out += sv.str[i] - '0';
        i++;
    }
    if (is_neg) out *= -1;
    if (AIL_UNLIKELY(i == 1 && (is_neg || sv.str[0] == '+'))) *len = 0;
    else *len = (u32)i;
    return out;
}

AIL_SV_DEF f64 ail_sv_parse_float(AIL_SV sv, u32 *len)
{
    (void)sv;
    (void)len;
    AIL_TODO();
    return 0;
}

AIL_SV_DEF bool ail_sv_full_eq(const char *astr, u64 alen, const char *bstr, u64 blen)
{
    if (alen != blen) return false;
    for (u32 i = 0; i < alen; i++) {
        if (astr[i] != bstr[i]) return false;
    }
    return true;
}

AIL_SV_DEF i32  ail_sv_full_cmp(const char *astr, u64 alen, const char *bstr, u64 blen)
{
    for (u64 i = 0; i < alen && i < blen; i++) {
        if (astr[i] != bstr[i]) return astr[i] - bstr[i];
    }
    if      (alen  > blen) return astr[blen];
    else if (alen == blen) return 0;
    else                   return bstr[alen];
}

AIL_SV_DEF bool ail_sv_starts_with(AIL_SV str, AIL_SV prefix)
{
    if (prefix.len > str.len) return false;
    for (u32 i = 0; i < prefix.len; i++) {
        if (str.str[i] != prefix.str[i]) return false;
    }
    return true;
}

AIL_SV_DEF bool ail_sv_starts_with_char(AIL_SV str, char prefix)
{
    return str.len > 0 && str.str[0] == prefix;
}

AIL_SV_DEF bool ail_sv_ends_with(AIL_SV str, AIL_SV suffix)
{
    if (suffix.len > str.len) return false;
    for (u32 i = suffix.len; i > 0; i--) {
        if (str.str[str.len - i] != suffix.str[suffix.len - i]) return false;
    }
    return true;
}

AIL_SV_DEF bool ail_sv_ends_with_char(AIL_SV str, char suffix)
{
    return str.len > 0 && str.str[str.len - 1] == suffix;
}

AIL_SV_DEF i64 ail_sv_find(AIL_SV str, AIL_SV needle)
{
    for (u64 i = 0; i <= str.len - needle.len; i++) {
        if (ail_sv_starts_with(ail_sv_offset(str, i), needle)) return (i64)i;
    }
    return -1;
}

AIL_SV_DEF i64 ail_sv_find_last(AIL_SV str, AIL_SV needle)
{
    for (i64 i = str.len - needle.len; i >= 0; i--) {
        if (ail_sv_starts_with(ail_sv_offset(str, i), needle)) return i;
    }
    return -1;
}

AIL_SV_DEF AIL_SV_Find_Of_Res ail_sv_find_of(AIL_SV str, AIL_SV *needles, i32 needles_count)
{
    for (i64 i = 0; i < (i64)str.len; i++) {
        AIL_SV offset = ail_sv_offset(str, i);
        for (i32 j = 0; j < needles_count; j++) {
            if (ail_sv_starts_with(offset, needles[j])) return (AIL_SV_Find_Of_Res){ .sv_idx = i, .needle_idx = j };
        }
    }
    return (AIL_SV_Find_Of_Res){ -1, -1 };
}

AIL_SV_DEF AIL_SV_Find_Of_Res ail_sv_find_last_of(AIL_SV str, AIL_SV *needles, i32 needles_count)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
       AIL_SV offset = ail_sv_offset(str, i);
        for (i32 j = 0; j < needles_count; j++) {
            if (ail_sv_starts_with(offset, needles[j])) return (AIL_SV_Find_Of_Res){ .sv_idx = i, .needle_idx = j };
        }
    }
    return (AIL_SV_Find_Of_Res){ -1, -1 };
}

AIL_SV_DEF i64 ail_sv_find_char(AIL_SV str, char needle)
{
    for (u64 i = 0; i < str.len; i++) {
        if (str.str[i] == needle) return (i64)i;
    }
    return -1;
}

AIL_SV_DEF i64 ail_sv_find_last_char(AIL_SV str, char needle)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
        if (str.str[i] == needle) return i;
    }
    return -1;
}

AIL_SV_DEF AIL_SV_Find_Of_Res ail_sv_find_of_chars(AIL_SV str, char *needles, i32 needles_count)
{
    for (i64 i = 0; i < (i64)str.len; i++) {
        char c = str.str[i];
        for (i32 j = 0; j < needles_count; j++) {
            if (c == needles[j]) return (AIL_SV_Find_Of_Res){ .sv_idx = i, .needle_idx = j };
        }
    }
    return (AIL_SV_Find_Of_Res){ -1, -1 };
}

AIL_SV_DEF AIL_SV_Find_Of_Res ail_sv_find_last_of_chars(AIL_SV str, char *needles, i32 needles_count)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
        char c = str.str[i];
        for (i32 j = 0; j < needles_count; j++) {
            if (c == needles[j]) return (AIL_SV_Find_Of_Res){ .sv_idx = i, .needle_idx = j };
        }
    }
    return (AIL_SV_Find_Of_Res){ -1, -1 };
}

AIL_SV_DEF AIL_SV ail_sv_split_next_char(AIL_SV *sv, char split_by, bool ignore_empty)
{
    size_t i = 0;
    if (ignore_empty) {
        while (i < sv->len && sv->str[i] == split_by) i++;
    }
    size_t j = i;
    while (j < sv->len) {
        size_t tmp_idx = j;
        j++;
        if (sv->str[tmp_idx] == split_by) break;
    }
    *sv = ail_sv_offset(*sv, j);
    return (AIL_SV) {
        .str = &sv->str[i],
        .len = j - i,
    };
}

AIL_SV_DEF AIL_SV ail_sv_split_next(AIL_SV *sv, AIL_SV split_by, bool ignore_empty)
{
    size_t i = 0;
    if (ignore_empty) {
        while (i < sv->len - split_by.len && ail_sv_starts_with(ail_sv_offset(*sv, i), split_by)) {
            i += split_by.len;
        }
    }
    size_t j = i;
    while (j < sv->len - split_by.len && !ail_sv_starts_with(ail_sv_offset(*sv, j), split_by)) {
        j++;
    }
    *sv = ail_sv_offset(*sv, j);
    return (AIL_SV) {
        .str = &sv->str[i],
        .len = j - i,
    };
}

AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_char(AIL_SV sv, char split_by, bool ignore_empty)
{
    AIL_DA(AIL_SV) res = ail_da_new(AIL_SV);
    u64 lstart = 0;
    u64 llen   = 0;
    for (u64 i = 0; i < sv.len; i++) {
        if (sv.str[i] == split_by) {
            if (!ignore_empty || llen > 0) {
                ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
            }
            lstart = i + 1;
            llen   = 0;
        } else {
            llen++;
        }
    }
    if (lstart < sv.len) {
        ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
    }
    return res;
}

AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split(AIL_SV sv, AIL_SV split_by, bool ignore_empty)
{
    AIL_DA(AIL_SV) res = ail_da_new(AIL_SV);
    u64 lstart = 0;
    u64 llen   = 0;
    if (sv.len >= split_by.len) {
        u64 i = 0;
        for (; i <= sv.len - split_by.len; i++) {
            if (ail_sv_starts_with(ail_sv_offset(sv, i), split_by)) {
                if (!ignore_empty || llen > 0) {
                    ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
                }
                i += split_by.len - 1;
                lstart = i + 1;
                llen   = 0;
            } else {
                llen++;
            }
        }
        if (!ail_sv_starts_with(ail_sv_offset(sv, i), split_by)) llen += sv.len - i;
    } else {
        llen = sv.len;
    }
    if (lstart < sv.len) {
        ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
    }
    return res;
}

AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_lines(AIL_SV sv, bool ignore_empty)
{
    // @Cleanup: Almost identical to ail_sv_split - maybe we can unite them somehow
    AIL_DA(AIL_SV) res = ail_da_new(AIL_SV);
    u64 lstart = 0;
    u64 llen   = 0;
    if (sv.len > 0) {
        for (u64 i = 0; i < sv.len; i++) {
            if (sv.str[i] == '\n') {
                if (!ignore_empty || llen > 0) {
                    ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
                }
                lstart = i + 1;
                llen   = 0;
            } else if (sv.str[i] == '\r') {
                if (AIL_LIKELY(i + 1 < sv.len && sv.str[i + 1] == '\n')) continue;
                else AIL_UNREACHABLE();
            } else {
                llen++;
            }
        }
    }
    if (lstart < sv.len && (!ignore_empty || llen > 0)) {
        ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
    }
    return res;
}

AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_whitespace(AIL_SV sv, bool ignore_empty)
{
    // @Cleanup: Almost identical to ail_sv_split_lines - maybe we can unite them somehow
    AIL_DA(AIL_SV) res = ail_da_new(AIL_SV);
    u64 lstart = 0;
    u64 llen   = 0;
    if (sv.len > 0) {
        for (u64 i = 0; i < sv.len; i++) {
            if (ail_sv_is_space(sv.str[i])) {
                if (!ignore_empty || llen > 0) {
                    ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
                }
                lstart = i + 1;
                llen   = 0;
            } else {
                llen++;
            }
        }
    }
    if (lstart < sv.len && (!ignore_empty || llen > 0)) {
        ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
    }
    return res;
}

AIL_SV_DEF AIL_SV ail_sv_offset(AIL_SV sv, u64 offset)
{
    if (AIL_UNLIKELY(offset >= sv.len))
        return ail_sv_from_parts(sv.str + sv.len, 0);
    else
        return ail_sv_from_parts(sv.str + offset, sv.len - offset);
}

AIL_SV_DEF bool ail_sv_is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

AIL_SV_DEF AIL_SV ail_sv_ltrim(AIL_SV sv)
{
    if (sv.len == 0) return sv;
    u64 start = 0;
    while (start < sv.len && ail_sv_is_space(sv.str[start])) start++;
    return ail_sv_offset(sv, start);
}

AIL_SV_DEF AIL_SV ail_sv_rtrim(AIL_SV sv)
{
    u64 end = sv.len;
    while (end > 0 && ail_sv_is_space(sv.str[end - 1])) end--;
    return ail_sv_from_parts(sv.str, end);
}


AIL_SV_DEF AIL_SV ail_sv_trim(AIL_SV sv)
{
    // return ail_sv_rtrim(ail_sv_ltrim(sv));
    if (sv.len == 0) return sv;
    u64 start = 0;
    u64 end   = sv.len - 1;
    while (start < sv.len && ail_sv_is_space(sv.str[start])) start++;
    while (end > start && ail_sv_is_space(sv.str[end])) end--;
    if (start < sv.len) return ail_sv_from_parts(&sv.str[start], end - start + 1);
    else return ail_sv_from_parts("", 0);
}

AIL_SV_DEF AIL_Str ail_sv_join(AIL_SV *list, u64 n, AIL_SV joiner)
{
    if (n == 0) return ail_str_from_parts("", 0);
    u64 res_len = joiner.len*(n - 1);
    for (u64 i = 0; i < n; i++) res_len += list[i].len;
    char *res = AIL_SV_MALLOC(res_len + 1);
    for (u64 i = 0, j = 0; i < n; i++) {
        memcpy(&res[j], list[i].str, list[i].len);
        j += list[i].len;
        if (i < n - 1) {
            memcpy(&res[j], joiner.str, joiner.len);
            j += joiner.len;
        }
    }
    res[res_len] = 0;
    return ail_str_from_parts(res, res_len);
}

AIL_SV_DEF AIL_Str ail_sv_join_da(AIL_DA(AIL_SV) list, AIL_SV joiner)
{
    return ail_sv_join(list.data, list.len, joiner);
}

AIL_SV_DEF AIL_Str ail_sv_rev_join(AIL_SV *list, u64 n, AIL_SV joiner)
{
    if (n == 0) return ail_str_from_parts("", 0);
    u64 res_len = joiner.len*(n - 1);
    for (u64 i = 0; i < n; i++) res_len += list[i].len;
    char *res = AIL_SV_MALLOC(res_len + 1);
    for (u64 i = n-1, j = res_len; i > 0; i--) {
        u64 el_len = list[i].len;
        memcpy(&res[j - el_len], list[i].str, el_len);
        j -= el_len;
        if (i > 1) {
            el_len = joiner.len;
            memcpy(&res[j - el_len], joiner.str, el_len);
            j -= el_len;
        }
    }
    res[res_len] = 0;
    return ail_str_from_parts(res, res_len);
}

AIL_SV_DEF AIL_Str ail_sv_rev_join_da(AIL_DA(AIL_SV) list, AIL_SV joiner)
{
    return ail_sv_rev_join(list.data, list.len, joiner);
}

AIL_SV_DEF AIL_Str ail_sv_concat(AIL_SV a, AIL_SV b)
{
    char *s = AIL_SV_MALLOC(a.len + b.len + 1);
    memcpy(&s[0],     a.str, a.len);
    memcpy(&s[a.len], b.str, b.len);
    s[a.len + b.len] = 0;
    return (AIL_Str) {
        .str = s,
        .len = a.len + b.len
    };
}

AIL_SV_DEF AIL_Str ail_sv_replace(AIL_SV sv, AIL_SV to_replace, AIL_SV replace_with)
{
    AIL_DA(AIL_SV) list = ail_sv_split(sv, to_replace, false);
    AIL_Str out = ail_sv_join(list.data, list.len, replace_with);
    ail_da_free(&list);
    return out;
}

#endif // _AIL_SV_IMPL_GUARD_
#endif // AIL_SV_IMPL
