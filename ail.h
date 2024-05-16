// This header contains general utilities used throughout the other ail.h libraries
//
// LICENSE
/*
Copyright (c) 2024 Val Richter

Permission is hereby granted, free_one of charge, to any person obtaining a copy
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

#ifndef AIL_H_
#define AIL_H_

#if   !defined(AIL_MALLOC) && !defined(AIL_REALLOC) && !defined(AIL_CALLOC) && !defined(AIL_FREE)
#include <stdlib.h>
#define AIL_MALLOC(sz)          malloc(sz)
#define AIL_REALLOC(ptr, sz)    realloc(ptr, sz)
#define AIL_CALLOC(len, elSize) calloc(len, elSize)
#define AIL_FREE(ptr)           free(ptr)
#elif !defined(AIL_MALLOC) || !defined(AIL_REALLOC) || !defined(AIL_CALLOC) || !defined(AIL_FREE)
#error "You must define AIL_CALLOC, AIL_MALLOC, AIL_REALLOC and AIL_FREE or none of them. You cannot only define one or two of them."
#endif

#ifndef AIL_MEMCPY
#include <string.h>
#define AIL_MEMCPY(dst, src, n) memcpy(dst, src, n)
#endif

// AIL_DEF and AIL_DEF_INLINE only effect the AIL_ALLOC functions
// They do however serve as defaults for the other ail headers
#ifndef AIL_DEF
#define AIL_DEF static
#endif // AIL_DEF
#ifndef AIL_DEF_INLINE
#define AIL_DEF_INLINE static inline
#endif // AIL_DEF_INLINE

#ifdef _MSC_VER
#define AIL_UNUSED(v)  (void)sizeof(v)
#else
#define AIL_UNUSED(v)  (void)(v)
#endif

// @TODO: Add support for short names via `AIL_SHORT_NAMES` macro

// Implement all functionalities with `#define AIL_ALL_IMPL`
#ifdef  AIL_ALL_IMPL
#define AIL_TYPES_IMPL
#define AIL_ALLOCATOR_IMPL
#define AIL_DA_IMPL
#endif // AIL_ALL_IMPL


/////////////////////////
// Custom Typedefs
// enable with `#define AIL_TYPES_IMPL`
/////////////////////////
#ifdef  AIL_TYPES_IMPL
#ifndef _AIL_TYPES_GUARD_
#define _AIL_TYPES_GUARD_
#include <stdint.h>
#include <stdbool.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint32_t b32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;
typedef char*    str;
#endif // _AIL_TYPES_GUARD_
#endif // AIL_TYPES_IMPL


/////////////////////////
// Some useful predefined Macros
/////////////////////////

// Full list here: https://sourceforge.net/p/predef/wiki/Home/

// To detect Compilers
// Clang + Zig:       __clang__
// MSVC:              _MSC_VER
// GCC (+Clang+Zig):  __GNUC__
// MinGW:             __MINGW32__ (defined on 32- and 64-bit version, use __MINGW64__ for detecting 64-bit version only)
// TinyC:             __TINYC__
// Emscripten:        __EMSCRIPTEN__

// To detect C standard
// __STDC__                    - C89 or higher Standard
// __STDC_VERSION__ == 199901L - C99 Standard
// __STDC_VERSION__ == 201112L - C11 Standard
// __STDC_VERSION__ == 201710L - C17 Standard
// __STDC_VERSION__ == 202311L - C23 Standard
// __cplusplus                 - C++ Standard
// __cplusplus == 1            - pre C++98 Standard
// __cplusplus == 199711L      - C++98 Standard
// __cplusplus == 201103L      - C++11 Standard
// __cplusplus == 201402L      - C++14 Standard
// __cplusplus == 201703L      - C++17 Standard
// __cplusplus == 202002L      - C++20 Standard
// __cplusplus == 202302L      - C++23 Standard

// To detect OS
// Win32   - _WIN32 || __WIN32__
// Win64   - _WIN64
// POSIX   - __posix
// UNIX    - __unix
// Linux   - __linux__
// GNU     - __GNU__
// BSD     - BSD
// Android - __ANDROID__
// MinGW32 - __MINGW32__
// MinGW64 - __MINGW64__
// Cygwin  - __CYGWIN__
// SPARC   - __sparc__ || __sparc
// MacOS   - __APPLE__ && __MACH__
// IOS     - See example below: (Source: https://stackoverflow.com/a/5920028/13764271)
// #if __APPLE__
//     #include <TargetConditionals.h>
//     #if TARGET_IPHONE_SIMULATOR
//          // iOS, tvOS, or watchOS Simulator
//     #elif TARGET_OS_MACCATALYST
//          // Mac's Catalyst (ports iOS API into Mac, like UIKit).
//     #elif TARGET_OS_IPHONE
//         // iOS, tvOS, or watchOS device
//     #elif TARGET_OS_MAC
//         // Other kinds of Apple platforms
//     #else
//     #   error "Unknown Apple platform"
//     #endif
// #endif

// To detect Architecture
// x86-64    - __x86_64  || __x86_64__
// AMD64     - __amd64__ || _M_AMD64
// ARM       - __arm__   || _M_ARM
// ARM64     - __aarch64__
// Intel x86 - __i386    || _M_IX86 || _X86_
// MIPS      - __mips    || __mips__


/////////////////////////
// Custom Utility Macros
// always enabled
/////////////////////////
#include <stdlib.h> // For exit

#ifdef __cplusplus
    #define AIL_TYPEOF(x) decltype(x)
#elif defined(__GNUC__) || defined(_MINGW)
    #define AIL_TYPEOF(x) __typeof__(x)
#else
// No typeof possible
#endif

#ifndef AIL_DBG_PRINT
#include <stdio.h>
#define AIL_DBG_PRINT printf
#endif // AIL_DBG_PRINT

// @Note: Do not include "enum" in the declaration
#if defined(__GNUC__)
    #define AIL_PACK_BEGIN() __attribute__((__packed__))
    #define AIL_PACK_END()
#elif defined(_MSC_VER)
    #define AIL_PACK_BEGIN() __pragma(pack(push, 1))
    #define AIL_PACK_END()   __pragma(pack(pop))
#elif defined(__clang__) || defined(__TINYC__)
    #define AIL_PACK_BEGIN() __attribute__((packed))
    #define AIL_PACK_END()
#endif

// To allow overloading macros based on number of arguments (taken from here: https://stackoverflow.com/a/26408195/13764271)
#define __AIL_RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0
#define __AIL_ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __AIL_NARG_I_(...) __AIL_ARG_N(__VA_ARGS__)
#define __AIL_NARG__(...)  __AIL_NARG_I_(__VA_ARGS__,__AIL_RSEQ_N())
#define _AIL_VFUNC_(name, n) name##n
#define _AIL_VFUNC(name, n) _AIL_VFUNC_(name, n)
#define AIL_VFUNC(func, ...) _AIL_VFUNC(func, __AIL_NARG__(__VA_ARGS__)) (__VA_ARGS__)

#define _AIL_STRINGIZE2(x) #x
#define AIL_STRINGIZE(x) _AIL_STRINGIZE2(x)
#define AIL_STR_LINE AIL_STRINGIZE(__LINE__)

#define _AIL_CONCAT2(A, B) A##B
#define AIL_CONCAT(A, B) _AIL_CONCAT2(A, B)

// stolen from here (https://gcher.com/posts/2015-02-13-c-tricks/) and was originally stolen from linux kernel apparently
#define _AIL_IS_DEF3(_, v, ...) v
#define _AIL_IS_DEF2(comma) _AIL_IS_DEF3(comma 1, 0)
#define _AIL_IS_DEF1(value) _AIL_IS_DEF2(MACROTEST_##value)
#define MACROTEST_1 ,
#define AIL_IS_DEF(macro) _AIL_IS_DEF1(macro)

#define AIL_ARRLEN(arr) (sizeof(arr) / sizeof(*(arr)))

// @Note: Not safe to use with expressions, that have side-effects (like AIL_MAX(x++, y++))
#define AIL_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define AIL_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define AIL_CLAMP(x, min, max) ((x) > (max) ? (max) : (x) < (min) ? (min) : (x))

#define AIL_SWAP_PORTABLE2(T, x, y) do { T _swap_tmp_ = x; x = y; y = _swap_tmp_; } while(0)
#define AIL_SWAP_PORTABLE(T, x, y) AIL_SWAP_PORTABLE2(T, x, y);
#ifdef AIL_TYPEOF
    #define AIL_SWAP(x, y) do { AIL_TYPEOF(x) _swap_tmp_ = x; x = y; y = _swap_tmp_; } while(0)
#else
    #define AIL_SWAP(x, y) do { x ^= y; y ^= x; x ^= y; } while(0)
#endif

// AIL_LERP(AIL_REV_LERP(x, min, max), min, max) = x
#define AIL_LERP(t, min, max) ((min) + (t)*((max) - (min)))
#define AIL_REV_LERP(x, min, max) ((x) - (min)) / ((max) - (min))

#if defined(__GNUC__) || defined(__clang__)
    #define AIL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
    #define AIL_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#else
    #define AIL_UNLIKELY(expr) (expr)
    #define AIL_LIKELY(expr)   (expr)
#endif

#define AIL_DBG_EXIT()               do { int *X = 0; *X = 0; exit(1); } while(0)
#define AIL_ASSERT_COMMON(expr, msg) do { if (!(expr)) { AIL_DBG_PRINT("Assertion failed in " __FILE__ ":" AIL_STR_LINE "\n  " msg); AIL_DBG_EXIT(); } } while(0)
#define AIL_ASSERT_MSG(expr, msg)    AIL_ASSERT_COMMON(expr, "with message '" msg "'")
#define AIL_ASSERT(expr)             AIL_ASSERT_COMMON(expr, "with expression 'AIL_ASSERT(" #expr ")'")

#define AIL_PANIC(...)    do { AIL_DBG_PRINT(__VA_ARGS__); AIL_DBG_PRINT("\n"); AIL_DBG_EXIT(); } while(0)
#define AIL_TODO()        do { AIL_DBG_PRINT("Hit TODO in " __FILE__ ":" AIL_STR_LINE "\n"); AIL_DBG_EXIT(); } while(0)
#define AIL_UNREACHABLE() do { AIL_DBG_PRINT("Reached an unreachable place in " __FILE__ ":" AIL_STR_LINE "\n"); AIL_DBG_EXIT(); } while(0)

// @TODO: Better static assert message
#ifdef __cpp_static_assert
    #define _AIL_STATIC_ASSERT2(cond, msg) static_assert(!!(cond), msg)
#elif __STDC_VERSION__ >= 202311L
    #define _AIL_STATIC_ASSERT2(cond, msg) static_assert(!!(cond), msg)
#elif __STDC_VERSION__ >= 201112L
    #include <assert.h>
    #define _AIL_STATIC_ASSERT2(cond, msg) _Static_assert(!!(cond), msg)
#else
    #define _AIL_STATIC_ASSERT_MSG2(cond, msg, line) do { char ail_static_assertion_at_line##line[((!!(cond))*(AIL_ARRLEN(msg)+2))-1] = msg; AIL_UNUSED(ail_static_assertion_at_line##line); } while(0)
    #define _AIL_STATIC_ASSERT_MSG1(cond, msg, line) _AIL_STATIC_ASSERT_MSG2(cond, msg, line)
    #define _AIL_STATIC_ASSERT2(cond, msg)           _AIL_STATIC_ASSERT_MSG1(cond, msg, __LINE__)
#endif
#define _AIL_STATIC_ASSERT1(cond) _AIL_STATIC_ASSERT2(cond, __FILE__ ":" AIL_STR_LINE ": Static Assert failed")
#define AIL_STATIC_ASSERT(...) AIL_VFUNC(_AIL_STATIC_ASSERT, __VA_ARGS__)

#define AIL_OFFSETOF(var, field) (((char *) &(var)->field) - ((char *) (var)))

#define AIL_IS_2POWER(x) x && !(x & (x - 1))
#define AIL_NEXT_2POWER(x, out) do {                                                                                                          \
        out = x;                                                                                                                              \
        out--;                                                                                                                                \
        for (size_t _ail_next_2power_shift_ = 1; _ail_next_2power_shift_ < 8 * sizeof(x); _ail_next_2power_shift_ += _ail_next_2power_shift_) \
            out |= out >> _ail_next_2power_shift_;                                                                                            \
        out++;                                                                                                                                \
        out += (out==0);                                                                                                                      \
    } while(0)



/////////////////////////
// General Allocator Interface
// enable implementaton with `#define AIL_ALLOCATOR_IMPL`
// automatically enabled when, AIL_DA is also enabled
/////////////////////////

// If a specific allocator supports additional functions,
// they should be kept inside the data attribute
typedef struct AIL_Allocator {
    void *data; // Metadata required by allocator and provided in all function calls
    void *(*alloc)(void* data, size_t size);
    void *(*zero_alloc)(void* data, size_t nelem, size_t elsize);
    void *(*re_alloc)(void* data, void* ptr, size_t size);
    void  (*free_one)(void* data, void* ptr);
    void  (*free_all)(void* data);
} AIL_Allocator;

#if defined(AIL_ALLOCATOR_IMPL) || defined(AIL_DA_IMPL)
#ifndef _AIL_ALLOCATOR_GUARD_
#define _AIL_ALLOCATOR_GUARD_

AIL_DEF void* ail_default_malloc(void *data, size_t size)
{
    (void)data;
    return AIL_MALLOC(size);
}
AIL_DEF void* ail_default_calloc(void* data, size_t nelem, size_t elsize)
{
    (void)data;
    return AIL_CALLOC(nelem, elsize);
}
AIL_DEF void* ail_default_realloc(void* data, void* ptr, size_t size)
{
    (void)data;
    return AIL_REALLOC(ptr, size);
}
AIL_DEF void ail_default_free(void* data, void* ptr)
{
    (void)data;
    AIL_FREE(ptr);
}
AIL_DEF void ail_default_free_all(void* data)
{
    (void)data;
}

static AIL_Allocator ail_default_allocator = {
    .data       = NULL,
    .alloc      = &ail_default_malloc,
    .zero_alloc = &ail_default_calloc,
    .re_alloc   = &ail_default_realloc,
    .free_one   = &ail_default_free,
    .free_all   = &ail_default_free_all,
};

// Function just exists to suppress of potential "unused ail_default_allocator" warning
AIL_DEF void __ail_default_allocator_unused__(void)
{
    (void)ail_default_allocator;
}

#endif // _AIL_ALLOCATOR_GUARD_
#endif // AIL_ALLOCATOR_IMPL


/////////////////////////
// Dynamic Array Implementation
// Define `AIL_DA_INIT_CAP` to set a different initial capacity for dynamic arrays
// This implementation is heavily inspired by:
// - nob.h (https://github.com/tsoding/musializer/blob/master/src/nob.h)
// - stb_ds.h (https://github.com/nothings/stb/blob/master/stb_ds.h)
/////////////////////////
#ifndef AIL_DA_PRINT
#include <stdio.h>
#define AIL_DA_PRINT printf
#endif

#ifndef AIL_DA_INIT_CAP
#define AIL_DA_INIT_CAP 256
#endif

#define AIL_DA_INIT(T) typedef struct AIL_DA_##T { T *data; unsigned int len; unsigned int cap; AIL_Allocator *allocator; } AIL_DA_##T
#define AIL_DA(T) AIL_DA_##T

#ifdef AIL_DA_IMPL
#ifndef _AIL_DA_GUARD_
#define _AIL_DA_GUARD_
AIL_DA_INIT(void);
AIL_DA_INIT(char);

#ifdef AIL_TYPES_IMPL
AIL_DA_INIT(u8);
AIL_DA_INIT(u16);
AIL_DA_INIT(u32);
AIL_DA_INIT(u64);
AIL_DA_INIT(i8);
AIL_DA_INIT(i16);
AIL_DA_INIT(i32);
AIL_DA_INIT(i64);
AIL_DA_INIT(f32);
AIL_DA_INIT(f64);
AIL_DA_INIT(str);
#endif
#endif // _AIL_DA_GUARD_
#endif // AIL_DA_IMPL

// #define AIL_DA(T) AIL_DA // To allow adding the element T in array definitions - serves only as documentation

#define ail_da_from_parts(T, d, l, c, alPtr) (AIL_DA(T)) { .data = (d), .len = (l), .cap = (c), .allocator = (alPtr) }
#define ail_da_new_with_alloc(T, c, alPtr)   (AIL_DA(T)) { .data = (alPtr)->alloc((alPtr)->data, sizeof(T) * (c)), .len = 0, .cap = (c), .allocator = (alPtr) }
#define ail_da_new_zero_alloc(T, c, alPtr)   (AIL_DA(T)) { .data = (alPtr)->zero_alloc((alPtr)->data, sizeof(T) * (c)), .len = 0, .cap = (c), .allocator = (alPtr) }
#define ail_da_new_with_cap(T, c)            (AIL_DA(T)) { .data = ail_default_allocator.alloc(ail_default_allocator.data, sizeof(T) * (c)), .len = 0, .cap = (c), .allocator = &ail_default_allocator }
#define ail_da_new(T)                        (AIL_DA(T)) { .data = ail_default_allocator.alloc(ail_default_allocator.data, sizeof(T) * AIL_DA_INIT_CAP), .len = 0, .cap = AIL_DA_INIT_CAP, .allocator = &ail_default_allocator }
#define ail_da_new_empty(T)                  (AIL_DA(T)) { .data = NULL, .len = 0, .cap = 0, .allocator = &ail_default_allocator }
#define ail_da_new_zero_init(T, c)           (AIL_DA(T)) { .data = ail_default_allocator.zero_alloc(ail_default_allocator.data, (c), sizeof(T)), .len = 0, .cap = (c), .allocator = &ail_default_allocator }
#define ail_da_free(daPtr) do { (daPtr)->allocator->free_one((daPtr)->allocator->data, (daPtr)->data); (daPtr)->data = NULL; (daPtr)->len = 0; (daPtr)->cap = 0; } while(0);

#define ail_da_printf(da, format, ...) do {                                       \
        AIL_DA_PRINT("{\n  cap: %d,\n  len: %d,\n  data: [", (da).cap, (da).len); \
        for (u32 i = 0; i < (da).len; i++) {                                      \
            AIL_DA_PRINT("\n    " format ",", __VA_ARGS__);                       \
        }                                                                         \
        if ((da).len > 0) AIL_DA_PRINT("\n");                                     \
        AIL_DA_PRINT("  ]\n}\n");                                                 \
    } while(0)

#define ail_da_setn(daPtr, idx, elems, n) do {                                             \
        for (unsigned int _ail_da_setn_i_ = 0; _ail_da_setn_i_ < (n); _ail_da_setn_i_++) { \
            (daPtr)->data[(idx) + _ail_da_setn_i_] = (elems)[_ail_da_setn_i_];             \
        }                                                                                  \
    } while(0)

#define ail_da_resize(daPtr, newCap) do {                                                                                         \
        (daPtr)->data = (daPtr)->allocator->re_alloc((daPtr)->allocator->data, (daPtr)->data, sizeof(*((daPtr)->data))*(newCap)); \
        (daPtr)->cap  = (newCap);                                                                                                 \
        if ((daPtr)->len > (daPtr)->cap) (daPtr)->len = (daPtr)->cap;                                                             \
    } while(0)

#define ail_da_maybe_grow(daPtr, n) do {                                       \
        if ((daPtr)->len + (n) > (daPtr)->cap)								   \
            ail_da_resize(daPtr, AIL_MAX(2*(daPtr)->cap, (daPtr)->cap + (n))); \
    } while(0)

#define ail_da_push(daPtr, elem) do {           \
        ail_da_maybe_grow(daPtr, 1);            \
        (daPtr)->data[(daPtr)->len++] = (elem); \
    } while(0)

#define ail_da_pushn(daPtr, elems, n) do {                                               \
        ail_da_maybe_grow(daPtr, n);                                                     \
        AIL_MEMCPY((daPtr)->data + (daPtr)->len, (elems), sizeof(*((daPtr)->data))*(n)); \
        (daPtr)->len += (n);                                                             \
    } while(0)

#define ail_da_grow_with_gap(daPtr, gapStart, gapLen, newCap, elSize) do {                                                                 \
        (daPtr)->cap = (newCap);                                                                                                           \
        char *_ail_da_gwg_ptr_ = (char *) (daPtr)->data;                                                                                   \
        (daPtr)->data = (daPtr)->allocator->alloc((daPtr)->allocator->data, (elSize)*(newCap));                                            \
        AIL_ASSERT((daPtr)->data != NULL);                                                                                                 \
        AIL_MEMCPY((daPtr)->data, _ail_da_gwg_ptr_, (elSize)*(gapStart));                                                                  \
        AIL_MEMCPY(&(daPtr)->data[((gapStart) + (gapLen))], &_ail_da_gwg_ptr_[(elSize)*(gapStart)], (elSize)*((daPtr)->len - (gapStart))); \
        (daPtr)->allocator->free_one((daPtr)->allocator->data, _ail_da_gwg_ptr_);                                                          \
        (daPtr)->len += (gapLen);                                                                                                          \
    } while(0)

#define ail_da_maybe_grow_with_gap(daPtr, idx, n) do {                                                                        \
        if ((daPtr)->len + (n) > (daPtr)->cap) {                                                                              \
            ail_da_grow_with_gap((daPtr), (idx), (n), AIL_MAX(2*(daPtr)->cap, (daPtr)->cap + (n)), sizeof(*((daPtr)->data))); \
        } else {                                                                                                              \
            for (unsigned int _ail_da_i_ = 1; _ail_da_i_ <= ((daPtr)->len - (idx)); _ail_da_i_++) {                           \
                (daPtr)->data[((daPtr)->len + (n)) - _ail_da_i_] =  (daPtr)->data[(daPtr)->len - _ail_da_i_];                 \
            }                                                                                                                 \
            (daPtr)->len += (n);                                                                                              \
        }                                                                                                                     \
    } while(0)

#define ail_da_insert(daPtr, idx, elem) do {       \
        ail_da_maybe_grow_with_gap(daPtr, idx, 1); \
        (daPtr)->data[(idx)] = (elem);             \
    } while(0)

#define ail_da_insertn(daPtr, idx, elems, n) do {  \
        ail_da_maybe_grow_with_gap(daPtr, idx, n); \
        ail_da_setn(daPtr, idx, elems, n);         \
    } while(0)

// @TODO: Add ail_da_shrink & ail_da_maybe_shrink (they should only ever be explicitly called by the user)

#define ail_da_rm(daPtr, idx) do {                                                       \
        (daPtr)->len--;                                                                  \
        for (u32 _ail_da_rm_i_ = (idx); _ail_da_rm_i_ < (daPtr)->len; _ail_da_rm_i_++) { \
            (daPtr)->data[_ail_da_rm_i_] = (daPtr)->data[_ail_da_rm_i_ + 1];             \
        }                                                                                \
    } while(0)

#define ail_da_rm_swap(daPtr, idx) (daPtr)->data[(idx)] = (daPtr)->data[--(daPtr)->len]

#endif // AIL_H_
