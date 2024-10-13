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
#   ifdef  AIL_DEF
#       define AIL_SV_DEF AIL_DEF
#   else
#       define AIL_SV_DEF
#   endif // AIL_DEF
#endif // AIL_SV_DEF

#ifndef AIL_SV_DEF_INLINE
#   ifdef  AIL_DEF_INLINE
#       define AIL_SV_DEF_INLINE AIL_DEF_INLINE
#   else
#       define AIL_SV_DEF_INLINE static inline
#   endif // AIL_DEF_INLINE
#endif // AIL_SV_DEF_INLINE

#ifndef AIL_SB_INIT_CAP
#   ifdef AIL_DA_INIT_CAP
#       define AIL_SB_INIT_CAP AIL_DA_INIT_CAP
#   else
#       define AIL_SB_INIT_CAP 256
#   endif // AIL_DA_INIT_CAP
#endif // AIL_SB_INIT_CAP


typedef struct AIL_SV { // Sized String View
    char *str;
    u64 len;
} AIL_SV;
AIL_DA_INIT(AIL_SV);

typedef struct AIL_Str { // Sized String (with nul-terminator for std/os interop)
    char *str;
    u64   len;
} AIL_Str;
AIL_DA_INIT(AIL_Str);

typedef AIL_DA(char) AIL_SB; // String Builder
AIL_DA_INIT(AIL_SB);

typedef struct AIL_SV_Find_Of_Res { // Result from ail_sv_find* functions
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
AIL_SV_DEF_INLINE AIL_Str ail_str_from_parts(char *s, u64 len);       // @Assert: s[len] == '\0'
AIL_SV_DEF_INLINE AIL_Str ail_str_from_da_nil_term(AIL_DA(char) str); // @Assert: str.data[str.len] == '\0'
AIL_SV_DEF_INLINE AIL_Str ail_str_from_cstr (char *s);
#define ail_str_from_sv(sv) ail_str_from_parts(sv.str, sv.len)

AIL_SV_DEF_INLINE AIL_SLICE(u8) ail_slice_from_sv(AIL_SV sv);
AIL_SV_DEF_INLINE AIL_SLICE(u8) ail_slice_from_str(AIL_Str str);
AIL_SV_DEF_INLINE AIL_ARR(u8) ail_arr_from_sv(AIL_SV sv);
AIL_SV_DEF_INLINE AIL_ARR(u8) ail_arr_from_str(AIL_Str str);

// @Important: Copies the underlying string to a new memory region. Remember to free the Str with ail_str_free
AIL_SV_DEF AIL_Str ail_str_new_sv_a(AIL_SV sv, AIL_Allocator allocator);
AIL_SV_DEF AIL_Str ail_str_new_cstr_a(char *str, AIL_Allocator allocator);
AIL_SV_DEF AIL_Str ail_str_new_da_a(AIL_DA(char) str, AIL_Allocator allocator);
AIL_SV_DEF AIL_Str ail_str_new_unsigned_a(u64 num, AIL_Allocator allocator);
AIL_SV_DEF AIL_Str ail_str_new_signed_a  (i64 num, AIL_Allocator allocator);
AIL_SV_DEF AIL_Str ail_str_new_float_a   (f64 num, AIL_Allocator allocator);
#define ail_str_new_sv(sv)        ail_str_new_sv_a(sv,  ail_default_allocator)
#define ail_str_new_da(str)       ail_str_new_da_a(str, ail_default_allocator)
#define ail_str_new_unsigned(num) ail_str_new_unsigned_a(num, ail_default_allocator)
#define ail_str_new_signed(num)   ail_str_new_signed_a(num, ail_default_allocator)
#define ail_str_new_float(num)    ail_str_new_float_a(num, ail_default_allocator)

AIL_SV_DEF void ail_str_free_a(AIL_Str str, AIL_Allocator allocator);
#define ail_str_free(str) ail_str_free_a(str, ail_default_allocator)

///////////////////
// Creating a SV //
///////////////////

#define AIL_SV_FROM_LITERAL(clit) { .str = (clit), .len = sizeof(clit)-1 }
#define AIL_SV_FROM_LITERAL_T(clit) (AIL_SV)AIL_SV_FROM_LITERAL(clit)
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_parts(char *str, u64 len);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_cstr (char *str);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_str(AIL_Str str);
AIL_SV_DEF_INLINE AIL_SV ail_sv_from_da(AIL_DA(char) str);
AIL_SV_DEF_INLINE AIL_SV ail_sv_new_unsigned_a(u64 num, AIL_Allocator allocator);
AIL_SV_DEF_INLINE AIL_SV ail_sv_new_signed_a  (i64 num, AIL_Allocator allocator);
AIL_SV_DEF_INLINE AIL_SV ail_sv_new_float_a   (f64 num, AIL_Allocator allocator);
#define ail_sv_new_unsigned(num) ail_sv_new_unsigned_a(num, ail_default_allocator)
#define ail_sv_new_signed(num)   ail_sv_new_signed_a(num, ail_default_allocator)
#define ail_sv_new_float(num)    ail_sv_new_float_a(num, ail_default_allocator)


///////////////////
// Creating a SB //
///////////////////

AIL_SV_DEF_INLINE AIL_SB ail_sb_from_parts(char *data, u32 len, u32 cap, AIL_Allocator allocator);
AIL_SV_DEF_INLINE AIL_SB ail_sb_from_da(AIL_DA(char) da);
AIL_SV_DEF_INLINE AIL_SB ail_sb_new_a(AIL_Allocator allocator);
AIL_SV_DEF_INLINE AIL_SB ail_sb_new_cap_a(u64 initial_cap, AIL_Allocator allocator);
AIL_SV_DEF AIL_SB ail_sb_new_cstr_a(char *str, AIL_Allocator allocator);
AIL_SV_DEF AIL_SB ail_sb_new_str_a (AIL_Str str, AIL_Allocator allocator);
AIL_SV_DEF AIL_SB ail_sb_new_da_a  (AIL_DA(char) str, AIL_Allocator allocator);
AIL_SV_DEF AIL_SB ail_sb_new_unsigned_a(u64 num, AIL_Allocator allocator);
AIL_SV_DEF AIL_SB ail_sb_new_signed_a  (i64 num, AIL_Allocator allocator);
AIL_SV_DEF AIL_SB ail_sb_new_float_a   (f64 num, AIL_Allocator allocator);

#define ail_sb_new()             ail_sb_new_a(ail_default_allocator)
#define ail_sb_new_cap(cap)      ail_sb_new_cap_a(cap, ail_default_allocator)
#define ail_sb_new_cstr(str)     ail_sb_new_cstr_a(str, ail_default_allocator)
#define ail_sb_new_str(str)      ail_sb_new_str_a(str, ail_default_allocator)
#define ail_sb_new_da(str)       ail_sb_new_da_a(str, ail_default_allocator)
#define ail_sb_new_unsigned(num) ail_sb_new_unsigned_a(num, ail_default_allocator)
#define ail_sb_new_signed(num)   ail_sb_new_signed_a(num, ail_default_allocator)
#define ail_sb_new_float(num)    ail_sb_new_float_a(num, ail_default_allocator)


/////////////////////////////////
// Converting back to C-String //
/////////////////////////////////

// @Important: Remmber to free the string you receive from ail_sv_copy_to_cstr
AIL_SV_DEF char* ail_sv_to_cstr_a(AIL_SV sv, AIL_Allocator allocator);
#define ail_sv_to_cstr(sv) ail_sv_to_cstr_a(sv, ail_default_allocator)


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
AIL_SV_DEF bool ail_sv_full_eq (char *astr, u64 alen, char *bstr, u64 blen);
AIL_SV_DEF i32  ail_sv_full_cmp(char *astr, u64 alen, char *bstr, u64 blen);
AIL_SV_DEF bool ail_sv_starts_with     (AIL_SV str, AIL_SV prefix);
AIL_SV_DEF bool ail_sv_starts_with_char(AIL_SV str, char   prefix);
AIL_SV_DEF bool ail_sv_ends_with     (AIL_SV str, AIL_SV suffix);
AIL_SV_DEF bool ail_sv_ends_with_char(AIL_SV str, char   suffix);


///////////
// Find  //
///////////

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
AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_char_a(AIL_SV sv, char   split_by, bool ignore_empty, AIL_Allocator allocator);
AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_a     (AIL_SV sv, AIL_SV split_by, bool ignore_empty, AIL_Allocator allocator);
AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_lines_a(AIL_SV sv, bool ignore_empty, AIL_Allocator allocator);
AIL_SV_DEF AIL_DA(AIL_SV) ail_sv_split_whitespace_a(AIL_SV sv, bool ignore_empty, AIL_Allocator allocator);
#define ail_sv_split_char(sv, split_by, ignore_empty) ail_sv_split_char_a(sv, split_by, ignore_empty, ail_default_allocator)
#define ail_sv_split(sv, split_by, ignore_empty)      ail_sv_split_a(sv, split_by, ignore_empty, ail_default_allocator)
#define ail_sv_split_lines(sv, ignore_empty)          ail_sv_split_lines_a(sv, ignore_empty, ail_default_allocator)
#define ail_sv_split_whitespace(sv, ignore_empty)     ail_sv_split_whitespace_a(sv, ignore_empty, ail_default_allocator)

// @Note: rev_join joins the splitted substrings in reverse order
// @Important: To avoid memory leaks, make sure to free the underlying string
AIL_SV_DEF AIL_Str ail_sv_join_a    (AIL_SV *list, u64 n, AIL_SV joiner, AIL_Allocator allocator);
AIL_SV_DEF AIL_Str ail_sv_rev_join_a(AIL_SV *list, u64 n, AIL_SV joiner, AIL_Allocator allocator);
AIL_SV_DEF AIL_Str ail_sv_join_da_a    (AIL_DA(AIL_SV) list, AIL_SV joiner, AIL_Allocator allocator);
AIL_SV_DEF AIL_Str ail_sv_rev_join_da_a(AIL_DA(AIL_SV) list, AIL_SV joiner, AIL_Allocator allocator);
#define ail_sv_join(list, n, joiner)     ail_sv_join_a(list, n, joiner, ail_default_allocator)
#define ail_sv_rev_join(list, n, joiner) ail_sv_rev_join_a(list, n, joiner, ail_default_allocator)
#define ail_sv_join_da(list, joiner)     ail_sv_join_da_a(list, joiner, ail_default_allocator)
#define ail_sv_rev_join_da(list, joiner) ail_sv_rev_join_da_a(list, joiner, ail_default_allocator)

//////////////////
// Miscellanous //
//////////////////

#define ail_sb_push_sv(sbptr, sv) ail_da_pushn(sbptr, sv.str, sv.len)
AIL_SV_DEF AIL_Str ail_sb_to_str(AIL_SB sb);

AIL_SV_DEF bool ail_sv_is_space(char c);
AIL_SV_DEF bool ail_sv_is_alpha(char c);
AIL_SV_DEF bool ail_sv_is_digit(char c);

// Receive a new SV, that points at the same underlying string as `sv` but offset by `offset` bytes.
// If `offset` is greater than the length of `sv`, then a SV with zero length is returned
AIL_SV_DEF AIL_SV ail_sv_offset(AIL_SV sv, u64 offset);

// Receive a new SV, that has the beginning and ending whitespace from `sv` removed
AIL_SV_DEF AIL_SV ail_sv_trim(AIL_SV sv);
AIL_SV_DEF AIL_SV ail_sv_ltrim(AIL_SV sv);
AIL_SV_DEF AIL_SV ail_sv_rtrim(AIL_SV sv);

// Concatenate two String-Views to a single String
// @Important: To avoid memory leaks, make sure to free the underlying string
AIL_SV_DEF AIL_Str ail_sv_concat_full_a(char *astr, u32 alen, char *bstr, u32 blen, AIL_Allocator allocator);
#define ail_sv_concat_full(astr, alen, bstr, blen) ail_sv_concat_full_a(astr, alen, bstr, blen, ail_default_allocator)
#define ail_sv_concat_a(a, b, allocator) ail_sv_concat_full_a((a).str, (a).len, (b).str, (b).len, allocator)
#define ail_sv_concat(a, b) ail_sv_concat_full_a((a).str, (a).len, (b).str, (b).len, ail_default_allocator)

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
AIL_SV_DEF AIL_Str ail_sv_replace_a(AIL_SV sv, AIL_SV to_replace, AIL_SV replace_with, AIL_Allocator allocator);
#define ail_sv_replace(sv, to_replace, replace_with) ail_sv_replace_a(sv, to_replace, replace_with, ail_default_allocator)

#endif // AIL_SV_H_


////////////////////
// IMPLEMENTATION //
////////////////////

#ifdef AIL_SV_IMPL
#ifndef _AIL_SV_IMPL_GUARD_
#define _AIL_SV_IMPL_GUARD_

AIL_SLICE(u8) ail_slice_from_sv(AIL_SV sv)
{
    return (AIL_SLICE(u8)) {
        .data = (u8*) sv.str,
        .len  = sv.len,
    };
}

AIL_SLICE(u8) ail_slice_from_str(AIL_Str str)
{
    return (AIL_SLICE(u8)) {
        .data = (u8*) str.str,
        .len  = str.len + 1,
    };
}

AIL_ARR(u8) ail_arr_from_sv(AIL_SV sv)
{
    return (AIL_ARR(u8)) {
        .data = (u8*) sv.str,
        .len  = sv.len,
        .cap  = sv.len,
    };
}

AIL_ARR(u8) ail_arr_from_str(AIL_Str str)
{
    return (AIL_ARR(u8)) {
        .data = (u8*) str.str,
        .len  = str.len + 1,
        .cap  = str.len + 1,
    };
}


AIL_SV ail_sv_from_parts(char *s, u64 len)
{
    return (AIL_SV) {
        .str = s,
        .len = len,
    };
}

AIL_SV ail_sv_from_da(AIL_DA(char) str)
{
    return ail_sv_from_parts(str.data, str.len);
}

AIL_SV ail_sv_from_str(AIL_Str str)
{
    return ail_sv_from_parts(str.str, str.len);
}

AIL_SV ail_sv_from_cstr(char *str)
{
    u64 len = 0;
    while (str[len]) len++;
    return ail_sv_from_parts(str, len);
}

AIL_Str ail_str_from_parts(char *s, u64 len)
{
    AIL_ASSERT(s[len] == 0);
    return (AIL_Str) {
        .str = s,
        .len = len,
    };
}

AIL_Str ail_str_from_cstr(char *s)
{
    return ail_str_from_parts(s, strlen(s));
}

AIL_Str ail_str_from_da_nil_term(AIL_DA(char) str)
{
    AIL_ASSERT(str.data[str.len] == 0);
    return ail_str_from_parts(str.data, str.len);
}

AIL_Str ail_str_new_da_a(AIL_DA(char) str, AIL_Allocator allocator)
{
    return ail_str_new_sv_a(ail_sv_from_da(str), allocator);
}

AIL_Str ail_str_new_cstr_a(char *str, AIL_Allocator allocator)
{
    return ail_str_new_sv_a(ail_sv_from_cstr(str), allocator);
}

AIL_Str ail_str_new_sv_a(AIL_SV sv, AIL_Allocator allocator)
{
    char *buf = AIL_CALL_ALLOC(allocator, sv.len + 1);
    memcpy(buf, sv.str, sv.len);
    buf[sv.len] = 0;
    return ail_str_from_parts(buf, sv.len);
}

void ail_str_free_a(AIL_Str str, AIL_Allocator allocator)
{
    AIL_CALL_FREE(allocator, str.str);
}

char* ail_sv_to_cstr_a(AIL_SV sv, AIL_Allocator allocator)
{
    return ail_str_new_sv_a(sv, allocator).str;
}

AIL_Str ail_sb_to_str(AIL_SB sb)
{
    if (!sb.len || sb.data[sb.len-1] != 0) ail_da_push(&sb, 0);
    return ail_str_from_parts(sb.data, sb.len - 1);
}

AIL_DA(char) _ail_da_from_unsigned_a(u64 num, AIL_Allocator allocator)
{
    AIL_DA(char) da = ail_da_new_with_alloc(char, 24, allocator);
    if (num == 0) {
        ail_da_push(&da, '0');
    } else {
        while (num > 0) {
            ail_da_push(&da, '0' + (num % 10));
            num /= 10;
        }
        for (u64 i = 0, j = da.len - 1; i < da.len/2; i++, j--) {
            char tmp   = da.data[i];
            da.data[i] = da.data[j];
            da.data[j] = tmp;
        }
    }
    ail_da_push(&da, 0);
    da.len--;
    return da;
}

AIL_DA(char) _ail_da_from_signed_a(i64 num, AIL_Allocator allocator)
{
    AIL_DA(char) da = ail_da_new_with_alloc(char, 24, allocator);
    if (num == 0) {
        ail_da_push(&da, '0');
    } else {
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
    }
    ail_da_push(&da, 0);
    da.len--;
    return da;
}

AIL_DA(char) _ail_da_from_float_a(f64 num, AIL_Allocator allocator)
{
    AIL_UNUSED(num);
    AIL_UNUSED(allocator);
    AIL_TODO();
    return (AIL_DA(char)) {0};
}

AIL_Str ail_str_new_unsigned_a(u64 num, AIL_Allocator allocator)
{
    AIL_DA(char) da = _ail_da_from_unsigned_a(num, allocator);
    return (AIL_Str) {
        .str = da.data,
        .len = da.len,
    };
}

AIL_Str ail_str_new_signed_a(i64 num, AIL_Allocator allocator)
{
    AIL_DA(char) da = _ail_da_from_signed_a(num, allocator);
    return (AIL_Str) {
        .str = da.data,
        .len = da.len,
    };
}

AIL_Str ail_str_new_float_a(f64 num, AIL_Allocator allocator)
{
    AIL_DA(char) da = _ail_da_from_float_a(num, allocator);
    return (AIL_Str) {
        .str = da.data,
        .len = da.len,
    };
}

AIL_SV ail_sv_new_unsigned_a(u64 num, AIL_Allocator allocator)
{
    return ail_sv_from_str(ail_str_new_unsigned_a(num, allocator));
}

AIL_SV ail_sv_new_signed_a(i64 num, AIL_Allocator allocator)
{
    return ail_sv_from_str(ail_str_new_signed_a(num, allocator));
}

AIL_SV ail_sv_new_float_a(f64 num, AIL_Allocator allocator)
{
    return ail_sv_from_str(ail_str_new_float_a(num, allocator));
}

AIL_SB ail_sb_from_parts(char *data, u32 len, u32 cap, AIL_Allocator allocator)
{
    return ail_da_from_parts_t(char, data, len, cap, allocator);
}

AIL_SB ail_sb_from_da(AIL_DA(char) da)
{
    return da;
}

AIL_SB ail_sb_new_a(AIL_Allocator allocator)
{
    return ail_da_new_with_alloc_t(char, AIL_SB_INIT_CAP, allocator);
}

AIL_SB ail_sb_new_cap_a(u64 initial_cap, AIL_Allocator allocator)
{
    return ail_da_new_with_alloc_t(char, initial_cap, allocator);
}

AIL_SB ail_sb_new_cstr_a(char *str, AIL_Allocator allocator)
{
    return ail_sb_new_str_a(ail_str_new_cstr_a(str, allocator), allocator);
}

AIL_SB ail_sb_new_str_a(AIL_Str str, AIL_Allocator allocator)
{
    u64 cap;
    if (str.len < 2048) AIL_NEXT_2POWER(str.len, cap);
    else cap = str.len;
    AIL_SB res = ail_sb_new_cap_a(cap, allocator);
    res.len = str.len;
    memcpy(res.data, str.str, str.len);
    return res;
}

AIL_SB ail_sb_new_da_a(AIL_DA(char) str, AIL_Allocator allocator)
{
    // @Note: Usually converting to da to str is not ok, bc da may not have a nul-terminator
    // but sb_from_str doesn't require a nul-terminator, so it's totally fine
    return ail_sb_new_str_a(ail_str_from_parts(str.data, str.len), allocator);
}

AIL_SB ail_sb_new_unsigned_a(u64 num, AIL_Allocator allocator)
{
    return ail_sb_from_da(_ail_da_from_unsigned_a(num, allocator));
}

AIL_SB ail_sb_new_signed_a(i64 num, AIL_Allocator allocator)
{
    return ail_sb_from_da(_ail_da_from_unsigned_a(num, allocator));
}

AIL_SB ail_sb_new_float_a(f64 num, AIL_Allocator allocator)
{
    return ail_sb_from_da(_ail_da_from_float_a(num, allocator));
}



u64 ail_sv_parse_unsigned(AIL_SV sv, u32 *len)
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

i64 ail_sv_parse_signed(AIL_SV sv, u32 *len)
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

f64 ail_sv_parse_float(AIL_SV sv, u32 *len)
{
    (void)sv;
    (void)len;
    AIL_TODO();
    return 0;
}

bool ail_sv_full_eq(char *astr, u64 alen, char *bstr, u64 blen)
{
    if (alen != blen) return false;
    for (u32 i = 0; i < alen; i++) {
        if (astr[i] != bstr[i]) return false;
    }
    return true;
}

i32  ail_sv_full_cmp(char *astr, u64 alen, char *bstr, u64 blen)
{
    for (u64 i = 0; i < alen && i < blen; i++) {
        if (astr[i] != bstr[i]) return astr[i] - bstr[i];
    }
    if      (alen  > blen) return astr[blen];
    else if (alen == blen) return 0;
    else                   return bstr[alen];
}

bool ail_sv_starts_with(AIL_SV str, AIL_SV prefix)
{
    if (prefix.len > str.len) return false;
    for (u32 i = 0; i < prefix.len; i++) {
        if (str.str[i] != prefix.str[i]) return false;
    }
    return true;
}

bool ail_sv_starts_with_char(AIL_SV str, char prefix)
{
    return str.len > 0 && str.str[0] == prefix;
}

bool ail_sv_ends_with(AIL_SV str, AIL_SV suffix)
{
    if (suffix.len > str.len) return false;
    for (u32 i = suffix.len; i > 0; i--) {
        if (str.str[str.len - i] != suffix.str[suffix.len - i]) return false;
    }
    return true;
}

bool ail_sv_ends_with_char(AIL_SV str, char suffix)
{
    return str.len > 0 && str.str[str.len - 1] == suffix;
}

i64 ail_sv_find(AIL_SV str, AIL_SV needle)
{
    for (u64 i = 0; i <= str.len - needle.len; i++) {
        if (ail_sv_starts_with(ail_sv_offset(str, i), needle)) return (i64)i;
    }
    return -1;
}

i64 ail_sv_find_last(AIL_SV str, AIL_SV needle)
{
    for (i64 i = str.len - needle.len; i >= 0; i--) {
        if (ail_sv_starts_with(ail_sv_offset(str, i), needle)) return i;
    }
    return -1;
}

AIL_SV_Find_Of_Res ail_sv_find_of(AIL_SV str, AIL_SV *needles, i32 needles_count)
{
    for (i64 i = 0; i < (i64)str.len; i++) {
        AIL_SV offset = ail_sv_offset(str, i);
        for (i32 j = 0; j < needles_count; j++) {
            if (ail_sv_starts_with(offset, needles[j])) return (AIL_SV_Find_Of_Res){ .sv_idx = i, .needle_idx = j };
        }
    }
    return (AIL_SV_Find_Of_Res){ -1, -1 };
}

AIL_SV_Find_Of_Res ail_sv_find_last_of(AIL_SV str, AIL_SV *needles, i32 needles_count)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
       AIL_SV offset = ail_sv_offset(str, i);
        for (i32 j = 0; j < needles_count; j++) {
            if (ail_sv_starts_with(offset, needles[j])) return (AIL_SV_Find_Of_Res){ .sv_idx = i, .needle_idx = j };
        }
    }
    return (AIL_SV_Find_Of_Res){ -1, -1 };
}

i64 ail_sv_find_char(AIL_SV str, char needle)
{
    for (u64 i = 0; i < str.len; i++) {
        if (str.str[i] == needle) return (i64)i;
    }
    return -1;
}

i64 ail_sv_find_last_char(AIL_SV str, char needle)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
        if (str.str[i] == needle) return i;
    }
    return -1;
}

AIL_SV_Find_Of_Res ail_sv_find_of_chars(AIL_SV str, char *needles, i32 needles_count)
{
    for (i64 i = 0; i < (i64)str.len; i++) {
        char c = str.str[i];
        for (i32 j = 0; j < needles_count; j++) {
            if (c == needles[j]) return (AIL_SV_Find_Of_Res){ .sv_idx = i, .needle_idx = j };
        }
    }
    return (AIL_SV_Find_Of_Res){ -1, -1 };
}

AIL_SV_Find_Of_Res ail_sv_find_last_of_chars(AIL_SV str, char *needles, i32 needles_count)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
        char c = str.str[i];
        for (i32 j = 0; j < needles_count; j++) {
            if (c == needles[j]) return (AIL_SV_Find_Of_Res){ .sv_idx = i, .needle_idx = j };
        }
    }
    return (AIL_SV_Find_Of_Res){ -1, -1 };
}

AIL_SV ail_sv_split_next_char(AIL_SV *sv, char split_by, bool ignore_empty)
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

AIL_SV ail_sv_split_next(AIL_SV *sv, AIL_SV split_by, bool ignore_empty)
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

AIL_DA(AIL_SV) ail_sv_split_char_a(AIL_SV sv, char split_by, bool ignore_empty, AIL_Allocator allocator)
{
    AIL_DA(AIL_SV) res = ail_da_new_with_alloc(AIL_SV, AIL_DA_INIT_CAP, allocator);
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

AIL_DA(AIL_SV) ail_sv_split_a(AIL_SV sv, AIL_SV split_by, bool ignore_empty, AIL_Allocator allocator)
{
    AIL_DA(AIL_SV) res = ail_da_new_with_alloc(AIL_SV, AIL_DA_INIT_CAP, allocator);
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

AIL_DA(AIL_SV) ail_sv_split_lines_a(AIL_SV sv, bool ignore_empty, AIL_Allocator allocator)
{
    // @Cleanup: Almost identical to ail_sv_split - maybe we can unite them somehow
    AIL_DA(AIL_SV) res = ail_da_new_with_alloc(AIL_SV, AIL_DA_INIT_CAP, allocator);
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

AIL_DA(AIL_SV) ail_sv_split_whitespace_a(AIL_SV sv, bool ignore_empty, AIL_Allocator allocator)
{
    // @Cleanup: Almost identical to ail_sv_split_lines - maybe we can unite them somehow
    AIL_DA(AIL_SV) res = ail_da_new_with_alloc(AIL_SV, AIL_DA_INIT_CAP, allocator);
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

AIL_SV ail_sv_offset(AIL_SV sv, u64 offset)
{
    if (AIL_UNLIKELY(offset >= sv.len))
        return ail_sv_from_parts(sv.str + sv.len, 0);
    else
        return ail_sv_from_parts(sv.str + offset, sv.len - offset);
}

bool ail_sv_is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool ail_sv_is_digit(char c)
{
    return '0' <= c && c <= '9';
}

bool ail_sv_is_alpha(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

AIL_SV ail_sv_ltrim(AIL_SV sv)
{
    if (sv.len == 0) return sv;
    u64 start = 0;
    while (start < sv.len && ail_sv_is_space(sv.str[start])) start++;
    return ail_sv_offset(sv, start);
}

AIL_SV ail_sv_rtrim(AIL_SV sv)
{
    u64 end = sv.len;
    while (end > 0 && ail_sv_is_space(sv.str[end - 1])) end--;
    return ail_sv_from_parts(sv.str, end);
}


AIL_SV ail_sv_trim(AIL_SV sv)
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

AIL_Str ail_sv_join_a(AIL_SV *list, u64 n, AIL_SV joiner, AIL_Allocator allocator)
{
    if (n == 0) return ail_str_from_parts("", 0);
    u64 res_len = joiner.len*(n - 1);
    for (u64 i = 0; i < n; i++) res_len += list[i].len;
    char *res = AIL_CALL_ALLOC(allocator, res_len + 1);
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

AIL_Str ail_sv_join_da_a(AIL_DA(AIL_SV) list, AIL_SV joiner, AIL_Allocator allocator)
{
    return ail_sv_join_a(list.data, list.len, joiner, allocator);
}

AIL_Str ail_sv_rev_join_a(AIL_SV *list, u64 n, AIL_SV joiner, AIL_Allocator allocator)
{
    if (n == 0) return ail_str_from_parts("", 0);
    u64 res_len = joiner.len*(n - 1);
    for (u64 i = 0; i < n; i++) res_len += list[i].len;
    char *res = AIL_CALL_ALLOC(allocator, res_len + 1);
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

AIL_Str ail_sv_rev_join_da_a(AIL_DA(AIL_SV) list, AIL_SV joiner, AIL_Allocator allocator)
{
    return ail_sv_rev_join_a(list.data, list.len, joiner, allocator);
}

AIL_Str ail_sv_concat_full_a(char *astr, u32 alen, char *bstr, u32 blen, AIL_Allocator allocator)
{
    char *s = AIL_CALL_ALLOC(allocator, alen + blen + 1);
    memcpy(&s[0],     astr, alen);
    memcpy(&s[alen], bstr, blen);
    s[alen + blen] = 0;
    return ail_str_from_parts(s, alen + blen);
}

AIL_Str ail_sv_replace_a(AIL_SV sv, AIL_SV to_replace, AIL_SV replace_with, AIL_Allocator allocator)
{
    AIL_DA(AIL_SV) list = ail_sv_split_a(sv, to_replace, false, allocator);
    AIL_Str out = ail_sv_join_a(list.data, list.len, replace_with, allocator);
    ail_da_free(&list);
    return out;
}

#endif // _AIL_SV_IMPL_GUARD_
#endif // AIL_SV_IMPL
