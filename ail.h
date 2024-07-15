/*
* This header contains general utilities used throughout the other ail.h libraries
*
* By default this file only includes a bunch of useful macros (see list below), other features can be included by defining the following:
  * AIL_ALL_IMPL:   include everything
  * AIL_TYPES_IMPL: include typedefs in the style of u8, i16, f32 and b32 (for 32-bit bools) and str for char*
  * AIL_ALLOC_IMPL: include the AIL_Allocator struct for custom allocators
  * AIL_DA_IMPL:    include macro-template for dynamic arrays (automatically enables AIL_ALLOC_IMPL as well)
* For the documentation of each of these, see below
*
* Define AIL_DEF (defaults to `static`), AIL_DEF_INLINE (defaults to `static inline`) if you want different function declarations as a default for all ail.h libraries
*
* Define AIL_MALLOC, AIL_CALLOC, AIL_REALLOC, AIL_FREE to redefine the std's memory allocator as a default for all ail.h libraries
* Define AIL_MEMCPY to redefine memcpy as a default for all ail.h libraries
*
*
*** Useful macros ***
* The following list only contains the public API of macros, not any internally used macros (which are always prefixed with an underscore)
* None of these macros are safe in regards to side-effects, so be aware to avoid something like AIL_MIN(x++, --y);
  * AIL_UNUSED(x): to ignore compiler warnings if x is unused
  * AIL_ARRLEN(arr): get the size of a fixed-sized, stack-allocated array
  * AIL_STRINGIZE(x): Turn the token x into a string (useful for macros working with printing)
  * AIL_STR_LINE: The current line (via __LINE__) as a string
  * AIL_CONCAT(a, b): Concatenate two tokens to a single token (mainly useful for macros)
  * AIL_EXPAND(x): Expand a token given to a macro (mainly useful for macros)
  * AIL_IS_DEF(macro): Check whether macro is defined (basically ifdef but as a runtime value)
  * AIL_VFUNC(name, ...): Overload a macro on the amount of its arguments
    * only works if there's 64 or fewer arguments
    * every overloaded version of the macro must be named '<name>_<number_of_arguments>'
  * AIL_IS_EMPTY(...): Check whether the given var-arg list is empty or not
    * Does not work for conditional compilation
  * AIL_MAX(a, b): get the maximum of two values
  * AIL_MIN(a, b): get the minimum of two values
  * AIL_CLAMP(x, min, max): Returns the closest value to x in the range [min; max]
  * AIL_TYPEOF(x): get the type of x (only available with certain compiler extensions or on C++/C23)
  * AIL_SWAP_PORTABLE(Type, x, y): swap x and y in the most portable way (requires you to provide their type)
  * AIL_SWAP(x, y): swap x and y (without providing their type) (only works where AIL_TYPEOF works)
  * AIL_LERP(t, min, max): linearly interpolate between min and max
  * AIL_REV_LERP(x, min, max): does the reverse of a lerp, returning the interpolater
    * AIL_LERP(AIL_REV_LERP(x, min, max), min, max) == x
  * AIL_LIKELY(expr): Indicate to the compiler, that the expression expr is likely to be true
  * AIL_UNLIKELY(expr): Indicate to the compiler, that the expression expr is likely to be false
  * AIL_ASSERT_MSG(expr, msg): Assert that expr is true and print msg (while panicking) otherwise
  * AIL_ASSERT(expr): Assert that expr is true
  * AIL_PANIC(...): Panic and exit the program. Any input will be given as input to printf
  * AIL_TODO(): Panic when hitting this place while the program is running
  * AIL_UNREACHABLE(): Panic when hitting this place while the program is running
  * AIL_STATIC_ASSERT(cond, [msg]): Statically assert that cond is true with an optional message
  * AIL_OFFSETOF(ptr, field): Return the offset in bytes from a struct-field from a pointer to a struct
  * AIL_IS_2POWER(x): Returns whether x is a power of 2
  * AIL_NEXT_2POWER(x): Get the next highest value above x that is a power of 2 (if x isn't already a power of 2)
*
*
*** Custom Allocator interface ***
* An allocator is an AIL_Allocator struct containing an opaque pointer 'data' and an allocator function
* Every different allocation action is covered in this function and is picked through an enum
* The AIL_Allocator_Mode enum contains all the different allocation actions:
  * AIL_MEM_ALLOC:     Allcoate a new memory region
  * AIL_MEM_CALLOC:    Allocate a new memory region, that is cleared to 0
  * AIL_MEM_REALLOC:   Move an allocated memory region to be at least of the provided new size
  * AIL_MEM_FREE:      Free an allocated memory region
  * AIL_MEM_FREE_ALL:  Free all memory allocated by this allocator (if supported, it will give its underlying memory regions back to its underlying allocator (e.g. the OS))
  * AIL_MEM_CLEAR_ALL: Set all memory allocated by this allocator as free to be allocated again (does not give memory back to its underlying allocator (e.g. the OS))
* It is recommended to use the following convenience macros for using these custom allocators
  * AIL_CALL_ALLOC(allocator, size)
  * AIL_CALL_CALLOC(allocator, size) alternatively: AIL_CALL_CALLOC(allocator, el_count, el_size)
  * AIL_CALL_REALLOC(allocator, old_ptr, new_size)
  * AIL_CALL_FREE(allocator, ptr)
  * AIL_CALL_FREE_ALL(allocator)
  * AIL_CALL_CLEAR_ALL(allocator)
* To use the std's allocator, ail_default_allocator is provided
*
*** Dynamic Array Template ***
* @TODO: Documentation TBD
*
*
*** LICENSE ***
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
#error "You must define all of AIL_MALLOC, AIL_CALLOC, AIL_REALLOC and AIL_FREE or none of them. You cannot only define one or two of them."
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

// @TODO: Add support for short names via AIL_SHORT_NAMES macro

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

#ifndef _AIL_DBG_PRINT_
#include <stdio.h>
#define _AIL_DBG_PRINT_ puts
#endif // _AIL_DBG_PRINT_

#ifdef _MSC_VER
#define AIL_UNUSED(v)  (void)sizeof(v)
#else
#define AIL_UNUSED(v)  (void)(v)
#endif

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

#define AIL_ARRLEN(arr) (sizeof(arr) / sizeof(*(arr)))

#define _AIL_STRINGIZE2(x) #x
#define AIL_STRINGIZE(x) _AIL_STRINGIZE2(x)
#define AIL_STR_LINE AIL_STRINGIZE(__LINE__)

#define _AIL_CONCAT2(A, B) A##B
#define AIL_CONCAT(A, B) _AIL_CONCAT2(A, B)

#define AIL_EXPAND(x) x

// stolen from here (https://gcher.com/posts/2015-02-13-c-tricks/) and was originally stolen from linux kernel apparently
#define _AIL_IS_DEF3(_, v, ...) v
#define _AIL_IS_DEF2(comma) _AIL_IS_DEF3(comma 1, 0)
#define _AIL_IS_DEF1(value) _AIL_IS_DEF2(_AIL_MACROTEST_##value)
#define _AIL_MACROTEST_1 ,
#define AIL_IS_DEF(macro) _AIL_IS_DEF1(macro)

// AIL_VFUNC: To allow overloading macros based on number of arguments (taken from here: https://stackoverflow.com/a/26408195/13764271)
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
#define __AIL_NARG_I_(...) AIL_EXPAND(__AIL_ARG_N(__VA_ARGS__))
#define __AIL_NARG__(...)  __AIL_NARG_I_(__VA_ARGS__,__AIL_RSEQ_N())
#define AIL_VFUNC(func, ...) AIL_EXPAND(AIL_CONCAT(func, __AIL_NARG__(__VA_ARGS__))(__VA_ARGS__))

// Implementation taken from here: https://stackoverflow.com/a/55420185
#define AIL_IS_EMPTY(...) (sizeof((char[]){#__VA_ARGS__}) == 1)

// @Note: Not safe to use with expressions, that have side-effects (like AIL_MAX(x++, y++))
#define AIL_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define AIL_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define AIL_CLAMP(x, min, max) ((x) > (max) ? (max) : (x) < (min) ? (min) : (x))

#define _AIL_SWAP_PORTABLE_(T, x, y) do { T _swap_tmp_ = x; x = y; y = _swap_tmp_; } while(0)
#define AIL_SWAP_PORTABLE(T, x, y) _AIL_SWAP_PORTABLE_(T, x, y);
#ifdef AIL_TYPEOF
    #define AIL_SWAP(x, y) do { AIL_TYPEOF(x) _swap_tmp_ = x; x = y; y = _swap_tmp_; } while(0)
#else
    #define AIL_SWAP(x, y) do { x ^= y; y ^= x; x ^= y; } while(0)
#endif

// AIL_LERP(AIL_REV_LERP(x, min, max), min, max) = x
#define AIL_LERP(t, min, max) ((min) + (t)*((max) - (min)))
#define AIL_REV_LERP(x, min, max) ((x) - (min)) / ((max) - (min))

#define AIL_KB(x) (((u64)(x)) << 10)
#define AIL_MB(x) (((u64)(x)) << 20)
#define AIL_GB(x) (((u64)(x)) << 30)
#define AIL_TB(x) (((u64)(x)) << 40)

#if defined(__GNUC__) || defined(__clang__)
    #define AIL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
    #define AIL_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#else
    #define AIL_UNLIKELY(expr) (expr)
    #define AIL_LIKELY(expr)   (expr)
#endif

#define _AIL_DBG_EXIT_()                 do { int *X = 0; *X = 0; exit(1); } while(0)
#define _AIL_ASSERT_COMMON_(expr, msg)   do { if (!(expr)) { _AIL_DBG_PRINT_("Assertion failed in " __FILE__ ":" AIL_STR_LINE "\n  " msg); _AIL_DBG_EXIT_(); } } while(0)
#define AIL_ASSERT_MSG(expr, msg)        _AIL_ASSERT_COMMON_(expr, "with message '" msg "'")
#define AIL_ASSERT(expr)                 _AIL_ASSERT_COMMON_(expr, "with expression 'AIL_ASSERT(" AIL_STRINGIZE(expr) ")'")

#define AIL_PANIC(...)    do { _AIL_DBG_PRINT_(__VA_ARGS__); _AIL_DBG_PRINT_("\n"); _AIL_DBG_EXIT_(); } while(0)
#define AIL_TODO()        do { _AIL_DBG_PRINT_("Hit TODO in " __FILE__ ":" AIL_STR_LINE "\n"); _AIL_DBG_EXIT_(); } while(0)
#define AIL_UNREACHABLE() do { _AIL_DBG_PRINT_("Reached an unreachable place in " __FILE__ ":" AIL_STR_LINE "\n"); _AIL_DBG_EXIT_(); } while(0)

// @TODO: Better static assert message
#ifdef __cpp_static_assert
    #define _AIL_STATIC_ASSERT2(cond, msg) static_assert(!!(cond), msg)
#elif __STDC_VERSION__ >= 202311L
    #define _AIL_STATIC_ASSERT2(cond, msg) static_assert(!!(cond), msg)
#elif __STDC_VERSION__ >= 201112L
    #include <assert.h>
    #define _AIL_STATIC_ASSERT2(cond, msg) _Static_assert(!!(cond), msg)
#else
    #define _AIL_STATIC_ASSERT_MSG2(cond, msg, line) char __ail_static_assertion_at_line##line[((!!(cond))*2)-1]; char *__ail_static_assertion_at_line##line_message = AIL_STRINGIZE(msg)
    #define _AIL_STATIC_ASSERT_MSG1(cond, msg, line) _AIL_STATIC_ASSERT_MSG2(cond, msg, line)
    #define _AIL_STATIC_ASSERT2(cond, msg)           _AIL_STATIC_ASSERT_MSG1(cond, msg, __LINE__)
#endif
#define _AIL_STATIC_ASSERT1(cond) _AIL_STATIC_ASSERT2(cond, __FILE__ ":" AIL_STR_LINE ": Static Assert failed")
#define AIL_STATIC_ASSERT(...) AIL_VFUNC(_AIL_STATIC_ASSERT, __VA_ARGS__)

#define AIL_OFFSETOF(ptr, field) (((char *) &(ptr)->field) - ((char *) (ptr)))

#define AIL_IS_2POWER(x) (x && ((x & (x - 1)) == 0))
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
#if defined(AIL_ALLOCATOR_IMPL) || defined(AIL_DA_IMPL)
#ifndef _AIL_ALLOCATOR_GUARD_
#define _AIL_ALLOCATOR_GUARD_

#ifdef AIL_TYPES_IMPL
#define _AIL_ALLOCATOR_SIZE_TYPE_ u64
#else
#define _AIL_ALLOCATOR_SIZE_TYPE_ size_t
#endif

#define AIL_CALL_ALLOC(allocator, size) (allocator).alloc((allocator).data, AIL_MEM_ALLOC, (size), NULL)

#define _AIL_CALL_CALLOC3(allocator, nelem, size_el) (allocator).alloc((allocator).data, AIL_MEM_CALLOC, (nelem)*(size_el), NULL)
#define _AIL_CALL_CALLOC2(allocator, size)           (allocator).alloc((allocator).data, AIL_MEM_CALLOC, (size),            NULL)
// @Note: Allocate a chunk of memory, that is cleared to zero, either by providing the size of the amount of elements and size of each element
#define AIL_CALL_CALLOC(...) AIL_VFUNC(_AIL_CALL_CALLOC, __VA_ARGS__)

#define AIL_CALL_REALLOC(allocator, old_ptr, size) (allocator).alloc((allocator).data, AIL_MEM_REALLOC, (size), (old_ptr))

// @Note: Frees a single chunk of memory. Many allocators only mark the given memory-chunk as allocatable again, without actually freeing it
#define AIL_CALL_FREE(allocator, old_ptr) (allocator).alloc((allocator).data, AIL_MEM_FREE, 0, (old_ptr))

// @Note: If the allocator holds several memory regions, it keeps all these regions, but marks them as unused
#define AIL_CALL_CLEAR_ALL(allocator) (allocator).alloc((allocator).data, AIL_MEM_CLEAR_ALL, 0, NULL)

// @Note: If the allocator holds several memory regions, it frees all of them except for one
#define AIL_CALL_FREE_ALL(allocator) (allocator).alloc((allocator).data, AIL_MEM_FREE_ALL, 0, NULL)

// The action that should be executed when calling the allocator proc
typedef enum AIL_Allocator_Mode {
    AIL_MEM_ALLOC,
    AIL_MEM_CALLOC,
    AIL_MEM_REALLOC,
    AIL_MEM_FREE,
    AIL_MEM_FREE_ALL,
    AIL_MEM_CLEAR_ALL,
} AIL_Allocator_Mode;

typedef void* (AIL_Allocator_Func)(void *data, AIL_Allocator_Mode mode, _AIL_ALLOCATOR_SIZE_TYPE_ size, void *old_ptr);
typedef void* (*AIL_Allocator_Func_Ptr)(void *data, AIL_Allocator_Mode mode, _AIL_ALLOCATOR_SIZE_TYPE_ size, void *old_ptr);

typedef struct AIL_Allocator {
    void *data; // Metadata required by allocator and provided in all function calls
    AIL_Allocator_Func_Ptr alloc;
} AIL_Allocator;

AIL_DEF void* ail_default_alloc(void *data, AIL_Allocator_Mode mode, _AIL_ALLOCATOR_SIZE_TYPE_ size, void *old_ptr)
{
    AIL_UNUSED(data);
    switch (mode) {
        case AIL_MEM_ALLOC:    return AIL_MALLOC(size);
        case AIL_MEM_CALLOC:   return AIL_CALLOC(size, 1);
        case AIL_MEM_REALLOC:  return AIL_REALLOC(old_ptr, size);
        case AIL_MEM_FREE:
        case AIL_MEM_CLEAR_ALL:
        case AIL_MEM_FREE_ALL: return NULL;
    }
    AIL_UNREACHABLE();
    return NULL;
}

static AIL_Allocator ail_default_allocator = {
    .data  = NULL,
    .alloc = &ail_default_alloc,
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
#ifdef AIL_DA_IMPL
#ifndef _AIL_DA_GUARD_
#define _AIL_DA_GUARD_

#ifndef AIL_DA_PRINT
#include <stdio.h>
#define AIL_DA_PRINT printf
#endif

#ifndef AIL_DA_INIT_CAP
#define AIL_DA_INIT_CAP 256
#endif

#ifdef AIL_TYPES_IMPL
#define AIL_DA_INIT(T) typedef struct AIL_DA_##T { T *data; u32 len; u32 cap; AIL_Allocator allocator; } AIL_DA_##T
#define AIL_DA(T) AIL_DA_##T
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
#else
#define AIL_DA_INIT(T) typedef struct AIL_DA_##T { T *data; unsigned int len; unsigned int cap; AIL_Allocator *allocator; } AIL_DA_##T
#define AIL_DA(T) AIL_DA_##T
#endif

AIL_DA_INIT(void);
AIL_DA_INIT(char);
#endif // _AIL_DA_GUARD_
#endif // AIL_DA_IMPL

// #define AIL_DA(T) AIL_DA // To allow adding the element T in array definitions - serves only as documentation

#define ail_da_from_parts(T, d, l, c, al) { .data = (d), .len = (l), .cap = (c), .allocator = (al) }
#define ail_da_new_with_alloc(T, c, al)   { .data = AIL_CALL_ALLOC ((al), sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = (al) }
#define ail_da_new_zero_alloc(T, c, al)   { .data = AIL_CALL_CALLOC((al), sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = (al) }
#define ail_da_new_with_cap(T, c)         { .data = AIL_CALL_ALLOC(ail_default_allocator, sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = ail_default_allocator }
#define ail_da_new(T)                     { .data = AIL_CALL_ALLOC(ail_default_allocator, sizeof(T) * AIL_DA_INIT_CAP), .len = 0, .cap = AIL_DA_INIT_CAP, .allocator = ail_default_allocator }
#define ail_da_new_empty(T)               { .data = NULL, .len = 0, .cap = 0, .allocator = ail_default_allocator }
#define ail_da_new_zero_init(T, c)        { .data = AIL_CALL_CALLOC(ail_default_allocator, sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = ail_default_allocator }
#define ail_da_new_zero_init_t(T, c)        (AIL_DA((T)))ail_da_new_zero_init()
#define ail_da_new_empty_t(T)               (AIL_DA((T)))ail_da_new_empty(T)
#define ail_da_new_t(T)                     (AIL_DA((T)))ail_da_new(T)
#define ail_da_new_with_cap_t(T, c)         (AIL_DA((T)))ail_da_new_with_cap(T, c)
#define ail_da_new_zero_alloc_t(T, c, al)   (AIL_DA((T)))ail_da_new_zero_alloc(T, c, al)
#define ail_da_new_with_alloc_t(T, c, al)   (AIL_DA((T)))ail_da_new_with_alloc(T, c, al)
#define ail_da_from_parts_t(T, d, l, c, al) (AIL_DA((T)))ail_da_from_parts(T, d, l, c, al)

#define ail_da_free(daPtr) ail_da_free_a(daPtr, (daPtr)->allocator)
#define ail_da_free_a(daPtr, al) do { AIL_CALL_FREE((al), (daPtr)->data); (daPtr)->data = NULL; (daPtr)->len = 0; (daPtr)->cap = 0; } while(0);

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

#define ail_da_resize(daPtr, newCap) ail_da_resize_a(daPtr, newCap, (daPtr)->allocator)
#define ail_da_resize_a(daPtr, newCap, al) do {                                                   \
        (daPtr)->data = AIL_CALL_REALLOC((al), (daPtr)->data, sizeof(*((daPtr)->data))*(newCap)); \
        (daPtr)->cap  = (newCap);                                                                 \
        if ((daPtr)->len > (daPtr)->cap) (daPtr)->len = (daPtr)->cap;                             \
    } while(0)

#define ail_da_maybe_grow(daPtr, n) ail_da_maybe_grow_a(daPtr, n, (daPtr)->allocator)
#define ail_da_maybe_grow_a(daPtr, n, al) do {                                         \
        if ((daPtr)->len + (n) > (daPtr)->cap)                                         \
            ail_da_resize_a(daPtr, AIL_MAX(2*(daPtr)->cap, (daPtr)->cap + (n)), (al)); \
    } while(0)

#define ail_da_push(daPtr, elem) ail_da_push_a(daPtr, elem, (daPtr)->allocator)
#define ail_da_push_a(daPtr, elem, al) do {     \
        ail_da_maybe_grow_a(daPtr, 1, (al));    \
        (daPtr)->data[(daPtr)->len++] = (elem); \
    } while(0)

#define ail_da_pushn(daPtr, elems, n) ail_da_pushn_a(daPtr, elems, n, (daPtr)->allocator)
#define ail_da_pushn_a(daPtr, elems, n, al) do {                                         \
        ail_da_maybe_grow_a(daPtr, n, (al));                                             \
        AIL_MEMCPY((daPtr)->data + (daPtr)->len, (elems), sizeof(*((daPtr)->data))*(n)); \
        (daPtr)->len += (n);                                                             \
    } while(0)

#define ail_da_grow_with_gap(daPtr, gapStart, gapLen, newCap, elSize) ail_da_grow_with_gap_a(daPtr, gapStart, gapLen, newCap, elSize, (daPtr)->allocator)
#define ail_da_grow_with_gap_a(daPtr, gapStart, gapLen, newCap, elSize, al) do {                                                           \
        (daPtr)->cap = (newCap);                                                                                                           \
        char *_ail_da_gwg_ptr_ = (char *) (daPtr)->data;                                                                                   \
        (daPtr)->data = AIL_CALL_ALLOC((al), (elSize)*(newCap));                                                                           \
        AIL_ASSERT((daPtr)->data != NULL);                                                                                                 \
        AIL_MEMCPY((daPtr)->data, _ail_da_gwg_ptr_, (elSize)*(gapStart));                                                                  \
        AIL_MEMCPY(&(daPtr)->data[((gapStart) + (gapLen))], &_ail_da_gwg_ptr_[(elSize)*(gapStart)], (elSize)*((daPtr)->len - (gapStart))); \
        AIL_CALL_FREE((al), _ail_da_gwg_ptr_);                                                                                             \
        (daPtr)->len += (gapLen);                                                                                                          \
    } while(0)

#define ail_da_maybe_grow_with_gap(daPtr, idx, n) ail_da_maybe_grow_with_gap_a(daPtr, idx, n, (daPtr)->allocator)
#define ail_da_maybe_grow_with_gap_a(daPtr, idx, n, al) do {                                                                          \
        if ((daPtr)->len + (n) > (daPtr)->cap) {                                                                                      \
            ail_da_grow_with_gap_a((daPtr), (idx), (n), AIL_MAX(2*(daPtr)->cap, (daPtr)->cap + (n)), sizeof(*((daPtr)->data)), (al)); \
        } else {                                                                                                                      \
            for (unsigned int _ail_da_i_ = 1; _ail_da_i_ <= ((daPtr)->len - (idx)); _ail_da_i_++) {                                   \
                (daPtr)->data[((daPtr)->len + (n)) - _ail_da_i_] =  (daPtr)->data[(daPtr)->len - _ail_da_i_];                         \
            }                                                                                                                         \
            (daPtr)->len += (n);                                                                                                      \
        }                                                                                                                             \
    } while(0)

#define ail_da_insert(daPtr, idx, elem) ail_da_insert_a(daPtr, idx, elem, (daPtr)->allocator)
#define ail_da_insert_a(daPtr, idx, elem, al) do {         \
        ail_da_maybe_grow_with_gap_a(daPtr, idx, 1, (al)); \
        (daPtr)->data[(idx)] = (elem);                     \
    } while(0)

#define ail_da_insertn(daPtr, idx, elems, n) ail_da_insertn_a(daPtr, idx, elems, n, (daPtr)->allocator)
#define ail_da_insertn_a(daPtr, idx, elems, n, al) do {    \
        ail_da_maybe_grow_with_gap_a(daPtr, idx, n, (al)); \
        ail_da_setn(daPtr, idx, elems, n);                 \
    } while(0)

// @TODO: Add ail_da_shrink & ail_da_maybe_shrink (they should only ever be explicitly called by the user)

#define ail_da_rm(daPtr, idx) do {                                                       \
        (daPtr)->len--;                                                                  \
        for (u32 _ail_da_rm_i_ = (idx); _ail_da_rm_i_ < (daPtr)->len; _ail_da_rm_i_++) { \
            (daPtr)->data[_ail_da_rm_i_] = (daPtr)->data[_ail_da_rm_i_ + 1];             \
        }                                                                                \
    } while(0)

#define ail_da_rmn(daPtr, idx, n) do {                                                      \
        (daPtr)->len -= (n);                                                                \
        for (u32 _ail_da_rmn_i_ = (idx); _ail_da_rmn_i_ < (daPtr)->len; _ail_da_rmn_i_++) { \
            (daPtr)->data[_ail_da_rmn_i_] = (daPtr)->data[_ail_da_rmn_i_ + (n)];            \
        }                                                                                   \
    } while(0)

#define ail_da_rm_swap(daPtr, idx) (daPtr)->data[(idx)] = (daPtr)->data[--(daPtr)->len]

#endif // AIL_H_
