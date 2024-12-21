/*
*** String Processing ***
* @TODO: Update description
*
* A String View (sometimes also called string-slice, here AIL_Str) is a lightweight structure
* containing a pointer to a string and its length.
* None of the functions here change the underlying string.
* In other words: A String View does not own the memory it points at.
*
* This library also contains a String structure (here AIL_Str), which does own its memory.
* The AIL_Str is guarantueed to be null-terminated, but stores its length as well.
*
* Any String View function that would require changes to the underlying string (e.g. ail_str_replace)
* instead allocates a new string, on which to produce the desired changes.
* Usually this newly produced string is then returned as an AIL_Str
* Any function that does allocate a new string, is annotated with '@Important'
* and a reminder to free said memory.
*
* To prevent you from needing to copy the string back to a null-terminated string
* (see ail_str_copy_to_cstr) as much as possible, this library implements most standard
* C functions that work with strings to allow working with String-Views instead.
* This includes among others also formatting and printing functions.
*
* This library was inspired by tsoding's STR library (https://github.com/tsoding/str)
*
* @TODO
*
* @Note: As this library is not finished, some of the above mentioned functions
* might not yet be Implemented. This should be fixed soon
*/

#ifndef _AIL_Str_H_
#define _AIL_Str_H_

#include "ail_base.h"
#include "ail_base_math.h"
#include "ail_arr.h"

AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)
AIL_WARN_DISABLE(AIL_WARN_UNUSED_VARIABLE)
#ifndef AIL_SB_INIT_CAP
#   ifdef AIL_DA_INIT_CAP
#       define AIL_SB_INIT_CAP AIL_DA_INIT_CAP
#   else
#       define AIL_SB_INIT_CAP 256
#   endif // AIL_DA_INIT_CAP
#endif // AIL_SB_INIT_CAP

// Sized Strings
typedef struct AIL_Str8 {
    u8 *data;
    u64 len;
} AIL_Str8;
AIL_SA_INIT(AIL_Str8); AIL_CA_INIT(AIL_Str8); AIL_DA_INIT(AIL_Str8);

typedef struct AIL_Str16 {
    u16 *data;
    u64  len;
} AIL_Str16;
AIL_SA_INIT(AIL_Str16); AIL_CA_INIT(AIL_Str16); AIL_DA_INIT(AIL_Str16);

typedef struct AIL_Str32 {
    u32 *data;
    u64  len;
} AIL_Str32;
AIL_SA_INIT(AIL_Str32); AIL_CA_INIT(AIL_Str32); AIL_DA_INIT(AIL_Str32);

// String-Builders
typedef AIL_DA(u8)  AIL_SB8;
typedef AIL_DA(u16) AIL_SB16;
typedef AIL_DA(u32) AIL_SB32;
AIL_SA_INIT(AIL_SB8);  AIL_CA_INIT(AIL_SB8);  AIL_DA_INIT(AIL_SB8);
AIL_SA_INIT(AIL_SB16); AIL_CA_INIT(AIL_SB16); AIL_DA_INIT(AIL_SB16);
AIL_SA_INIT(AIL_SB32); AIL_CA_INIT(AIL_SB32); AIL_DA_INIT(AIL_SB32);

// Default Types
typedef AIL_Str8 AIL_Str;
AIL_SA_INIT(AIL_Str);  AIL_CA_INIT(AIL_Str);  AIL_DA_INIT(AIL_Str);
typedef AIL_SB8 AIL_SB;
AIL_SA_INIT(AIL_SB);  AIL_CA_INIT(AIL_SB);  AIL_DA_INIT(AIL_SB);

///////////////////
// Utility Types //
///////////////////

typedef struct AIL_Str_Find_Of_Res { // Result from ail_str_find* functions
    i64 str_idx;
    i32 needle_idx;
} AIL_Str_Find_Of_Res;

typedef struct AIL_Str_Parse_U_Res {
    u64 val;
    u32 len;
} AIL_Str_Parse_U_Res;
typedef struct AIL_Str_Parse_I_Res {
    i64 val;
    u32 len;
} AIL_Str_Parse_I_Res;
typedef struct AIL_Str_Parse_F_Res {
    f64 val;
    u32 len;
} AIL_Str_Parse_F_Res;

// @TODO: Implement the following functions:
// - split by list of splitters
// - parse float
// - convert to float


////////////////////
// Creating a Str //
////////////////////

#define ail_str8_from_lit(lit)  { .data = (u8*)(lit),  .len = (sizeof(lit)/1) - 1 }
#define ail_str32_from_lit(lit) { .data = (u32*)(lit), .len = (sizeof(lit)/2) - 1 }
#define ail_str16_from_lit(lit) { .data = (u16*)(lit), .len = (sizeof(lit)/4) - 1 }
#define ail_str8_from_lit_t(lit)  ((AIL_Str8)  ail_str8_from_lit(lit))
#define ail_str16_from_lit_t(lit) ((AIL_Str16) ail_str16_from_lit(lit))
#define ail_str32_from_lit_t(lit) ((AIL_Str32) ail_str32_from_lit(lit))

internal u64 ail_cstr_len(char *cstr);

inline_func AIL_Str8 ail_str_from_parts(u8 *data, u64 len);
#define ail_str_from_cstr(cstr) ail_str_from_parts((u8*)cstr, ail_cstr_len(cstr))
#define ail_str_from_arr(da) ail_str_from_parts((u8*)(da).data, (da).len)


// @Important: Copies the underlying string to a new memory region. Remember to free the Str8 with ail_str_free
internal AIL_Str8 ail_str_new_from_parts_a(u8 *data, u64 len, AIL_Allocator allocator);
internal AIL_Str8 ail_str_new_unsigned_a(u64 num, AIL_Allocator allocator);
internal AIL_Str8 ail_str_new_signed_a  (i64 num, AIL_Allocator allocator);
internal AIL_Str8 ail_str_new_float_a   (f64 num, AIL_Allocator allocator);
#define ail_str_new_arr_a(arr, al)   ail_str_new_from_parts_a((arr).data, (arr).len, (al))
#define ail_str_new_arr(arr)         ail_str_new_from_parts_a((arr).data, (arr).len, ail_default_allocator)
#define ail_str_new_cstr_a(cstr, al) ail_str_new_from_parts_a(cstr, ail_cstr_len(cstr), (al))
#define ail_str_new_cstr(cstr)       ail_str_new_from_parts_a(cstr, ail_cstr_len(cstr), ail_default_allocator)
#define ail_str_new_unsigned(num)    ail_str_new_unsigned_a(num, ail_default_allocator)
#define ail_str_new_signed(num)      ail_str_new_signed_a(num,   ail_default_allocator)
#define ail_str_new_float(num)       ail_str_new_float_a(num,    ail_default_allocator)

#define ail_str_free_a(str,al) ail_call_free(al, (str).data)
#define ail_str_free(str) ail_str_free_a(str, ail_default_allocator)


///////////////////
// Creating a SB //
///////////////////

// inline_func AIL_SB8 ail_sb_from_parts(u8 *data, u64 len, u64 cap, AIL_Allocator allocator);
// inline_func AIL_SB8 ail_sb_from_da(AIL_DA(u8) da);
// inline_func AIL_SB8 ail_sb_new_a(AIL_Allocator allocator);
// inline_func AIL_SB8 ail_sb_new_cap_a(u64 initial_cap, AIL_Allocator allocator);
// internal AIL_SB8 ail_sb_new_cstr_a(u8 *str, AIL_Allocator allocator);
// internal AIL_SB8 ail_sb_new_str_a (AIL_Str str, AIL_Allocator allocator);
// internal AIL_SB8 ail_sb_new_da_a  (AIL_DA(u8) str, AIL_Allocator allocator);
// internal AIL_SB8 ail_sb_new_unsigned_a(u64 num, AIL_Allocator allocator);
// internal AIL_SB8 ail_sb_new_signed_a  (i64 num, AIL_Allocator allocator);
// internal AIL_SB8 ail_sb_new_float_a   (f64 num, AIL_Allocator allocator);

// #define ail_sb_new()             ail_sb_new_a(ail_default_allocator)
// #define ail_sb_new_cap(cap)      ail_sb_new_cap_a(cap, ail_default_allocator)
// #define ail_sb_new_cstr(str)     ail_sb_new_cstr_a(str, ail_default_allocator)
// #define ail_sb_new_str(str)      ail_sb_new_str_a(str, ail_default_allocator)
// #define ail_sb_new_da(str)       ail_sb_new_da_a(str, ail_default_allocator)
// #define ail_sb_new_unsigned(num) ail_sb_new_unsigned_a(num, ail_default_allocator)
// #define ail_sb_new_signed(num)   ail_sb_new_signed_a(num, ail_default_allocator)
// #define ail_sb_new_float(num)    ail_sb_new_float_a(num, ail_default_allocator)


/////////////////////////////////
// Converting back to C-String //
/////////////////////////////////

// @Important: Remmber to free the string you receive
internal char* ail_str_to_cstr_full(char *s, u64 len, AIL_Allocator allocator);
#define ail_str8_to_cstr_a(str8, al) ail_str_to_cstr_full((char*)(str8).data, (str8).len, al)
#define ail_str16_to_cstr_a(str16, al) ail_str_to_cstr_full((char*)(str16).data, 2*(str16).len, al)
#define ail_str32_to_cstr_a(str32, al) ail_str_to_cstr_full((char*)(str32).data, 4*(str32).len, al)
#define ail_str8_to_cstr(str) ail_str8_to_cstr_a(str, ail_default_allocator)
#define ail_str16_to_cstr(str) ail_str16_to_cstr_a(str, ail_default_allocator)
#define ail_str32_to_cstr(str) ail_str32_to_cstr_a(str, ail_default_allocator)


/////////////////////
// Parsing numbers //
/////////////////////

// When parsing, `len` will be set to he number of bytes that the number takes in the string
// @Important: If `str` doesn't start witha digit, '+', '-' or '.', 0 will be returned with len set to 0
// To check for errors, you thus need to check whether `len` is 0
internal AIL_Str_Parse_U_Res ail_str_parse_unsigned(AIL_Str8 str);
internal AIL_Str_Parse_I_Res ail_str_parse_signed  (AIL_Str8 str);
internal AIL_Str_Parse_F_Res ail_str_parse_float   (AIL_Str8 str);


/////////////
// Unicode //
/////////////

// @TODO: Implement
internal b32 ail_str8_is_valid_utf8    (AIL_Str8  str);
internal b32 ail_str16_is_valid_utf16  (AIL_Str16 str);
internal b32 ail_str32_is_valid_utf32  (AIL_Str32 str);
internal AIL_Str8  ail_str8_from_str16 (AIL_Str16 str);
internal AIL_Str8  ail_str8_from_str32 (AIL_Str32 str);
internal AIL_Str8  ail_str8_new_str16  (AIL_Str16 str);
internal AIL_Str8  ail_str8_new_str32  (AIL_Str32 str);
internal AIL_Str16 ail_str16_from_str8 (AIL_Str8  str);
internal AIL_Str16 ail_str16_from_str32(AIL_Str32 str);
internal AIL_Str16 ail_str16_new_str8  (AIL_Str8  str);
internal AIL_Str16 ail_str16_new_str32 (AIL_Str32 str);
internal AIL_Str32 ail_str32_from_str16(AIL_Str16 str);
internal AIL_Str32 ail_str32_from_str8 (AIL_Str8  str);
internal AIL_Str32 ail_str32_new_str16 (AIL_Str16 str);
internal AIL_Str32 ail_str32_new_str8  (AIL_Str8  str);

////////////////////////////////
// Comparison, Prefix, Suffix //
////////////////////////////////

// @TODO: Implement codepoint functions
internal b32 ail_str_full_eq (char *astr, u64 alen, char *bstr, u64 blen);
internal i32 ail_str_full_cmp(char *astr, u64 alen, char *bstr, u64 blen);
#define ail_str_eq(a, b)  ail_str_full_eq ((char*)(a).data, (a).len, (char*)(b).data, (b).len)
#define ail_str_cmp(a, b) ail_str_full_cmp((char*)(a).data, (a).len, (char*)(b).data, (b).len)
internal b32 ail_str_starts_with     (AIL_Str8 str, AIL_Str8 prefix);
internal b32 ail_str_starts_with_cp  (AIL_Str8 str, u32    prefix);
internal b32 ail_str_starts_with_char(AIL_Str8 str, char   prefix);
internal b32 ail_str_ends_with     (AIL_Str8 str, AIL_Str8 suffix);
internal b32 ail_str_ends_with_cp  (AIL_Str8 str, u32    suffix);
internal b32 ail_str_ends_with_char(AIL_Str8 str, char   suffix);
internal b32 ail_str_contains     (AIL_Str8 str, AIL_Str8 needle);
internal b32 ail_str_contains_cp  (AIL_Str8 str, u32    needle);
internal b32 ail_str_contains_char(AIL_Str8 str, char   needle);


///////////
// Find  //
///////////

// Get the first index in `str` that the substring `neddle` appears at
internal i64 ail_str_find(AIL_Str str, AIL_Str needle);
// Get the last index in `str` that the substring `neddle` appears at
internal i64 ail_str_find_last(AIL_Str str, AIL_Str needle);
// Get the first index in `str` that any of the substrings in `neddles` appears at
internal AIL_Str_Find_Of_Res ail_str_find_of(AIL_Str str, AIL_Str *needles, i32 needles_count);
// Get the last index in `str` that any of the substrings in `neddles` appears at
internal AIL_Str_Find_Of_Res ail_str_find_last_of(AIL_Str str, AIL_Str *needles, i32 needles_count);

// Get the first index in `str` that `needle` appears at
internal i64 ail_str_find_char(AIL_Str str, char needle);
// Get the last index in `str` that `needle` appears at
internal i64 ail_str_find_last_char(AIL_Str str, char needle);
// Get the first index in `str` that any of the `neddles` appears at
internal AIL_Str_Find_Of_Res ail_str_find_of_chars(AIL_Str str, char *needles, i32 needles_count);
// Get the last index in `str` that any of the `neddles` appears at
internal AIL_Str_Find_Of_Res ail_str_find_last_of_chars(AIL_Str str, char *needles, i32 needles_count);

////////////////////////
// Splitting, Joining //
////////////////////////

// @Note: There are both lazy and greedy functions for splitting a string
// The lazy splitting functions all have 'next' in their name and change the inputted original STR
// After calling a split_next function, the original STR is changed to point at the start of the string
// afer the first splitting result. Take the following example:
/*
    b32 ignore_empty = true;
    AIL_Str str     = ail_str_from_cstr(",,Hello,World,");
    AIL_Str first  = ail_str_split_next_char(&str, ',', ignore_empty);
    // str points at ",World," now
    AIL_Str second = ail_str_split_next_char(&str, ',', ignore_empty);
    assert(ail_str_eq(ail_str_from_cstr("Hello"), first));
    assert(ail_str_eq(ail_str_from_cstr("World"), first));
*/

// @Note: Every splitting function further takes a boolean parameter determining whether empty values should be ignored
// If `ignore_empty` was false in the above example, first would be an empty string and second would be "Hello"

internal AIL_Str ail_str_split_next_char(AIL_Str *str, char   split_by, b32 ignore_empty);
internal AIL_Str ail_str_split_next     (AIL_Str *str, AIL_Str split_by, b32 ignore_empty);
internal AIL_DA(AIL_Str) ail_str_split_char_a(AIL_Str str, char   split_by, b32 ignore_empty, AIL_Allocator allocator);
internal AIL_DA(AIL_Str) ail_str_split_a     (AIL_Str str, AIL_Str split_by, b32 ignore_empty, AIL_Allocator allocator);
internal AIL_DA(AIL_Str) ail_str_split_lines_a(AIL_Str str, b32 ignore_empty, AIL_Allocator allocator);
internal AIL_DA(AIL_Str) ail_str_split_whitespace_a(AIL_Str str, b32 ignore_empty, AIL_Allocator allocator);
#define ail_str_split_char(str, split_by, ignore_empty) ail_str_split_char_a(str, split_by, ignore_empty, ail_default_allocator)
#define ail_str_split(str, split_by, ignore_empty)      ail_str_split_a(str, split_by, ignore_empty, ail_default_allocator)
#define ail_str_split_lines(str, ignore_empty)          ail_str_split_lines_a(str, ignore_empty, ail_default_allocator)
#define ail_str_split_whitespace(str, ignore_empty)     ail_str_split_whitespace_a(str, ignore_empty, ail_default_allocator)

// @Note: rev_join joins the splitted substrings in reverse order
// @Important: To avoid memory leaks, make sure to free the underlying string
internal AIL_Str ail_str_join_a    (AIL_Str *list, u64 n, AIL_Str joiner, AIL_Allocator allocator);
internal AIL_Str ail_str_rev_join_a(AIL_Str *list, u64 n, AIL_Str joiner, AIL_Allocator allocator);
#define ail_str_join(list, n, joiner)     ail_str_join_a(list, n, joiner, ail_default_allocator)
#define ail_str_rev_join(list, n, joiner) ail_str_rev_join_a(list, n, joiner, ail_default_allocator)
#define ail_str_join_da(list, joiner)     ail_str_join_a((list).data, (list).len, joiner, ail_default_allocator)
#define ail_str_rev_join_da(list, joiner) ail_str_rev_join_da_a((list).data, (list).len, joiner, ail_default_allocator)

//////////////////
// Miscellanous //
//////////////////

#define ail_sb_push_str(sbptr, str) ail_da_pushn(sbptr, str.data, str.len)
internal ail_print_format(2, 3) void ail_sb_print(AIL_SB *sb, char *format, ...);
internal AIL_Str ail_sb_to_str(AIL_SB sb);

internal b32 ail_str_is_space(char c);
internal b32 ail_str_is_alpha(char c);
internal b32 ail_str_is_digit(char c);

// Receive a new STR, that points at the same underlying string as `str` but offset by `offset` bytes.
// If `offset` is greater than the length of `str`, then a STR with zero length is returned
internal AIL_Str ail_str_offset(AIL_Str str, u64 offset);

// Receive a new STR, that has the beginning and ending whitespace from `str` removed
internal AIL_Str ail_str_trim(AIL_Str str);
internal AIL_Str ail_str_ltrim(AIL_Str str);
internal AIL_Str ail_str_rtrim(AIL_Str str);

// Concatenate two String-Views to a single String
// @Important: To avoid memory leaks, make sure to free the underlying string
internal AIL_Str ail_str_concat2_full_a(char *str1, u64 len1, char *str2, u64 len2, AIL_Allocator allocator);
#define ail_str_concat2_full(str1, len1, str2, len2) ail_str_concat2_full_a(str1, len1, str2, len2, ail_default_allocator)
#define ail_str_concat2_a(str1, str2, allocator) ail_str_concat2_full_a((str1).data, (str1).len, (str2).data, (str2).len, allocator)
#define ail_str_concat2(str1, str2) ail_str_concat2_full_a((str1).data, (str1).len, (str2).data, (str2).len, ail_default_allocator)

// Concatenate an arbitrary amount of String-Views to a single String
// @Important: To avoid memory leaks, make sure to free the underlying string
// @Note: `str_count` refers to the amount of string-length pairs that follow. So you must supply 2*n further arguments after n
internal AIL_Str ail_str_concat_full_a(AIL_Allocator allocator, u32 str_count, ...);
#define ail_str_concat_full(str1, len1, str2, len2, ...) ail_str_concat_full_a(ail_default_allocator, 2 + (AIL_VA_LEN(__VA_ARGS__)/2), (str1), (len1), (str2), (len2), __VA_ARGS__)

internal AIL_Str ail_str_concat_list_full_a(AIL_Allocator allocator, u32 str_count, AIL_Str *strs);
#define ail_str_concat_list_full(str_count, strs) ail_str_concat_list_full_a(ail_default_allocator, (str_count), (strs))
#define ail_str_concat_list_a(allocator, str_list) ail_str_concat_list_full_a((allocator), (str_list).len, (str_list).data)
#define ail_str_concat_list(str_list) ail_str_concat_list_full_a(ail_default_allocator, (str_list).len, (str_list).data)

internal AIL_Str ail_str_concat_a(AIL_Allocator allocator, u32 str_count, ...);
#define ail_str_concat(str1, str2, ...) ail_str_concat_a(ail_default_allocator, 2 + AIL_VA_LEN(__VA_ARGS__), (str1), (str2), __VA_ARGS__)

// Receive a new STR, that has all appearances of `to_replace` replaced with `replace_with`
// @Note: Since this only works by changing the underlying string, an allocation and copy of the original string is required
// This operation can thus potentially be relatively expensive.
// @Important: To avoid memory leaks, make sure to free the underlying string
// Make sure that you don't lose the pointer to the original start of the string. See the following for example:
/*
    AIL_Str hello = ail_str_from_cstr("Hello");
    AIL_Str hi    = ail_str_from_cstr("Hi");
    AIL_Str a     = ail_str_from_cstr("  Hello World! ");

    // This would lose the original pointer:
    // AIL_Str b = ail_str_trim(ail_str_from_str(ail_str_replace(a, hello, hi)));

    // This keeps the original pointer
    AIL_Str b         = ail_str_replace(a, hello, hi);
    AIL_Str trimmed_b = ail_str_trim(ail_str_from_str(b));
    uses_the_str_somehow(trimmed_b);
    free(b.data);
    // @Note: if AIL_Str_MALLOC is defined as something other than malloc, you probably need to use a different function for freeing too
*/
internal AIL_Str ail_str_replace_a(AIL_Str str, AIL_Str to_replace, AIL_Str replace_with, AIL_Allocator allocator);
#define ail_str_replace(str, to_replace, replace_with) ail_str_replace_a(str, to_replace, replace_with, ail_default_allocator)

AIL_WARN_POP
#endif // _AIL_Str_H_


////////////////////
// IMPLEMENTATION //
////////////////////

#if !defined(AIL_NO_STR_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_Str_IMPL_GUARD_
#define _AIL_Str_IMPL_GUARD_
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)
#include <stdarg.h> // For va_<>


u64 ail_cstr_len(char *cstr)
{
    // @Performance: Use SIMD
    u64 len = 0;
    while (cstr[len]) len++;
    return len;
}

AIL_Str8 ail_str_from_parts(u8 *data, u64 len)
{
    return (AIL_Str8) {
        .data = data,
        .len  = len,
    };
}

AIL_Str8 ail_str_new_from_parts_a(u8 *data, u64 len, AIL_Allocator allocator)
{
    u8 *mem = ail_call_alloc(allocator, len);
    ail_mem_copy(mem, data, len);
    return ail_str_from_parts(mem, len);
}

AIL_DA(u8) _ail_da_from_unsigned_a(u64 num, AIL_Allocator allocator)
{
    AIL_DA(u8) da = ail_da_new_with_alloc(u8, 24, allocator);
    if (num == 0) {
        ail_da_push(&da, '0');
    } else {
        while (num > 0) {
            ail_da_push(&da, '0' + (num % 10));
            num /= 10;
        }
        for (u64 i = 0, j = da.len - 1; i < da.len/2; i++, j--) {
            u8 tmp   = da.data[i];
            da.data[i] = da.data[j];
            da.data[j] = tmp;
        }
    }
    ail_da_push(&da, 0);
    da.len--;
    return da;
}

AIL_DA(u8) _ail_da_from_signed_a(i64 num, AIL_Allocator allocator)
{
    AIL_DA(u8) da = ail_da_new_with_alloc(u8, 24, allocator);
    if (num == 0) {
        ail_da_push(&da, '0');
    } else {
        b32 is_neg = num < 0;
        if (is_neg) {
            ail_da_push(&da, '-');
            num *= -1;
        }
        while (num > 0) {
            ail_da_push(&da, '0' + (num % 10));
            num /= 10;
        }
        for (u64 i = (u64)is_neg, j = da.len - 1; i < da.len/2; i++, j--) {
            u8 tmp   = da.data[i];
            da.data[i] = da.data[j];
            da.data[j] = tmp;
        }
    }
    ail_da_push(&da, 0);
    da.len--;
    return da;
}

AIL_DA(u8) _ail_da_from_float_a(f64 num, AIL_Allocator allocator)
{
    AIL_UNUSED(num);
    AIL_UNUSED(allocator);
    AIL_TODO();
    return (AIL_DA(u8)) {0};
}

AIL_Str8 ail_str_new_unsigned_a(u64 num, AIL_Allocator allocator)
{
    AIL_DA(u8) da = _ail_da_from_unsigned_a(num, allocator);
    return (AIL_Str) {
        .data = da.data,
        .len = da.len,
    };
}

AIL_Str8 ail_str_new_signed_a(i64 num, AIL_Allocator allocator)
{
    AIL_DA(u8) da = _ail_da_from_signed_a(num, allocator);
    return (AIL_Str) {
        .data = da.data,
        .len = da.len,
    };
}

AIL_Str8 ail_str_new_float_a(f64 num, AIL_Allocator allocator)
{
    AIL_DA(u8) da = _ail_da_from_float_a(num, allocator);
    return (AIL_Str) {
        .data = da.data,
        .len = da.len,
    };
}

char* ail_str_to_cstr_full(char *s, u64 len, AIL_Allocator allocator)
{
    char *res = ail_call_alloc(allocator, len + 1);
    ail_mem_copy(res, s, len);
    res[len] = 0;
    return res;
}

AIL_Str_Parse_U_Res ail_str_parse_unsigned(AIL_Str8 str)
{
    AIL_Str_Parse_U_Res res = {0};
    u64 i   = 0;
    if (str.len && str.data[0] == '+') i++;
    while (i < str.len && str.data[i] >= '0' && str.data[i] <= '9') {
        res.val *= 10;
        res.val += str.data[i] - '0';
        i++;
    }
    if (!(i == 1 && str.data[0] == '+')) res.len = (u32)i;
    return res;
}

AIL_Str_Parse_I_Res ail_str_parse_signed(AIL_Str8 str)
{
    AIL_Str_Parse_I_Res res = {0};
    b32 is_neg = false;
    u64 i   = 0;
    if (str.len) {
        if (str.data[0] == '+') {
            i++;
        } else if (str.data[0] == '-') {
            is_neg = true;
            i++;
        }
    }
    while (i < str.len && str.data[i] >= '0' && str.data[i] <= '9') {
        res.val *= 10;
        res.val += str.data[i] - '0';
        i++;
    }
    if (is_neg) res.val *= -1;
    if (!(i == 1 && (is_neg || str.data[0] == '+'))) res.len = (u32)i;
    return res;
}

AIL_Str_Parse_F_Res ail_str_parse_float(AIL_Str8 str)
{
    (void)str;
    AIL_Str_Parse_F_Res res = {0};
    AIL_TODO();
    return res;
}

b32 ail_str_full_eq(char *astr, u64 alen, char *bstr, u64 blen)
{
    if (alen != blen) return false;
    for (u64 i = 0; i < alen; i++) {
        if (astr[i] != bstr[i]) return false;
    }
    return true;
}

i32  ail_str_full_cmp(char *astr, u64 alen, char *bstr, u64 blen)
{
    for (u64 i = 0; i < alen && i < blen; i++) {
        if (astr[i] != bstr[i]) return astr[i] - bstr[i];
    }
    if      (alen  > blen) return astr[blen];
    else if (alen == blen) return 0;
    else                   return bstr[alen];
}

b32 ail_str_starts_with(AIL_Str8 str, AIL_Str8 prefix)
{
    if (prefix.len > str.len) return false;
    for (u64 i = 0; i < prefix.len; i++) {
        if (str.data[i] != prefix.data[i]) return false;
    }
    return true;
}

b32 ail_str_starts_with_char(AIL_Str8 str, char prefix)
{
    return str.len > 0 && str.data[0] == prefix;
}

b32 ail_str_ends_with(AIL_Str8 str, AIL_Str8 suffix)
{
    if (suffix.len > str.len) return false;
    for (u64 i = suffix.len; i > 0; i--) {
        if (str.data[str.len - i] != suffix.data[suffix.len - i]) return false;
    }
    return true;
}

b32 ail_str_ends_with_char(AIL_Str8 str, char suffix)
{
    return str.len > 0 && str.data[str.len - 1] == suffix;
}

b32 ail_str_contains(AIL_Str8 str, AIL_Str8 needle)
{
    if (needle.len > str.len) return false;
    for (u64 i = 0; i < str.len - needle.len; i++) {
        if (ail_str_eq(ail_str_offset(str, i), needle)) return true;
    }
    return false;
}

b32 ail_str_contains_char(AIL_Str8 str, char needle)
{
    for (u64 i = 0; i < str.len; i++) {
        if (str.data[i] == needle) return true;
    }
    return false;
}

i64 ail_str_find(AIL_Str8 str, AIL_Str8 needle)
{
    for (u64 i = 0; i <= str.len - needle.len; i++) {
        if (ail_str_starts_with(ail_str_offset(str, i), needle)) return (i64)i;
    }
    return -1;
}

i64 ail_str_find_last(AIL_Str8 str, AIL_Str8 needle)
{
    for (i64 i = str.len - needle.len; i >= 0; i--) {
        if (ail_str_starts_with(ail_str_offset(str, i), needle)) return i;
    }
    return -1;
}

AIL_Str_Find_Of_Res ail_str_find_of(AIL_Str8 str, AIL_Str8 *needles, i32 needles_count)
{
    for (i64 i = 0; i < (i64)str.len; i++) {
        AIL_Str8 offset = ail_str_offset(str, i);
        for (i32 j = 0; j < needles_count; j++) {
            if (ail_str_starts_with(offset, needles[j])) return (AIL_Str_Find_Of_Res){ .str_idx = i, .needle_idx = j };
        }
    }
    return (AIL_Str_Find_Of_Res){ -1, -1 };
}

AIL_Str_Find_Of_Res ail_str_find_last_of(AIL_Str8 str, AIL_Str8 *needles, i32 needles_count)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
       AIL_Str8 offset = ail_str_offset(str, i);
        for (i32 j = 0; j < needles_count; j++) {
            if (ail_str_starts_with(offset, needles[j])) return (AIL_Str_Find_Of_Res){ .str_idx = i, .needle_idx = j };
        }
    }
    return (AIL_Str_Find_Of_Res){ -1, -1 };
}

i64 ail_str_find_char(AIL_Str8 str, char needle)
{
    for (u64 i = 0; i < str.len; i++) {
        if (str.data[i] == needle) return (i64)i;
    }
    return -1;
}

i64 ail_str_find_last_char(AIL_Str8 str, char needle)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
        if (str.data[i] == needle) return i;
    }
    return -1;
}

AIL_Str_Find_Of_Res ail_str_find_of_chars(AIL_Str8 str, char *needles, i32 needles_count)
{
    for (i64 i = 0; i < (i64)str.len; i++) {
        char c = str.data[i];
        for (i32 j = 0; j < needles_count; j++) {
            if (c == needles[j]) return (AIL_Str_Find_Of_Res){ .str_idx = i, .needle_idx = j };
        }
    }
    return (AIL_Str_Find_Of_Res){ -1, -1 };
}

AIL_Str_Find_Of_Res ail_str_find_last_of_chars(AIL_Str8 str, char *needles, i32 needles_count)
{
    for (i64 i = str.len - 1; i >= 0; i--) {
        char c = str.data[i];
        for (i32 j = 0; j < needles_count; j++) {
            if (c == needles[j]) return (AIL_Str_Find_Of_Res){ .str_idx = i, .needle_idx = j };
        }
    }
    return (AIL_Str_Find_Of_Res){ -1, -1 };
}

AIL_Str8 ail_str_split_next_char(AIL_Str8 *str, char split_by, b32 ignore_empty)
{
    size_t i = 0;
    if (ignore_empty) {
        while (i < str->len && str->data[i] == split_by) i++;
    }
    size_t j = i;
    while (j < str->len) {
        size_t tmp_idx = j;
        j++;
        if (str->data[tmp_idx] == split_by) break;
    }
    AIL_Str8 res = ail_str_from_parts(&str->data[i], j - i);
    *str = ail_str_offset(*str, j);
    return res;
}

AIL_Str8 ail_str_split_next(AIL_Str8 *str, AIL_Str8 split_by, b32 ignore_empty)
{
    size_t i = 0;
    if (ignore_empty) {
        while (i < str->len - split_by.len && ail_str_starts_with(ail_str_offset(*str, i), split_by)) {
            i += split_by.len;
        }
    }
    size_t j = i;
    while (j < str->len - split_by.len && !ail_str_starts_with(ail_str_offset(*str, j), split_by)) {
        j++;
    }
    *str = ail_str_offset(*str, j);
    return (AIL_Str) {
        .data = &str->data[i],
        .len = j - i,
    };
}

AIL_DA(AIL_Str) ail_str_split_char_a(AIL_Str str, char split_by, b32 ignore_empty, AIL_Allocator allocator)
{
    AIL_DA(AIL_Str) res = ail_da_new_with_alloc(AIL_Str, AIL_DA_INIT_CAP, allocator);
    u64 lstart = 0;
    u64 llen   = 0;
    for (u64 i = 0; i < str.len; i++) {
        if (str.data[i] == split_by) {
            if (!ignore_empty || llen > 0) {
                ail_da_push(&res, ail_str_from_parts(&str.data[lstart], llen));
            }
            lstart = i + 1;
            llen   = 0;
        } else {
            llen++;
        }
    }
    if (lstart < str.len) {
        ail_da_push(&res, ail_str_from_parts(&str.data[lstart], llen));
    }
    return res;
}

AIL_DA(AIL_Str) ail_str_split_a(AIL_Str str, AIL_Str split_by, b32 ignore_empty, AIL_Allocator allocator)
{
    AIL_DA(AIL_Str) res = ail_da_new_with_alloc(AIL_Str, AIL_DA_INIT_CAP, allocator);
    u64 lstart = 0;
    u64 llen   = 0;
    if (str.len >= split_by.len) {
        u64 i = 0;
        for (; i <= str.len - split_by.len; i++) {
            if (ail_str_starts_with(ail_str_offset(str, i), split_by)) {
                if (!ignore_empty || llen > 0) {
                    ail_da_push(&res, ail_str_from_parts(&str.data[lstart], llen));
                }
                i += split_by.len - 1;
                lstart = i + 1;
                llen   = 0;
            } else {
                llen++;
            }
        }
        if (!ail_str_starts_with(ail_str_offset(str, i), split_by)) llen += str.len - i;
    } else {
        llen = str.len;
    }
    if (lstart < str.len) {
        ail_da_push(&res, ail_str_from_parts(&str.data[lstart], llen));
    }
    return res;
}

AIL_DA(AIL_Str) ail_str_split_lines_a(AIL_Str str, b32 ignore_empty, AIL_Allocator allocator)
{
    // @Cleanup: Almost identical to ail_str_split - maybe we can unite them somehow
    AIL_DA(AIL_Str) res = ail_da_new_with_alloc(AIL_Str, AIL_DA_INIT_CAP, allocator);
    u64 lstart = 0;
    u64 llen   = 0;
    if (str.len > 0) {
        for (u64 i = 0; i < str.len; i++) {
            if (str.data[i] == '\n') {
                if (!ignore_empty || llen > 0) {
                    do {
                        do {
                            if ((&res)->len + (1) > (&res)->cap) do {
                                (&res)->data = (((((&res)->allocator)))).alloc((((((&res)->allocator)))).data, AIL_MEM_REALLOC, (sizeof((&res)->data[0])*((((2*(&res)->cap) > ((&res)->cap + (1))) ? (2*(&res)->cap) : ((&res)->cap + (1))))), ((&res)->data));
                                (&res)->cap = ((((2*(&res)->cap) > ((&res)->cap + (1))) ? (2*(&res)->cap) : ((&res)->cap + (1))));
                                if ((&res)->len > (&res)->cap) (&res)->len = (&res)->cap;
                            } while(0);
                        } while(0);
                        (&res)->data[(&res)->len++] = (ail_str_from_parts(&str.data[lstart], llen));
                    } while(0);
                }
                lstart = i + 1;
                llen   = 0;
            } else if (str.data[i] == '\r') {
                if (AIL_LIKELY(i + 1 < str.len && str.data[i + 1] == '\n')) continue;
                else AIL_UNREACHABLE();
            } else {
                llen++;
            }
        }
    }
    if (lstart < str.len && (!ignore_empty || llen > 0)) {
        ail_da_push(&res, ail_str_from_parts(&str.data[lstart], llen));
    }
    return res;
}

AIL_DA(AIL_Str) ail_str_split_whitespace_a(AIL_Str str, b32 ignore_empty, AIL_Allocator allocator)
{
    // @Cleanup: Almost identical to ail_str_split_lines - maybe we can unite them somehow
    AIL_DA(AIL_Str) res = ail_da_new_with_alloc(AIL_Str, AIL_DA_INIT_CAP, allocator);
    u64 lstart = 0;
    u64 llen   = 0;
    if (str.len > 0) {
        for (u64 i = 0; i < str.len; i++) {
            if (ail_str_is_space(str.data[i])) {
                if (!ignore_empty || llen > 0) {
                    ail_da_push(&res, ail_str_from_parts(&str.data[lstart], llen));
                }
                lstart = i + 1;
                llen   = 0;
            } else {
                llen++;
            }
        }
    }
    if (lstart < str.len && (!ignore_empty || llen > 0)) {
        ail_da_push(&res, ail_str_from_parts(&str.data[lstart], llen));
    }
    return res;
}

AIL_Str ail_str_offset(AIL_Str str, u64 offset)
{
    if (AIL_UNLIKELY(offset >= str.len))
        return ail_str_from_parts(str.data + str.len, 0);
    else
        return ail_str_from_parts(str.data + offset, str.len - offset);
}

b32 ail_str_is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

b32 ail_str_is_digit(char c)
{
    return '0' <= c && c <= '9';
}

b32 ail_str_is_alpha(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

AIL_Str ail_str_ltrim(AIL_Str str)
{
    if (str.len == 0) return str;
    u64 start = 0;
    while (start < str.len && ail_str_is_space(str.data[start])) start++;
    return ail_str_offset(str, start);
}

AIL_Str ail_str_rtrim(AIL_Str str)
{
    u64 end = str.len;
    while (end > 0 && ail_str_is_space(str.data[end - 1])) end--;
    return ail_str_from_parts(str.data, end);
}


AIL_Str ail_str_trim(AIL_Str str)
{
    // return ail_str_rtrim(ail_str_ltrim(str));
    if (str.len == 0) return str;
    u64 start = 0;
    u64 end   = str.len - 1;
    while (start < str.len && ail_str_is_space(str.data[start])) start++;
    while (end > start && ail_str_is_space(str.data[end])) end--;
    if (start < str.len) return ail_str_from_parts(&str.data[start], end - start + 1);
    else return (AIL_Str) {0};
}

AIL_Str ail_str_join_a(AIL_Str *list, u64 n, AIL_Str joiner, AIL_Allocator allocator)
{
    if (n == 0) return (AIL_Str) {0};
    u64 res_len = joiner.len*(n - 1);
    for (u64 i = 0; i < n; i++) res_len += list[i].len;
    u8 *res = ail_call_alloc(allocator, res_len + 1);
    for (u64 i = 0, j = 0; i < n; i++) {
        memcpy(&res[j], list[i].data, list[i].len);
        j += list[i].len;
        if (i < n - 1) {
            memcpy(&res[j], joiner.data, joiner.len);
            j += joiner.len;
        }
    }
    res[res_len] = 0;
    return ail_str_from_parts(res, res_len);
}

AIL_Str ail_str_rev_join_a(AIL_Str *list, u64 n, AIL_Str joiner, AIL_Allocator allocator)
{
    if (n == 0) return (AIL_Str) {0};
    u64 res_len = joiner.len*(n - 1);
    for (u64 i = 0; i < n; i++) res_len += list[i].len;
    u8 *res = ail_call_alloc(allocator, res_len + 1);
    for (u64 i = n-1, j = res_len; i > 0; i--) {
        u64 el_len = list[i].len;
        memcpy(&res[j - el_len], list[i].data, el_len);
        j -= el_len;
        if (i > 1) {
            el_len = joiner.len;
            memcpy(&res[j - el_len], joiner.data, el_len);
            j -= el_len;
        }
    }
    res[res_len] = 0;
    return ail_str_from_parts(res, res_len);
}

AIL_Str ail_str_concat2_full_a(char *astr, u64 alen, char *bstr, u64 blen, AIL_Allocator allocator)
{
    u8 *s = ail_call_alloc(allocator, alen + blen + 1);
    memcpy(&s[0],     astr, alen);
    memcpy(&s[alen], bstr, blen);
    s[alen + blen] = 0;
    return ail_str_from_parts(s, alen + blen);
}

AIL_Str ail_str_concat_list_full_a(AIL_Allocator allocator, u32 str_count, AIL_Str *strs)
{
    u64 size = 0;
    for (u32 i = 0; i < str_count; i++) {
        size += strs[i].len;
    }
    u8 *s = ail_call_alloc(allocator, size + 1);
    u64 filled_count = 0;
    for (u32 i = 0; i < str_count; i++) {
        memcpy(s + filled_count, strs[i].data, strs[i].len);
        filled_count += strs[i].len;
    }
    ail_assert(filled_count == size);
    s[filled_count] = 0;
    return ail_str_from_parts(s, filled_count);
}

AIL_Str ail_str_concat_full_a(AIL_Allocator allocator, u32 str_count, ...)
{
    va_list args;
    va_start(args, str_count);
    u64 size = 0;
    for (u32 i = 0; i < str_count; i++) {
        va_arg(args, u8*);
        size += va_arg(args, u64);
    }
    va_end(args);
    u8 *s = ail_call_alloc(allocator, size + 1);
    u64 filled_count = 0;
    va_start(args, str_count);
    for (u32 i = 0; i < str_count; i++) {
        u8 *str = va_arg(args, u8*);
        u64   len = va_arg(args, u64);
        memcpy(s + filled_count, str, len);
        filled_count += len;
    }
    va_end(args);
    ail_assert(filled_count == size);
    s[filled_count] = 0;
    return ail_str_from_parts(s, filled_count);
}

AIL_Str ail_str_concat_a(AIL_Allocator allocator, u32 str_count, ...)
{
    va_list args;
    va_start(args, str_count);
    u64 size = 0;
    for (u32 i = 0; i < str_count; i++) {
        AIL_Str str = va_arg(args, AIL_Str);
        size += str.len;
    }
    va_end(args);
    u8 *s = ail_call_alloc(allocator, size + 1);
    u64 filled_count = 0;
    va_start(args, str_count);
    for (u32 i = 0; i < str_count; i++) {
        AIL_Str str = va_arg(args, AIL_Str);
        memcpy(s + filled_count, str.data, str.len);
        filled_count += str.len;
    }
    va_end(args);
    ail_assert(filled_count == size);
    s[filled_count] = 0;
    return ail_str_from_parts(s, filled_count);
}

AIL_Str ail_str_replace_a(AIL_Str str, AIL_Str to_replace, AIL_Str replace_with, AIL_Allocator allocator)
{
    AIL_DA(AIL_Str) list = ail_str_split_a(str, to_replace, false, allocator);
    AIL_Str out = ail_str_join_a(list.data, list.len, replace_with, allocator);
    ail_da_free(&list);
    return out;
}

AIL_WARN_POP
#endif // _AIL_Str_IMPL_GUARD_
#endif // AIL_NO_STR_IMPL
