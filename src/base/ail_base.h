/*
*** Keywords ***
* internal:    prefix for all functions; defaults to `static` but can be defined to anything else
* inline:      prefix for all functions that are recommended to be inlined; defaults to `static` but can be defined to anything else
* persist:     storage class for static local variables
* global:      storage class for static global variables
* export:      specifies function to be exported in a dll
* import:      specifies function to be imported from a dll
* fallthrough: Indicate that falling through is intended for this particular case of a switch-statement.
*
*
*** Type Definitions ***
* stdint.h is included for specifically sized integer types and boolean types
* via typedefs, the following types are declared as shortened forms of the original
* Integers:   u8, u16, u32, u64, i8, i16, i32, i64
* Floats:     f32, f64
* Boolean:    b32, bool, true, false
* Characters: c8, c16, c32
* Nul-terminated C-Strings: pchar
*
*
*** Useful Macros ***
* The following list only contains the public API of macros, but none of the internally used macros (which are always prefixed with an underscore)
* None of these macros guarantuee safety in regards to side-effects, so be aware to avoid something like AIL_MIN(x++, --y);
  * AIL_ARRLEN(arr): Get the size of a fixed-sized, stack-allocated array
  * AIL_TYPEOF(x):   Get the type of x (only available with certain compiler extensions or on C++/C23)
  * AIL_OFFSETOF(ptr, field): Return the offset in bytes from a struct-field from a pointer to a struct
  *
  * AIL_SWAP(x, y):             Swap x and y (without providing their type) (only works when AIL_TYPEOF works)
  * AIL_SWAP_PORTABLE(T, x, y): Swap x and y in the most portable way (requires you to provide their type T)
  * AIL_MAX(a, b):              Get the maximum of two values
  * AIL_MIN(a, b):              Get the minimum of two values
  * AIL_CLAMP(x, min, max):     Returns the closest value to x in the range [min; max]
  * AIL_IS_2POWER(x):           Returns whether x is a power of 2
  * AIL_NEXT_2POWER(x):         Get the next highest value above x that is a power of 2 (if x isn't already a power of 2)
  * AIL_LERP(t, min, max):      Linearly interpolate between min and max
  * AIL_INV_LERP(x, min, max):  Does the inverse of a linear interpolation, returning the interpolater, such that the following holds: AIL_LERP(AIL_INV_LERP(x, min, max), min, max) == x
  *
  * AIL_KB(n): Get n kilobyte
  * AIL_MB(n): Get n megabyte
  * AIL_GB(n): Get n gigabyte
  * AIL_TB(n): Get n terabyte
  *
  * AIL_ASSERT(expr, [msg]):        Assert that expr is true with an optional message
  * AIL_STATIC_ASSERT(cond, [msg]): Statically assert that cond is true with an optional message
  * AIL_PANIC(...):                 Panic and exit the program. Any input will be given as input to printf
  * AIL_TODO():                     Panic when hitting this place while the program is running
  * AIL_UNREACHABLE():              Panic when hitting this place while the program is running
  *
  * AIL_UNUSED(x):      To ignore compiler warnings if x is unused
  * AIL_LIKELY(expr):   Indicate that the expression expr is most often true
  * AIL_UNLIKELY(expr): Indicate that the expression expr is most often false
  * AIL_FLAG_ENUM:      Mark this enum as a bitfield
  *
  * AIL_WARN_PUSH: Store the current warning level (presumably to change it temporarily)
  * AIL_WARN_POP:  Reset the latest saved warning level
  * AIL_WARN_ENABLE(warning):  Enable warning for the specified warning
  * AIL_WARN_DISABLE(warning): Disable warning for the specified warning
  * AIL_WARN_ERROR(wanring):   Set the specified warning as an error
  * AIL_Warn_Kind is an enum containing all the warnings you can specify in macros like AIL_WARN_ENABLE
  *
  * AIL_STRINGIFY(x): Turn the token x into a string (useful for macros working with printing)
  * AIL_STR_LINE:     The current line (via __LINE__) as a string
  * AIL_CONCAT(a, b): Concatenate two tokens to a single token (mainly useful for macros)
  * AIL_EXPAND(x):    Expand a token given to a macro (mainly useful for macros)
  * AIL_VA_LEN(...):  Get the amount of arguments provided to AIL_VA_LEN - useful for finding the amount of VA_ARGS
  * AIL_IS_EMPTY(...):                Check whether the given var-arg list is empty or not (doesn't work for conditional compilation)
  * AIL_VFUNC(name, ...): Overload a macro on the amount of its arguments
    * only works if there's 64 or fewer arguments
    * every overloaded version of the macro must be named '<name><number_of_arguments>'
*
*
*** Compiler Extensions ***
* Macros for checking whether specific compiler extensions exist
* These should not be necessary to use in most cases, as `ail` already provides portable wrappers for most compiler extensions
  * ail_has_builtin(builtin):         Check whether the given built-in function is supported by the compiler/architecture
  * ail_has_attribute(attribute):     Check whether the given attribute is supported by the compiler.
  * ail_has_cpp_attribute(attribute): Check whether the given c++-specific attribute is supported by the compiler
  * ail_has_extension(extension):     Check whether the given extension is enabled by the compiler
  * ail_has_warning(warning):         Check whether the provided warning is active. The warning must be one of the AIL_Warn_Kind enum
*
*
*** Function Specifiers ***
* Functions specifiers can be listed before a function declaration to provide additional documentation and extended warnings if supported by the compiler
  * ail_print_format(str_idx, first_arg_idx): Mark the parameters of this function to abide by the formatting used in printf. `str_idx` is the index of the parameter containing the format string, while `first_arg_idx` is index of the first input to the format
  * ail_deprecated([since, [replacement]]): Mark a function as deprecated, with optional strings indicating since when the function's deprecated and what should be used instead
  * ail_no_unused_result([msg]): Forbid ignoring the return value of this function
  * ail_no_return: Declare that this function never returns. Examples for such functions would be `abort` or `exit`
  * ail_pure:      Mark this function as a pure function, meaning this function doesn't change the state of the program outside of its return value
  * ail_const:     Mark this function as constant, meaning the result is not influenced by the state of the program outside of its input values
  * ail_no_inline: Prevent this function from being inlined
  * ail_non_null:  Indicates that the function's return value is a non-null pointer
*
*
* A substantial amount of code for macros was adapted from "Hedley" (https://nemequ.github.io/hedley)
* Hedley was created by Evan Nemerson <evan@nemerson.com>
* Hedley is under public domain. For details, see <http://creativecommons.org/publicdomain/zero/1.0/>.
* Hedley's SPDX-License-Identifier: CC0-1.0
*/

#ifndef _AIL_BASE_H
#define _AIL_BASE_H_

#include <stdint.h> // For sized integer types
#include "./ail_platform.h"
#include "./ail_warn.h"


/////////////////////////
// Keywords
// internal, inline, persist, global, export, import
/////////////////////////
#ifndef internal
#   define internal static
#endif
#ifndef inline
#   define inline static inline
#endif
#define persist static
#define global  static
// export, import
#if AIL_OS_WIN || AIL_OS_CYGWIN
#   define export __declspec(dllexport)
#   define import __declspec(dllimport)
#elif AIL_COMP_CLANG || AIL_COMP_GCC
#   define export __attribute__((visibility("default")))
#   define import __attribute__((visibility("default")))
#else
#   define export
#   define import
#endif
// thread_local
#if !defined(thread_local) && (AIL_LANG_C && AIL_LANG_STANDARD >= 2023) || (AIL_LANG_CPP && AIL_LANG_STANDARD >= 2011)
    // Do nothing, because thread_local is already a keyword / defined macro
#elif AIL_LANG_C && AIL_LANG_STANDARD >= 2011
#   define thread_local _Thread_local
#elif AIL_COMP_MSVC
#   define thread_local __declspec(thread)
#elif AIL_COMP_CLANG || AIL_COMP_GCC
#   define thread_local __thread
#endif


/////////////////////////
// Integral Types
/////////////////////////
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
typedef char     c8;
#if AIL_LANG_C && AIL_LANG_STANDARD >= 2011
#   include <uchar.h>
    typedef char16_t c16;
    typedef char32_t c32;
#else
    typedef i16 c16;
    typedef i32 c32;
#endif
typedef char* pchar;
#if (!AIL_LANG_C || AIL_LANG_STANDARD < 2017) && !defined(bool)
#   define bool _Bool
#   define true  1
#   define false 0
#endif


/////////////////////////
// Allocator Interface
/////////////////////////
// Allocate a region of memory holding at least <size> bytes
#define AIL_CALL_ALLOC(allocator, size) (allocator).alloc((allocator).data, AIL_MEM_ALLOC, (size), NULL)

#define _AIL_CALL_CALLOC_3(allocator, nelem, size_el) (allocator).alloc((allocator).data, AIL_MEM_CALLOC, (nelem)*(size_el), NULL)
#define _AIL_CALL_CALLOC_2(allocator, size)           (allocator).alloc((allocator).data, AIL_MEM_CALLOC, (size),            NULL)
// Allocate a chunk of memory, that is cleared to zero, either by providing the size of the amount of elements and size of each element
#define AIL_CALL_CALLOC(allocator, ...) AIL_VFUNC(_AIL_CALL_CALLOC_, allocator, __VA_ARGS__)

// Move a previously allocated region of memory to a new place with at least <new_size> bytes
#define AIL_CALL_REALLOC(allocator, old_ptr, new_size) (allocator).alloc((allocator).data, AIL_MEM_REALLOC, (new_size), (old_ptr))

// Provide a hint to the allocator, that the previously allocated region of memory can be shrunk to only contain <new_size> bytes.
// The purpose of this is to reduce memory usage when finding out later that less memory than expected was needed.
// Some allcoators treat this as a no-op
#define AIL_CALL_SHRINK(allocator, old_ptr, new_size) (allocator).alloc((allocator).data, AIL_MEM_SHRINK, (new_size), (old_ptr))

// Frees a single chunk of memory. Many allocators only mark the given memory-chunk as allocatable again, without actually freeing it
#define AIL_CALL_FREE(allocator, old_ptr) (allocator).alloc((allocator).data, AIL_MEM_FREE, 0, (old_ptr))

// If the allocator holds several memory regions, it keeps all these regions, but marks them as unused
#define AIL_CALL_CLEAR_ALL(allocator) (allocator).alloc((allocator).data, AIL_MEM_CLEAR_ALL, 0, NULL)

// If the allocator holds several memory regions, it frees all of them except for one
#define AIL_CALL_FREE_ALL(allocator) (allocator).alloc((allocator).data, AIL_MEM_FREE_ALL, 0, NULL)

// The action that should be executed when calling the allocator proc
// @TODO: Potential other functions:
//   - RESERVE (only really makes sense for pager)
//   - MARK/RESET (only really makes sense for linear allocators)
typedef enum AIL_Allocator_Mode {
    AIL_MEM_ALLOC,
    AIL_MEM_CALLOC,
    AIL_MEM_REALLOC,
    AIL_MEM_SHRINK,
    AIL_MEM_FREE,
    AIL_MEM_FREE_ALL,
    AIL_MEM_CLEAR_ALL,
    AIL_MEM_MODE_COUNT,
} AIL_Allocator_Mode;

typedef void* (AIL_Allocator_Func)(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr);
typedef void* (*AIL_Allocator_Func_Ptr)(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr);

typedef struct AIL_Allocator {
    void *data; // Metadata required by allocator and provided in all function calls
    AIL_Allocator_Func_Ptr alloc;
} AIL_Allocator;


/////////////////////////
// Compiler Version & Extensions
/////////////////////////

#define _AIL_VERSION_ENCODE_(major, minor, patch) (((major) * 1000000) + ((minor) * 1000) + (patch))
#define _AIL_VERSION_CHECK_(version, major, minor, patch) ((version) >= _AIL_VERSION_ENCODE_(major, minor, patch))
// _AIL_VERSION_GCC_
#if AIL_COMP_GCC && AIL_COMP_GCC
#   if defined(__GNUC_PATCHLEVEL__)
#       define _AIL_VERSION_GCC_ _AIL_VERSION_ENCODE_(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#   else
#       define _AIL_VERSION_GCC_ _AIL_VERSION_ENCODE_(__GNUC__, __GNUC_MINOR__, 0)
#   endif
#else
#   define _AIL_VERSION_GCC_ (0)
#endif
// _AIL_VERSION_MSVC_
#if defined(_MSC_FULL_VER) && !defined(__ICL)
#   if (_MSC_FULL_VER >= 140000000)
#       define _AIL_VERSION_MSVC_ _AIL_VERSION_ENCODE_(_MSC_FULL_VER / 10000000, (_MSC_FULL_VER % 10000000) / 100000, (_MSC_FULL_VER % 100000) / 100)
#   else
#       define _AIL_VERSION_MSVC_ _AIL_VERSION_ENCODE_(_MSC_FULL_VER / 1000000, (_MSC_FULL_VER % 1000000) / 10000, (_MSC_FULL_VER % 10000) / 10)
#   endif
#elif AIL_COMP_MSVC && !defined(__ICL)
#   define _AIL_VERSION_MSVC_ _AIL_VERSION_ENCODE_(_MSC_VER / 100, _MSC_VER % 100, 0)
#else
#   define _AIL_VERSION_MSVC_ (0)
#endif
// _AIL_VERSION_INTEL_
#if defined(__INTEL_COMPILER) && !defined(__ICL)
#   if defined(__INTEL_COMPILER_UPDATE)
#       define _AIL_VERSION_INTEL_ _AIL_VERSION_ENCODE_(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, __INTEL_COMPILER_UPDATE)
#   else
#       define _AIL_VERSION_INTEL_ _AIL_VERSION_ENCODE_(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, 0)
#   endif
#else
#   define _AIL_VERSION_INTEL_ (0)
#endif
// _AIL_VERSION_ICL_
#if defined(__INTEL_COMPILER) && defined(__ICL) && defined(__INTEL_COMPILER_UPDATE)
#   define _AIL_VERSION_ICL_ _AIL_VERSION_ENCODE_(__INTEL_COMPILER, __INTEL_COMPILER_UPDATE, 0)
#else
#   define _AIL_VERSION_ICL_ (0)
#endif
// _AIL_VERSION_TCC_
#if defined(__TINYC__)
#   define _AIL_VERSION_TCC_ _AIL_VERSION_ENCODE_(__TINYC__ / 1000, (__TINYC__ / 100) % 10, __TINYC__ % 100)
#else
#   define _AIL_VERSION_TCC_ (0)
#endif
// _AIL_VERSION_DMC_
#if defined(__DMC__)
#   define _AIL_VERSION_DMC_ _AIL_VERSION_ENCODE_(__DMC__ >> 8, (__DMC__ >> 4) & 0xf, __DMC__ & 0xf)
#else
#   define _AIL_VERSION_DMC_ (0)
#endif
// _AIL_VERSION_PELLES_
#if defined(__POCC__)
#   define _AIL_VERSION_PELLES_ _AIL_VERSION_ENCODE_(__POCC__ / 100, __POCC__ % 100, 0)
#else
#   define _AIL_VERSION_PELLES_ (0)
#endif

// ail_has_attribute
#if defined(__has_attribute)
#   define ail_has_attribute(attribute) __has_attribute(attribute)
#else
#   define ail_has_attribute(attribute) (0)
#endif
// ail_has_cpp_attribute
#if defined(__has_cpp_attribute) && AIL_LANG_CPP
#   define ail_has_cpp_attribute(attribute) __has_cpp_attribute(attribute)
#else
#   define ail_has_cpp_attribute(attribute) (0)
#endif
// ail_has_builtin
#if defined(__has_builtin)
#   define ail_has_builtin(builtin) __has_builtin(builtin)
#else
#   define ail_has_builtin(builtin) (0)
#endif
// ail_has_extension
#if defined(__has_extension)
#   define ail_has_extension(extension) __has_extension(extension)
#else
#   define ail_has_extension(extension) (0)
#endif
// ail_has_warning
#if defined(__has_warning)
#   define ail_has_warning(warning) __has_warning(warning)
#else
#   define ail_has_warning(warning) (0)
#endif


/////////////////////////
// Function specifiers
/////////////////////////

// ail_deprecated([since[, replacement]])
#if _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 14, 0, 0) || _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define _ail_deprecated_0()                   __declspec(deprecated())
#   define _ail_deprecated_1(since)              __declspec(deprecated("Since " #since))
#   define _ail_deprecated_2(since, replacement) __declspec(deprecated("Since " #since "; use " #replacement))
#elif ail_has_extension(attribute_deprecated_with_message) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 5, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define _ail_deprecated_0()                   __attribute__(deprecated())
#   define _ail_deprecated_1(since)              __attribute__(deprecated("Since " #since))
#   define _ail_deprecated_2(since, replacement) __attribute__(deprecated("Since " #since "; use " #replacement))
#elif AIL_LANG_CPP && (AIL_LANG_STANDARD >= 2014)
#   define _ail_deprecated_0()                   [[deprecated()]]
#   define _ail_deprecated_1(since)              [[deprecated("Since " #since)]]
#   define _ail_deprecated_2(since, replacement) [[deprecated("Since " #since "; use " #replacement)]]
#elif ail_has_attribute(deprecated) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 3, 1, 0)
#   define _ail_deprecated_0()                   __attribute__((__deprecated__))
#   define _ail_deprecated_1(since)              __attribute__((__deprecated__))
#   define _ail_deprecated_2(since, replacement) __attribute__((__deprecated__))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 13, 10, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_,  6, 50, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_,   2021, 1, 0)
#   define _ail_deprecated_0()                   __declspec(deprecated)
#   define _ail_deprecated_1(since)              __declspec(deprecated)
#   define _ail_deprecated_2(since, replacement) __declspec(deprecated)
#else
#   define _ail_deprecated_0()
#   define _ail_deprecated_1(since)
#   define _ail_deprecated_2(since, replacement)
#endif
#define ail_deprecated(...) AIL_VFUNC(_ail_deprecated_, __VA_ARGS__)

// ail_no_unused_result([msg])
#if ail_has_attribute(warn_unused_result) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_,    3, 4, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define _ail_no_unused_result_0()    __attribute__((__warn_unused_result__))
#   define _ail_no_unused_result_1(msg) __attribute__((__warn_unused_result__))
#elif (ail_has_cpp_attribute(nodiscard) >= 201907L)
#   define _ail_no_unused_result_0()    [[nodiscard]]
#   define _ail_no_unused_result_1(msg) [[nodiscard(msg)]]
#elif ail_has_cpp_attribute(nodiscard)
#   define _ail_no_unused_result_0()    [[nodiscard]]
#   define _ail_no_unused_result_1(msg) [[nodiscard]]
#elif defined(_Check_return_)
#   define _ail_no_unused_result_0()    _Check_return_
#   define _ail_no_unused_result_1(msg) _Check_return_
#else
#   define _ail_no_unused_result_0()
#   define _ail_no_unused_result_1(msg)
#endif
#define ail_no_unused_result(...) AIL_VFUNC(_ail_no_unused_result_, __VA_ARGS__)

// ail_no_return
#if (AIL_LANG_C && AIL_LANG_STANDARD >= 2011)
#   define ail_no_return _Noreturn
#elif AIL_LANG_CPP && (AIL_LANG_STANDARD >= 2011)
#   define ail_no_return [[noreturn]]
#elif ail_has_attribute(noreturn) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_,    3, 3, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define ail_no_return __attribute__((__noreturn__))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 13, 10, 0) || _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define ail_no_return __declspec((noreturn))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_, 9, 0, 0)
#   define ail_no_return __declspec(noreturn)
#else
#   define ail_no_return
#endif

// ail_print_format(str_idx, first_arg_idx)
#if defined(__MINGW32__) && ail_has_attribute(format)
#   if defined(__USE_MINGW_ANSI_STDIO)
#       define ail_print_format(str_idx, first_arg_idx) __attribute__((__format__(ms_printf, str_idx, first_arg_idx)))
#   else
#       define ail_print_format(str_idx, first_arg_idx) __attribute__((__format__(gnu_printf, str_idx, first_arg_idx)))
#   endif
#elif ail_has_attribute(format) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 3, 1, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define ail_print_format(str_idx, first_arg_idx) __attribute__((__format__(__printf__, str_idx, first_arg_idx)))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_, 6, 0, 0)
#   define ail_print_format(str_idx, first_arg_idx) __declspec(vaformat(printf, str_idx, first_arg_idx))
#else
#   define ail_print_format(str_idx, first_arg_idx)
#endif

// ail_pure
#if ail_has_attribute(pure) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 2, 96, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define ail_pure __attribute__((__pure__))
#else
#   define ail_pure
#endif

// ail_const
#if ail_has_attribute(const) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 2, 5, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define ail_const __attribute__((__const__))
#else
#   define ail_const ail_pure
#endif

// ail_no_inline
#if ail_has_attribute(noinline) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 0, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define ail_no_inline __attribute__((__noinline__))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 13, 10, 0) || _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define ail_no_inline __declspec((noinline))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_, 9, 0, 0)
#   define ail_no_inline __declspec(noinline)
#else
#   define ail_no_inline
#endif

// ail_non_null
#if ail_has_attribute(returns_nonnull) || _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 9, 0)
#   define ail_non_null __attribute__((__returns_nonnull__))
#elif defined(_Ret_notnull_)
#   define ail_non_null _Ret_notnull_
#else
#   define ail_non_null
#endif


/////////////////////////
// Useful Macros
/////////////////////////

#define AIL_EXPAND(x) x

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
#define AIL_VA_LEN(...)  __AIL_NARG_I_(__VA_ARGS__,__AIL_RSEQ_N())

#define __AIL_CONCAT__1(A, B) A##B
#define __AIL_CONCAT__(A, B) __AIL_CONCAT__1(A, B)
#define AIL_VFUNC(func, ...) AIL_EXPAND(__AIL_CONCAT__(func, AIL_VA_LEN(__VA_ARGS__))(__VA_ARGS__))

// Implementation taken from here: https://stackoverflow.com/a/55420185
#define AIL_IS_EMPTY(...) (sizeof((char[]){#__VA_ARGS__}) == 1)

#define _AIL_STRINGIFY2(x) #x
#define AIL_STRINGIFY(x) _AIL_STRINGIFY2(x)
#define AIL_STR_LINE AIL_STRINGIFY(__LINE__)

#define _AIL_CONCAT_0()
#define _AIL_CONCAT_1(X) A
#define _AIL_CONCAT_2(A, B) A##B
#define _AIL_CONCAT_3(A, B, C)  A##B##C
#define _AIL_CONCAT_4(A, B, C, D)  A##B##C##D
#define _AIL_CONCAT_5(A, B, C, D, E) A##B##C##D##E
#define _AIL_CONCAT_6(A, B, C, D, E, F) A##B##C##D##E##F
#define _AIL_CONCAT_7(A, B, C, D, E, F, G) A##B##C##D##E##F##G
#define _AIL_CONCAT_8(A, B, C, D, E, F, G, H) A##B##C##D##E##F##G##H
#define AIL_CONCAT(...) AIL_VFUNC(_AIL_CONCAT_, __VA_ARGS__)


// AIL_WARN_PUSH && AIL_WARN_POP
#if AIL_COMP_CLANG
#   define AIL_WARN_PUSH _Pragma("clang diagnostic push")
#   define AIL_WARN_POP  _Pragma("clang diagnostic pop")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_WARN_PUSH _Pragma("warning(push)")
#   define AIL_WARN_POP  _Pragma("warning(pop)")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 6, 0)
#   define AIL_WARN_PUSH _Pragma("GCC diagnostic push")
#   define AIL_WARN_POP  _Pragma("GCC diagnostic pop")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 15, 0, 0) || _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define AIL_WARN_PUSH _Pragma("warning(push)")
#   define AIL_WARN_POP  _Pragma("warning(pop)")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_, 2, 90, 0)
#   define AIL_WARN_PUSH _Pragma("warning(push)")
#   define AIL_WARN_POP  _Pragma("warning(pop)")
#else
#   define AIL_WARN_PUSH
#   define AIL_WARN_POP
#endif

#define AIL_WARN_ENABLE(warning_name)  AIL_EXPAND(AIL_CONCAT(_AIL_WARN_ENABLE_,  warning_name))
#define AIL_WARN_DISABLE(warning_name) AIL_EXPAND(AIL_CONCAT(_AIL_WARN_DISABLE_, warning_name))
#define AIL_WARN_ERROR(warning_name)   AIL_EXPAND(AIL_CONCAT(_AIL_WARN_ERROR_,   warning_name))

// AIL_LIKELY && AIL_UNLIKELY
#if (ail_has_builtin(__builtin_expect) && !_AIL_VERSION_ICL_) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 3, 0, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_TCC_, 0, 9, 27) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0) || \
    AIL_COMP_CLANG
#   define AIL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#   define AIL_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#else
#   define AIL_UNLIKELY(expr) (expr)
#   define AIL_LIKELY(expr)   (expr)
#endif

// fallthrough
#if ail_has_attribute(fallthrough) || _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 7, 0, 0)
#   define fallthrough __attribute__((__fallthrough__))
#elif ail_has_cpp_attribute(fallthrough)
#   define fallthrough [[fallthrough]]
#elif defined(__fallthrough)
#   define fallthrough __fallthrough
#else
#   define fallthrough
#endif

// AIL_FLAG_ENUM
#if ail_has_attribute(flag_enum) && (!AIL_LANG_CPP || ail_has_warning("-Wbitfield-enum-conversion"))
#   define AIL_FLAG_ENUM __attribute__((__flag_enum__))
#else
#   define AIL_FLAG_ENUM
#endif

// AIL_TYPEOF
#if AIL_LANG_C && AIL_LANG_STANDARD >= 2023
#   define AIL_TYPEOF(x) typeof_unqual(x)
#elif AIL_LANG_CPP
#   define AIL_TYPEOF(x) decltype(x)
#elif AIL_COMP_GCC || AIL_OS_MINGW || AIL_COMP_CLANG
#   define AIL_TYPEOF(x) __typeof__(x)
#else
// No typeof possible
#endif

#if AIL_COMP_MSVC
#   define AIL_UNUSED(v)  (void)sizeof(v)
#else
#   define AIL_UNUSED(v)  (void)(v)
#endif

// @Note: Do not include "enum" in the declaration
#if defined(__GNUC__)
#   define AIL_PACK_BEGIN() __attribute__((__packed__))
#   define AIL_PACK_END()
#elif defined(_MSC_VER)
#   define AIL_PACK_BEGIN() _Pragma(pack(push, 1))
#   define AIL_PACK_END()   _Pragma(pack(pop))
#elif defined(__clang__) || defined(__TINYC__)
#   define AIL_PACK_BEGIN() __attribute__((packed))
#   define AIL_PACK_END()
#endif

#define AIL_ARRLEN(arr) (sizeof(arr) / sizeof(*(arr)))

// @Note: Not safe to use with expressions, that have side-effects (like AIL_MAX(x++, y++))
#define AIL_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define AIL_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define AIL_CLAMP(x, min, max) ((x) > (max) ? (max) : (x) < (min) ? (min) : (x))

#define _AIL_SWAP_PORTABLE_(T, x, y) do { T _swap_tmp_ = x; x = y; y = _swap_tmp_; } while(0)
#define AIL_SWAP_PORTABLE(T, x, y) _AIL_SWAP_PORTABLE_(T, x, y);
#ifdef AIL_TYPEOF
#   define AIL_SWAP(x, y) do { AIL_TYPEOF(x) _swap_tmp_ = x; x = y; y = _swap_tmp_; } while(0)
#else
#   define AIL_SWAP(x, y) do { x ^= y; y ^= x; x ^= y; } while(0)
#endif

// AIL_LERP(AIL_INV_LERP(x, min, max), min, max) = x
#define AIL_LERP(t, min, max) ((min) + (t)*((max) - (min)))
#define AIL_INV_LERP(x, min, max) (((double)(x) - (double)(min)) / ((double)(max) - (double)(min)))

#define AIL_KB(x) (((u64)(x)) << 10)
#define AIL_MB(x) (((u64)(x)) << 20)
#define AIL_GB(x) (((u64)(x)) << 30)
#define AIL_TB(x) (((u64)(x)) << 40)

// ail_debugtrap - largely copied from https://github.com/nemequ/portable-snippets/
#if ail_has_builtin(__builtin_debugtrap)
#   define ail_debugtrap() __builtin_debugtrap()
#elif ail_has_builtin(__debugbreak) || AIL_COMP_MSVC || AIL_COMP_INTEL
#   define ail_debugtrap() __debugbreak()
#elif AIL_COMP_DIGITAL_MARS && AIL_ARCH_X86 && AIL_32BIT
    inline void ail_debugtrap(void) { __asn int 3h; }
#elif AIL_ARCH_X86 && AIL_64BIT && (AIL_COMP_CLANG || AIL_COMP_GCC || AIL_COMP_TCC)
    inline void ail_debugtrap(void) { __asm__ __volatile__("int3"); }
#elif AIL_ARCH_ARM && AIL_64BIT && (AIL_COMP_CLANG || AIL_COMP_GCC || AIL_COMP_TCC)
    inline void ail_debugtrap(void) { __asm__ __volatile__(".inst 0xd4200000"); }
#elif AIL_ARCH_ARM && (AIL_COMP_CLANG || AIL_COMP_GCC || AIL_COMP_TCC)
    inline void ail_debugtrap(void) { __asm__ __volatile__(".inst 0xe7f001f0"); }
#elif AIL_OS_POSIX && defined(SIGTRAP)
#   include <signal.h>
#   define ail_debugtrap() raise(SIGTRAP)
#elif AIL_OS_POSIX && defined(SIGABRT)
#   include <signal.h>
#   define ail_debugtrap() raise(SIGABRT)
#else
#   define ail_debugtrap() (*(volatile size_t *)0 = 0)
#endif

#define _AIL_ASSERT_COMMON_(expr, msg) do { if (!(expr)) { _AIL_DBG_PRINT_("Assertion failed in " __FILE__ ":" AIL_STR_LINE "\n  " msg); ail_debugtrap(); } } while(0)
#define _AIL_ASSERT_2(expr, msg)       _AIL_ASSERT_COMMON_(expr, "with message '" msg "'")
#define _AIL_ASSERT_1(expr)            _AIL_ASSERT_COMMON_(expr, "with expression 'AIL_ASSERT(" AIL_STRINGIFY(expr) ")'")
#define AIL_ASSERT(...) AIL_VFUNC(_AIL_ASSERT_, __VA_ARGS__);

#define AIL_PANIC(...)    do { _AIL_DBG_PRINT_(__VA_ARGS__); _AIL_DBG_PRINT_("\n"); ail_debugtrap(); } while(0)
#define AIL_TODO()        do { _AIL_DBG_PRINT_("Hit TODO in " __FILE__ ":" AIL_STR_LINE "\n"); ail_debugtrap(); } while(0)
#define AIL_UNREACHABLE() do { _AIL_DBG_PRINT_("Reached an unreachable place in " __FILE__ ":" AIL_STR_LINE "\n"); ail_debugtrap(); } while(0)

// @TODO: Better static assert message
#ifdef __cpp_static_assert
#   define _AIL_STATIC_ASSERT2(expr, msg) static_assert(!!(expr), msg)
#elif __STDC_VERSION__ >= 202311L
#   define _AIL_STATIC_ASSERT2(expr, msg) static_assert(!!(expr), msg)
#elif __STDC_VERSION__ >= 201112L
#   include <assert.h>
#   define _AIL_STATIC_ASSERT2(expr, msg) _Static_assert(!!(expr), msg)
#else
#   define _AIL_STATIC_ASSERT_MSG2(expr, msg, line) char __ail_static_assertion_at_line##line[((!!(expr))*2)-1]; char *__ail_static_assertion_at_line##line_message = AIL_STRINGIFY(msg)
#   define _AIL_STATIC_ASSERT_MSG1(expr, msg, line) _AIL_STATIC_ASSERT_MSG2(expr, msg, line)
#   define _AIL_STATIC_ASSERT2(expr, msg)           _AIL_STATIC_ASSERT_MSG1(expr, msg, __LINE__)
#endif
#define _AIL_STATIC_ASSERT1(expr) _AIL_STATIC_ASSERT2(expr, __FILE__ ":" AIL_STR_LINE ": Static Assert failed")
#define AIL_STATIC_ASSERT(...) AIL_VFUNC(_AIL_STATIC_ASSERT, __VA_ARGS__)

#define AIL_OFFSETOF(ptr, field) (((char *) &(ptr)->field) - ((char *) (ptr)))

#define AIL_IS_2POWER_POS(x) (x && ((x & (x - 1)) == 0))
#define AIL_IS_2POWER(x)     ((x < 0) ? AIL_IS_2POWER_POS(-(x)) : AIL_IS_2POWER_POS(x))
#define AIL_NEXT_2POWER_POS(x, out) do {                                                                                                      \
        out = (x) - 1;                                                                                                                        \
        for (size_t _ail_next_2power_shift_ = 1; _ail_next_2power_shift_ < 8 * sizeof(x); _ail_next_2power_shift_ += _ail_next_2power_shift_) \
            out |= out >> _ail_next_2power_shift_;                                                                                            \
        out += (out+1 <= 1) + 1;                                                                                                              \
    } while(0)
#define AIL_NEXT_2POWER(x, out) do { if (x >= 0) AIL_NEXT_2POWER_POS(x, out); else { AIL_NEXT_2POWER_POS(-(x), out); out = -out; } } while(0)


#endif // _AIL_BASE_H_
