////////
// From ail.h
////////


/*
* This header contains general utilities used throughout the other ail.h libraries
*
* By default this file includes all implementation alongside the declarations. To not include any implementation, define the following before including this file
  * AIL_NO_IMPL:       include no implementations from any of the ail_* libraries included
  * AIL_NO_ALLOC_IMPL: include no implementation for allocators
* For the documentation of each of these, see below
*
* Define AIL_DEF (defaults to `static`), AIL_DEF_INLINE (defaults to `static inline`) if you want different function declarations as a default for all ail.h libraries
*
* Define AIL_MALLOC, AIL_CALLOC, AIL_REALLOC, AIL_FREE to redefine the default allocator for all ail.h libraries
* Alternatively reassign ail_default_allocator to your default allocator of choice
*
*
*** Type Definitions ***
* stdint.h & stdbool.h are included for specifically sized integer types and boolean types
* via typedefs, the following types are declared as shortened forms of the original
* Integers:  u8, u16, u32, u64, i8, i16, i32, i64
* Floats:    f32, f64
* Boolean:   b32
* C-Strings: pchar
* Furthermore, aliases for the static keyword are defined, that make the keyword's usage clearer:
* 'internal', 'persist', 'global'
*
*
*** Platform Identification Macros ***
* Heavily inspired by https://sourceforge.net/p/predef/wiki/Home/ and https://github.com/nemequ/hedley
* Each of the following macros is defined to be either 1 or 0
* Operating System (several can be true at once):
  * AIL_OS_WIN32
  * AIL_OS_WIN64
  * AIL_OS_LINUX
  * AIL_OS_UNIX
  * AIL_OS_POSIX
  * AIL_OS_GNU
  * AIL_OS_MAC
  * AIL_OS_IOS
  * AIL_OS_ANDROID
  * AIL_OS_WEB
  * AIL_OS_BSD
  * AIL_OS_MINGW32
  * AIL_OS_MINGW64
  * AIL_OS_CYGWIN
  * AIL_OS_SPARC
* CPU Architecture (several can be true at once):
  * AIL_ARCH_X86
  * AIL_ARCH_X64
  * AIL_ARCH_ARM
  * AIL_ARCH_ARM64
  * AIL_ARCH_MIPS
  * AIL_ARCH_RISCV
  * AIL_ARCH_RISCV64
* Compiler (at most one is true):
  * AIL_COMP_CLANG - https://clang.llvm.org/
  * AIL_COMP_MSVC - https://learn.microsoft.com/en-us/cpp/build/reference/compiling-a-c-cpp-program
  * AIL_COMP_GCC - https://gcc.gnu.org/
  * AIL_COMP_TCC - https://www.bellard.org/tcc/
  * AIL_COMP_PELLES - http://www.smorgasbordet.com/pellesc/
  * AIL_COMP_DIGITAL_MARS - https://digitalmars.com/
  * AIL_COMP_INTEL - https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html
  * AIL_COMP_EMSCRIPTEN - https://emscripten.org/
* Language (exactly one is true):
  * AIL_LANG_CPP
  * AIL_LANG_C
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
  * AIL_FALLTHROUGH:    Indicate that falling through is intended for this particular case of the switch-statement.
  * AIL_FLAG_ENUM:      Mark this enum as a bitfield
  *
  * AIL_IS_EMPTY(...):                Check whether the given var-arg list is empty or not (doesn't work for conditional compilation)
  * AIL_HAS_BUILTIN(builtin):         Check whether the given built-in function is supported by the compiler/architecture
  * AIL_HAS_ATTRIBUTE(attribute):     Check whether the given attribute is supported by the compiler.
  * AIL_HAS_CPP_ATTRIBUTE(attribute): Check whether the given c++-specific attribute is supported by the compiler
  * AIL_HAS_EXTENSION(extension):     Check whether the given extension is enabled by the compiler
  * AIL_HAS_WARNING(warning):         Check whether the provided warning is active. For example: AIL_HAS_WARNING("-Wformat")
  *
  * AIL_WARN_PUSH: Store the current warning level (presumably to change it temporarily)
  * AIL_WARN_POP:  Reset the latest saved warning level
  * AIL_WARN_NO_DEPRECATED:      Disable warnings for usage of deprecated functions
  * AIL_WARN_NO_UNKNOWN_PRAGMAS: Disable warnings regarding unknown pragmas
  * AIL_WARN_NO_CAST_QUAL:       Disable warning for casts that change qualifiers like 'const'
  * AIL_WARN_NO_UNUSED_FUNCTION: Disable warnings for unused functions
  *
  * AIL_PRINTF_FORMAT(str_idx, first_arg_idx): Mark the parameters of this function to abide by the formatting used in printf. `str_idx` is the index of the parameter containing the format string, while `first_arg_idx` is index of the first input to the format
  * AIL_DEPRECATED([since, [replacement]]): Mark a function as deprecated, with optional strings indicating since when the function's deprecated and what should be used instead
  * AIL_NO_UNUSED_RESULT([msg]): Forbid ignoring the return value of this function
  * AIL_NO_RETURN: Declare that this function never returns. Examples for such functions would be `abort` or `exit`
  * AIL_PURE:      Mark this function as a pure function, meaning this function doesn't change the state of the program outside of its return value
  * AIL_CONST:     Mark this function as constant, meaning the result is not influenced by the state of the program outside of its input values
  * AIL_NO_INLINE: Prevent this function from being inlined
  * AIL_NON_NULL:  Indicates that the function's return value is a non-null pointer
  *
  * AIL_STRINGIFY(x): Turn the token x into a string (useful for macros working with printing)
  * AIL_STR_LINE:     The current line (via __LINE__) as a string
  * AIL_CONCAT(a, b): Concatenate two tokens to a single token (mainly useful for macros)
  * AIL_EXPAND(x):    Expand a token given to a macro (mainly useful for macros)
  * AIL_VA_LEN(...):  Get the amount of arguments provided to AIL_VA_LEN - useful for finding the amount of VA_ARGS
  * AIL_VFUNC(name, ...): Overload a macro on the amount of its arguments
    * only works if there's 64 or fewer arguments
    * every overloaded version of the macro must be named '<name><number_of_arguments>'
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
*
*** Dynamic Array Template ***
* There are two common ways for implementing dynamic arrays in C.
* One is to put the required metadata right in front of the allocated array.
  * The main benefit here is the ease of use, since no custom type needs to be declared and NULL can be used as a valid initialization for an empty array.
  * However, this means also that there is no seperate type for dynamic arrays, making code more ambiguous and thus harder to read and potentially even more error-prone as the type checker is effectively bypassed here.
  * Furthermore, it using a pointer as a type to the dynamic array also forces one to remember to overwrite the variable whenever the array was potentially resized.
* The other way is to create a seperate structure for each type that a dynamic array is required for
  * The main negative here is the overhead of these additional type-declarations
  * Zero-Initialization stays possible here. However, the type system is not bypassed and the variable containing the array never needs to be overwritten by the user, as this can be ensured by macros instead
  * Additionally, since all functionality can be implemented via macros, users can provide any custom structures as long as they contain the fields `data`, `len` and `cap`
* This library implements the latter option
* Since all functionality is implemented via macros, dynamic arrays are implemented similarly to a template in C++ or an interface in a duck-typed language like python
* Every dynamic array structure is expected to have the following fields by the macros:
  * data:      The pointer to the actual array of elementss
  * len:       The amount of elements currently stored in this array
  * cap:       The amount of elements currently allocated for this array
  * allcoator: [optional] An AIL_Allocator struct. All frees/allocations for this dynamic array are done through this allocator
    * This field is optional. Instead of storing this allocator as a field of the struct, you can also provide the allocator with every macro
* To reduce the overhead of the additional type-declarations, it offers two utility macros:
  * AIL_DA_INIT(T): Declare the type "AIL_DA_<T>" as a dynamic array of Ts
    * This type includes the required fields `data`, `len`, `cap` as well as the optional `allocator` field, which contains an AIL_Allocator struct
    * Because the type T is used for the struct's name, "T" cannot contain any spaces. Instead of "AIL_DA_INIT(unsigned int)" you have to use a typedef to map "unsigned int" to something like "uint"
  * AIL_DA(T): Get the name of the type declared in AIL_DA_INIT(T) - this simply maps to "AIL_DA_<T>", but is easier to remember & removes reliance on this implementation detail
* The following is a list of macros to ease the initialization of dynamic arrays
* @Note: Zero-initialization is also valid if you do not store the allocator as part of the dynamic array
* @Note: All of these macros expect that this dynamic array contains the `allocator` field
* @Note: As with all initialization macros in the ail libraries, each macro also exists with a `_t` suffix to provide the type name before the struct literal
  * ail_da_from_parts(d, l, c, al):   Create a dynamic array of elemments with d pointing at the array, l providing the length, c the capacity and al the allocator
  * ail_da_new_with_alloc(T, c, al): Allocate a new array with a capacity of `c` using the allocator `al`. Length is set to 0
  * ail_da_new_zero_alloc(T, c, al): Same as ail_da_new_with_alloc, but calls calloc instead of alloc for the array
  * ail_da_new_with_cap(T, c):       Same as ail_da_new_with_alloc, but using the default allocator
  * ail_da_new_zero_init(T, c):      Same as ail_da_new_with_cap, but using calloc instead of alloc
  * ail_da_new(T):                   Same as ail_da_new_with_cap, but with a default capacity defined in AIL_DA_INIT_CAP
  * ail_da_new_empty(T):             Creates an empty list without any allocations
* In the following, all other available macros are listed.
* @Note: For each macro potentially requiring allocation/freeing, another version with the suffix `_a` exists, allowing you to provide an allocator, if no allocator is stored with the dynamic array
  * ail_da_free(daPtr):                   Free the underlying array and set capacity and length to 0
  * ail_da_setn(daPtr, idx, elems, n):    Overwrite the elements from idx to idx+n to the elements in the array elems
  * ail_da_push(daPtr, elem):             Add a new element to the end of the array
  * ail_da_pushn(daPtr, elems, n):        Add n new elements from the array elems to the end of the array
  * ail_da_insert(daPtr, idx, elem):      Insert a new element at index idx of the array, pushing all elements afterwards back. The new element will be stored at index idx afterwards
  * ail_da_insertn(daPtr, idx, elems, n): Insert n new elements from the array elems into the array at index idx
  * ail_da_rm(daPtr, idx):                Remove an element from the array, moving all elements afterwards forward - this keeps all elements in the same order
  * ail_da_rmn(daPtr, idx, n):            Remove n elements from the array, moving all elements afterwards forward - this keeps all elements in the same order
  * ail_da_rm_swap(daPtr, idx):           Remove the element at index idx from the array, by swapping it with the last element - this does not keep the array's order, but is potentially much faster
  * ail_da_printf(da, format, ...):       Print all elements from the array to the console using printf. The format is regarding an individual element, which can be accessed using (da).data[i]
  * ail_da_resize(daPtr, newCap):              Resize the underlying array to the new capacity. Usually not directly called by user code
  * ail_da_maybe_grow(daPtr, n):               Ensure the underlying array has the capacity for at least n elements, potentially resizing it if necessary. Usually not directly called by user code
  * ail_da_maybe_grow_with_gap(daPtr, idx, n): Same as ail_da_maybe_grow, but leaving a gap at index idx when resizing. This is needed for insertions. Usually not directly called by user code
  * ail_da_grow_with_gap(daPtr, gapStart, gapLen, newCap, elSize): Grows the array to the new capacity, but leaving a gap at gapStart. This is needed for insertions. Usually not directly called by user code
*
*
* A substantial amount of code for macros was adapted from "Hedley" (https://nemequ.github.io/hedley)
* Hedley was created by Evan Nemerson <evan@nemerson.com>
* Hedley is under public domain. For details, see <http://creativecommons.org/publicdomain/zero/1.0/>.
* Hedley's SPDX-License-Identifier: CC0-1.0
*
*** LICENSE ***
Copyright (c) 2024 Lily Val Richter

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
#   include <stdlib.h>
#   define AIL_MALLOC(sz)          malloc(sz)
#   define AIL_REALLOC(ptr, sz)    realloc(ptr, sz)
#   define AIL_CALLOC(len, elSize) calloc(len, elSize)
#   define AIL_FREE(ptr)           free(ptr)
#elif !defined(AIL_MALLOC) || !defined(AIL_REALLOC) || !defined(AIL_CALLOC) || !defined(AIL_FREE)
#   error "You must define all of AIL_MALLOC, AIL_CALLOC, AIL_REALLOC and AIL_FREE or none of them. You cannot only define one or two of them."
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


/////////////////////////
// Custom Typedefs
/////////////////////////
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
typedef char*    pchar;

#define internal static
#define persist  static
#define global   static


/////////////////////////
// Platform Identification Macros
/////////////////////////

#if defined(_WIN32) || defined(__WIN32__)
#	define AIL_OS_WIN32 1
#else
#   define AIL_OS_WIN32 0
#endif
#if defined(_WIN64)
#	define AIL_OS_WIN64 1
#else
#   define AIL_OS_WIN64 0
#endif
#if defined(__linux__)
#	define AIL_OS_LINUX 1
#else
#   define AIL_OS_LINUX 0
#endif
#if defined(__unix)
#	define AIL_OS_UNIX 1
#else
#   define AIL_OS_UNIX 0
#endif
#if defined(__posix)
#	define AIL_OS_POSIX 1
#else
#   define AIL_OS_POSIX 0
#endif
#if defined(__GNU__)
#	define AIL_OS_GNU 1
#else
#   define AIL_OS_GNU 0
#endif
#if defined(__APPLE__) && defined(__MACH__)
#	define AIL_OS_MAC 1
#else
#   define AIL_OS_MAC 0
#endif
#if defined(AIL_OS_IOS) // @TODO
#	define AIL_OS_IOS 1
#else
#   define AIL_OS_IOS 0
#endif
#if defined(__ANDROID__)
#	define AIL_OS_ANDROID 1
#else
#   define AIL_OS_ANDROID 0
#endif
#if defined(AIL_OS_WEB) // @TODO
#	define AIL_OS_WEB 1
#else
#   define AIL_OS_WEB 0
#endif
#if defined(BSD)
#	define AIL_OS_BSD 1
#else
#   define AIL_OS_BSD 0
#endif
#if defined(__MINGW32__)
#	define AIL_OS_MINGW32 1
#else
#   define AIL_OS_MINGW32 0
#endif
#if defined(__MINGW64__)
#	define AIL_OS_MINGW64 1
#else
#   define AIL_OS_MINGW64 0
#endif
#if defined(__CYGWIN__)
#	define AIL_OS_CYGWIN 1
#else
#   define AIL_OS_CYGWIN 0
#endif
#if defined(__sparc__) || defined(__sparc)
#	define AIL_OS_SPARC 1
#else
#   define AIL_OS_SPARC 0
#endif

#if defined(__i386) || defined(_M_IX86) || defined(_X86_) || defined(__X86__) || defined(__I86__)
#	define AIL_ARCH_X86 1
#else
#   define AIL_ARCH_X86 0
#endif
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || defined(_M_AMD64) || defined(_M_X64)
#	define AIL_ARCH_X64 1
#else
#   define AIL_ARCH_X64 0
#endif
#if defined(__arm__) || defined(_M_ARM) || defined(_ARM)
#	define AIL_ARCH_ARM 1
#else
#   define AIL_ARCH_ARM 0
#endif
#if defined(__aarch64__)
#	define AIL_ARCH_ARM64 1
#else
#   define AIL_ARCH_ARM64 0
#endif
#if defined(__mips) || defined(__mips__) || defined(__MIPS__)
#	define AIL_ARCH_MIPS 1
#else
#   define AIL_ARCH_MIPS 0
#endif
#if defined(__riscv)
#	define AIL_ARCH_RISCV 1
#else
#   define AIL_ARCH_RISCV 0
#endif
#if defined(__riscv64)
#	define AIL_ARCH_RISCV64 1
#else
#   define AIL_ARCH_RISCV64 0
#endif

#if defined(__clang__)
#   define AIL_COMP_CLANG 1
#else
#   define AIL_COMP_CLANG 0
#endif
#if defined(_MSC_VER)
#   define AIL_COMP_MSVC 1
#else
#   define AIL_COMP_MSVC 0
#endif
#if defined(__GNUC__) && !AIL_COMP_CLANG
#   define AIL_COMP_GCC 1
#else
#   define AIL_COMP_GCC 0
#endif
#if defined(__TINYC__)
#   define AIL_COMP_TCC 1
#else
#   define AIL_COMP_TCC 0
#endif
#if defined(__POCC__)
#   define AIL_COMP_PELLES 1
#else
#   define AIL_COMP_PELLES 0
#endif
#if defined(__DMC__)
#   define AIL_COMP_DIGITAL_MARS 1
#else
#   define AIL_COMP_DIGITAL_MARS 0
#endif
#if defined(__INTEL_COMPILER)
#   define AIL_COMP_INTEL 1
#else
#   define AIL_COMP_INTEL 0
#endif
#if defined(__EMSCRIPTEN__)
#   define AIL_COMP_EMSCRIPTEN 1
#else
#   define AIL_COMP_EMSCRIPTEN 0
#endif

#if defined(__cplusplus)
#   define AIL_LANG_CPP 1
#   define AIL_LANG_C   0
#else
#   define AIL_LANG_CPP 0
#   define AIL_LANG_C   1
#endif


/////////////////////////
// Custom Utility Macros
/////////////////////////
#include <stdlib.h> // For exit

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

// Most of the following macros that deal with providing a unified interface for compiler extensions across compilers, have been adapted from Hedley:
// https://github.com/nemequ/hedley
// The original Hedley project supports even more compilers as well as older C versions
#define _AIL_VERSION_ENCODE_(major, minor, patch) (((major) * 1000000) + ((minor) * 1000) + (patch))
#define _AIL_VERSION_CHECK_(version, major, minor, patch) ((version) >= _AIL_VERSION_ENCODE_(major, minor, patch))
// _AIL_VERSION_GNUC_
#if defined(__GNUC__)
#   if defined(__GNUC_PATCHLEVEL__)
#       define _AIL_VERSION_GNUC_ _AIL_VERSION_ENCODE_(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#   else
#       define _AIL_VERSION_GNUC_ _AIL_VERSION_ENCODE_(__GNUC__, __GNUC_MINOR__, 0)
#   endif
#else
#   define _AIL_VERSION_GNUC_ (0)
#endif
// _AIL_VERSION_MSVC_
#if defined(_MSC_FULL_VER) && !defined(__ICL)
#   if (_MSC_FULL_VER >= 140000000)
#       define _AIL_VERSION_MSVC_ _AIL_VERSION_ENCODE_(_MSC_FULL_VER / 10000000, (_MSC_FULL_VER % 10000000) / 100000, (_MSC_FULL_VER % 100000) / 100)
#   else
#       define _AIL_VERSION_MSVC_ _AIL_VERSION_ENCODE_(_MSC_FULL_VER / 1000000, (_MSC_FULL_VER % 1000000) / 10000, (_MSC_FULL_VER % 10000) / 10)
#   endif
#elif defined(_MSC_VER) && !defined(__ICL)
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
// _AIL_VERSION_GCC_
#if defined(_AIL_VERSION_GNUC_) && !defined(__clang__) && !defined(_AIL_VERSION_INTEL_)
#   define _AIL_VERSION_GCC_ _AIL_VERSION_GNUC_
#else
#   define _AIL_VERSION_GCC_ (0)
#endif

// AIL_HAS_ATTRIBUTE
#if defined(__has_attribute)
#   define AIL_HAS_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
#   define AIL_HAS_ATTRIBUTE(attribute) (0)
#endif
// AIL_HAS_CPP_ATTRIBUTE
#if defined(__has_cpp_attribute) && defined(__cplusplus)
#   define AIL_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
#   define AIL_HAS_CPP_ATTRIBUTE(attribute) (0)
#endif
// AIL_HAS_BUILTIN
#if defined(__has_builtin)
#   define AIL_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
#   define AIL_HAS_BUILTIN(builtin) (0)
#endif
// AIL_HAS_EXTENSION
#if defined(__has_extension)
#   define AIL_HAS_EXTENSION(extension) __has_extension(extension)
#else
#   define AIL_HAS_EXTENSION(extension) (0)
#endif
// AIL_HAS_WARNING
#if defined(__has_warning)
#   define AIL_HAS_WARNING(warning) __has_warning(warning)
#else
#   define AIL_HAS_WARNING(warning) (0)
#endif

// AIL_WARN_PUSH && AIL_WARN_POP
#if defined(__clang__)
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
// AIL_WARN_NO_DEPRECATED
#if AIL_HAS_WARNING("-Wdeprecated-declarations")
#   define AIL_WARN_NO_DEPRECATED _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_WARN_NO_DEPRECATED _Pragma("warning(disable:1478 1786)")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define AIL_WARN_NO_DEPRECATED _Pragma("warning(disable:1478 1786)")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 3, 0)
#   define AIL_WARN_NO_DEPRECATED _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 15, 0, 0)
#   define AIL_WARN_NO_DEPRECATED _Pragma("warning(disable:4996)")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_, 2, 90, 0)
#   define AIL_WARN_NO_DEPRECATED _Pragma("warn(disable:2241)")
#else
#   define AIL_WARN_NO_DEPRECATED
#endif
// AIL_WARN_NO_UNKNOWN_PRAGMAS
#if AIL_HAS_WARNING("-Wunknown-pragmas")
#   define AIL_WARN_NO_UNKNOWN_PRAGMAS _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_WARN_NO_UNKNOWN_PRAGMAS _Pragma("warning(disable:161)")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define AIL_WARN_NO_UNKNOWN_PRAGMAS _Pragma("warning(disable:161)")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 3, 0)
#   define AIL_WARN_NO_UNKNOWN_PRAGMAS _Pragma("GCC diagnostic ignored \"-Wunknown-pragmas\"")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 15, 0, 0)
#   define AIL_WARN_NO_UNKNOWN_PRAGMAS _Pragma("warning(disable:4068)")
#else
#   define AIL_WARN_NO_UNKNOWN_PRAGMAS
#endif
// AIL_WARN_NO_CAST_QUAL
#if AIL_HAS_WARNING("-Wcast-qual")
#   define AIL_WARN_NO_CAST_QUAL _Pragma("clang diagnostic ignored \"-Wcast-qual\"")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_WARN_NO_CAST_QUAL _Pragma("warning(disable:2203 2331)")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 3, 0, 0)
#   define AIL_WARN_NO_CAST_QUAL _Pragma("GCC diagnostic ignored \"-Wcast-qual\"")
#else
#   define AIL_WARN_NO_CAST_QUAL
#endif
// AIL_WARN_NO_UNUSED_FUNCTION
#if AIL_HAS_WARNING("-Wunknown-function")
#   define AIL_WARN_NO_UNUSED_FUNCTION _Pragma("clang diagnostic ignored \"-Wunknown-function\"")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 3, 4, 0)
#   define AIL_WARN_NO_UNUSED_FUNCTION _Pragma("GCC diagnostic ignored \"-Wunknown-function\"")
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 1, 0, 0)
#   define AIL_WARN_NO_UNUSED_FUNCTION _Pragma("warning(disable:4505)")
#else
#   define AIL_WARN_NO_UNUSED_FUNCTION
#endif

// AIL_DEPRECATED([since[, replacement]])
#if _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 14, 0, 0) || _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define _AIL_DEPRECATED_0()                   __declspec(deprecated())
#   define _AIL_DEPRECATED_1(since)              __declspec(deprecated("Since " #since))
#   define _AIL_DEPRECATED_2(since, replacement) __declspec(deprecated("Since " #since "; use " #replacement))
#elif AIL_HAS_EXTENSION(attribute_deprecated_with_message) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 5, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define _AIL_DEPRECATED_0()                   __attribute__(deprecated())
#   define _AIL_DEPRECATED_1(since)              __attribute__(deprecated("Since " #since))
#   define _AIL_DEPRECATED_2(since, replacement) __attribute__(deprecated("Since " #since "; use " #replacement))
#elif defined(__cplusplus) && (__cplusplus >= 201402L)
#   define _AIL_DEPRECATED_0()                   [[deprecated()]]
#   define _AIL_DEPRECATED_1(since)              [[deprecated("Since " #since)]]
#   define _AIL_DEPRECATED_2(since, replacement) [[deprecated("Since " #since "; use " #replacement)]]
#elif AIL_HAS_ATTRIBUTE(deprecated) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 3, 1, 0)
#   define _AIL_DEPRECATED_0()                   __attribute__((__deprecated__))
#   define _AIL_DEPRECATED_1(since)              __attribute__((__deprecated__))
#   define _AIL_DEPRECATED_2(since, replacement) __attribute__((__deprecated__))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 13, 10, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_,  6, 50, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_,   2021, 1, 0)
#   define _AIL_DEPRECATED_0()                   __declspec(deprecated)
#   define _AIL_DEPRECATED_1(since)              __declspec(deprecated)
#   define _AIL_DEPRECATED_2(since, replacement) __declspec(deprecated)
#else
#   define _AIL_DEPRECATED_0()
#   define _AIL_DEPRECATED_1(since)
#   define _AIL_DEPRECATED_2(since, replacement)
#endif
#define AIL_DEPRECATED(...) AIL_VFUNC(_AIL_DEPRECATED_, __VA_ARGS__)

// AIL_NO_UNUSED_RESULT([msg])
#if AIL_HAS_ATTRIBUTE(warn_unused_result) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_,    3, 4, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define _AIL_NO_UNUSED_RESULT_0()    __attribute__((__warn_unused_result__))
#   define _AIL_NO_UNUSED_RESULT_1(msg) __attribute__((__warn_unused_result__))
#elif (AIL_HAS_CPP_ATTRIBUTE(nodiscard) >= 201907L)
#   define _AIL_NO_UNUSED_RESULT_0()    [[nodiscard]]
#   define _AIL_NO_UNUSED_RESULT_1(msg) [[nodiscard(msg)]]
#elif AIL_HAS_CPP_ATTRIBUTE(nodiscard)
#   define _AIL_NO_UNUSED_RESULT_0()    [[nodiscard]]
#   define _AIL_NO_UNUSED_RESULT_1(msg) [[nodiscard]]
#elif defined(_Check_return_)
#   define _AIL_NO_UNUSED_RESULT_0()    _Check_return_
#   define _AIL_NO_UNUSED_RESULT_1(msg) _Check_return_
#else
#   define _AIL_NO_UNUSED_RESULT_0()
#   define _AIL_NO_UNUSED_RESULT_1(msg)
#endif
#define AIL_NO_UNUSED_RESULT(...) AIL_VFUNC(_AIL_NO_UNUSED_RESULT_, __VA_ARGS__)

// AIL_NO_RETURN
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
#   define AIL_NO_RETURN _Noreturn
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
#   define AIL_NO_RETURN [[noreturn]]
#elif AIL_HAS_ATTRIBUTE(noreturn) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_,    3, 3, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_NO_RETURN __attribute__((__noreturn__))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 13, 10, 0) || _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define AIL_NO_RETURN __declspec((noreturn))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_, 9, 0, 0)
#   define AIL_NO_RETURN __declspec(noreturn)
#else
#   define AIL_NO_RETURN
#endif

// AIL_PRINTF_FORMAT(str_idx, first_arg_idx)
#if defined(__MINGW32__) && AIL_HAS_ATTRIBUTE(format)
#   if defined(__USE_MINGW_ANSI_STDIO)
#       define AIL_PRINTF_FORMAT(str_idx, first_arg_idx) __attribute__((__format__(ms_printf, str_idx, first_arg_idx)))
#   else
#       define AIL_PRINTF_FORMAT(str_idx, first_arg_idx) __attribute__((__format__(gnu_printf, str_idx, first_arg_idx)))
#   endif
#elif AIL_HAS_ATTRIBUTE(format) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 3, 1, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_PRINTF_FORMAT(str_idx, first_arg_idx) __attribute__((__format__(__printf__, str_idx, first_arg_idx)))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_, 6, 0, 0)
#   define AIL_PRINTF_FORMAT(str_idx, first_arg_idx) __declspec(vaformat(printf, str_idx, first_arg_idx))
#else
#   define AIL_PRINTF_FORMAT(str_idx, first_arg_idx)
#endif

// AIL_PURE
#if AIL_HAS_ATTRIBUTE(pure) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 2, 96, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_PURE __attribute__((__pure__))
#else
#   define AIL_PURE
#endif

// AIL_CONST
#if AIL_HAS_ATTRIBUTE(const) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 2, 5, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_CONST __attribute__((__const__))
#else
#   define AIL_CONST AIL_PURE
#endif

// AIL_NO_INLINE
#if AIL_HAS_ATTRIBUTE(noinline) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 0, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0)
#   define AIL_NO_INLINE __attribute__((__noinline__))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_MSVC_, 13, 10, 0) || _AIL_VERSION_CHECK_(_AIL_VERSION_ICL_, 2021, 1, 0)
#   define AIL_NO_INLINE __declspec((noinline))
#elif _AIL_VERSION_CHECK_(_AIL_VERSION_PELLES_, 9, 0, 0)
#   define AIL_NO_INLINE __declspec(noinline)
#else
#   define AIL_NO_INLINE
#endif

// AIL_LIKELY && AIL_UNLIKELY
#if (AIL_HAS_BUILTIN(__builtin_expect) && !_AIL_VERSION_ICL_) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 3, 0, 0) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_TCC_, 0, 9, 27) || \
    _AIL_VERSION_CHECK_(_AIL_VERSION_INTEL_, 13, 0, 0) || \
    defined(__clang__)
#   define AIL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#   define AIL_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#else
#   define AIL_UNLIKELY(expr) (!!(expr))
#   define AIL_LIKELY(expr)   (!!(expr))
#endif

// AIL_FALLTHROUGH
#if AIL_HAS_ATTRIBUTE(fallthrough) || _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 7, 0, 0)
#   define AIL_FALLTHROUGH __attribute__((__fallthrough__))
#elif AIL_HAS_CPP_ATTRIBUTE(fallthrough)
#   define AIL_FALLTHROUGH [[fallthrough]]
#elif defined(__fallthrough)
#   define AIL_FALLTHROUGH __fallthrough
#else
#   define AIL_FALLTHROUGH
#endif

// AIL_NON_NULL
#if AIL_HAS_ATTRIBUTE(returns_nonnull) || _AIL_VERSION_CHECK_(_AIL_VERSION_GCC_, 4, 9, 0)
#   define AIL_NON_NULL __attribute__((__returns_nonnull__))
#elif defined(_Ret_notnull_)
#   define AIL_NON_NULL _Ret_notnull_
#else
#   define AIL_NON_NULL
#endif

// AIL_FLAG_ENUM
#if AIL_HAS_ATTRIBUTE(flag_enum) && (!defined(__cplusplus) || AIL_HAS_WARNING("-Wbitfield-enum-conversion"))
#   define AIL_FLAG_ENUM __attribute__((__flag_enum__))
#else
#   define AIL_FLAG_ENUM
#endif

// AIL_TYPEOF
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L)
#   define AIL_TYPEOF(x) typeof_unqual(x)
#elif defined(__cplusplus)
#   define AIL_TYPEOF(x) decltype(x)
#elif defined(__GNUC__) || defined(_MINGW) || defined(__clang__)
#   define AIL_TYPEOF(x) __typeof__(x)
#else
// No typeof possible
#endif

#ifndef _AIL_DBG_PRINT_
#include <stdio.h>
#define _AIL_DBG_PRINT_ printf
#endif // _AIL_DBG_PRINT_

#if defined(_MSC_VER)
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

#define _AIL_DBG_EXIT_()               do { exit(1); } while(0)
#define _AIL_ASSERT_COMMON_(expr, msg) do { if (!(expr)) { _AIL_DBG_PRINT_("Assertion failed in " __FILE__ ":" AIL_STR_LINE "\n  " msg); _AIL_DBG_EXIT_(); } } while(0)
#define _AIL_ASSERT_2(expr, msg)       _AIL_ASSERT_COMMON_(expr, "with message '" msg "'")
#define _AIL_ASSERT_1(expr)            _AIL_ASSERT_COMMON_(expr, "with expression 'AIL_ASSERT(" AIL_STRINGIFY(expr) ")'")
#define AIL_ASSERT(...) AIL_VFUNC(_AIL_ASSERT_, __VA_ARGS__);

#define AIL_PANIC(...)    do { _AIL_DBG_PRINT_(__VA_ARGS__); _AIL_DBG_PRINT_("\n"); _AIL_DBG_EXIT_(); } while(0)
#define AIL_TODO()        do { _AIL_DBG_PRINT_("Hit TODO in " __FILE__ ":" AIL_STR_LINE "\n"); _AIL_DBG_EXIT_(); } while(0)
#define AIL_UNREACHABLE() do { _AIL_DBG_PRINT_("Reached an unreachable place in " __FILE__ ":" AIL_STR_LINE "\n"); _AIL_DBG_EXIT_(); } while(0)

// @TODO: Better static assert message
#ifdef __cpp_static_assert
#   define _AIL_STATIC_ASSERT2(cond, msg) static_assert(!!(cond), msg)
#elif __STDC_VERSION__ >= 202311L
#   define _AIL_STATIC_ASSERT2(cond, msg) static_assert(!!(cond), msg)
#elif __STDC_VERSION__ >= 201112L
#   include <assert.h>
#   define _AIL_STATIC_ASSERT2(cond, msg) _Static_assert(!!(cond), msg)
#else
#   define _AIL_STATIC_ASSERT_MSG2(cond, msg, line) char __ail_static_assertion_at_line##line[((!!(cond))*2)-1]; char *__ail_static_assertion_at_line##line_message = AIL_STRINGIFY(msg)
#   define _AIL_STATIC_ASSERT_MSG1(cond, msg, line) _AIL_STATIC_ASSERT_MSG2(cond, msg, line)
#   define _AIL_STATIC_ASSERT2(cond, msg)           _AIL_STATIC_ASSERT_MSG1(cond, msg, __LINE__)
#endif
#define _AIL_STATIC_ASSERT1(cond) _AIL_STATIC_ASSERT2(cond, __FILE__ ":" AIL_STR_LINE ": Static Assert failed")
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



/////////////////////////
// General Allocator Interface
// implementation is at bottom of file
/////////////////////////
#define _AIL_ALLOCATOR_SIZE_TYPE_ u64

// Allocate a region of memory holding at least <size> bytes
#define AIL_CALL_ALLOC(allocator, size) (allocator).alloc((allocator).data, AIL_MEM_ALLOC, (size), NULL)

#define _AIL_CALL_CALLOC3(allocator, nelem, size_el) (allocator).alloc((allocator).data, AIL_MEM_CALLOC, (nelem)*(size_el), NULL)
#define _AIL_CALL_CALLOC2(allocator, size)           (allocator).alloc((allocator).data, AIL_MEM_CALLOC, (size),            NULL)
// Allocate a chunk of memory, that is cleared to zero, either by providing the size of the amount of elements and size of each element
#define AIL_CALL_CALLOC(...) AIL_VFUNC(_AIL_CALL_CALLOC, __VA_ARGS__)

// Move a previously allocated region of memory to a new place with at least <new_size> bytes
#define AIL_CALL_REALLOC(allocator, old_ptr, new_size) (allocator).alloc((allocator).data, AIL_MEM_REALLOC, (new_size), (old_ptr))

// Provide a hint to the allocator, that the previously allocated region of memory can be shrunk to only contain <new_size> bytes.
// The purpose of this is to reduce memory usage when finding out later that less memory than expected was needed.
// Some allcoators treat this as a no-op
#define AIL_CALL_SHRINK(allocator, old_ptr, new_size) (allocator).alloc((allocator).data, AIL_MEM_SHRINK, (new_size), (old_ptr))

// Frees a single chunk of memory. Many allocators only mark the given memory-chunk as allocatable again, without actually freeing it
#define AIL_CALL_FREE(allocator, old_ptr) (allocator).alloc((allocator).data, AIL_MEM_FREE, 0, (old_ptr))

// If the allocator holds several memory regions, it keeps all these regions, but marks them as unused
// @Note: Not implemented by the default c-runtime allocator
#define AIL_CALL_CLEAR_ALL(allocator) (allocator).alloc((allocator).data, AIL_MEM_CLEAR_ALL, 0, NULL)

// If the allocator holds several memory regions, it frees all of them except for one
// @Note: Not implemented by the default c-runtime allocator
#define AIL_CALL_FREE_ALL(allocator) (allocator).alloc((allocator).data, AIL_MEM_FREE_ALL, 0, NULL)

// The action that should be executed when calling the allocator proc
typedef enum AIL_Allocator_Mode {
    AIL_MEM_ALLOC,
    AIL_MEM_CALLOC,
    AIL_MEM_REALLOC,
    AIL_MEM_SHRINK,
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

AIL_DEF void* ail_default_alloc(void *data, AIL_Allocator_Mode mode, _AIL_ALLOCATOR_SIZE_TYPE_ size, void *old_ptr);


/////////////////////////
// Dynamic Array Implementation
// Define `AIL_DA_INIT_CAP` to set a different initial capacity for dynamic arrays
// This implementation is heavily inspired by:
// - nob.h (https://github.com/tsoding/musializer/blob/master/src/nob.h)
// - stb_ds.h (https://github.com/nothings/stb/blob/master/stb_ds.h)
/////////////////////////
#include <string.h> // For memcpy
#ifndef AIL_DA_PRINT
#include <stdio.h> // For printf
#define AIL_DA_PRINT printf
#endif

#ifndef AIL_DA_INIT_CAP
#define AIL_DA_INIT_CAP 256
#endif

// @TODO: Rename AIL_ARR
#define AIL_SLICE_INIT(T) typedef struct AIL_SLICE_##T { T *data; u64 len; } AIL_SLICE_##T
#define AIL_SLICE(T) AIL_SLICE_##T
#define AIL_ARR_INIT(T) typedef struct AIL_ARR_##T { T *data; u64 len; u64 cap; } AIL_ARR_##T
#define AIL_ARR(T) AIL_ARR_##T
#define AIL_DA_INIT(T) typedef struct AIL_DA_##T { T *data; u64 len; u64 cap; AIL_Allocator allocator; } AIL_DA_##T
#define AIL_DA(T) AIL_DA_##T
AIL_DA_INIT(u8);    AIL_SLICE_INIT(u8);    AIL_ARR_INIT(u8);
AIL_DA_INIT(u16);   AIL_SLICE_INIT(u16);   AIL_ARR_INIT(u16);
AIL_DA_INIT(u32);   AIL_SLICE_INIT(u32);   AIL_ARR_INIT(u32);
AIL_DA_INIT(u64);   AIL_SLICE_INIT(u64);   AIL_ARR_INIT(u64);
AIL_DA_INIT(i8);    AIL_SLICE_INIT(i8);    AIL_ARR_INIT(i8);
AIL_DA_INIT(i16);   AIL_SLICE_INIT(i16);   AIL_ARR_INIT(i16);
AIL_DA_INIT(i32);   AIL_SLICE_INIT(i32);   AIL_ARR_INIT(i32);
AIL_DA_INIT(i64);   AIL_SLICE_INIT(i64);   AIL_ARR_INIT(i64);
AIL_DA_INIT(f32);   AIL_SLICE_INIT(f32);   AIL_ARR_INIT(f32);
AIL_DA_INIT(f64);   AIL_SLICE_INIT(f64);   AIL_ARR_INIT(f64);
AIL_DA_INIT(pchar); AIL_SLICE_INIT(pchar); AIL_ARR_INIT(pchar);
AIL_DA_INIT(void);  AIL_SLICE_INIT(void);  AIL_ARR_INIT(void);
AIL_DA_INIT(char);  AIL_SLICE_INIT(char);  AIL_ARR_INIT(char);


#define ail_slice_from_parts(d, l) { .data = (d),       .len = (l) }
#define ail_slice_from_arr(arr)    { .data = (arr),     .len = AIL_ARRLEN(arr) }
#define ail_slice_from_da(da)      { .data = (da).data, .len = (da).len }
#define ail_slice_from_parts_t(T, d, l) (AIL_SLICE(T))ail_slice_from_parts(d, l)
#define ail_slice_from_arr_t(T, arr)    (AIL_SLICE(T))ail_slice_from_arr(arr)
#define ail_slice_from_da_t(T, da)      (AIL_SLICE(T))ail_slice_from_da(da)

#define ail_da_from_parts(d, l, c, al)  { .data = (d), .len = (l), .cap = (c), .allocator = (al) }
#define ail_da_new_with_alloc(T, c, al) { .data = AIL_CALL_ALLOC ((al), sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = (al) }
#define ail_da_new_zero_alloc(T, c, al) { .data = AIL_CALL_CALLOC((al), sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = (al) }
#define ail_da_new_with_cap(T, c)       { .data = AIL_CALL_ALLOC(ail_default_allocator, sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = ail_default_allocator }
#define ail_da_new_zero_init(T, c)      { .data = AIL_CALL_CALLOC(ail_default_allocator, sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = ail_default_allocator }
#define ail_da_new(T)                   { .data = AIL_CALL_ALLOC(ail_default_allocator, sizeof(T) * AIL_DA_INIT_CAP), .len = 0, .cap = AIL_DA_INIT_CAP, .allocator = ail_default_allocator }
#define ail_da_new_empty(T)             { .data = NULL, .len = 0, .cap = 0, .allocator = ail_default_allocator }
#define ail_da_new_zero_init_t(T, c)        (AIL_DA(T))ail_da_new_zero_init()
#define ail_da_new_empty_t(T)               (AIL_DA(T))ail_da_new_empty(T)
#define ail_da_new_t(T)                     (AIL_DA(T))ail_da_new(T)
#define ail_da_new_with_cap_t(T, c)         (AIL_DA(T))ail_da_new_with_cap(T, c)
#define ail_da_new_zero_alloc_t(T, c, al)   (AIL_DA(T))ail_da_new_zero_alloc(T, c, al)
#define ail_da_new_with_alloc_t(T, c, al)   (AIL_DA(T))ail_da_new_with_alloc(T, c, al)
#define ail_da_from_parts_t(T, d, l, c, al) (AIL_DA(T))ail_da_from_parts(d, l, c, al)

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
        memcpy((daPtr)->data + (daPtr)->len, (elems), sizeof(*((daPtr)->data))*(n)); \
        (daPtr)->len += (n);                                                             \
    } while(0)

#define ail_da_grow_with_gap(daPtr, gapStart, gapLen, newCap, elSize) ail_da_grow_with_gap_a(daPtr, gapStart, gapLen, newCap, elSize, (daPtr)->allocator)
#define ail_da_grow_with_gap_a(daPtr, gapStart, gapLen, newCap, elSize, al) do {                                                           \
        (daPtr)->cap = (newCap);                                                                                                           \
        char *_ail_da_gwg_ptr_ = (char *) (daPtr)->data;                                                                                   \
        (daPtr)->data = AIL_CALL_ALLOC((al), (elSize)*(newCap));                                                                           \
        AIL_ASSERT((daPtr)->data != NULL);                                                                                                 \
        memcpy((daPtr)->data, _ail_da_gwg_ptr_, (elSize)*(gapStart));                                                                  \
        memcpy(&(daPtr)->data[((gapStart) + (gapLen))], &_ail_da_gwg_ptr_[(elSize)*(gapStart)], (elSize)*((daPtr)->len - (gapStart))); \
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



#if !defined(AIL_NO_ALLOC_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_ALLOCATOR_GUARD_
#define _AIL_ALLOCATOR_GUARD_

AIL_DEF void* ail_default_alloc(void *data, AIL_Allocator_Mode mode, _AIL_ALLOCATOR_SIZE_TYPE_ size, void *old_ptr)
{
    AIL_UNUSED(data);
    switch (mode) {
        case AIL_MEM_ALLOC:    return AIL_MALLOC(size);
        case AIL_MEM_CALLOC:   return AIL_CALLOC(size, 1);
        case AIL_MEM_REALLOC:  return AIL_REALLOC(old_ptr, size);
        case AIL_MEM_SHRINK:
        case AIL_MEM_FREE:
        case AIL_MEM_CLEAR_ALL:
        case AIL_MEM_FREE_ALL: return NULL;
    }
    AIL_UNREACHABLE();
    return NULL;
}

global AIL_Allocator ail_default_allocator = {
    .data  = NULL,
    .alloc = &ail_default_alloc,
};

// Function just exists to suppress of potential "unused ail_default_allocator" warning
AIL_DEF void __ail_default_allocator_unused__(void)
{
    AIL_UNUSED(ail_default_allocator);
}

#endif // _AIL_ALLOCATOR_GUARD_
#endif // AIL_NO_ALLOC_IMPL






////////
// From ail_alloc.h
////////


// Different types of Allocators
//
// Define AIL_NO_ALLOC_IMPL in some file, to not include any implementations
// Define AIL_ALLOC_ALIGNMENT to change the alignment used by all custom allocators
// Define AIL_ALLOC_PRINT_MEM to track (all) allocations
//
// Implementation of the Arena Allocator was inspired by tsoding's arena library (https://github.com/tsoding/arena/)
// and by gingerBill's blog post on Arena Allocators (https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/)
//
// @TODO: Add some way to drop-in replace C malloc calls without having to change the code
// @TODO: Add documentation explaining the different available Allocators
// @TODO: Add a way to only track allocations of a single allocator / allocator-type (?)
// @TODO: Add a way to track amount of used memory
// @TODO: Change the semantics of alloc/calloc to mean that if `old_ptr` is unequal to 0, it should try to allocate there (like mmap & VirtualAlloc do too)
// @TODO: Add "mark()" and "reset()" function for (temporary) allocators (could this be added to general purpose allocators like the freelist too?)
//
// LICENSE
/*
Copyright (c) 2024 Lily Val Richter

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


#ifndef AIL_ALLOC_H_
#define AIL_ALLOC_H_

#include <string.h> // For memcpy

#ifndef AIL_ALLOC_ALIGNMENT
#define AIL_ALLOC_ALIGNMENT 8 // Reasonable default for all 64bit machines
#endif // AIL_ALLOC_ALIGNMENT
#if AIL_IS_2POWER(AIL_ALLOC_ALIGNMENT) == false
#warning "AIL_ALLOC_ALIGNMENT should best be set to a power of two for almost all computer architectures"
#endif

#ifndef AIL_ALLOC_DEF
#ifdef  AIL_DEF
#define AIL_ALLOC_DEF AIL_DEF
#else
#define AIL_ALLOC_DEF
#endif // AIL_DEF
#endif // AIL_ALLOC_DEF
#ifndef AIL_ALLOC_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_ALLOC_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_ALLOC_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_ALLOC_DEF_INLINE

#define AIL_ALLOC_GET_HEADER(ptr, headerType) ((headerType *)(ptr) - 1)

// @Impl: sizes of allocations stored in headers never include the size of the header itself

typedef struct AIL_Alloc_Size_Header {
    u64 size;
} AIL_Alloc_Size_Header;

#define AIL_ALLOC_INIT_ALLOCATOR(name, region_params, params) \
    typedef struct AIL_Alloc_##name##_Region {                \
        region_params                                         \
        u64 region_size;                                      \
        struct AIL_Alloc_##name##_Region *region_next;        \
        u8 mem[0];                                            \
    } AIL_Alloc_##name##_Region;                              \
    typedef struct AIL_Alloc_##name {                         \
        params                                                \
        AIL_Allocator *backing_allocator;                     \
        u64 region_block_size;                                \
        AIL_Alloc_##name##_Region region_head;                \
    } AIL_Alloc_##name;

typedef AIL_Alloc_Size_Header AIL_Alloc_Page_Header;

typedef struct AIL_Alloc_Buffer {
    u64 size;
    u64 idx;
} AIL_Alloc_Buffer;
typedef AIL_Alloc_Buffer AIL_Alloc_Ring;

AIL_ALLOC_INIT_ALLOCATOR(Arena, u64 used;,)
typedef AIL_Alloc_Size_Header AIL_Alloc_Arena_Header;

typedef struct AIL_Alloc_Pool_Free_Node { struct AIL_Alloc_Pool_Free_Node *next; } AIL_Alloc_Pool_Free_Node;
AIL_ALLOC_INIT_ALLOCATOR(Pool,
    AIL_Alloc_Pool_Free_Node *head;,
    u64 bucket_amount;
    u64 bucket_size;
)

typedef struct AIL_Alloc_Freelist_Free_Node {
    u64 size;
    struct AIL_Alloc_Freelist_Free_Node *next;
} AIL_Alloc_Freelist_Free_Node;
typedef struct AIL_Alloc_Freelist_Header {
    u64 size;
    u64 pad;
} AIL_Alloc_Freelist_Header;
AIL_ALLOC_INIT_ALLOCATOR(Freelist,
    AIL_Alloc_Freelist_Free_Node *head;
    u64 region_used;,
)

typedef struct AIL_Alloc_Freelist_Node_Couple {
    AIL_Alloc_Freelist_Free_Node *node;
    AIL_Alloc_Freelist_Free_Node *prev;
} AIL_Alloc_Freelist_Node_Couple;


void* _ail_alloc_get_last_region_(u8 *list, u32 region_head_offset, u32 region_next_offset);
void* _ail_alloc_region_of_(u8 *list, u32 region_head_offset, u32 region_next_offset, u32 mem_offset, u32 region_size_offset, u8 *ptr);

//////////////
// Utility Functions
//////////////

// Aligns size to AIL_ALLOC_ALIGNMENT
AIL_ALLOC_DEF u64 ail_alloc_align_size(u64 size);
// @Note: Alignment must be greater than 0 and a power of 2
AIL_ALLOC_DEF u64 ail_alloc_align_forward(u64 n, u64 alignment);
// @Note: Alignment must be greater than 0 and a power of 2
AIL_ALLOC_DEF u64 ail_alloc_align_backward(u64 n, u64 alignment);
// Returns padding from aligning to AIL_ALLOC_ALIGNMENT
AIL_ALLOC_DEF u64 ail_alloc_size_aligned_pad(u64 size);
AIL_ALLOC_DEF u64 ail_alloc_size_aligned_backward_pad(u64 size, u64 alignment);
AIL_ALLOC_DEF u64 ail_alloc_size_aligned_forward_pad(u64 size, u64 alignment);


//////////////
// Std Allocator
// C's stdlib allocator
// @Note: free_all is a no-op
//////////////
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_std_alloc;
AIL_ALLOC_DEF_INLINE void __ail_alloc_std_unused__(void);

//////////////
// Page Allocator
// Allocates always complete pages
// Especially useful for use as a backing allocator
// @Note: When allocating in Page-Sizes, make sure to subtract sizeof(AIL_Alloc_Page_Header) from the size to allocate
// @Note: free_all is a no-op
//////////////
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_page_alloc;
AIL_ALLOC_DEF_INLINE void __ail_alloc_page_unused__(void);

//////////////
// Buffer Allocator
// Does not have a backing allocator and can thus not grow
// Especially useful for using stack-allocated memory
// @Note: free is a no-op, use free_all instead
//////////////
AIL_ALLOC_DEF AIL_Allocator ail_alloc_buffer_new(u64 n, u8 *mem);
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_buffer_alloc;

//////////////
// Ring-Buffer Allocator
// Does not have a backing allocator and can thus not grow
// Same as the Buffer Allocator, except it wraps around allocating from the start again
//////////////
AIL_ALLOC_DEF AIL_Allocator ail_alloc_ring_new(u64 n, u8 *mem);
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_ring_alloc;

//////////////
// Arena Allocator
// Last-In-First-Out kind of allocator (i.e. free only frees memory, when it was the memory that was allocated last)
// Recommended to call free_all to prevent leaking memory for too long
//////////////
AIL_ALLOC_DEF AIL_Allocator ail_alloc_arena_new(u64 cap, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_arena_alloc;

//////////////
// Pool Allocator
// Allocates only fixed-size chunks
// Allows freeing any chunk at any time
// @TODO: Allow allocating several chunks at once (make sure that all of those chunks get freed at once too)
//////////////
AIL_ALLOC_DEF AIL_Allocator ail_alloc_pool_new(u64 bucket_amount, u64 el_size, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_pool_alloc;

//////////////
// Free-List Allocator
// Allows allocating arbitrarily sized regions
// Stores a linked-list internally to keep track of allocations
//////////////
AIL_ALLOC_DEF AIL_Allocator ail_alloc_freelist_new(u64 cap, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_freelist_alloc;

#endif // AIL_ALLOC_H_


#if !defined(AIL_NO_ALLOC_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_ALLOC_IMPL_GUARD_
#define _AIL_ALLOC_IMPL_GUARD_

#include <stdlib.h> // For std_allocator and memset

// @TODO: Provide reserve/committ capacities for allocators instead of just one capacity
// @TODO: Implement Page Allocations for OSes other than WINDOWS and UNIX
#if AIL_OS_WIN32
#include <Windows.h> // For VirtualAlloc, VirtualFree
#else
#include <sys/mman.h> // For mmap, munmap
#endif

// For tracing memory
#ifndef AIL_ALLOC_PRINT_MEM
#define _AIL_ALLOC_LOG(...) do { if (0) printf(__VA_ARGS__); } while(0)
#else
#define _AIL_ALLOC_LOG(...) do { _AIL_DBG_PRINT_("Memory Trace: " __VA_ARGS__); _AIL_DBG_PRINT_("\n"); } while(0)
#endif // AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_LOG_ALLOC(allocator, ptr, size)          _AIL_ALLOC_LOG("Malloc   %4zu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CALLOC(allocator, ptr, size)         _AIL_ALLOC_LOG("Calloc   %4zu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_REALLOC(allocator, nptr, optr, size) _AIL_ALLOC_LOG("Relloc   %4zu bytes from %p to %p in '" allocator "' allocator", (size), (optr), (nptr));
#define AIL_ALLOC_LOG_SHRINK(allocator, ptr, osize, nsize) _AIL_ALLOC_LOG("Shrink   %4zu bytes at %p %4zu bytes '" allocator "' allocator", (osize), (ptr), (nsize));
#define AIL_ALLOC_LOG_FREE(allocator, ptr, size)           _AIL_ALLOC_LOG("Free     %4zu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CLEAR_ALL(allocator, size)           _AIL_ALLOC_LOG("ClearAll %4zu bytes in '" allocator "' allocator", (size));
#define AIL_ALLOC_LOG_FREE_ALL(allocator, size)            _AIL_ALLOC_LOG("FreeAll  %4zu bytes in '" allocator "' allocator", (size));
#define AIL_ALLOC_LOG(allocator, mode, ptr, osize, nsize, optr) do { \
        switch(mode) {                                               \
            case AIL_MEM_ALLOC:                                      \
                AIL_ALLOC_LOG_ALLOC(allocator, ptr, osize);          \
                break;                                               \
            case AIL_MEM_CALLOC:                                     \
                AIL_ALLOC_LOG_CALLOC(allocator, ptr, osize);         \
                break;                                               \
            case AIL_MEM_REALLOC:                                    \
                AIL_ALLOC_LOG_REALLOC(allocator, ptr, optr, osize);  \
                break;                                               \
            case AIL_MEM_SHRINK:                                     \
                AIL_ALLOC_LOG_SHRINK(allocator, ptr, osize, nsize);  \
                break;                                               \
            case AIL_MEM_FREE:                                       \
                AIL_ALLOC_LOG_FREE(allocator, optr, osize);          \
                break;                                               \
            case AIL_MEM_CLEAR_ALL:                                  \
                AIL_ALLOC_LOG_CLEAR_ALL(allocator, osize);           \
                break;                                               \
            case AIL_MEM_FREE_ALL:                                   \
                AIL_ALLOC_LOG_FREE_ALL(allocator, osize);            \
                break;                                               \
        }                                                            \
    } while(0)

#define AIL_ALLOC_GET_LAST_REGION(listPtr) _ail_alloc_get_last_region_( \
        (listPtr),                                                      \
        AIL_OFFSETOF(listPtr, region_head),                             \
        AIL_OFFSETOF(&(listPtr)->region_head, region_next)              \
    )
#define AIL_ALLOC_REGION_OF(listPtr, ptr) _ail_alloc_region_of_( \
        (u8 *)(listPtr),                                         \
        (u32)AIL_OFFSETOF(listPtr, region_head),                 \
        (u32)AIL_OFFSETOF(&(listPtr)->region_head, region_next), \
        (u32)AIL_OFFSETOF(&(listPtr)->region_head, mem),         \
        (u32)AIL_OFFSETOF(&(listPtr)->region_head, region_size), \
        ptr                                                      \
    )

#define _AIL_ALLOC_NEW_REGION_(allocatorName, allocator, new_region_var, min_region_size) do {                  \
        u64 __ail_alloc_new_region_size__  = AIL_MAX((allocator).region_block_size, (min_region_size));         \
        new_region_var = AIL_CALL_ALLOC(                                                                        \
            *(allocator).backing_allocator,                                                                     \
            __ail_alloc_new_region_size__ + ail_alloc_align_size(sizeof(AIL_CONCAT(AIL_Alloc_, allocatorName))) \
        );                                                                                                      \
        new_region_var->region_next = NULL;                                                                     \
        new_region_var->region_size = __ail_alloc_new_region_size__;                                            \
    } while(0)

#define AIL_ALLOC_FOR_EACH_REGION(allocatorName, it_name, first_region_ptr, body) \
    for (AIL_CONCAT(AIL_Alloc_, AIL_CONCAT(allocatorName, _Region)) *it_name = first_region_ptr, *_ail_alloc_for_each_next_region_; it_name != NULL; it_name = _ail_alloc_for_each_next_region_) { \
        _ail_alloc_for_each_next_region_ = it_name->region_next; \
        body; \
    }

#define AIL_ALLOC_FIND_REGION(region_ptr_var, last_region_ptr_var, cond) \
    while (region_ptr_var) {                                             \
        if ((cond)) break;                                               \
        last_region_ptr_var = region_ptr_var;                            \
        region_ptr_var = region_ptr_var->region_next;                    \
    }

#define AIL_ALLOC_CAN_MERGE_REGIONS(r1Ptr, r2Ptr) ((r1Ptr)->mem + (r1Ptr)->region_size == (u8 *)(r2Ptr))
#define AIL_ALLOC_MERGE_REGIONS(r1Ptr, r2Ptr) do {                       \
        AIL_ASSERT(AIL_ALLOC_CAN_MERGE_REGIONS(r1Ptr, r2Ptr));           \
        (r1Ptr)->region_size += (r2Ptr)->region_size + sizeof(*(r2Ptr)); \
    } while(0)

#define AIL_ALLOC_NEW_REGION(allocatorName, allocator, out_region_ptr_var, last_region_ptr_var, min_region_size, unmerged_body) do { \
        AIL_CONCAT(AIL_Alloc_, AIL_CONCAT(allocatorName, _Region)) *_ail_alloc_new_region_ptr_var_;                                  \
        _AIL_ALLOC_NEW_REGION_(allocatorName, allocator, _ail_alloc_new_region_ptr_var_, min_region_size);                           \
        if (AIL_LIKELY(_ail_alloc_new_region_ptr_var_)) {                                                                            \
            if (false && AIL_ALLOC_CAN_MERGE_REGIONS(last_region_ptr_var, _ail_alloc_new_region_ptr_var_)) {                         \
                AIL_ALLOC_MERGE_REGIONS(last_region_ptr_var, _ail_alloc_new_region_ptr_var_);                                        \
                out_region_ptr_var = last_region_ptr_var;                                                                            \
            } else {                                                                                                                 \
                last_region_ptr_var->region_next = _ail_alloc_new_region_ptr_var_;                                                   \
                out_region_ptr_var = _ail_alloc_new_region_ptr_var_;                                                                 \
                unmerged_body;                                                                                                       \
            }                                                                                                                        \
        } else {                                                                                                                     \
            out_region_ptr_var = NULL;                                                                                               \
        }                                                                                                                            \
    } while(0)

void* _ail_alloc_get_last_region_(u8 *list, u32 region_head_offset, u32 region_next_offset)
{
    u8 *cur = *((u8**)(list + region_head_offset));
    u8 *nxt = *((u8**)(cur  + region_next_offset));
    while (nxt) {
        cur = nxt;
        nxt = *((u8**)(cur + region_next_offset));
    }
    return cur;
}

void* _ail_alloc_region_of_(u8 *list, u32 region_head_offset, u32 region_next_offset, u32 mem_offset, u32 region_size_offset, u8 *ptr)
{
    u8 *region = list + region_head_offset;
    while (region) {
        u8 *mem  = region + mem_offset;
        u64 size = *(u64*)(region + region_size_offset);
        if (mem <= ptr && ptr <= mem + size) return region;
        region = *((u8**)(region + region_next_offset));
    }
    return NULL;
}

u64 ail_alloc_align_size(u64 size)
{
    // @Performance: ail_alloc_align_forward should be doing the same but faster, right?
    u64 mod;
#if AIL_IS_2POWER_POS(AIL_ALLOC_ALIGNMENT) == 1
    // size % alignment but faster, bc alignment is a power of two
    mod = size & (AIL_ALLOC_ALIGNMENT - 1);
#else
    mod = size % AIL_ALLOC_ALIGNMENT;
#endif
    return size + (mod > 0)*(AIL_ALLOC_ALIGNMENT - mod);
}

u64 ail_alloc_align_forward(u64 n, u64 alignment)
{
    AIL_ASSERT(alignment > 0 && AIL_IS_2POWER_POS(alignment));
    return ail_alloc_align_backward(n + (alignment - 1), alignment);
}

u64 ail_alloc_align_backward(u64 n, u64 alignment)
{
    AIL_ASSERT(alignment > 0 && AIL_IS_2POWER_POS(alignment));
    return n & ~(alignment - 1);
}

u64 ail_alloc_size_aligned_pad(u64 size)
{
    return ail_alloc_align_size(size) - size;
}

u64 ail_alloc_size_aligned_forward_pad(u64 size, u64 alignment)
{
    return ail_alloc_align_forward(size, alignment) - size;
}

u64 ail_alloc_size_aligned_backward_pad(u64 size, u64 alignment)
{
    return size - ail_alloc_align_backward(size, alignment);
}


/////////
// Std //
/////////

static AIL_Allocator ail_alloc_std = {
    .data  = NULL,
    .alloc = &ail_alloc_std_alloc,
};

// This function only exists to suppress the "unused ail_alloc_std" warning
void __ail_alloc_std_unused__(void)
{
    AIL_UNUSED(ail_alloc_std);
}

void* ail_alloc_std_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{
    AIL_UNUSED(data); AIL_UNUSED(size);
    void *res = NULL;
    switch (mode) {
        case AIL_MEM_ALLOC:     res = malloc(size); break;
        case AIL_MEM_CALLOC:    res = calloc(size, 1); break;
        case AIL_MEM_REALLOC:   res = realloc(old_ptr, size); break;
        case AIL_MEM_FREE:      free(old_ptr); break;
        case AIL_MEM_SHRINK:    break;
        case AIL_MEM_CLEAR_ALL: break;
        case AIL_MEM_FREE_ALL:  break;
    }
    AIL_ALLOC_LOG("std", mode, res, size, size, old_ptr);
    return res;
}


///////////
// Pager //
///////////

static AIL_Allocator ail_alloc_pager = {
    .data       = NULL,
    .alloc      = &ail_alloc_page_alloc,
};

// This function only exists to suppress the "unused ail_alloc_std" warning
static void __ail_alloc_page_unused__(void)
{
    AIL_UNUSED(ail_alloc_pager);
}

// @TODO: Add page size of 64 * 1024 for WASM
// Sizes taken from Zig's page_size implementation (https://ziglang.org/documentation/master/std/src/std/mem.zig.html)
#if defined(__APPLE__)
#define AIL_ALLOC_PAGE_SIZE (16*1024)
#elif defined(__sparc__) || defined(__sparc)
#define AIL_ALLOC_PAGE_SIZE (8*1024)
#else
#define AIL_ALLOC_PAGE_SIZE (4*1024)
#endif

// Assumes that size is page-size-aligned
static inline void _ail_alloc_internal_free_pages_(void *ptr, u64 size)
{
    ptr   = (u8 *)ptr - sizeof(AIL_Alloc_Page_Header);
    size += sizeof(AIL_Alloc_Page_Header);
#if defined(_WIN32)
    VirtualFree(ptr, size, MEM_DECOMMIT);
#else
    munmap(ptr, size);
#endif
}

static void* _ail_alloc_page_internal_alloc_(void *addr, u64 size)
{
    u64 header_size  = ail_alloc_align_size(sizeof(AIL_Alloc_Page_Header));
    u64 aligned_size = ail_alloc_align_forward(size + header_size, AIL_ALLOC_PAGE_SIZE);
    size = aligned_size - header_size;
#if defined(_WIN32)
    void *ptr = VirtualAlloc(addr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    void *ptr = mmap(addr, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
#endif
    ((AIL_Alloc_Page_Header *)ptr)->size = aligned_size - header_size;
    return (u8 *)ptr + header_size;
}

static void* _ail_alloc_page_internal_realloc_(void *old_ptr, u64 size)
{
    AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
    u64 old_size = header->size;
    u64 aligned_size = ail_alloc_align_forward(size + sizeof(AIL_Alloc_Page_Header), AIL_ALLOC_PAGE_SIZE);
    if (aligned_size <= old_size + sizeof(AIL_Alloc_Page_Header)) {
        u64 sz_diff = aligned_size - old_size;
        if (sz_diff >= AIL_ALLOC_PAGE_SIZE) {
            _ail_alloc_internal_free_pages_((void *)ail_alloc_align_forward((u64)((u8 *)old_ptr + size), AIL_ALLOC_PAGE_SIZE), sz_diff);
        }
        header->size = aligned_size - sizeof(AIL_Alloc_Page_Header);
        return old_ptr;
    } else {
        // @TODO: VirtualAlloc can take the previous pointer to potentially just increase the size
        // @TODO: mmap has some kind of hint system, that probably does more or less the same
        void *res = _ail_alloc_page_internal_alloc_(NULL, size);
        AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
        memcpy(res, old_ptr, header->size);
        _ail_alloc_internal_free_pages_(old_ptr, header->size);
        return res;
    }
}

void* ail_alloc_page_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{
    AIL_UNUSED(data);
    void *res = NULL;
    switch (mode) {
        case AIL_MEM_ALLOC: {
            res = _ail_alloc_page_internal_alloc_(old_ptr, size);
        } break;
        case AIL_MEM_CALLOC: {
            // @Note: Pages are already set to zero when returned by the OS, so memseting to 0 is unnecessary
            res = _ail_alloc_page_internal_alloc_(old_ptr, size);
        } break;
        case AIL_MEM_REALLOC: {
            res = _ail_alloc_page_internal_realloc_(old_ptr, size);
        } break;
        case AIL_MEM_SHRINK: break;
        case AIL_MEM_FREE: {
            AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
            size = header->size;
            _ail_alloc_internal_free_pages_(old_ptr, header->size);
        } break;
        case AIL_MEM_CLEAR_ALL: break;
        case AIL_MEM_FREE_ALL:  break;
    }
    AIL_ALLOC_LOG("page", mode, res, size, size, old_ptr);
    return res;
}


////////////
// Buffer //
////////////

AIL_Allocator ail_alloc_buffer_new(u64 n, u8 *buf)
{
    AIL_Alloc_Buffer *buffer = (AIL_Alloc_Buffer *)buf;
    buffer->idx  = 0;
    buffer->size = n - sizeof(AIL_Alloc_Buffer);
    return (AIL_Allocator) {
        .data       = buffer,
        .alloc      = &ail_alloc_buffer_alloc,
    };
}

static void* _ail_alloc_buffer_internal_alloc_(AIL_Alloc_Buffer *buffer, u8 *mem, u64 size)
{
    void *ptr = NULL;
    if (AIL_LIKELY(size + buffer->idx < buffer->size)) {
        ptr = &mem[buffer->idx];
        buffer->idx += size;
    }
    return ptr;
}

void *ail_alloc_buffer_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{
    void *ptr = NULL;
    AIL_Alloc_Buffer *buffer = (AIL_Alloc_Buffer *)data;
    u8 *mem = (u8 *)&buffer[1];
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_buffer_internal_alloc_(buffer, mem, size);
        } break;
        case AIL_MEM_CALLOC: {
            ptr = _ail_alloc_buffer_internal_alloc_(buffer, mem, size);
            if (ptr) memset(ptr, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            if (!old_ptr) {
                ptr = _ail_alloc_buffer_internal_alloc_(buffer, mem, size);
            } else {
                u64 max_old_size = AIL_MIN(size, (u64)mem + buffer->idx - (u64)ptr);
                ptr = _ail_alloc_buffer_internal_alloc_(buffer, mem, size);
                memcpy(ptr, old_ptr, max_old_size);
            }
        } break;
        case AIL_MEM_SHRINK: break;
        case AIL_MEM_FREE: break;
        case AIL_MEM_CLEAR_ALL:
        case AIL_MEM_FREE_ALL: buffer->idx = 0;
    }
    AIL_ALLOC_LOG("buffer", mode, ptr, size, size, old_ptr);
    return ptr;
}


/////////////////
// Ring-Buffer //
/////////////////

AIL_Allocator ail_alloc_ring_new(u64 n, u8 *buf)
{
    AIL_Alloc_Ring *ring = (AIL_Alloc_Ring *)buf;
    ring->idx  = 0;
    ring->size = n - sizeof(AIL_Alloc_Ring);
    return (AIL_Allocator) {
        .data  = ring,
        .alloc = &ail_alloc_ring_alloc,
    };
}

static void* _ail_alloc_ring_internal_alloc_(AIL_Alloc_Ring *ring, u8 *mem, u64 size)
{
    void *ptr = NULL;
    if (AIL_UNLIKELY(size + ring->idx >= ring->size)) ring->idx = 0;
    ptr = &mem[ring->idx];
    ring->idx += size;
    return ptr;
}

void *ail_alloc_ring_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{

    void *ptr = NULL;
    AIL_Alloc_Ring *ring = (AIL_Alloc_Ring *)data;
    u8 *mem = (u8 *)&ring[1];
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_ring_internal_alloc_(ring, mem, size);
        } break;
        case AIL_MEM_CALLOC: {
            ptr = _ail_alloc_ring_internal_alloc_(ring, mem, size);
            if (ptr) memset(ptr, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            if (!old_ptr) {
                ptr = _ail_alloc_ring_internal_alloc_(ring, mem, size);
            } else {
                u64 max_old_size = AIL_MIN(size, (u64)mem + ring->idx - (u64)old_ptr);
                ptr = _ail_alloc_ring_internal_alloc_(ring, mem, size);
                memcpy(ptr, old_ptr, max_old_size); // @Bug: memcpy might not work correctly, if the new poiner wrapped around and its region overlaps with the old region
            }
        } break;
        case AIL_MEM_SHRINK: break;
        case AIL_MEM_FREE: break;
        case AIL_MEM_CLEAR_ALL:
        case AIL_MEM_FREE_ALL: ring->idx = 0;
    }
    AIL_ALLOC_LOG("ring", mode, ptr, size, size, old_ptr);
    return ptr;
}


///////////
// Arena //
///////////

AIL_Allocator ail_alloc_arena_new(u64 cap, AIL_Allocator *backing_allocator)
{
    AIL_ASSERT(cap > sizeof(AIL_Alloc_Arena));
    u8 *ptr = (u8 *)AIL_CALL_ALLOC(*backing_allocator, cap);
    AIL_ASSERT(ptr != NULL);

    AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)ptr;
    arena->backing_allocator = backing_allocator;
    arena->region_block_size = cap - sizeof(AIL_Alloc_Arena);
    arena->region_head.used = 0;
    arena->region_head.region_next = NULL;
    arena->region_head.region_size = arena->region_block_size;
    return (AIL_Allocator) {
        .data  = arena,
        .alloc = &ail_alloc_arena_alloc,
    };
}

// @Note: Expects size to be aligned to AIL_ALLOC_ALIGNMENT
void* _ail_alloc_arena_internal_alloc_(AIL_Alloc_Arena *arena, u64 header_size, u64 size)
{
    AIL_Alloc_Arena_Region *last_region, *region = &arena->region_head;
    // AIL_BENCH_PROFILE_START(Arena_Alloc_Find_Region);
    AIL_ALLOC_FIND_REGION(region, last_region, (region->used + size + header_size <= region->region_size));
    if (AIL_UNLIKELY(!region)) {
        AIL_ALLOC_NEW_REGION(Arena, *arena, region, last_region, size,
            region->used = 0;
        );
    }
    // AIL_BENCH_PROFILE_END(Arena_Alloc_Find_Region);
    AIL_Alloc_Arena_Header *header = (AIL_Alloc_Arena_Header *)&region->mem[region->used];
    header->size  = size;
    void *ptr     = (u8 *)header + header_size;
    region->used += size + header_size;
    return ptr;
}

// @Note: Expects size to be aligned to AIL_ALLOC_ALIGNMENT
void *_ail_alloc_arena_internal_realloc_(AIL_Alloc_Arena *arena, u64 header_size, void *old_ptr, u64 size)
{
    u8   *optr = (u8 *)old_ptr;
    void *nptr = optr;
    if (!optr) return _ail_alloc_arena_internal_alloc_(arena, header_size, size);

    AIL_Alloc_Arena_Region *region = AIL_ALLOC_REGION_OF(arena, optr);
    if (!region) { // @Note: Bounds check failure -> crash in debug mode and return null otherwise
        AIL_UNREACHABLE();
        return NULL;
    }

    u64 old_size = AIL_ALLOC_GET_HEADER(optr, AIL_Alloc_Arena_Header)->size;
    if (size <= old_size) {
        AIL_ALLOC_GET_HEADER(optr, AIL_Alloc_Arena_Header)->size = size;
    } else if (optr + old_size == region->mem + region->used) { // Was the last allocation
        if (region->mem + region->region_size < optr + size) { // New size doesn't fit into this region
            region->used -= old_size + sizeof(AIL_Alloc_Arena_Header); // Free memory from this region
            nptr = _ail_alloc_arena_internal_alloc_(arena, header_size, size);
            if (nptr != optr) memcpy(nptr, optr, old_size);
        } else {
            AIL_ALLOC_GET_HEADER(optr, AIL_Alloc_Arena_Header)->size = size;
        }
    } else {
        // @Note: This leaks memory, since the old allocation cannot be freed in such a simple arena
        nptr = _ail_alloc_arena_internal_alloc_(arena, header_size, size);
        if (nptr) memcpy(nptr, optr, old_size);
    }
    return nptr;
}

void* ail_alloc_arena_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{
    u64 old_size = size;
    AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
    u64 header_size = ail_alloc_align_size(sizeof(AIL_Alloc_Arena_Header));
    size = ail_alloc_align_size(size);
    void *ptr = NULL;
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_arena_internal_alloc_(arena, header_size, size);
        } break;
        case AIL_MEM_CALLOC: {
            ptr = _ail_alloc_arena_internal_alloc_(arena, header_size, size);
            if (ptr) memset(ptr, 0, AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size);
        } break;
        case AIL_MEM_REALLOC: {
            ptr = _ail_alloc_arena_internal_realloc_(arena, header_size, old_ptr, size);
        } break;
        case AIL_MEM_SHRINK: {
            if (old_ptr) {
                old_size = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Arena_Header)->size;
                AIL_Alloc_Arena_Region *region = AIL_ALLOC_REGION_OF(arena, old_ptr);
                if (size < old_size && (u8*)old_ptr + old_size == region->mem + region->used) {
                    AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Arena_Header)->size = size;
                }
            }
        } break;
        case AIL_MEM_FREE: {
            // AIL_BENCH_PROFILE_START(Arena_Free_Find_Region);
            AIL_Alloc_Arena_Region *region = AIL_ALLOC_REGION_OF(arena, old_ptr);
            if (!region) { // @Note Bounds checking failed -> crash in debug mode and just ignore it otherwise
                AIL_UNREACHABLE();
                goto done;
            }
            // AIL_BENCH_PROFILE_END(Arena_Free_Find_Region);
            // Free element, if it was the last one allocated
            u64 old_size = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Arena_Header)->size;
            if ((u8 *)old_ptr + old_size == region->mem + region->used) region->used -= old_size + sizeof(AIL_Alloc_Arena_Header);
        } break;
        case AIL_MEM_CLEAR_ALL: {
            size = 0;
            AIL_ALLOC_FOR_EACH_REGION(Arena, region, &arena->region_head,
                size += region->used;
                region->used = 0;
            );
        } break;
        case AIL_MEM_FREE_ALL: {
            size = 0;
            AIL_ALLOC_FOR_EACH_REGION(Arena, region, arena->region_head.region_next,
                size += region->used;
                region->region_next = NULL;
                AIL_CALL_FREE(*arena->backing_allocator, region);
            );
            arena->region_head.region_next = NULL;
            arena->region_head.used = 0;
        } break;
    }
done:
    AIL_ALLOC_LOG("arena", mode, ptr, old_size, size, old_ptr);
    return ptr;
}


//////////
// Pool //
//////////

static void _ail_alloc_pool_internal_clear_region_(AIL_Alloc_Pool_Region *region, u64 bucket_amount, u64 bucket_size)
{
    region->head = NULL;
    for (u64 i = 0; i < bucket_amount; i++) {
        AIL_Alloc_Pool_Free_Node *node = (AIL_Alloc_Pool_Free_Node *)&region->mem[i*bucket_size];
        node->next   = region->head;
        region->head = node;
    }
}

static void* _ail_alloc_pool_internal_alloc_(AIL_Alloc_Pool *pool)
{
    void *ptr;
    AIL_Alloc_Pool_Region *last_region, *region = &pool->region_head;
    AIL_ALLOC_FIND_REGION(region, last_region, (region->head));
    if (region) {
        ptr = region->head;
        region->head = region->head->next;
    } else {
        // @TODO: Merge with other regions if possible - more difficult because region-header & new region have to become part of the freelist
        AIL_Alloc_Pool_Region *new_region;
        _AIL_ALLOC_NEW_REGION_(Pool, *pool, new_region, pool->bucket_size*pool->bucket_amount);
        if (!new_region) return NULL;
        last_region->region_next = new_region;
        _ail_alloc_pool_internal_clear_region_(new_region, pool->bucket_amount, pool->bucket_size);
        ptr = new_region->head;
        AIL_ASSERT(new_region->region_size == pool->region_block_size);
        new_region->head = new_region->head->next;
    }
    return ptr;
}

static u64 _ail_alloc_pool_internal_count_used_(AIL_Alloc_Pool_Region *region, u64 bucket_size)
{
    u64 n = 0;
    AIL_Alloc_Pool_Free_Node *node = region->head;
    while (node) {
        n++;
        node = node->next;
    }
    return region->region_size - (n*bucket_size);
}

AIL_Allocator ail_alloc_pool_new(u64 bucket_amount, u64 el_size, AIL_Allocator *backing_allocator)
{
    u64 bucket_size         = ail_alloc_align_size(AIL_MAX(el_size, sizeof(AIL_Alloc_Pool_Free_Node)));
    u64 region_size         = bucket_amount*bucket_size;
    AIL_Alloc_Pool *pool    = (AIL_Alloc_Pool *)AIL_CALL_ALLOC(*backing_allocator, region_size + sizeof(AIL_Alloc_Pool));
    pool->backing_allocator = backing_allocator;
    pool->bucket_size       = bucket_size;
    pool->bucket_amount     = bucket_amount;
    pool->region_block_size = region_size;
    pool->region_head.region_next = NULL;
    pool->region_head.region_size = region_size;
    _ail_alloc_pool_internal_clear_region_(&pool->region_head, bucket_amount, bucket_size); // Set up free-list
    return (AIL_Allocator) {
        .data  = pool,
        .alloc = &ail_alloc_pool_alloc,
    };
}

void* ail_alloc_pool_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{
    // @TODO: Allow allocation of several buckets at once
    AIL_UNUSED(size);
    u64 old_size = size;
    AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
    AIL_ASSERT(size <= pool->bucket_size);
    void *ptr = NULL;
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_pool_internal_alloc_(pool);
        } break;
        case AIL_MEM_CALLOC: {
            ptr = _ail_alloc_pool_internal_alloc_(pool);
            if (ptr) memset(ptr, 0, pool->bucket_size);
        } break;
        case AIL_MEM_REALLOC: {
            // Since all buckets are the same size, reallocating for more space doesn't make sense and becomes a no-op
            ptr = old_ptr;
        } break;
        case AIL_MEM_SHRINK: break;
        case AIL_MEM_FREE: {
            AIL_Alloc_Pool_Region *region = AIL_ALLOC_REGION_OF(pool, old_ptr);
            if (!region) { // @Note Bounds checking failed -> crash in debug mode and just ignore it otherwise
                AIL_UNREACHABLE();
                goto done;
            }
            size = pool->bucket_size;
            AIL_Alloc_Pool_Free_Node *node = old_ptr;
            node->next   = region->head;
            region->head = node;
        } break;
        case AIL_MEM_CLEAR_ALL: {
            size = 0;
            AIL_ALLOC_FOR_EACH_REGION(Pool, region, &pool->region_head,
                size = _ail_alloc_pool_internal_count_used_(region, pool->bucket_size);
                _ail_alloc_pool_internal_clear_region_(region, pool->bucket_amount, pool->bucket_size);
            );
        } break;
        case AIL_MEM_FREE_ALL: {
            size = 0;
            AIL_ALLOC_FOR_EACH_REGION(Pool, region, pool->region_head.region_next,
                size += _ail_alloc_pool_internal_count_used_(region, pool->bucket_size);
                region->region_next = NULL;
                AIL_CALL_FREE(*pool->backing_allocator, region);
            );
            pool->region_head.region_next = NULL;
            _ail_alloc_pool_internal_clear_region_(&pool->region_head, pool->bucket_amount, pool->bucket_size);
        } break;
    }
done:
    AIL_ALLOC_LOG("pool", mode, ptr, old_size, size, old_ptr);
    return ptr;
}


///////////////
// Free-List //
///////////////

void _ail_alloc_freelist_node_print_(u64 id, u64 size)
{
    printf("\033[42m| %zd, %zd \033[0m", id, size);
}

void _ail_alloc_freelist_block_print_(AIL_Alloc_Freelist_Header header)
{
    printf("\033[41m| %zd ", header.size);
    AIL_ASSERT(header.pad < AIL_ALLOC_ALIGNMENT);
    if (header.pad) printf("+ %zd ", header.pad);
    printf("\033[0m");
}

typedef struct _AIL_Alloc_FL_Node_Print_Data_ {
    u64 id;
    u64 size;
    u8 *ptr;
} _AIL_Alloc_FL_Node_Print_Data_;
AIL_DA_INIT(_AIL_Alloc_FL_Node_Print_Data_);

void _ail_alloc_freelist_region_print_(AIL_Alloc_Freelist_Region *region)
{
    AIL_DA(_AIL_Alloc_FL_Node_Print_Data_) nodes = ail_da_new(_AIL_Alloc_FL_Node_Print_Data_);
    AIL_Alloc_Freelist_Free_Node *node = region->head;
    u64 id = 0;
    while (node) {
        _AIL_Alloc_FL_Node_Print_Data_ node_data = (_AIL_Alloc_FL_Node_Print_Data_) {
            .id   = id++,
            .size = node->size,
            .ptr  = (u8 *)node,
        };
        ail_da_push(&nodes, node_data);
        node = node->next;
    }
    for (u32 i = 0; i < nodes.len - 1; i++) {
        u32 m = i;
        for (u32 j = i + 1; j < nodes.len; j++) {
            if (nodes.data[j].ptr < nodes.data[m].ptr) m = j;
        }
        _AIL_Alloc_FL_Node_Print_Data_ tmp = nodes.data[m];
        nodes.data[m] = nodes.data[i];
        nodes.data[i] = tmp;
    }

    printf("\033[48;5;0m%zd/%zd used: ", region->region_used, region->region_size);
    u32 idx = 0;
    u8 *ptr = region->mem;
    while (ptr < region->mem + region->region_size) {
        AIL_ASSERT(nodes.data[idx].ptr >= ptr);
        if (nodes.data[idx].ptr == ptr) {
            _ail_alloc_freelist_node_print_(nodes.data[idx].id, nodes.data[idx].size);
            ptr += nodes.data[idx].size;
            idx++;
        } else {
            AIL_Alloc_Freelist_Header *header = (AIL_Alloc_Freelist_Header *)ptr;
            _ail_alloc_freelist_block_print_(*header);
            ptr += header->size + header->pad + sizeof(AIL_Alloc_Freelist_Header);
        }
    }
    printf("|\033[48;5;0m\n");
}

void _ail_alloc_freelist_print_(AIL_Alloc_Freelist *fl)
{
    printf("----- Freelist -----\n");
    AIL_ALLOC_FOR_EACH_REGION(Freelist, region, &fl->region_head, _ail_alloc_freelist_region_print_(region));
}

void _ail_alloc_freelist_internal_clear_region_(AIL_Alloc_Freelist_Region *region)
{
    AIL_Alloc_Freelist_Free_Node *head = (AIL_Alloc_Freelist_Free_Node *)region->mem;
    head->size          = region->region_size;
    head->next          = NULL;
    region->head        = head;
    region->region_used = 0;
}

AIL_Alloc_Freelist_Node_Couple _ail_alloc_freelist_internal_find_node_(AIL_Alloc_Freelist_Region *region, u64 required_size)
{
    AIL_Alloc_Freelist_Node_Couple couple = {
        .node = region->head,
        .prev = NULL,
    };
    while (couple.node && couple.node->size < required_size) {
        couple.prev = couple.node;
        couple.node = couple.node->next;
    }
    return couple;
}

void* _ail_alloc_freelist_internal_alloc_(AIL_Alloc_Freelist *fl, u64 size)
{
    // AIL_BENCH_PROFILE_START(Freelist_Alloc);
    AIL_Alloc_Freelist_Header header = {
        .pad  = ail_alloc_size_aligned_pad(sizeof(AIL_Alloc_Freelist_Header) + size),
        .size = size,
    };
    u64 req_size = sizeof(AIL_Alloc_Freelist_Header) + size + header.pad;

    // AIL_BENCH_PROFILE_START(Freelist_Alloc_Find_Region_Node);
    AIL_Alloc_Freelist_Region *prev_region, *region = &fl->region_head;
    AIL_Alloc_Freelist_Node_Couple node_couple = {0};
    while (region) {
        prev_region = region;
        if (region->region_size - region->region_used >= req_size) {
            // AIL_BENCH_PROFILE_START(Freelist_Alloc_Find_Node);
            node_couple = _ail_alloc_freelist_internal_find_node_(region, req_size);
            // AIL_BENCH_PROFILE_END(Freelist_Alloc_Find_Node);
            if (node_couple.node) break;
        }
        region = region->region_next;
    }
    // AIL_BENCH_PROFILE_END(Freelist_Alloc_Find_Region_Node);
    if (!node_couple.node) {
        AIL_ASSERT(prev_region != NULL);
        _AIL_ALLOC_NEW_REGION_(Freelist, *fl, region, req_size);
        if (!region) return NULL;
        _ail_alloc_freelist_internal_clear_region_(region);
        prev_region->region_next = region;
        node_couple = (AIL_Alloc_Freelist_Node_Couple) {
            .node = region->head,
            .prev = NULL,
        };
    }

    AIL_Alloc_Freelist_Free_Node *next;
    if (node_couple.node->size - req_size > AIL_MAX(sizeof(AIL_Alloc_Freelist_Free_Node), sizeof(AIL_Alloc_Freelist_Header) + AIL_ALLOC_ALIGNMENT)) {
        next = (AIL_Alloc_Freelist_Free_Node *)((u8 *)node_couple.node + req_size);
        AIL_ASSERT(next != node_couple.prev);
        next->next = node_couple.node->next;
        next->size = node_couple.node->size - req_size;
    } else {
        next = node_couple.node->next;
    }
    if (node_couple.prev) node_couple.prev->next = next;
    else                  region->head           = next;

    AIL_ASSERT(node_couple.prev == NULL || node_couple.prev->next != node_couple.prev);
    AIL_ASSERT(node_couple.node->next != node_couple.node);
    region->region_used += req_size;
    *(AIL_Alloc_Freelist_Header *)node_couple.node = header;
    // AIL_BENCH_PROFILE_END(Freelist_Alloc);
    return (u8 *)node_couple.node + sizeof(AIL_Alloc_Freelist_Header);
}

// @Note: Returns the amount of freed bytes
u64 _ail_alloc_freelist_internal_free_(AIL_Alloc_Freelist *fl, u8 *old_ptr)
{
    // AIL_BENCH_PROFILE_START(Freelist_Free);
    AIL_ASSERT(old_ptr != NULL);
    AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Freelist_Header);
    u64 freed_size  = header->size;
    u64 block_size  = header->size + header->pad + sizeof(AIL_Alloc_Freelist_Header);
    u8 *block_start = (u8 *)header;
    // Find region containing the old allocation
    // AIL_BENCH_PROFILE_START(Freelist_Free_Find_Region);
    AIL_Alloc_Freelist_Region *region = AIL_ALLOC_REGION_OF(fl, old_ptr);
    if (!region) { // @Note Bounds checking failed -> crash in debug mode and just ignore it otherwise
        AIL_UNREACHABLE();
        return 0;
    }
    region->region_used -= block_size;
    // AIL_BENCH_PROFILE_END(Freelist_Free_Find_Region);

    // Try to merge the old allocation back into the freelist
    b32 merged = false;
    AIL_Alloc_Freelist_Free_Node *merged_prev = NULL;
    AIL_Alloc_Freelist_Free_Node *node = region->head;
    AIL_Alloc_Freelist_Free_Node *prev = NULL;
    while (node) {
        if ((u8 *)node + node->size == block_start) {
            node->size += block_size;
            if (merged) {
                if (!merged_prev) region->head = node;
                else merged_prev->next = ((AIL_Alloc_Freelist_Free_Node *)block_start)->next;
                break;
            }
            merged_prev = prev;
            block_start = (u8 *)node;
            block_size  = node->size;
            merged      = true;
            AIL_ASSERT(node->next != node);
        } else if (block_start + block_size == (u8 *)node) {
            AIL_Alloc_Freelist_Free_Node *new_node = (AIL_Alloc_Freelist_Free_Node *)block_start;
            if (merged) {
                if (!merged_prev) region->head = new_node;
                else merged_prev->next = node->next;
            }
            new_node->next = node->next;
            new_node->size = block_size + node->size;
            if (prev && prev != new_node) prev->next = new_node;
            else region->head = new_node;
            if (merged) break;
            block_size  = new_node->size;
            merged_prev = prev;
            merged      = true;
            AIL_ASSERT(node->next != node);
        }
        prev = node;
        node = node->next;
    }
    if (!merged) {
        node = (AIL_Alloc_Freelist_Free_Node *)block_start;
        node->size   = block_size;
        node->next   = region->head;
        region->head = node;
    }
    AIL_ASSERT(node == NULL || node->next != node);
    AIL_ASSERT(prev == NULL || prev->next != prev);
    // AIL_BENCH_PROFILE_END(Freelist_Free);
    return freed_size;
}

AIL_Allocator ail_alloc_freelist_new(u64 cap, AIL_Allocator *backing_allocator)
{
    u8 *mem = (u8 *)AIL_CALL_ALLOC(*backing_allocator, cap);
    AIL_Alloc_Freelist *fl      = (AIL_Alloc_Freelist *)mem;
    fl->region_block_size       = cap - sizeof(AIL_Alloc_Freelist);
    fl->region_head.region_size = cap - sizeof(AIL_Alloc_Freelist);
    fl->backing_allocator       = backing_allocator;
    _ail_alloc_freelist_internal_clear_region_(&fl->region_head); // Sets all other parameters of fl
    return (AIL_Allocator) {
        .data       = fl,
        .alloc      = &ail_alloc_freelist_alloc,
    };
}

void *ail_alloc_freelist_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{
    u64 old_size = size;
    void *ptr = NULL;
    AIL_Alloc_Freelist *fl = (AIL_Alloc_Freelist *)data;
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_freelist_internal_alloc_(fl, size);
        } break;
        case AIL_MEM_CALLOC: {
            ptr = _ail_alloc_freelist_internal_alloc_(fl, size);
            if (ptr) memset(ptr, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            if (!old_ptr) {
                ptr = _ail_alloc_freelist_internal_alloc_(fl, size);
            } else {
                // @TODO: This can definitely be optimized
                AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Freelist_Header);
                if (size <= header->size + header->pad) {
                    header->size = size;
                    header->pad  = header->size + header->pad - size;
                    ptr = old_ptr;
                } else {
                    ptr = _ail_alloc_freelist_internal_alloc_(fl, size);
                    if (ptr) {
                        u64 sz = AIL_MIN(header->size, size);
                        AIL_ASSERT(sz > 0);
                        AIL_ASSERT(((u8*)ptr < (u8*)old_ptr && (u8*)ptr + sz < (u8*)old_ptr) || ((u8*)old_ptr < (u8*)ptr && (u8*)old_ptr + sz < (u8*)ptr));
                        memcpy(ptr, old_ptr, sz);
                    }
                    _ail_alloc_freelist_internal_free_(fl, (u8 *)old_ptr);
                }
            }
        } break;
        case AIL_MEM_SHRINK: {
            if (old_ptr) {
                AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Freelist_Header);
                if (size < header->size) {
                    header->size = size;
                    header->pad  = header->size + header->pad - size;
                }
            }
        } break;
        case AIL_MEM_FREE: {
            size = _ail_alloc_freelist_internal_free_(fl, (u8 *)old_ptr);
        } break;
        case AIL_MEM_CLEAR_ALL: {
            AIL_Alloc_Freelist_Region *region = &fl->region_head;
            size = 0;
            while (region) {
                size += region->region_used;
                _ail_alloc_freelist_internal_clear_region_(region);
                region = region->region_next;
            }
        } break;
        case AIL_MEM_FREE_ALL: {
            size = 0;
            AIL_ALLOC_FOR_EACH_REGION(Freelist, region, fl->region_head.region_next,
                size += region->region_used;
                region->region_next = NULL;
                AIL_CALL_FREE(*fl->backing_allocator, region);
            );
            fl->region_head.region_next = NULL;
            _ail_alloc_freelist_internal_clear_region_(&fl->region_head);
        } break;
    }
#if 0
    if (mode == AIL_MEM_FREE) {
        printf("Mode: ");
        switch (mode) {
            case AIL_MEM_ALLOC:     printf("Alloc");    break;
            case AIL_MEM_CALLOC:    printf("Calloc");   break;
            case AIL_MEM_REALLOC:   printf("Realloc");  break;
            case AIL_MEM_FREE:      printf("Free");     break;
            case AIL_MEM_CLEAR_ALL: printf("ClearAll"); break;
            case AIL_MEM_FREE_ALL:  printf("FreeAll");  break;
        }
        printf("  ");
        _ail_alloc_freelist_print_(fl);
    }
#endif
    AIL_ALLOC_LOG("freelist", mode, ptr, old_size, size, old_ptr);
    return ptr;
}


#endif // _AIL_ALLOC_IMPL_GUARD_
#endif // AIL_NO_ALLOC_IMPL





////////
// From ail_bench.h
////////


// A simple instrumenting Profiler
//
// Define `AIL_NO_BENCH_IMPL` to not include any implementation
// Define `AIL_BENCH_PROFILE` to profile code blocks
// Define `AIL_BENCH_PROFILE_ANCHOR_COUNT` to set the maximum amount of Profile Anchors
//
// The profiler was heavily inspired by Casey Muratori's Computer Enhance series
// See https://github.com/cmuratori/computer_enhance and https://www.computerenhance.com
// for more information on Computer Enhance
//
// LICENSE
/*
Copyright (c) 2024 Lily Val Richter

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

#ifndef AIL_BENCH_H_
#define AIL_BENCH_H_


#ifndef AIL_BENCH_DEF
#define AIL_BENCH_DEF AIL_DEF
#endif // AIL_BENCH_DEF
#ifndef AIL_BENCH_DEF_INLINE
#define AIL_BENCH_DEF_INLINE AIL_DEF_INLINE
#endif // AIL_BENCH_DEF_INLINE

#if AIL_OS_WIN32
#   include <windows.h>
    typedef struct AIL_Bench_Global_Info {
        u64 cpu_timer_freq;
        HANDLE proc;
    } AIL_Bench_Global_Info;
#else
    typedef struct AIL_Bench_Global_Info {
        u64 cpu_timer_freq;
        HANDLE proc;
    } AIL_Bench_Global_Info;
#endif

AIL_BENCH_DEF_INLINE void ail_bench_init(void);
AIL_BENCH_DEF_INLINE u64 ail_bench_count_page_faults(void);
AIL_BENCH_DEF_INLINE u64 ail_bench_os_timer_freq(void);
AIL_BENCH_DEF_INLINE u64 ail_bench_os_timer(void);
AIL_BENCH_DEF_INLINE u64 ail_bench_calc_cpu_timer_freq(void);
AIL_BENCH_DEF_INLINE u64 ail_bench_cpu_timer(void);
AIL_BENCH_DEF_INLINE f64 ail_bench_cpu_elapsed_to_ms(u64 cpu_elapsed);


#ifdef AIL_BENCH_PROFILE
#ifndef AIL_BENCH_PROFILE_ANCHOR_COUNT
#define AIL_BENCH_PROFILE_ANCHOR_COUNT 4096
#endif // AIL_BENCH_PROFILE_ANCHOR_COUNT
#include <stdio.h> // For printf

typedef struct AIL_Bench_Profiler {
    u64 start_tsc;
    u64 end_tsc;
} AIL_Bench_Profiler;
static AIL_Bench_Profiler ail_bench_global_profiler;

typedef struct AIL_Bench_Profile_Anchor {
    const char *label;
    u64 elapsed_with_children;
    u64 elapsed_wo_children;
    u64 old_elapsed_wo_children;
    u64 min_with_children;
    u64 min_wo_children;
    u64 mem_size;
    u32 hit_count;
    u32 child_depth;
} AIL_Bench_Profile_Anchor;

typedef struct AIL_Bench_Profile_Block {
    const char *label;
    u64 old_elapsed; // old elapsed_with_children value
    u64 start_time;
    u32 parent_idx;
    u32 anchor_idx;
} AIL_Bench_Profile_Block;

AIL_BENCH_DEF void ail_bench_begin_profile(void);
AIL_BENCH_DEF void ail_bench_end_profile(void);
AIL_BENCH_DEF void ail_bench_print_profile(u32 depth, b32 clear_anchors);
AIL_BENCH_DEF void ail_bench_end_and_print_profile(u32 depth, b32 clear_anchors);
AIL_BENCH_DEF void ail_bench_clear_anchors(void);
AIL_BENCH_DEF AIL_Bench_Profile_Block ail_bench_profile_block_start(const char *label, u32 anchor_idx, u64 mem_size);
AIL_BENCH_DEF void ail_bench_profile_block_end(AIL_Bench_Profile_Block *block);
AIL_BENCH_DEF void ail_bench_profile_print_anchors(u64 total_tsc_elapsed, u32 depth, b32 clear_anchors);

#define AIL_BENCH_PROFILE_BLOCK_NAME(name) AIL_CONCAT(ail_bench_profile_block_, name)
#define AIL_BENCH_PROFILE_MEM_START(name, mem_size) AIL_Bench_Profile_Block AIL_BENCH_PROFILE_BLOCK_NAME(name) = ail_bench_profile_block_start(AIL_STRINGIFY(name), __COUNTER__ + 1, (mem_size))
#define AIL_BENCH_PROFILE_START(name) AIL_BENCH_PROFILE_MEM_START(name, 0)
#define AIL_BENCH_PROFILE_END(name) ail_bench_profile_block_end(&AIL_BENCH_PROFILE_BLOCK_NAME(name))
#define AIL_BENCH_END_OF_COMPILATION_UNIT() AIL_STATIC_ASSERT(__COUNTER__ < AIL_BENCH_PROFILE_ANCHOR_COUNT, "Number of profile points exceeds size of profiler::Anchors array")
#endif // AIL_BENCH_PROFILE


#endif // AIL_BENCH_H_



#if !defined(AIL_NO_BENCH_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_BENCH_IMPL_GUARD_
#define _AIL_BENCH_IMPL_GUARD_
global AIL_Bench_Global_Info ail_bench_global_info = {0};

#ifdef AIL_BENCH_PROFILE
global AIL_Bench_Profile_Anchor ail_bench_global_anchors[AIL_BENCH_PROFILE_ANCHOR_COUNT] = {0};
global u32 ail_bench_gloabl_parent_idx = 0;

void ail_bench_begin_profile(void)
{
    ail_bench_global_profiler.start_tsc = ail_bench_cpu_timer();
}

void ail_bench_end_profile(void)
{
    ail_bench_global_profiler.end_tsc = ail_bench_cpu_timer();
}

void ail_bench_print_profile(u32 depth, b32 clear_anchors)
{
    u64 total_tsc_elapsed = ail_bench_global_profiler.end_tsc - ail_bench_global_profiler.start_tsc;
    if (ail_bench_global_info.cpu_timer_freq) {
        printf("Total time: %0.4fms (CPU frequency %0.3fMHz)\n", ail_bench_cpu_elapsed_to_ms(total_tsc_elapsed), (f64)ail_bench_global_info.cpu_timer_freq / 1e6);
    }
    ail_bench_profile_print_anchors(total_tsc_elapsed, depth, clear_anchors);
}

void ail_bench_end_and_print_profile(u32 depth, b32 clear_anchors)
{
    ail_bench_end_profile();
    ail_bench_print_profile(depth, clear_anchors);
}

AIL_Bench_Profile_Block ail_bench_profile_block_start(const char *label, u32 anchor_idx, u64 mem_size)
{
    AIL_Bench_Profile_Block block;
    AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[anchor_idx];
    anchor->mem_size += mem_size;
    block.label       = label,
    block.anchor_idx  = anchor_idx;
    block.old_elapsed = anchor->elapsed_with_children,
    block.parent_idx  = ail_bench_gloabl_parent_idx,
    ail_bench_gloabl_parent_idx = anchor_idx;
    block.start_time  = ail_bench_cpu_timer();
    return block;
}

void ail_bench_profile_block_end(AIL_Bench_Profile_Block *block)
{
    u64 elapsed = ail_bench_cpu_timer() - block->start_time;
    ail_bench_gloabl_parent_idx = block->parent_idx;

    AIL_Bench_Profile_Anchor *parent = &ail_bench_global_anchors[block->parent_idx];
    AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[block->anchor_idx];
    anchor->child_depth              = parent->child_depth + 1;
    parent->elapsed_wo_children     -= elapsed;
    anchor->elapsed_wo_children     += elapsed;
    anchor->elapsed_with_children    = block->old_elapsed + elapsed;

    u64 cur_elapsed_wo_children      = anchor->elapsed_wo_children - anchor->old_elapsed_wo_children;
    anchor->old_elapsed_wo_children  = anchor->elapsed_wo_children;
    if (!anchor->min_wo_children   || anchor->min_wo_children   < cur_elapsed_wo_children) anchor->min_wo_children   = cur_elapsed_wo_children;
    if (!anchor->min_with_children || anchor->min_with_children < elapsed)                 anchor->min_with_children = elapsed;

    ++anchor->hit_count;
    anchor->label = block->label;
}

void _ail_bench_clear_anchor(AIL_Bench_Profile_Anchor *anchor)
{
    anchor->hit_count               = 0;
    anchor->elapsed_wo_children     = 0;
    anchor->elapsed_with_children   = 0;
    anchor->min_wo_children         = 0;
    anchor->min_with_children       = 0;
    anchor->old_elapsed_wo_children = 0;
    anchor->mem_size                = 0;
}

void ail_bench_clear_anchors(void)
{
    for (u64 i = 0; i < AIL_BENCH_PROFILE_ANCHOR_COUNT; i++) {
        _ail_bench_clear_anchor(ail_bench_global_anchors + i);
    }
}

// Calculates amount of characters required to write x as a decimal number
static inline u32 ail_bench_u64_len(u64 x) {
    u32 len = 1;
    while (x > 9) {
        x /= 10;
        len++;
    }
    return len;
}

// Calculates amount of characters required to write x as a decimal number
static inline u32 ail_bench_f64_len(f64 x, u32 precision) {
    return ail_bench_u64_len((u64)x) + 1 + precision;
}

static inline u32 ail_bench_str_len(const char *str) {
    u32 len = 0;
    while (str[len]) len++;
    return len;
}

// @TODO: Support colored output
// This could maybe be done by adding an enum to anchors which indicates whether the anchor should
// a) always be highlighted
// b) compared with other "compare" anchors (i.e. green for fastest, red for slowest)
// Alternative approach would be to let the user provide this information when calling print()
// Mapping coloring information to a specific anchor could be done via the anchor's label then
void ail_bench_profile_print_anchors(u64 total_tsc_elapsed, u32 depth, b32 clear_anchors)
{
    // Calculate max lengths for proper alignment
    static const u32 float_print_precision = 4;
    u32 max_name_hit_count_len = 0;
    u32 max_elapsed_cycles_len = 0;
    u32 max_elapsed_ms_len     = 0;
    u32 max_min_elapsed_ms_len = 0;
    u32 max_bandwidth_len      = 0;
    b32 any_with_children      = 0;
    for (u32 anchor_idx = 0; anchor_idx < AIL_BENCH_PROFILE_ANCHOR_COUNT; anchor_idx++) {
        AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[anchor_idx];
        if (anchor->elapsed_with_children && anchor->child_depth <= depth) {
            u32 name_len           = ail_bench_str_len(anchor->label);
            u32 hit_count_len      = ail_bench_u64_len(anchor->hit_count);
            u64 elapsed            = (anchor->child_depth == depth) ? anchor->elapsed_with_children : anchor->elapsed_wo_children;
            u32 elapsed_cycles_len = ail_bench_u64_len(elapsed);
            u32 elapsed_ms_len     = ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(anchor->elapsed_wo_children), float_print_precision);
            u32 min_elapsed_ms_len = ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(anchor->min_wo_children), float_print_precision);
            u32 bandwidth_len      = ail_bench_f64_len((f64)anchor->mem_size / (f64)(AIL_GB(1) * (ail_bench_cpu_elapsed_to_ms(elapsed)/1000)), float_print_precision);
            max_name_hit_count_len = AIL_MAX(max_name_hit_count_len, name_len + hit_count_len);
            max_elapsed_cycles_len = AIL_MAX(max_elapsed_cycles_len, elapsed_cycles_len);
            max_elapsed_ms_len     = AIL_MAX(max_elapsed_ms_len,     elapsed_ms_len);
            max_min_elapsed_ms_len = AIL_MAX(max_min_elapsed_ms_len, min_elapsed_ms_len);
            max_bandwidth_len      = AIL_MAX(max_bandwidth_len,      bandwidth_len);
            any_with_children     |= (anchor->child_depth < depth) && (anchor->elapsed_with_children != anchor->elapsed_wo_children);
        }
    }
    char tmp_str[16] = {0};
    for (u32 anchor_idx = 0; anchor_idx < AIL_BENCH_PROFILE_ANCHOR_COUNT; anchor_idx++) {
        AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[anchor_idx];
        if (anchor->elapsed_with_children) {
            if (anchor->child_depth <= depth) {
                u64 elapsed_wo_children = (anchor->child_depth == depth) ? anchor->elapsed_with_children : anchor->elapsed_wo_children;
                f64 perc_wo_children = 100.0 * ((f64)elapsed_wo_children / (f64)total_tsc_elapsed);
                printf("  %s[%d]: ", anchor->label, anchor->hit_count);
                i32 len_diff = max_name_hit_count_len - (ail_bench_str_len(anchor->label) + ail_bench_u64_len(anchor->hit_count));
                while (len_diff-- > 0) printf(" ");
                // @Note: The commented-out lines print the elapsed amount of cycles, which seems a relatively unnecessary info to print
                // len_diff = max_elapsed_cycles_len - ail_bench_u64_len(elapsed_wo_children);
                // while (len_diff-- > 0) printf(" ");
                // printf("%zu cycles, ", elapsed_wo_children);
                f64 elapsed_in_ms = ail_bench_cpu_elapsed_to_ms(elapsed_wo_children);
                len_diff = max_elapsed_ms_len - ail_bench_f64_len(elapsed_in_ms, float_print_precision);
                while (len_diff-- > 0) printf(" ");
                printf("%0.*fms ", float_print_precision, elapsed_in_ms);
                printf(" ");

                b32 has_children = (anchor->elapsed_with_children != elapsed_wo_children);
                if (!has_children) {
                    printf("(%5.2f%%) ", perc_wo_children);
                } else {
                    f64 perc_with_children = 100.0 * ((f64)anchor->elapsed_with_children / (f64)total_tsc_elapsed);
                    sprintf(tmp_str, "%5.2f%%, ", perc_wo_children);
                    printf("(%-8s%5.2f%% w/children) ", tmp_str, perc_with_children);
                }
                printf(" ");

                if (!has_children && any_with_children) printf("                   ");
                f64 min_elapsed_in_ms = ail_bench_cpu_elapsed_to_ms((anchor->child_depth == depth) ? anchor->min_with_children : anchor->min_wo_children);
                len_diff = max_min_elapsed_ms_len - ail_bench_f64_len(min_elapsed_in_ms, float_print_precision);
                printf("Min: ");
                while (len_diff-- > 0) printf(" ");
                printf("%0.*fms ", float_print_precision, min_elapsed_in_ms);
                if (has_children && anchor->min_with_children != anchor->min_wo_children) {
                    printf("(%0.*fms w/children) ", float_print_precision, ail_bench_cpu_elapsed_to_ms(anchor->min_with_children));
                }
                printf(" ");

                if (anchor->mem_size) {
                    printf("Bandwidth: ");
                    f64 gb_per_sec = (f64)anchor->mem_size / (f64)(AIL_GB(1) * (elapsed_in_ms/1000));
                    i32 len_diff = max_bandwidth_len - ail_bench_f64_len(gb_per_sec, float_print_precision);
                    while (len_diff-- > 0) printf(" ");
                    if (gb_per_sec > 0.1f) printf("%2.*fGB/s ", float_print_precision, gb_per_sec);
                    else printf("%2.*fMB/s ", float_print_precision, gb_per_sec*1000.0f);
                }
                printf("\n");
            }

            if (clear_anchors) _ail_bench_clear_anchor(anchor);
        }
    }
}
#endif // AIL_BENCH_PROFILE


#if AIL_OS_WIN32
#include <intrin.h>  // For __rdtsc
#include <windows.h> // For QueryPerformanceCounter, OpenProcess, LARGE_INTEGER
#include <psapi.h>   // For PROCESS_MEMORY_COUNTERS_EX, GetProcessMemoryInfo

u64 ail_bench_os_timer_freq(void)
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return freq.QuadPart;
}

u64 ail_bench_os_timer(void)
{
    LARGE_INTEGER freq;
    QueryPerformanceCounter(&freq);
    return freq.QuadPart;
}

u64 ail_bench_count_page_faults(void)
{
    PROCESS_MEMORY_COUNTERS_EX counters = {0};
    counters.cb = sizeof(counters);
    GetProcessMemoryInfo(ail_bench_global_info.proc, (PROCESS_MEMORY_COUNTERS*)&counters, sizeof(counters));
    return counters.PageFaultCount;
}

#else

#include <x86intrin.h> // For __rdtsc
#include <sys/time.h>  // For gettimeofday, struct timeval
#include <sys/mman.h>  // For getrusage, struct rusage

u64 ail_bench_os_timer_freq(void)
{
    return 1000000;
}

u64 ail_bench_os_timer(void)
{
    struct timeval time;
    gettimeofday(&time, 0);
    return ail_bench_os_timer_freq()*(u64)time.tv_sec + (u64)time.tv_usec;
}

u64 ail_bench_count_page_faults(void)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    // ru_minflt  the number of page faults serviced without any I/O activity.
    // ru_majflt  the number of page faults serviced that required I/O activity.
    return usage.ru_minflt + usage.ru_majflt;
}

#endif


void ail_bench_init(void)
{
    ail_bench_global_info.cpu_timer_freq = ail_bench_calc_cpu_timer_freq();
#if AIL_OS_WIN32
    ail_bench_global_info.proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, GetCurrentProcessId());
#endif
}

f64 ail_bench_cpu_elapsed_to_ms(u64 cpu_elapsed)
{
    return 1000.0 * (f64)cpu_elapsed / (f64)ail_bench_global_info.cpu_timer_freq;
}

u64 ail_bench_calc_cpu_timer_freq(void)
{
    u64 millisToWait = 100;
    u64 osFreq       = ail_bench_os_timer_freq();
    u64 cpuStart     = ail_bench_cpu_timer();
    u64 osStart      = ail_bench_os_timer();
    u64 osEnd        = 0;
    u64 osElapsed    = 0;
    u64 osWaitTime   = osFreq * millisToWait / 1000;
    while(osElapsed < osWaitTime) {
        osEnd     = ail_bench_os_timer();
        osElapsed = osEnd - osStart;
    }
    u64 cpuEnd     = ail_bench_cpu_timer();
    u64 cpuElapsed = cpuEnd - cpuStart;
    u64 cpu_freq = 0;
    if (osElapsed) cpu_freq = osFreq * cpuElapsed / osElapsed;
    return cpu_freq;
}

u64 ail_bench_cpu_timer(void)
{
#if defined(AIL_ARCH_X86) || defined(AIL_ARCH_X64)
    return __rdtsc();
#else
#error "Benchmarking CPU Timers on non-x86_64 Architectures is not supported currently"
#endif
}

#endif // _AIL_BENCH_IMPL_GUARD_
#endif // AIL_NO_BENCH_IMPL





////////
// From ail_buf.h
////////


// Simple Buffer
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

#ifndef AIL_BUF_H_
#define AIL_BUF_H_

#include "ail_fs.h"
#include <string.h> // For memcpy

#ifndef AIL_BUF_DEF
#ifdef  AIL_DEF
#define AIL_BUF_DEF AIL_DEF
#else
#define AIL_BUF_DEF
#endif // AIL_DEF
#endif // AIL_BUF_DEF
#ifndef AIL_BUF_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_BUF_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_BUF_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_BUF_DEF_INLINE

#if !defined(AIL_BUF_FREE) && !defined(AIL_BUF_MALLOC)
#if  defined(AIL_MALLOC) &&  defined(AIL_FREE)
#define AIL_BUF_MALLOC AIL_MALLOC
#define AIL_BUF_FREE   AIL_FREE
#else
#include <stdlib.h>
#define AIL_BUF_FREE(ptr)    free(ptr)
#define AIL_BUF_MALLOC(sze) malloc(sze)
#endif
#elif !defined(AIL_BUF_FREE) || !defined(AIL_BUF_MALLOC)
#error "You must define both AIL_BUF_MALLOC and AIL_BUF_FREE, or neither."
#endif

typedef struct {
    u8 *data;
    u64 idx;
    u64 len;
    u64 cap;
} AIL_Buffer;

// @TODO: Add overflow checks when reading/peeking

AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_from_file(const char *filename);
AIL_BUF_DEF_INLINE bool ail_buf_copy_to_file(AIL_Buffer buf, const char *filename);
AIL_BUF_DEF_INLINE bool ail_buf_to_file(AIL_Buffer *buf, const char *filename);
AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_from_data(u8 *data, u64 len, u64 idx);
AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_new(u64 cap);
AIL_BUF_DEF void ail_buf_ensure_size(AIL_Buffer *buf, u64 n);
AIL_BUF_DEF_INLINE void ail_buf_free(AIL_Buffer buf);
AIL_BUF_DEF_INLINE u8  ail_buf_peek1   (AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u16 ail_buf_peek2lsb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u32 ail_buf_peek3lsb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u32 ail_buf_peek4lsb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u64 ail_buf_peek8lsb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u16 ail_buf_peek2msb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u32 ail_buf_peek3msb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u32 ail_buf_peek4msb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u64 ail_buf_peek8msb(AIL_Buffer  buf);
AIL_BUF_DEF        char *ail_buf_peekstr (AIL_Buffer buf, u64 len);
AIL_BUF_DEF        char *ail_buf_peekcstr(AIL_Buffer buf);
AIL_BUF_DEF u8  ail_buf_read1   (AIL_Buffer *buf);
AIL_BUF_DEF u16 ail_buf_read2lsb(AIL_Buffer *buf);
AIL_BUF_DEF u32 ail_buf_read3lsb(AIL_Buffer *buf);
AIL_BUF_DEF u32 ail_buf_read4lsb(AIL_Buffer *buf);
AIL_BUF_DEF u64 ail_buf_read8lsb(AIL_Buffer *buf);
AIL_BUF_DEF u16 ail_buf_read2msb(AIL_Buffer *buf);
AIL_BUF_DEF u32 ail_buf_read3msb(AIL_Buffer *buf);
AIL_BUF_DEF u32 ail_buf_read4msb(AIL_Buffer *buf);
AIL_BUF_DEF u64 ail_buf_read8msb(AIL_Buffer *buf);
AIL_BUF_DEF char *ail_buf_readstr (AIL_Buffer *buf, u64 len);
AIL_BUF_DEF char *ail_buf_readcstr(AIL_Buffer *buf);
AIL_BUF_DEF void ail_buf_write1   (AIL_Buffer *buf, u8  val);
AIL_BUF_DEF void ail_buf_write2lsb(AIL_Buffer *buf, u16 val);
AIL_BUF_DEF void ail_buf_write3lsb(AIL_Buffer *buf, u32 val);
AIL_BUF_DEF void ail_buf_write4lsb(AIL_Buffer *buf, u32 val);
AIL_BUF_DEF void ail_buf_write8lsb(AIL_Buffer *buf, u64 val);
AIL_BUF_DEF void ail_buf_write2msb(AIL_Buffer *buf, u16 val);
AIL_BUF_DEF void ail_buf_write3msb(AIL_Buffer *buf, u32 val);
AIL_BUF_DEF void ail_buf_write4msb(AIL_Buffer *buf, u32 val);
AIL_BUF_DEF void ail_buf_write8msb(AIL_Buffer *buf, u64 val);
AIL_BUF_DEF void ail_buf_writestr (AIL_Buffer *buf, char *str, u64 len);
AIL_BUF_DEF void ail_buf_writecstr(AIL_Buffer *buf, char *str);

#endif // AIL_BUF_H_


#if !defined(AIL_NO_BUF_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_BUF_IMPL_GUARD_
#define _AIL_BUF_IMPL_GUARD_

AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_from_file(const char *filename)
{
    u64 len;
    u8 *data = (u8 *)ail_fs_read_entire_file(filename, &len);
    return (AIL_Buffer) {
        .data = data,
        .idx  = 0,
        .len  = len,
        .cap  = len,
    };
}

AIL_BUF_DEF_INLINE  bool ail_buf_copy_to_file(AIL_Buffer buf, const char *filename)
{
    return ail_fs_write_file(filename, (char*) buf.data, buf.len);
}

// Unlike copyToFile, this function frees the buffer after writing it to the file
AIL_BUF_DEF_INLINE bool ail_buf_to_file(AIL_Buffer *buf, const char *filename)
{
    bool out = ail_fs_write_file(filename, (char*)(buf->data), buf->len);
    AIL_BUF_FREE((void *)buf->data);
    return out;
}

AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_new(u64 cap)
{
    AIL_Buffer buf;
    buf.data = AIL_BUF_MALLOC(cap);
    buf.len  = 0;
    buf.cap  = cap;
    buf.idx  = 0;
    return buf;
}

AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_from_data(u8 *data, u64 len, u64 idx)
{
    AIL_Buffer buf;
    buf.data = data;
    buf.idx  = idx;
    buf.len  = len;
    buf.cap  = len;
    return buf;
}

AIL_BUF_DEF_INLINE void ail_buf_free(AIL_Buffer buf)
{
    AIL_BUF_FREE((void *)buf.data);
}

// Ensures that there's enough capacity to write `n` more bytes into the buffer
AIL_BUF_DEF void ail_buf_ensure_size(AIL_Buffer *buf, u64 n)
{
    u64 min = buf->len + n;
    if (AIL_UNLIKELY(min > buf->cap)) {
        u64 new_cap = buf->cap * 2;
        if (AIL_UNLIKELY(min > new_cap)) new_cap = min;
        u8 *new_data = (u8 *) AIL_BUF_MALLOC(new_cap);
        memcpy(new_data, buf->data, buf->len);
        AIL_BUF_FREE(buf->data);
        buf->data = new_data;
        buf->cap = new_cap;
    }
}

AIL_BUF_DEF_INLINE u8 ail_buf_peek1(AIL_Buffer buf)
{
    return buf.data[buf.idx];
}
#define ail_buf_peek1lsb(buf) ail_buf_peek1f)
#define ail_buf_peek1msb(buf) ail_buf_peek1f)

AIL_BUF_DEF_INLINE u16 ail_buf_peek2lsb(AIL_Buffer buf)
{
    return ((u16)buf.data[buf.idx + 1] << 8) | ((u16)buf.data[buf.idx + 0] << 0);
}

AIL_BUF_DEF_INLINE u32 ail_buf_peek3lsb(AIL_Buffer buf)
{
    return ((u32)buf.data[buf.idx + 2] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 0] << 0);
}

AIL_BUF_DEF_INLINE u32 ail_buf_peek4lsb(AIL_Buffer buf)
{
    return ((u32)buf.data[buf.idx + 3] << 24) | ((u32)buf.data[buf.idx + 2] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 0] << 0);
}

AIL_BUF_DEF_INLINE u64 ail_buf_peek8lsb(AIL_Buffer buf)
{
    return ((u64)buf.data[buf.idx + 7] << 56) | ((u64)buf.data[buf.idx + 6] << 48) | ((u64)buf.data[buf.idx + 5] << 40) | ((u64)buf.data[buf.idx + 4] << 32) |
           ((u64)buf.data[buf.idx + 3] << 24) | ((u64)buf.data[buf.idx + 2] << 16) | ((u64)buf.data[buf.idx + 1] <<  8) | ((u64)buf.data[buf.idx + 0] <<  0);
}

AIL_BUF_DEF_INLINE u16 ail_buf_peek2msb(AIL_Buffer buf)
{
    return ((u16)buf.data[buf.idx + 0] << 8) | ((u16)buf.data[buf.idx + 1] << 0);
}

AIL_BUF_DEF_INLINE u32 ail_buf_peek3msb(AIL_Buffer buf)
{
    return ((u32)buf.data[buf.idx + 0] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 2] << 0);
}

AIL_BUF_DEF_INLINE u32 ail_buf_peek4msb(AIL_Buffer buf)
{
    return ((u32)buf.data[buf.idx + 0] << 24) | ((u32)buf.data[buf.idx + 1] << 16) | ((u32)buf.data[buf.idx + 2] << 8) | ((u32)buf.data[buf.idx + 3] << 0);
}

AIL_BUF_DEF_INLINE u64 ail_buf_peek8msb(AIL_Buffer buf)
{
    return ((u64)buf.data[buf.idx + 0] << 56) | ((u64)buf.data[buf.idx + 1] << 48) | ((u64)buf.data[buf.idx + 2] << 40) | ((u64)buf.data[buf.idx + 3] << 32) |
           ((u64)buf.data[buf.idx + 4] << 24) | ((u64)buf.data[buf.idx + 5] << 16) | ((u64)buf.data[buf.idx + 6] <<  8) | ((u64)buf.data[buf.idx + 7] <<  0);
}

AIL_BUF_DEF char *ail_buf_peekstr(AIL_Buffer buf, u64 len)
{
    char *out = AIL_BUF_MALLOC(sizeof(char) * (len + 1));
    memcpy(out, &buf.data[buf.idx], len);
    out[len] = 0;
    return out;
}

AIL_BUF_DEF char *ail_buf_peekcstr(AIL_Buffer buf)
{
    u64 i = 0;
    while (buf.data[buf.idx + i] != 0 && buf.len > buf.idx + i) i++;
    return ail_buf_peekstr(buf, i);
}

AIL_BUF_DEF u8  ail_buf_read1(AIL_Buffer *buf)
{
    return buf->data[buf->idx++];
}
#define ail_buf_read1lsb(buf) ail_buf_read1(buf)
#define ail_buf_read1msb(buf) ail_buf_read1(buf)

AIL_BUF_DEF u16 ail_buf_read2lsb(AIL_Buffer *buf)
{
    u16 out   = ail_buf_peek2lsb(*buf);
    buf->idx += sizeof(u16);
    return out;
}

AIL_BUF_DEF u32 ail_buf_read3lsb(AIL_Buffer *buf)
{
    u32 out   = ail_buf_peek3lsb(*buf);
    buf->idx += 3;
    return out;
}

AIL_BUF_DEF u32 ail_buf_read4lsb(AIL_Buffer *buf)
{
    u32 out   = ail_buf_peek4lsb(*buf);
    buf->idx += sizeof(u32);
    return out;
}

AIL_BUF_DEF u64 ail_buf_read8lsb(AIL_Buffer *buf)
{
    u64 out   = ail_buf_peek8lsb(*buf);
    buf->idx += sizeof(u64);
    return out;
}

AIL_BUF_DEF u16 ail_buf_read2msb(AIL_Buffer *buf)
{
    u16 out   = ail_buf_peek2msb(*buf);
    buf->idx += sizeof(u16);
    return out;
}

AIL_BUF_DEF u32 ail_buf_read3msb(AIL_Buffer *buf)
{
    u32 out   = ail_buf_peek3msb(*buf);
    buf->idx += 3;
    return out;
}

AIL_BUF_DEF u32 ail_buf_read4msb(AIL_Buffer *buf)
{
    u32 out   = ail_buf_peek4msb(*buf);
    buf->idx += sizeof(u32);
    return out;
}

AIL_BUF_DEF u64 ail_buf_read8msb(AIL_Buffer *buf)
{
    u64 out   = ail_buf_peek8msb(*buf);
    buf->idx += sizeof(u64);
    return out;
}

AIL_BUF_DEF char *ail_buf_readstr(AIL_Buffer *buf, u64 len)
{
    char *out = ail_buf_peekstr(*buf, len);
    buf->idx += len;
    return out;
}

AIL_BUF_DEF char *ail_buf_readcstr(AIL_Buffer *buf)
{
    u64 len = 0;
    while (buf->data[buf->idx + len] != 0 && buf->len > buf->idx + len) len++;
    char *out = ail_buf_readstr(buf, len);
    buf->idx = len + 1;
    return out;
}


AIL_BUF_DEF void ail_buf_write1(AIL_Buffer *buf, u8  val)
{
    ail_buf_ensure_size(buf, 1);
    buf->data[buf->idx++] = val;
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}
#define ail_buf_write1lsb(buf, val) ail_buf_write1(buf, val)
#define ail_buf_write1msb(buf, val) ail_buf_write1(buf, val)

AIL_BUF_DEF void ail_buf_write2lsb(AIL_Buffer *buf, u16 val)
{
    ail_buf_ensure_size(buf, sizeof(u16));
    for (u8 i = 0; i < (u8)sizeof(u16); i++) {
        buf->data[buf->idx++] = (u8)(val & 0xff);
        val >>= 8;
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write3lsb(AIL_Buffer *buf, u32 val)
{
    ail_buf_ensure_size(buf, 3);
    for (u8 i = 0; i < 3; i++) {
        buf->data[buf->idx++] = (u8)(val & 0xff);
        val >>= 8;
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write4lsb(AIL_Buffer *buf, u32 val)
{
    ail_buf_ensure_size(buf, sizeof(u32));
    for (u8 i = 0; i < (u8)sizeof(u32); i++) {
        buf->data[buf->idx++] = (u8)(val & 0xff);
        val >>= 8;
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write8lsb(AIL_Buffer *buf, u64 val)
{
    ail_buf_ensure_size(buf, sizeof(u64));
    for (u8 i = 0; i < (u8)sizeof(u64); i++) {
        buf->data[buf->idx++] = (u8)(val & 0xff);
        val >>= 8;
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write2msb(AIL_Buffer *buf, u16 val)
{
    ail_buf_ensure_size(buf, sizeof(u16));
    for (u8 i = 0; i < (u8)sizeof(u16); i++) {
        buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u16) - i - 1)));
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write3msb(AIL_Buffer *buf, u32 val)
{
    ail_buf_ensure_size(buf, 3);
    for (u8 i = 0; i < 3; i++) {
        buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(3 - i - 1)));
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write4msb(AIL_Buffer *buf, u32 val)
{
    ail_buf_ensure_size(buf, sizeof(u32));
    for (u8 i = 0; i < (u8)sizeof(u32); i++) {
        buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u32) - i - 1)));
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write8msb(AIL_Buffer *buf, u64 val)
{
    ail_buf_ensure_size(buf, sizeof(u64));
    for (u8 i = 0; i < (u8)sizeof(u64); i++) {
        buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u64) - i - 1)));
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_writestr(AIL_Buffer *buf, char *str, u64 len)
{
    for (u64 i = 0; i < len; i++) buf->data[buf->idx++] = str[i];
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_writecstr(AIL_Buffer *buf, char *str)
{
    for (u64 i = 0; str[i] != 0; i++) buf->data[buf->idx++] = str[i];
    buf->data[buf->idx++] = 0;
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

#endif // _AIL_BUF_IMPL_GUARD_
#endif // AIL_NO_BUF_IMPL





////////
// From ail_build.h
////////


// Build System for writing build-scripts in C
//
// Strongly inspired by Tsoding's nob.h (https://github.com/tsoding/musializer/blob/master/nob.h)
// @Note: Works but barely useable yet
//
// LICENSE regarding Tsoding's code
/*
Copyright 2023 Alexey Kutepov <reximkut@gmail.com> and Musializer Contributors

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//
// LICENSE regarding modifications & original code
/*
Copyright (c) 2024 Lily Val Richter

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

#ifndef AIL_BUILD_H_
#define AIL_BUILD_H_

#define AIL_ALL_IMPL

#ifndef AIL_BUILD_DEF
#ifdef  AIL_DEF
#define AIL_BUILD_DEF AIL_DEF
#else
#define AIL_BUILD_DEF
#endif // AIL_DEF
#endif // AIL_BUILD_DEF
#ifndef AIL_BUILD_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_BUILD_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_BUILD_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_BUILD_DEF_INLINE

typedef enum AIL_Build_Comp_Type {
    AIL_BUILD_COMP_DEFAULT,
    AIL_BUILD_COMP_MSVC,
    AIL_BUILD_COMP_GCC,
    AIL_BUILD_COMP_CLANG,
    AIL_BUILD_COMP_ZIG,
    AIL_BUILD_COMP_TCC,
    AIL_BUILD_COMP_INTEL,
    AIL_BUILD_COMP_INTEL_CL,
    AIL_BUILD_COMP_DMC,
    AIL_BUILD_COMP_PELLESC,
    AIL_BUILD_COMP_OTHER,
    AIL_BUILD_COMP_COUNT,
} AIL_Build_Comp_Type;

typedef struct AIL_Build_Comp {
    AIL_Build_Comp_Type type;
    char *name;
} AIL_Build_Comp;

#define AIL_BUILD_COMP_IS_SPECIAL(cc) (cc == AIL_BUILD_COMP_DEFAULT || cc == AIL_BUILD_COMP_OTHER || cc == AIL_BUILD_COMP_COUNT)

typedef enum AIL_Build_Flags {
    AIL_BUILD_FLAG_DEBUG   = 1 << 0,
    AIL_BUILD_FLAG_O1      = 1 << 1,
    AIL_BUILD_FLAG_O2      = 1 << 2,
    AIL_BUILD_FLAG_O3      = 1 << 3,
    AIL_BUILD_FLAG_OSIZE   = 1 << 4,
    AIL_BUILD_FLAG_WALL    = 1 << 5,
    AIL_BUILD_FLAG_STD_C99 = 1 << 6,
    // @TODO: Add more options
} AIL_Build_Flags;

#include <stdio.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <shellapi.h>
typedef HANDLE AIL_Build_OS_Pipe;
typedef HANDLE AIL_Build_OS_Proc;
#define _AIL_BUILD_INVALID_OS_PROC_ INVALID_HANDLE_VALUE
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
typedef int AIL_Build_OS_Pipe;
typedef int AIL_Build_OS_Proc;
#define _AIL_BUILD_INVALID_OS_PROC_ (-1)
#endif // _WIN32

typedef struct {
    char *out;
    b32 succ;
} AIL_Build_Proc_Res;

AIL_BUILD_DEF void  ail_build_rebuild_urself(i32 argc, char **argv);
AIL_BUILD_DEF char* ail_build_get_exe_name(const char *s, u32 n, AIL_Allocator allocator);
AIL_BUILD_DEF AIL_Build_Proc_Res ail_build_proc_exec(AIL_DA(pchar) *argv, AIL_Allocator allocator);
AIL_BUILD_DEF void  ail_build(AIL_Build_Comp cc, AIL_Build_Flags cflags, char *out_name, char *fpath, AIL_Allocator allocator);
AIL_BUILD_DEF AIL_Build_Comp ail_build_comp_from_str(char *str);

#endif // AIL_BUILD_H_



#if !defined(AIL_NO_BUILD_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_BUILD_IMPL_GUARD_
#define _AIL_BUILD_IMPL_GUARD_

#ifndef AIL_BUILD_PIPE_SIZE
#define AIL_BUILD_PIPE_SIZE 2048
#endif

// Case-insensitive str-compare - expects `b` to be lowercase
#define _AIL_BUILD_STREQ_I(a_str, a_len, b_literal) ail_build_streq_i(a_str, a_len, b_literal, (sizeof(b_literal) - 1))
b32 ail_build_streq_i(const char *restrict a, u64 alen, const char *restrict b, u64 blen)
{
    if (alen != blen) return false;
    for (u32 i = 0; i < alen; i++) {
        if (tolower(a[i]) != b[i]) return false;
    }
    return true;
}

void  ail_build_rebuild_urself(i32 argc, char **argv)
{
    AIL_TODO();
}

AIL_Build_Comp ail_build_comp_from_str(char *str)
{
    AIL_Build_Comp comp = { 0 };
    u64 len = strlen(str);
    if (_AIL_BUILD_STREQ_I(str, len, "cl") || _AIL_BUILD_STREQ_I(str, len, "msvc")) {
        comp.type = AIL_BUILD_COMP_MSVC;;
    } else if (_AIL_BUILD_STREQ_I(str, len, "gcc") || _AIL_BUILD_STREQ_I(str, len, "gnu")) {
        comp.type = AIL_BUILD_COMP_GCC;
    } else if (_AIL_BUILD_STREQ_I(str, len, "clang") || _AIL_BUILD_STREQ_I(str, len, "llvm")) {
        comp.type = AIL_BUILD_COMP_CLANG;
    } else if (_AIL_BUILD_STREQ_I(str, len, "zig") || _AIL_BUILD_STREQ_I(str, len, "zig cc")) {
        comp.type = AIL_BUILD_COMP_ZIG;
    } else if (_AIL_BUILD_STREQ_I(str, len, "tcc") || _AIL_BUILD_STREQ_I(str, len, "tiny cc")) {
        comp.type = AIL_BUILD_COMP_TCC;
    } else if (_AIL_BUILD_STREQ_I(str, len, "intel") || _AIL_BUILD_STREQ_I(str, len, "icx-cc")) {
        comp.type = AIL_BUILD_COMP_INTEL;
    } else if (_AIL_BUILD_STREQ_I(str, len, "intel cl") || _AIL_BUILD_STREQ_I(str, len, "icx-cl")) {
        comp.type = AIL_BUILD_COMP_INTEL_CL;
    } else if (_AIL_BUILD_STREQ_I(str, len, "dmc")) {
        comp.type = AIL_BUILD_COMP_DMC;
    } else if (_AIL_BUILD_STREQ_I(str, len, "pocc") || _AIL_BUILD_STREQ_I(str, len, "pelles") || _AIL_BUILD_STREQ_I(str, len, "pelles c")) {
        comp.type = AIL_BUILD_COMP_PELLESC;
    } else {
        comp.type  = AIL_BUILD_COMP_OTHER;
        comp.name = str;
    }
    return comp;
}

char* ail_build_get_exe_name(const char *s, u32 n, AIL_Allocator allocator)
{
    // @TODO: Don't assume that we necessarily compile a file named '*.c'
    // @TODO: Add support for more platforms, like MacOS
#ifdef _WIN32
#   define _AIL_BUILD_EXE_EXT "exe"
#else
#   define _AIL_BUILD_EXE_EXT ""
#endif
    u32 ext_len = 1; // extension assumed to be '.c'
    char *buf = AIL_CALL_ALLOC(allocator, n - ext_len + sizeof(_AIL_BUILD_EXE_EXT));
    memcpy(buf, s, n - ext_len);
    memcpy(&buf[n - ext_len], _AIL_BUILD_EXE_EXT, sizeof(_AIL_BUILD_EXE_EXT));
    return buf;
}

// @Note: Code taken almost 1-to-1 from tsoding's nob.h (https://github.com/tsoding/musializer/blob/master/nob.h)
char* ail_build_cmd_to_str(AIL_DA(pchar) cmd)
{
    AIL_ASSERT(cmd.len);
    AIL_DA(char) str = ail_da_new_with_alloc(char, 256, cmd.allocator);
    ail_da_pushn(&str, cmd.data[0], strlen(cmd.data[0]));
    for (u32 i = 1; i < cmd.len; i++) {
        char *arg = cmd.data[i];
        AIL_ASSERT(arg);
        ail_da_push(&str, ' ');
        if (!strchr(arg, ' ')) {
            ail_da_pushn(&str, arg, strlen(arg));
        } else {
            ail_da_push(&str, '"');
            ail_da_pushn(&str, arg, strlen(arg));
            ail_da_push(&str, '"');
        }
    }
    ail_da_push(&str, 0);
    return str.data;
}

AIL_Build_Proc_Res ail_build_proc_exec(AIL_DA(pchar) *argv, AIL_Allocator allocator)
{
    AIL_Build_Proc_Res res = {
        .out  = "",
        .succ = false,
    };
    if (!argv->len) {
        printf("Error: Cannot run empty command.\n");
        return res;
    }
    char *arg_str = ail_build_cmd_to_str(*argv);
    printf("CMD: '%s'\n", arg_str);
#ifdef _WIN32
    AIL_Build_OS_Pipe pipe_read, pipe_write;
    SECURITY_ATTRIBUTES saAttr = {0};
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    if (!CreatePipe(&pipe_read, &pipe_write, &saAttr, 0)) {
        printf("Error: Could not establish pipe to child process\n");
        return res;
    }

    STARTUPINFO siStartInfo;
    ZeroMemory(&siStartInfo, sizeof(siStartInfo));
    siStartInfo.cb = sizeof(STARTUPINFO);

    siStartInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
    siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (siStartInfo.hStdOutput == INVALID_HANDLE_VALUE || siStartInfo.hStdError == INVALID_HANDLE_VALUE) {
        printf("Error: Could not get the handles to child process stdout/stderr.");
        return res;
    }
    siStartInfo.hStdOutput = pipe_write;
    siStartInfo.hStdError  = pipe_write;
    siStartInfo.dwFlags   |= STARTF_USESTDHANDLES;
    PROCESS_INFORMATION piProcInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    if (!CreateProcessA(NULL, arg_str, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)) {
        printf("Error: Could not create child process.\n");
        // @Note: Intentional Memory Leak - this library is only used for short-running build processes, leaking memory is fine here
        // CloseHandle(pipe_write);
        // CloseHandle(pipe_read);
        return res;
    }

    if (WaitForSingleObject(piProcInfo.hProcess, INFINITE) == WAIT_FAILED) {
        printf("Error: Failed to wait for child process to exit.\n");
        // @Note: Intentional Memory Leak - this library is only used for short-running build processes, leaking memory is fine here
        // CloseHandle(pipe_write);
        // CloseHandle(pipe_read);
        // CloseHandle(piProcInfo.hProcess);
        return res;
    }

    char *buf = AIL_CALL_ALLOC(allocator, AIL_BUILD_PIPE_SIZE);
    if (!buf) {
        printf("Error: Could not allocate enough memory to read child process' output.\n");
        return res;
    }
    DWORD nBytesRead;
    ReadFile(pipe_read, buf, AIL_BUILD_PIPE_SIZE, &nBytesRead, 0);
    res.out = buf;
#else
    AIL_Build_OS_Pipe pipefd[2];
    if (pipe(pipefd) != -1) {
        printf("Error: Could not establish pipe to child process.\n");
        return res;
    }

    AIL_DA(char) da = ail_da_new_with_alloc(char, AIL_BUILD_PIPE_SIZE, allocator);
    char buf[AIL_BUILD_PIPE_SIZE] = {0};
    pid_t cpid = fork();
    if (cpid < 0) {
        printf("Error: Could not create child process.\n");
        return res;
    } else if (cpid == 0) { // Run by child
        ail_da_push(argv, NULL);
        close(pipefd[0]);
        execvp(argv->data[0], (char* const*) argv->data);
        while (read(stdout, buf, AIL_BUILD_PIPE_SIZE) != EOF) {
            u64 len = strlen(buf);
            ail_da_pushn(&da, buf, len);
            memset(buf, 0, len);
        }
        write(pipefd[1], da.data, da.len);
        close(pipefd[1]); // Required for reader to see EOF
    } else { // Run by parent
        close(pipefd[1]);
        while (read(pipefd[0], buf, AIL_BUILD_PIPE_SIZE) != EOF) {
            u64 len = strlen(buf);
            ail_da_pushn(&da, buf, len);
            memset(buf, 0, len);
        }
        int wstatus = 0;
        if (waitpid(cpid, &wstatus, 0) < 0) {
            printf("Error: Failed to wait for child process to exit.\n");
            return res;
        }
        ail_da_push(&da, 0);
        res.out = da.data;
    }
#endif
    // @Note: Intentional Memory Leak - this library is only used for short-running build processes, leaking memory is fine here
    // AIL_CALL_FREE(argv->allocator, arg_str);
    res.succ = true;
    return res;
}

void ail_build(AIL_Build_Comp cc, AIL_Build_Flags cflags, char *out_name, char *fpath, AIL_Allocator allocator)
{
    AIL_DA(pchar) argv = ail_da_new_with_alloc(pchar, 256, allocator);
    switch (cc.type) {
        case AIL_BUILD_COMP_DEFAULT: {
            AIL_TODO(); // Pick compiler based on current platform and availability of compilers (how do I check that?)
        } break;
        case AIL_BUILD_COMP_MSVC: {
            ail_da_push(&argv, "cl");
            u64 out_name_len = strlen(out_name);
            char *buf = AIL_CALL_ALLOC(allocator, out_name_len + 4);
            memcpy(buf, "/Fe", 3);
            memcpy(&buf[3], out_name, out_name_len + 1);
            ail_da_push(&argv, buf);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "/Zi");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "/O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "/Ox");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "/O2");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "/Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "/Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "/std:c11"); // MSVC only supports C11, C17 & C-latest (https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)
        } break;
        case AIL_BUILD_COMP_GCC: {
            ail_da_push(&argv, "gcc");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_CLANG: {
            ail_da_push(&argv, "clang");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_ZIG: {
            ail_da_push(&argv, "zig cc");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_TCC: {
            ail_da_push(&argv, "tcc");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_INTEL: {
            ail_da_push(&argv, "icx-cc");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_INTEL_CL: {
            ail_da_push(&argv, "icx-cl");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_DMC: {
            ail_da_push(&argv, "dmc");
            u64 out_name_len = strlen(out_name);
            char *buf = AIL_CALL_ALLOC(allocator, out_name_len + 3);
            memcpy(buf, "-o", 2);
            memcpy(&buf[2], out_name, out_name_len + 1);
            ail_da_push(&argv, buf);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-o1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-o2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-o3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-os");
            if (cflags & AIL_BUILD_FLAG_WALL)    {}
            if (cflags & AIL_BUILD_FLAG_STD_C99) {}
        } break;
        case AIL_BUILD_COMP_PELLESC: {
            ail_da_push(&argv, "cc");
            u64 out_name_len = strlen(out_name);
            char *buf = AIL_CALL_ALLOC(allocator, out_name_len + 4);
            memcpy(buf, "/Fe", 3);
            memcpy(&buf[3], out_name, out_name_len + 1);
            ail_da_push(&argv, buf);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "/Zi");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "/Ob1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "/Ot");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "/Ox");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "/Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "/W2");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "/std:c99"); // MSVC only supports C11, C17 & C-latest (https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)
        } break;
        case AIL_BUILD_COMP_OTHER: {
            ail_da_push(&argv, cc.name);
            ail_da_push(&argv, fpath);
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_COUNT: {
            AIL_UNREACHABLE();
        } break;


    }
    AIL_Build_Proc_Res res = ail_build_proc_exec(&argv, allocator);
    if (res.succ) puts(res.out);
}

#endif // _AIL_BUILD_IMPL_GUARD_
#endif // AIL_NO_BUILD_IMPL





////////
// From ail_fs.h
////////


// File System Utilities
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

#ifndef AIL_FS_H_
#define AIL_FS_H_

#include "ail.h"      // For type-definitions
#include <stdlib.h>   // For memcpy (and malloc if not overwritten)
#include <fcntl.h>    // For file access flags
#include <sys/stat.h> // For file stats

#ifndef AIL_FS_DEF
#ifdef  AIL_DEF
#define AIL_FS_DEF AIL_DEF
#else
#define AIL_FS_DEF
#endif // AIL_DEF
#endif // AIL_FS_DEF
#ifndef AIL_FS_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_FS_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_FS_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_FS_DEF_INLINE

#ifndef AIL_FS_MALLOC
#ifdef  AIL_MALLOC
#define AIL_FS_MALLOC(sz) AIL_MALLOC(sz)
#else
#define AIL_FS_MALLOC(sz) malloc(sz)
#endif // AIL_MALLOC
#endif // AIL_FS_MALLOC

#ifndef AIL_FS_READ_TIMEOUT
#define AIL_FS_READ_TIMEOUT 500 // in milliseconds
#endif // AIL_FS_READ_TIMEOUT
#ifndef AIL_FS_WRITE_TIMEOUT
#define AIL_FS_WRITE_TIMEOUT 500 // in milliseconds
#endif // AIL_FS_WRITE_TIMEOUT
#ifndef AIL_FS_MAX_ATTEMPTS
#define AIL_FS_MAX_ATTEMPTS 8
#endif // AIL_FS_MAX_ATTEMPTS

#if AIL_OS_WIN32
#   include <windows.h>
#   include <direct.h>
#   define mkdir(dir, mode)      _mkdir(dir)
#   define open(name, ...)       _open(name, __VA_ARGS__)
#   define read(fd, buf, count)  _read(fd, buf, count)
#   define close(fd)             _close(fd)
#   define write(fd, buf, count) _write(fd, buf, count)
#   define dup2(fd1, fd2)        _dup2(fd1, fd2)
#   define unlink(file)          _unlink(file)
#   define rmdir(dir)            _rmdir(dir)

#   define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#   define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#else
#   include <dirent.h>
#endif

///////////////////
// Files/Directories
///////////////////

#if AIL_OS_WIN32
#   define AIL_FS_DIRENT_NAME_LEN (MAX_PATH + MAX_PATH + 1)
#else
#   define AIL_FS_DIRENT_NAME_LEN (256)
#endif

typedef enum AIL_FS_Error {
    AIL_FS_ERR_SUCCESS,           // No error
    AIL_FS_ERR_ACCESS,            // No access for the desired action
    AIL_FS_ERR_BAD_HANDLE,        // the provided handle was invalid
    AIL_FS_ERR_OPEN_HANDLE_LIMIT, // Reached limit of open files/directories
    AIL_FS_ERR_NO_ENTRY,          // The desired file/directory doesn't exist
    AIL_FS_ERR_NO_MEMORY,         // Not enough memory to do the desired action
    AIL_FS_ERR_NOT_DIR,           // The provided target is not a directory
    AIL_FS_ERR_NOT_FILE,          // The provided target is not a file
} AIL_FS_Error;

typedef enum AIL_FS_Entry_Type {
    AIL_FS_ENTRY_FILE,
    AIL_FS_ENTRY_DIR,
    AIL_FS_ENTRY_OTHER,
} AIL_FS_Entry_Type;

typedef struct AIL_FS_Dirent {
    char name[AIL_FS_DIRENT_NAME_LEN];
    AIL_FS_Entry_Type type;
    AIL_FS_Error err;
} AIL_FS_Dirent;
static const AIL_FS_Dirent AIL_FS_DIRENT_NIL = {0};

typedef struct AIL_FS_Read_Dir_Res {
    void* handle;
    AIL_FS_Dirent dirent;
    AIL_FS_Error err;
} AIL_FS_Read_Dir_Res;

AIL_FS_DEF_INLINE b32 ail_fs_dirent_is_nil(AIL_FS_Dirent a);
AIL_FS_DEF AIL_FS_Read_Dir_Res ail_fs_read_dir_init(const char *dirpath);
AIL_FS_DEF AIL_FS_Dirent ail_fs_read_dir_next(AIL_FS_Read_Dir_Res dir);
AIL_FS_DEF void ail_fs_read_dir_deinit(AIL_FS_Read_Dir_Res dir);

AIL_FS_DEF b32 ail_fs_open_file(const char *fpath, u64 *file, b32 writeable);

AIL_FS_DEF void ail_fs_close_file(u64 file);

// Reads n bytes from file 'fname' into 'buf'
// @Note: 'fd' be valid File-Handle on Windows (a void pointer cast to a u64) and a file-descriptor on Unix
// @Note: 'buf' must have space for 'maxN' bytes
// @Note: 'actualN' will contain the amount of bytes, that were actually read into 'buf'
AIL_FS_DEF b32 ail_fs_read_n_bytes(u64 fd, void *buf, u64 maxN, u64 *actualN);

// Same as ail_fs_read_n_bytes(), except that it handles opening and closing the file
AIL_FS_DEF b32 ail_fs_read_file(const char *fpath, void *buf, u64 maxN, u64 *actualN);

// Same as ail_fs_read_file(), except that it checks the file's size first and allocates an output buffer of the appropriate size for it
// Returns NULL on error
AIL_FS_DEF char* ail_fs_read_entire_file(const char *fpath, u64 *size);

// Write `size` many bytes from `buf` into `fpath`
AIL_FS_DEF b32  ail_fs_write_file(const char *fpath, const char *buf, u64 size);

//////////////////
// Miscellanous //
//////////////////

// @Important: Not yet implemented
AIL_FS_DEF AIL_DA(pchar) ail_fs_get_files_in_dir(const char *dirpath);

AIL_FS_DEF_INLINE b32 ail_fs_dir_exists(const char *dirpath);
AIL_FS_DEF const char *ail_fs_get_file_ext(const char *filename);
AIL_FS_DEF b32 ail_fs_is_file_ext(const char *restrict filename, const char *restrict ext);
AIL_FS_DEF b32 ail_fs_is_file(const char *path);

#endif // AIL_FS_H_



#if !defined(AIL_NO_FS_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_FS_IMPL_GUARD_
#define _AIL_FS_IMPL_GUARD_

///////////////////////
// Utility functions //
///////////////////////

b32 ail_fs_str_eq(const char *restrict a, const char *restrict b)
{
    while (*a && *b && *a++ == *b++) {}
    return *a == *b && !*a;
}

//////////////////////
// Read Directories //
//////////////////////

b32 ail_fs_dirent_is_nil(AIL_FS_Dirent a)
{
    return !a.name[0];
}

AIL_FS_Read_Dir_Res ail_fs_read_dir_init(const char *dirpath)
{
    AIL_ASSERT(dirpath);
    AIL_FS_Read_Dir_Res res = { 0 };
#if AIL_OS_WIN32
    WCHAR buffer[MAX_PATH];
    swprintf_s(buffer, MAX_PATH, L"%s\\*", dirpath);
    WIN32_FIND_DATAW ffd;
    res.handle = FindFirstFileW(buffer, &ffd);
    if (res.handle == INVALID_HANDLE_VALUE) res.err = AIL_FS_ERR_NO_ENTRY;
    else {
        // @TODO
        i32 len = WideCharToMultiByte(CP_UTF8, 0, buffer, lstrlenW(buffer), res.dirent.name, sizeof(res.dirent.name), NULL, NULL);
        AIL_UNUSED(len);
        res.dirent.type = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? AIL_FS_ENTRY_DIR : AIL_FS_ENTRY_FILE;
    }
#else
    AIL_TODO();
#endif
    return res;
}

AIL_FS_Dirent ail_fs_read_dir_next(AIL_FS_Read_Dir_Res dir)
{
    AIL_FS_Dirent res = dir.dirent;
    if (AIL_LIKELY(ail_fs_dirent_is_nil(res))) {
#if AIL_OS_WIN32
        WIN32_FIND_DATAW ffd;
        if (!FindNextFileW(dir.handle, &ffd)) {
            DWORD err = GetLastError();
            if (err != ERROR_NO_MORE_FILES) {
                // @TODO: Figure out what error actually happened
                res.err = AIL_FS_ERR_NO_ENTRY;
            }
        } else {
            AIL_ASSERT(AIL_ARRLEN(res.name) >= AIL_ARRLEN(ffd.cFileName));
            memcpy(res.name, ffd.cFileName, AIL_ARRLEN(ffd.cFileName));
            res.type = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? AIL_FS_ENTRY_DIR : AIL_FS_ENTRY_FILE;
        }
#else
        AIL_TODO();
#endif
    } else {
        dir.dirent = AIL_FS_DIRENT_NIL;
    }
    return res;
}

void ail_fs_read_dir_deinit(AIL_FS_Read_Dir_Res dir)
{
#if AIL_OS_WIN32
    FindClose(dir.handle);
#else
    AIL_TODO();
#endif
}



///////////////////
// Read/Write IO //
///////////////////

b32 ail_fs_open_file(const char *fpath, u64 *file, b32 writeable)
{
#if AIL_OS_WIN32
    u32 access = GENERIC_READ;
    if (writeable) access |= GENERIC_WRITE;
    void *handle = CreateFile(fpath, access, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, 0);
    if (handle == INVALID_HANDLE_VALUE) return false;
    *file = (u64)handle;
    return true;
#else
    u32 access = O_RDONLY;
    if (writeable) access = O_RDWR;
    int fd = open(fpath, access, 0777);
    if (fd == -1) return false;
    *file = fd;
    return true;
#endif // _WIN32
}

void ail_fs_close_file(u64 file)
{
#if AIL_OS_WIN32
    CloseHandle((void *)file);
#else
    close(file);
#endif // _WIN32
}

b32 ail_fs_read_n_bytes(u64 fd, void *buf, u64 maxN, u64 *actualN)
{
#if AIL_OS_WIN32
    *actualN = 0;
    void *file = (void *)fd;
    OVERLAPPED osReader = {0};
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osReader.hEvent == NULL) return false;

    // Issue Read operation
    DWORD nToRead = (DWORD) maxN;
    DWORD nRead = 0;
    i8 res = -1;
    if (!ReadFile(file, buf, nToRead, &nRead, &osReader)) {
        if(GetLastError() != ERROR_IO_PENDING) res = 0;
    } else {
        // Read completed immediately
        res = 1;
    }
    // We wait at most for AIL_FS_MAX_ATTEMPTS*AIL_FS_READ_TIMEOUT and return -1 otherwise
    for (u8 i = 0; i < AIL_FS_MAX_ATTEMPTS && res < 0; i++) {
        DWORD dwRes = WaitForSingleObject(osReader.hEvent, AIL_FS_READ_TIMEOUT);
        switch(dwRes) {
            case WAIT_OBJECT_0: {
                // Read completed.
                b32 succ = GetOverlappedResult(file, &osReader, &nRead, FALSE);
                res = succ ? 1 : 0;
            } break;
            case WAIT_TIMEOUT:
                // Gotta wait a little longer to finish operation
                break;
            default:
                // Error in the WaitForSingleObject
                // This indicates a problem with the OVERLAPPED structure's event handle.
                res = 0;
                break;
        }
    }
    CloseHandle(osReader.hEvent);
    *actualN = (u64) nRead;
    return res > 0;
#else
    *actualN = maxN;
    return read((int)fd, buf, maxN) != -1;
#endif // _WIN32
}

b32 ail_fs_read_file(const char *fpath, void *buf, u64 maxN, u64 *actualN)
{
    u64 file;
    b32 res = ail_fs_open_file(fpath, &file, false);
    if (res) res = ail_fs_read_n_bytes((u64)file, buf, maxN, actualN);
    ail_fs_close_file(file);
    return res;
}

char* ail_fs_read_entire_file(const char *fpath, u64 *size)
{
    // Adapted from https://stackoverflow.com/a/68156485/13764271
    char* buf = NULL;
    *size = 0;
    struct stat sb;
    if (stat(fpath, &sb) != -1) {
        if (sb.st_size) {
            buf = AIL_FS_MALLOC(sb.st_size);
            if (buf) {
                ail_fs_read_file(fpath, buf, sb.st_size, size);
            }
        }
    }
    return buf;
}

b32 ail_fs_write_n_bytes(u64 fd, const char *buf, u64 size)
{
#if AIL_OS_WIN32
    void *file = (void *)fd;
    OVERLAPPED osWrite = {0};
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osWrite.hEvent == NULL) return false;

    // Issue Write operation
    i8 res = -1;
    DWORD dwWritten;
    if (!WriteFile(file, buf, (DWORD)size, &dwWritten, &osWrite)) {
        if (GetLastError() != ERROR_IO_PENDING) res = 1;
    } else {
        // Write completed immediately.
        res = 1;
    }
    for (u32 i = 0; i < AIL_FS_MAX_ATTEMPTS && res < 0; i++) {
        DWORD dwRes = WaitForSingleObject(osWrite.hEvent, AIL_FS_WRITE_TIMEOUT);
        switch(dwRes) {
            case WAIT_OBJECT_0: {
                // Write completed
                b32 succ = GetOverlappedResult(file, &osWrite, &dwWritten, FALSE);
                res = succ ? 1 : 0;
            } break;
            case WAIT_TIMEOUT:
                // Gotta wait a little longer to finish operation
                break;
            default:
                // Error in the WaitForSingleObject
                // This indicates a problem with the OVERLAPPED structure's event handle.
                res = 1;
                break;
        }
    }
    CloseHandle(osWrite.hEvent);
    return res > 0 && dwWritten == size;
#else
    u64 written = 0;
    while (written < size) {
        int res = write(fd, &buf[written], size - written);
        if (res == -1) return false;
        written += res;
    }
    return true;
#endif
}

b32 ail_fs_write_file(const char *fpath, const char *buf, u64 size)
{
    u64 file;
    b32 res = ail_fs_open_file(fpath, &file, true);
    if (res) res = ail_fs_write_n_bytes((u64)file, buf, size);
    ail_fs_close_file(file);
    return res;
}

//////////////////
// Miscellanous //
//////////////////

AIL_DA(pchar) ail_fs_get_files_in_dir(const char *dirpath)
{
    AIL_UNUSED(dirpath);
    AIL_DA(pchar) files = ail_da_new_with_cap(pchar, 16);
    AIL_TODO();
    return files;
}

b32 ail_fs_dir_exists(const char *dirpath)
{
    struct stat sb;
    return stat(dirpath, &sb) == 0 && S_ISDIR(sb.st_mode);
}

const char *ail_fs_get_file_ext(const char *filename)
{
    u32 idx = 0;
    for (u32 i = 0; filename[i]; i++) {
        if (filename[i] == '.') idx = i + 1;
    }
    return &filename[idx];
}

b32 ail_fs_is_file_ext(const char *restrict filename, const char *restrict ext)
{
    const char *file_ext = ail_fs_get_file_ext(filename);
    return ail_fs_str_eq(file_ext, ext);
}

b32 ail_fs_is_file(const char *path)
{
    struct stat result = {0};
    stat(path, &result);
    return S_ISREG(result.st_mode);
}

#endif // _AIL_FS_IMPL_GUARD_
#endif // AIL_NO_FS_IMPL





////////
// From ail_hm.h
////////


// General Hashmap implementation
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

// @TODO: Improve Hashmap
// see for example Jai's hashmap implementation here: https://pastebin.com/xMUQXshn

#ifndef AIL_HM_H_
#define AIL_HM_H_


#ifndef AIL_HM_DEF
#ifdef  AIL_DEF
#define AIL_HM_DEF AIL_DEF
#else
#define AIL_HM_DEF
#endif // AIL_DEF
#endif // AIL_HM_DEF
#ifndef AIL_HM_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_HM_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_HM_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_HM_DEF_INLINE

#ifndef AIL_HM_MEMCPY
#ifdef  AIL_MEMCMPY
#define AIL_HM_MEMCPY AIL_MEMCMPY
#else
#include <string.h>
#define AIL_HM_MEMCPY(dst, src, n) memcpy(dst, src, n)
#endif
#endif

#ifndef AIL_HM_INIT_CAP
#define AIL_HM_INIT_CAP 256
#endif // AIL_HM_INIT_CAP

// @Note: Load factor should be specified per Hashmap maybe?
// @Note: Load factor is given in percent from 0 to 100
#ifndef AIL_HM_LOAD_FACTOR
#define AIL_HM_LOAD_FACTOR 80
#endif // AIL_HM_LOAD_FACTOR

typedef enum {
    AIL_HM_EMPTY         = 0, // 00
    AIL_HM_ONCE_OCCUPIED = 1, // 01
    AIL_HM_CUR_OCCUPIED  = 2, // 10
    AIL_HM_OCCUPIED      = 3, // 11
} AIL_HM_OCCUPATION;

AIL_HM_DEF u32 ail_hm_next_u32_2power(u32 x);

// @Note on Terminology: Box refers to an individual element in the list of elements in the hashmap
#define AIL_HM_KEY_VAL(K, V) AIL_HM_KEY_VAL_##K##_##V
#define AIL_HM_BOX(K, V) AIL_HM_BOX_##K##_##V
#define AIL_HM(K, V)     AIL_HM_##K##_##V
#define AIL_HM_INIT(K, V)                 \
    typedef struct AIL_HM_KEY_VAL(K, V) { \
        K key;                            \
        V val;                            \
    } AIL_HM_KEY_VAL(K, V);               \
    typedef struct AIL_HM_BOX(K, V) {     \
        K key;                            \
        V val;                            \
        AIL_HM_OCCUPATION occupied;       \
    } AIL_HM_BOX(K, V);                   \
    typedef struct AIL_HM(K, V) {         \
        AIL_HM_BOX(K, V) *data;           \
        u32 len;                          \
        u32 once_filled;                  \
        u32 cap;                          \
        u32(*hash)(K);                    \
        bool(*eq)(K, K);                  \
        AIL_Allocator *allocator;         \
    } AIL_HM(K, V)

#define ail_hm_from_parts(K, V, data, len, once_filled, cap, hashf, eqf, alPtr) (AIL_HM(K, V)) { (data), (len), (once_filled), (cap), (hashf), (eqf), (alPtr) }
#define ail_hm_new_with_alloc(K, V, c, hashf, eqf, alPtr) (AIL_HM(K, V)) { .data = AIL_CALL_CALLOC((*alPtr), ail_hm_next_u32_2power(c), sizeof(AIL_HM(K, V))), .len = 0, .once_filled = 0, .cap = (c), .hash = (hashf), .eq = (eqf), .allocator = (alPtr) }
#define ail_hm_new_with_cap(K, V, c, hashf, eqf) (AIL_HM(K, V)) { .data = AIL_CALL_CALLOC(ail_default_allocator, ail_hm_next_u32_2power(c), sizeof(AIL_HM(K, V))), .len = 0, .cap = (c), .hash = (hashf), .eq = (eqf), .allocator = &ail_default_allocator }
#define ail_hm_new(K, V, hashf, eqf) ail_hm_new_with_cap(K, V, AIL_HM_INIT_CAP, hashf, eqf)
#define ail_hm_new_empty(K, V, hashf, eqf) (AIL_HM(K, V)) { .data = NULL, .len = 0, .once_filled = 0, .cap = 0, .hash = (hashf), .eq = (eqf), .allocator = &ail_default_allocator }
#define ail_hm_free(hmPtr) do { AIL_CALL_FREE((*(hmPtr)->allocator), (hmPtr)->data); (hmPtr)->data = NULL; (hmPtr)->len = 0; (hmPtr)->cap = 0; } while(0)

// @TODO: Current probing stategy is simple and potentially inefficient. More efficient method (as used by jblow) requires that capacity is a power of 2 though
#if 0
#define ail_hm_probe_incr(idx, hash, cap) idx = ((idx) + 1 + ((hash) % ((cap) - 1))) % (cap) // This only works when capacity is a power of 2
#else
#define ail_hm_probe_incr(idx, hash, cap) idx = ((idx) + 1) % (cap)
#endif

// @Decide: Should we round the capacity up to the next power of 2? Alternatively we might get issues with our probing-strategy...
#define ail_hm_grow(hmPtr, newCap) do {                                                                                           \
        u32 _ail_hm_grow_new_cap_    =  (newCap); /* ail_hm_next_u32_2power(newCap); */                                           \
        u32 _ail_hm_grow_occ_offset_ = AIL_OFFSETOF(&(hmPtr)->data[0], occupied);                                                 \
        void *_ail_hm_grow_new_ptr_  = AIL_CALL_CALLOC((*(hmPtr)->allocator), _ail_hm_grow_new_cap_, sizeof(*((hmPtr)->data)));   \
        for (u32 _ail_hm_grow_i_ = 0; _ail_hm_grow_i_ < (hmPtr)->cap; _ail_hm_grow_i_++) {                                        \
            if ((hmPtr)->data[_ail_hm_grow_i_].occupied == AIL_HM_CUR_OCCUPIED) {                                                 \
                u32 _ail_hm_grow_hash_ = (hmPtr)->hash((hmPtr)->data[_ail_hm_grow_i_].key);                                       \
                u32 _ail_hm_grow_j_    = _ail_hm_grow_hash_ % _ail_hm_grow_new_cap_;                                              \
                char *_ail_hm_grow_tmp_ptr_ = &(((char *)_ail_hm_grow_new_ptr_)[_ail_hm_grow_j_ * sizeof(*((hmPtr)->data))]);     \
                AIL_HM_OCCUPATION _ail_hm_grow_occ_ = *((AIL_HM_OCCUPATION *)&(_ail_hm_grow_tmp_ptr_[_ail_hm_grow_occ_offset_])); \
                while (_ail_hm_grow_occ_ == AIL_HM_CUR_OCCUPIED) {                                                                \
                    ail_hm_probe_incr(_ail_hm_grow_j_, _ail_hm_grow_hash_, _ail_hm_grow_new_cap_);                                \
                    _ail_hm_grow_tmp_ptr_ = &(((char *)_ail_hm_grow_new_ptr_)[_ail_hm_grow_j_ * sizeof(*((hmPtr)->data))]);       \
                    _ail_hm_grow_occ_     = *((AIL_HM_OCCUPATION *)&(_ail_hm_grow_tmp_ptr_[_ail_hm_grow_occ_offset_]));           \
                }                                                                                                                 \
                AIL_HM_MEMCPY((void *)_ail_hm_grow_tmp_ptr_, (void *)&(hmPtr)->data[_ail_hm_grow_i_], sizeof(*((hmPtr)->data)));  \
            }                                                                                                                     \
        }                                                                                                                         \
        if ((hmPtr)->data) AIL_CALL_FREE((*(hmPtr)->allocator), (hmPtr)->data);                                                   \
        (hmPtr)->cap  = _ail_hm_grow_new_cap_;                                                                                    \
        (hmPtr)->data = _ail_hm_grow_new_ptr_;                                                                                    \
    } while(0)

#define ail_hm_maybe_grow(hmPtr, toAdd) do {                                               \
        if (((hmPtr)->len + (toAdd))*100 >= (hmPtr)->cap*AIL_HM_LOAD_FACTOR) {             \
            ail_hm_grow(hmPtr, 2*((hmPtr)->cap + 1));                                      \
            /* ail_hm_grow(hmPtr, ((hmPtr)->len + (toAdd) + 1)*100/AIL_HM_LOAD_FACTOR); */ \
        }                                                                                  \
    } while(0)

#define ail_hm_get_idx(hmPtr, k, outIdx, outFound) do {                                               \
            (outFound)         = false;                                                               \
        if ((hmPtr)->cap == 0) break; /* Necessary, bc mod 0 is undefined */                          \
        u32 _ail_hm_get_hash_  = (hmPtr)->hash((k));                                                  \
        u32 _ail_hm_get_idx_   = _ail_hm_get_hash_ % (hmPtr)->cap;                                    \
        for (u32 _ail_hm_get_count_ = 0; _ail_hm_get_count_ < (hmPtr)->len; _ail_hm_get_count_++) {   \
            if (((hmPtr)->data[_ail_hm_get_idx_].occupied & AIL_HM_OCCUPIED) == 0) break;             \
            if (((hmPtr))->eq(((hmPtr))->data[_ail_hm_get_idx_].key, (k))) {                          \
                (outIdx)   = _ail_hm_get_idx_;                                                        \
                (outFound) = true;                                                                    \
                break;                                                                                \
            }                                                                                         \
            ail_hm_probe_incr(_ail_hm_get_idx_, _ail_hm_get_hash_, (hmPtr)->cap);                     \
        }                                                                                             \
    } while(0)

#define ail_hm_get_ptr(hmPtr, k, outPtr) do {                                            \
        bool _ail_hm_get_ptr_found_;                                                     \
        u32  _ail_hm_get_ptr_idx_;                                                       \
        ail_hm_get_idx(hmPtr, k, _ail_hm_get_ptr_idx_, _ail_hm_get_ptr_found_);          \
        if (_ail_hm_get_ptr_found_) outPtr = &((hmPtr)->data[_ail_hm_get_ptr_idx_].val); \
        else outPtr = 0;                                                                 \
    } while(0)

#define ail_hm_get_val(hmPtr, k, outVal, outFound) do {                   \
        u32  _ail_hm_get_val_idx_;                                        \
        ail_hm_get_idx(hmPtr, k, _ail_hm_get_val_idx_, outFound);         \
        if ((outFound)) outVal = (hmPtr)->data[_ail_hm_get_val_idx_].val; \
    } while(0)

#define ail_hm_put(hmPtr, k, v) do {                                                                                                     \
        ail_hm_maybe_grow(hmPtr, 1);                                                                                                     \
        u32 _ail_hm_put_hash_ = (hmPtr)->hash((k));                                                                                      \
        u32 _ail_hm_put_idx_  = _ail_hm_put_hash_ % (hmPtr)->cap;                                                                        \
        u32 _ail_hm_put_once_filled_idx_;                                                                                                \
        u32 _ail_hm_put_found_once_filled_idx_ = false;                                                                                  \
        for (u32 _ail_hm_put_count_ = 0; _ail_hm_put_count_ < (hmPtr)->len &&                                                            \
            ((hmPtr)->data[_ail_hm_put_idx_].occupied & AIL_HM_OCCUPIED) > 0; _ail_hm_put_count_++) {                                    \
            if ((hmPtr)->eq((hmPtr)->data[_ail_hm_put_idx_].key, (k))) {                                                                 \
                _ail_hm_put_found_once_filled_idx_ = false;                                                                              \
                break;                                                                                                                   \
            }                                                                                                                            \
            if (AIL_UNLIKELY(!_ail_hm_put_found_once_filled_idx_ && (hmPtr)->data[_ail_hm_put_idx_].occupied == AIL_HM_ONCE_OCCUPIED)) { \
                _ail_hm_put_once_filled_idx_       = _ail_hm_put_idx_;                                                                   \
                _ail_hm_put_found_once_filled_idx_ = true;                                                                               \
            }                                                                                                                            \
            ail_hm_probe_incr(_ail_hm_put_idx_, _ail_hm_put_hash_, ((hmPtr))->cap);                                                      \
        }                                                                                                                                \
        if (_ail_hm_put_found_once_filled_idx_) _ail_hm_put_idx_ = _ail_hm_put_once_filled_idx_;                                         \
        if ((hmPtr)->data[_ail_hm_put_idx_].occupied != AIL_HM_CUR_OCCUPIED) {                                                           \
            (hmPtr)->len++;                                                                                                              \
            if ((hmPtr)->data[_ail_hm_put_idx_].occupied != AIL_HM_ONCE_OCCUPIED) (hmPtr)->once_filled++;                                \
        }                                                                                                                                \
        (hmPtr)->data[_ail_hm_put_idx_].key = (k);                                                                                       \
        (hmPtr)->data[_ail_hm_put_idx_].val = (v);                                                                                       \
        (hmPtr)->data[_ail_hm_put_idx_].occupied = AIL_HM_CUR_OCCUPIED;                                                                  \
    } while(0)

// @TODO
#define ail_hm_rm(hmPtr, k) do { \
    } while(0)

#endif // AIL_HM_H_


#if !defined(AIL_NO_HM_IMPL) && !defined(AIL_NO_IMPL)
#ifndef AIL_HM_IMPL_GUARD
#define AIL_HM_IMPL_GUARD

AIL_HM_DEF u32 ail_hm_next_u32_2power(u32 x)
{
    AIL_NEXT_2POWER(x, x);
    return x;
}

#endif // AIL_HM_IMPL_GUARD
#endif // AIL_NO_HM_IMPL





////////
// From ail_math.h
////////


// Common Math functions
//
// Heavily inspired by the following libraries:
// - HandmadeMath (https://github.com/HandmadeMath/HandmadeMath)
// - la (https://github.com/tsoding/la)
// - Raylib (https://github.com/raysan5/raylib/blob/master/src/raymath.h)
// - mathc (https://github.com/felselva/mathc)
// - fastmod (https://github.com/lemire/fastmod)
// - tiny-fixedpoint (https://github.com/kokke/tiny-fixedpoint-c)
// -
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

#ifndef AIL_MATH_H_


#ifndef AIL_MATH_DEF
#ifdef  AIL_DEF
#define AIL_MATH_DEF AIL_DEF
#else
#define AIL_MATH_DEF
#endif // AIL_DEF
#endif // AIL_MATH_DEF
#ifndef AIL_MATH_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_MATH_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_MATH_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_MATH_DEF_INLINE


///////////////////////////////////
// //
//      Function Overloding      //
// //
///////////////////////////////////

// @Important: These size definitions are not necessarily correct, but if you want proper type inference, you should just use properly sized types anyways
#define _AIL_MATH_GENERIC_1(pre, post, arg0, ...) _Generic((arg0), \
    unsigned char:          AIL_CONCAT(pre, u8, post),  \
    signed char:            AIL_CONCAT(pre, i8, post),  \
    unsigned short:         AIL_CONCAT(pre, u16, post), \
    signed short:           AIL_CONCAT(pre, i16, post), \
    unsigned int:           AIL_CONCAT(pre, u32, post), \
    signed int:             AIL_CONCAT(pre, i32, post), \
    unsigned long int:      AIL_CONCAT(pre, u64, post), \
    signed long int:        AIL_CONCAT(pre, i64, post), \
    unsigned long long int: AIL_CONCAT(pre, u64, post), \
    signed long long int:   AIL_CONCAT(pre, i64, post)  \
)

#define AIL_MATH_GENERIC(pre, post, arg0, ...) _Generic((arg0), \
    u8:  AIL_CONCAT(pre, u8, post),  \
    i8:  AIL_CONCAT(pre, i8, post),  \
    u16: AIL_CONCAT(pre, u16, post), \
    i16: AIL_CONCAT(pre, i16, post), \
    u32: AIL_CONCAT(pre, u32, post), \
    i32: AIL_CONCAT(pre, i32, post), \
    u64: AIL_CONCAT(pre, u64, post), \
    i64: AIL_CONCAT(pre, i64, post), \
    f32: AIL_CONCAT(pre, f32, post), \
    f64: AIL_CONCAT(pre, f64, post), \
    default: _AIL_MATH_GENERIC_1(pre, post, arg0, __VA_ARGS__) \
) ((arg0), __VA_ARGS__)

#define _AIL_MATH_GENERIC_BINOP_4(pre, post, arg0, arg1) AIL_MATH_GENERIC(pre, post, arg0, arg1)
#define _AIL_MATH_GENERIC_BINOP_4(pre, post, arg0, arg1) AIL_MATH_GENERIC(pre, post, arg0, arg1)
#define AIL_MATH_GENERIC_BINOP(pre, post, arg0, arg1, ...) AIL_VFUNC(_AIL_MATH_GENERIC_BINOP_, pre, post, arg0, arg1, __VA_ARGS__)


//////////////////////
//  Linear Algebra  //
//////////////////////

#define ail_vec2(x, y) AIL_MATH_GENERIC(ail_vec2, , x, y)
#define ail_vec3(x, y, z) AIL_MATH_GENERIC(ail_vec3, , x, y, z)
#define ail_vec4(x, y, z, w) AIL_MATH_GENERIC(ail_vec4, , x, y, z, w)

#define ail_vec2_add(a, b, ...) AIL_MATH_GENERIC(ail_vec2, _add, a, b, __VA_ARGS__)
#define ail_vec3_add(a, b, ...) AIL_MATH_GENERIC(ail_vec3, _add, a, b, __VA_ARGS__)
#define ail_vec4_add(a, b, ...) AIL_MATH_GENERIC(ail_vec4, _add, a, b, __VA_ARGS__)

#define ail_vec2_sub(a, b, ...) AIL_MATH_GENERIC(ail_vec2, _sub, a, b, __VA_ARGS__)
#define ail_vec3_sub(a, b, ...) AIL_MATH_GENERIC(ail_vec3, _sub, a, b, __VA_ARGS__)
#define ail_vec4_sub(a, b, ...) AIL_MATH_GENERIC(ail_vec4, _sub, a, b, __VA_ARGS__)

#define ail_vec2_eq(a, b) AIL_MATH_GENERIC(ail_vec2, _eq, a, b)
#define ail_vec3_eq(a, b) AIL_MATH_GENERIC(ail_vec3, _eq, a, b)
#define ail_vec4_eq(a, b) AIL_MATH_GENERIC(ail_vec4, _eq, a, b)

#define ail_vec2_dot(a, b) AIL_MATH_GENERIC(ail_vec2, _dot, a, b)
#define ail_vec3_dot(a, b) AIL_MATH_GENERIC(ail_vec3, _dot, a, b)
#define ail_vec4_dot(a, b) AIL_MATH_GENERIC(ail_vec4, _dot, a, b)

#define ail_mat2_add(a, b) AIL_MATH_GENERIC(ail_mat2, _add, a, b)
#define ail_mat3_add(a, b) AIL_MATH_GENERIC(ail_mat3, _add, a, b)
#define ail_mat4_add(a, b) AIL_MATH_GENERIC(ail_mat4, _add, a, b)

#define ail_mat2_sub(a, b) AIL_MATH_GENERIC(ail_mat2, _sub, a, b)
#define ail_mat3_sub(a, b) AIL_MATH_GENERIC(ail_mat3, _sub, a, b)
#define ail_mat4_sub(a, b) AIL_MATH_GENERIC(ail_mat4, _sub, a, b)



////////////////////////////////
// //
//      Type Definitions      //
// //
////////////////////////////////


//////////////////////
//  Linear Algebra  //
//////////////////////

typedef union AIL_Vec2u8 {
    u8 els[2];
    struct { u8 x, y; };
    struct { u8 r, g; };
    struct { u8 u, v; };
    struct { u8 width, height; };
} AIL_Vec2u8;

typedef union AIL_Vec2i8 {
    i8 els[2];
    struct { i8 x, y; };
    struct { i8 r, g; };
    struct { i8 u, v; };
    struct { i8 width, height; };
} AIL_Vec2i8;

typedef union AIL_Vec2u16 {
    u16 els[2];
    struct { u16 x, y; };
    struct { u16 r, g; };
    struct { u16 u, v; };
    struct { u16 width, height; };
} AIL_Vec2u16;

typedef union AIL_Vec2i16 {
    i16 els[2];
    struct { i16 x, y; };
    struct { i16 r, g; };
    struct { i16 u, v; };
    struct { i16 width, height; };
} AIL_Vec2i16;

typedef union AIL_Vec2u32 {
    u32 els[2];
    struct { u32 x, y; };
    struct { u32 r, g; };
    struct { u32 u, v; };
    struct { u32 width, height; };
} AIL_Vec2u32;

typedef union AIL_Vec2i32 {
    i32 els[2];
    struct { i32 x, y; };
    struct { i32 r, g; };
    struct { i32 u, v; };
    struct { i32 width, height; };
} AIL_Vec2i32;

typedef union AIL_Vec2u64 {
    u64 els[2];
    struct { u64 x, y; };
    struct { u64 r, g; };
    struct { u64 u, v; };
    struct { u64 width, height; };
} AIL_Vec2u64;

typedef union AIL_Vec2i64 {
    i64 els[2];
    struct { i64 x, y; };
    struct { i64 r, g; };
    struct { i64 u, v; };
    struct { i64 width, height; };
} AIL_Vec2i64;

typedef union AIL_Vec2f32 {
    f32 els[2];
    struct { f32 x, y; };
    struct { f32 r, g; };
    struct { f32 u, v; };
    struct { f32 width, height; };
} AIL_Vec2f32;

typedef union AIL_Vec2f64 {
    f64 els[2];
    struct { f64 x, y; };
    struct { f64 r, g; };
    struct { f64 u, v; };
    struct { f64 width, height; };
} AIL_Vec2f64;

typedef union AIL_Vec3u8 {
    u8 els[3];
    struct { u8 x, y, z; };
    struct { u8 r, g, b; };
    struct { u8 u, v, w; };
    struct { AIL_Vec2u8 xy; u8 _0; };
    struct { u8 _1; AIL_Vec2u8 yz; };
    struct { AIL_Vec2u8 rg; u8 _2; };
    struct { u8 _3; AIL_Vec2u8 gb; };
    struct { AIL_Vec2u8 uv; u8 _4; };
    struct { u8 _5; AIL_Vec2u8 vw; };
} AIL_Vec3u8;

typedef union AIL_Vec3i8 {
    i8 els[3];
    struct { i8 x, y, z; };
    struct { i8 r, g, b; };
    struct { i8 u, v, w; };
    struct { AIL_Vec2i8 xy; i8 _0; };
    struct { i8 _1; AIL_Vec2i8 yz; };
    struct { AIL_Vec2i8 rg; i8 _2; };
    struct { i8 _3; AIL_Vec2i8 gb; };
    struct { AIL_Vec2i8 uv; i8 _4; };
    struct { i8 _5; AIL_Vec2i8 vw; };
} AIL_Vec3i8;

typedef union AIL_Vec3u16 {
    u16 els[3];
    struct { u16 x, y, z; };
    struct { u16 r, g, b; };
    struct { u16 u, v, w; };
    struct { AIL_Vec2u16 xy; u16 _0; };
    struct { u16 _1; AIL_Vec2u16 yz; };
    struct { AIL_Vec2u16 rg; u16 _2; };
    struct { u16 _3; AIL_Vec2u16 gb; };
    struct { AIL_Vec2u16 uv; u16 _4; };
    struct { u16 _5; AIL_Vec2u16 vw; };
} AIL_Vec3u16;

typedef union AIL_Vec3i16 {
    i16 els[3];
    struct { i16 x, y, z; };
    struct { i16 r, g, b; };
    struct { i16 u, v, w; };
    struct { AIL_Vec2i16 xy; i16 _0; };
    struct { i16 _1; AIL_Vec2i16 yz; };
    struct { AIL_Vec2i16 rg; i16 _2; };
    struct { i16 _3; AIL_Vec2i16 gb; };
    struct { AIL_Vec2i16 uv; i16 _4; };
    struct { i16 _5; AIL_Vec2i16 vw; };
} AIL_Vec3i16;

typedef union AIL_Vec3u32 {
    u32 els[3];
    struct { u32 x, y, z; };
    struct { u32 r, g, b; };
    struct { u32 u, v, w; };
    struct { AIL_Vec2u32 xy; u32 _0; };
    struct { u32 _1; AIL_Vec2u32 yz; };
    struct { AIL_Vec2u32 rg; u32 _2; };
    struct { u32 _3; AIL_Vec2u32 gb; };
    struct { AIL_Vec2u32 uv; u32 _4; };
    struct { u32 _5; AIL_Vec2u32 vw; };
} AIL_Vec3u32;

typedef union AIL_Vec3i32 {
    i32 els[3];
    struct { i32 x, y, z; };
    struct { i32 r, g, b; };
    struct { i32 u, v, w; };
    struct { AIL_Vec2i32 xy; i32 _0; };
    struct { i32 _1; AIL_Vec2i32 yz; };
    struct { AIL_Vec2i32 rg; i32 _2; };
    struct { i32 _3; AIL_Vec2i32 gb; };
    struct { AIL_Vec2i32 uv; i32 _4; };
    struct { i32 _5; AIL_Vec2i32 vw; };
} AIL_Vec3i32;

typedef union AIL_Vec3u64 {
    u64 els[3];
    struct { u64 x, y, z; };
    struct { u64 r, g, b; };
    struct { u64 u, v, w; };
    struct { AIL_Vec2u64 xy; u64 _0; };
    struct { u64 _1; AIL_Vec2u64 yz; };
    struct { AIL_Vec2u64 rg; u64 _2; };
    struct { u64 _3; AIL_Vec2u64 gb; };
    struct { AIL_Vec2u64 uv; u64 _4; };
    struct { u64 _5; AIL_Vec2u64 vw; };
} AIL_Vec3u64;

typedef union AIL_Vec3i64 {
    i64 els[3];
    struct { i64 x, y, z; };
    struct { i64 r, g, b; };
    struct { i64 u, v, w; };
    struct { AIL_Vec2i64 xy; i64 _0; };
    struct { i64 _1; AIL_Vec2i64 yz; };
    struct { AIL_Vec2i64 rg; i64 _2; };
    struct { i64 _3; AIL_Vec2i64 gb; };
    struct { AIL_Vec2i64 uv; i64 _4; };
    struct { i64 _5; AIL_Vec2i64 vw; };
} AIL_Vec3i64;

typedef union AIL_Vec3f32 {
    f32 els[3];
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };
    struct { f32 u, v, w; };
    struct { AIL_Vec2f32 xy; f32 _0; };
    struct { f32 _1; AIL_Vec2f32 yz; };
    struct { AIL_Vec2f32 rg; f32 _2; };
    struct { f32 _3; AIL_Vec2f32 gb; };
    struct { AIL_Vec2f32 uv; f32 _4; };
    struct { f32 _5; AIL_Vec2f32 vw; };
} AIL_Vec3f32;

typedef union AIL_Vec3f64 {
    f64 els[3];
    struct { f64 x, y, z; };
    struct { f64 r, g, b; };
    struct { f64 u, v, w; };
    struct { AIL_Vec2f64 xy; f64 _0; };
    struct { f64 _1; AIL_Vec2f64 yz; };
    struct { AIL_Vec2f64 rg; f64 _2; };
    struct { f64 _3; AIL_Vec2f64 gb; };
    struct { AIL_Vec2f64 uv; f64 _4; };
    struct { f64 _5; AIL_Vec2f64 vw; };
} AIL_Vec3f64;

typedef union AIL_Vec4u8 {
    u8 els[4];
    struct { u8 x, y, z, w; };
    struct { u8 r, g, b, a; };
    struct { AIL_Vec3u8 xyz; u8 _0; };
    struct { u8 _1; AIL_Vec3u8 yzw; };
    struct { AIL_Vec2u8 xy; u8 _2[2]; };
    struct { u8 _3; AIL_Vec2u8 yz; u8 _4; };
    struct { u8 _5[2]; AIL_Vec2u8 zw; };
    struct { AIL_Vec3u8 rgb; u8 _6; };
    struct { u8 _7; AIL_Vec3u8 gba; };
    struct { AIL_Vec2u8 rg; u8 _8[2]; };
    struct { u8 _9; AIL_Vec2u8 gb; u8 _10; };
    struct { u8 _11[2]; AIL_Vec2u8 ba; };
} AIL_Vec4u8;

typedef union AIL_Vec4i8 {
    i8 els[4];
    struct { i8 x, y, z, w; };
    struct { i8 r, g, b, a; };
    struct { AIL_Vec3i8 xyz; i8 _0; };
    struct { i8 _1; AIL_Vec3i8 yzw; };
    struct { AIL_Vec2i8 xy; i8 _2[2]; };
    struct { i8 _3; AIL_Vec2i8 yz; i8 _4; };
    struct { i8 _5[2]; AIL_Vec2i8 zw; };
    struct { AIL_Vec3i8 rgb; i8 _6; };
    struct { i8 _7; AIL_Vec3i8 gba; };
    struct { AIL_Vec2i8 rg; i8 _8[2]; };
    struct { i8 _9; AIL_Vec2i8 gb; i8 _10; };
    struct { i8 _11[2]; AIL_Vec2i8 ba; };
} AIL_Vec4i8;

typedef union AIL_Vec4u16 {
    u16 els[4];
    struct { u16 x, y, z, w; };
    struct { u16 r, g, b, a; };
    struct { AIL_Vec3u16 xyz; u16 _0; };
    struct { u16 _1; AIL_Vec3u16 yzw; };
    struct { AIL_Vec2u16 xy; u16 _2[2]; };
    struct { u16 _3; AIL_Vec2u16 yz; u16 _4; };
    struct { u16 _5[2]; AIL_Vec2u16 zw; };
    struct { AIL_Vec3u16 rgb; u16 _6; };
    struct { u16 _7; AIL_Vec3u16 gba; };
    struct { AIL_Vec2u16 rg; u16 _8[2]; };
    struct { u16 _9; AIL_Vec2u16 gb; u16 _10; };
    struct { u16 _11[2]; AIL_Vec2u16 ba; };
} AIL_Vec4u16;

typedef union AIL_Vec4i16 {
    i16 els[4];
    struct { i16 x, y, z, w; };
    struct { i16 r, g, b, a; };
    struct { AIL_Vec3i16 xyz; i16 _0; };
    struct { i16 _1; AIL_Vec3i16 yzw; };
    struct { AIL_Vec2i16 xy; i16 _2[2]; };
    struct { i16 _3; AIL_Vec2i16 yz; i16 _4; };
    struct { i16 _5[2]; AIL_Vec2i16 zw; };
    struct { AIL_Vec3i16 rgb; i16 _6; };
    struct { i16 _7; AIL_Vec3i16 gba; };
    struct { AIL_Vec2i16 rg; i16 _8[2]; };
    struct { i16 _9; AIL_Vec2i16 gb; i16 _10; };
    struct { i16 _11[2]; AIL_Vec2i16 ba; };
} AIL_Vec4i16;

typedef union AIL_Vec4u32 {
    u32 els[4];
    struct { u32 x, y, z, w; };
    struct { u32 r, g, b, a; };
    struct { AIL_Vec3u32 xyz; u32 _0; };
    struct { u32 _1; AIL_Vec3u32 yzw; };
    struct { AIL_Vec2u32 xy; u32 _2[2]; };
    struct { u32 _3; AIL_Vec2u32 yz; u32 _4; };
    struct { u32 _5[2]; AIL_Vec2u32 zw; };
    struct { AIL_Vec3u32 rgb; u32 _6; };
    struct { u32 _7; AIL_Vec3u32 gba; };
    struct { AIL_Vec2u32 rg; u32 _8[2]; };
    struct { u32 _9; AIL_Vec2u32 gb; u32 _10; };
    struct { u32 _11[2]; AIL_Vec2u32 ba; };
} AIL_Vec4u32;

typedef union AIL_Vec4i32 {
    i32 els[4];
    struct { i32 x, y, z, w; };
    struct { i32 r, g, b, a; };
    struct { AIL_Vec3i32 xyz; i32 _0; };
    struct { i32 _1; AIL_Vec3i32 yzw; };
    struct { AIL_Vec2i32 xy; i32 _2[2]; };
    struct { i32 _3; AIL_Vec2i32 yz; i32 _4; };
    struct { i32 _5[2]; AIL_Vec2i32 zw; };
    struct { AIL_Vec3i32 rgb; i32 _6; };
    struct { i32 _7; AIL_Vec3i32 gba; };
    struct { AIL_Vec2i32 rg; i32 _8[2]; };
    struct { i32 _9; AIL_Vec2i32 gb; i32 _10; };
    struct { i32 _11[2]; AIL_Vec2i32 ba; };
} AIL_Vec4i32;

typedef union AIL_Vec4u64 {
    u64 els[4];
    struct { u64 x, y, z, w; };
    struct { u64 r, g, b, a; };
    struct { AIL_Vec3u64 xyz; u64 _0; };
    struct { u64 _1; AIL_Vec3u64 yzw; };
    struct { AIL_Vec2u64 xy; u64 _2[2]; };
    struct { u64 _3; AIL_Vec2u64 yz; u64 _4; };
    struct { u64 _5[2]; AIL_Vec2u64 zw; };
    struct { AIL_Vec3u64 rgb; u64 _6; };
    struct { u64 _7; AIL_Vec3u64 gba; };
    struct { AIL_Vec2u64 rg; u64 _8[2]; };
    struct { u64 _9; AIL_Vec2u64 gb; u64 _10; };
    struct { u64 _11[2]; AIL_Vec2u64 ba; };
} AIL_Vec4u64;

typedef union AIL_Vec4i64 {
    i64 els[4];
    struct { i64 x, y, z, w; };
    struct { i64 r, g, b, a; };
    struct { AIL_Vec3i64 xyz; i64 _0; };
    struct { i64 _1; AIL_Vec3i64 yzw; };
    struct { AIL_Vec2i64 xy; i64 _2[2]; };
    struct { i64 _3; AIL_Vec2i64 yz; i64 _4; };
    struct { i64 _5[2]; AIL_Vec2i64 zw; };
    struct { AIL_Vec3i64 rgb; i64 _6; };
    struct { i64 _7; AIL_Vec3i64 gba; };
    struct { AIL_Vec2i64 rg; i64 _8[2]; };
    struct { i64 _9; AIL_Vec2i64 gb; i64 _10; };
    struct { i64 _11[2]; AIL_Vec2i64 ba; };
} AIL_Vec4i64;

typedef union AIL_Vec4f32 {
    f32 els[4];
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };
    struct { AIL_Vec3f32 xyz; f32 _0; };
    struct { f32 _1; AIL_Vec3f32 yzw; };
    struct { AIL_Vec2f32 xy; f32 _2[2]; };
    struct { f32 _3; AIL_Vec2f32 yz; f32 _4; };
    struct { f32 _5[2]; AIL_Vec2f32 zw; };
    struct { AIL_Vec3f32 rgb; f32 _6; };
    struct { f32 _7; AIL_Vec3f32 gba; };
    struct { AIL_Vec2f32 rg; f32 _8[2]; };
    struct { f32 _9; AIL_Vec2f32 gb; f32 _10; };
    struct { f32 _11[2]; AIL_Vec2f32 ba; };
} AIL_Vec4f32;

typedef union AIL_Vec4f64 {
    f64 els[4];
    struct { f64 x, y, z, w; };
    struct { f64 r, g, b, a; };
    struct { AIL_Vec3f64 xyz; f64 _0; };
    struct { f64 _1; AIL_Vec3f64 yzw; };
    struct { AIL_Vec2f64 xy; f64 _2[2]; };
    struct { f64 _3; AIL_Vec2f64 yz; f64 _4; };
    struct { f64 _5[2]; AIL_Vec2f64 zw; };
    struct { AIL_Vec3f64 rgb; f64 _6; };
    struct { f64 _7; AIL_Vec3f64 gba; };
    struct { AIL_Vec2f64 rg; f64 _8[2]; };
    struct { f64 _9; AIL_Vec2f64 gb; f64 _10; };
    struct { f64 _11[2]; AIL_Vec2f64 ba; };
} AIL_Vec4f64;

typedef union AIL_Mat2u8 {
    u8 els[2][2];
    AIL_Vec2u8 cols[2];
    struct { u8 x11, x21, x12, x22; };
} AIL_Mat2u8;

typedef union AIL_Mat2i8 {
    i8 els[2][2];
    AIL_Vec2i8 cols[2];
    struct { i8 x11, x21, x12, x22; };
} AIL_Mat2i8;

typedef union AIL_Mat2u16 {
    u16 els[2][2];
    AIL_Vec2u16 cols[2];
    struct { u16 x11, x21, x12, x22; };
} AIL_Mat2u16;

typedef union AIL_Mat2i16 {
    i16 els[2][2];
    AIL_Vec2i16 cols[2];
    struct { i16 x11, x21, x12, x22; };
} AIL_Mat2i16;

typedef union AIL_Mat2u32 {
    u32 els[2][2];
    AIL_Vec2u32 cols[2];
    struct { u32 x11, x21, x12, x22; };
} AIL_Mat2u32;

typedef union AIL_Mat2i32 {
    i32 els[2][2];
    AIL_Vec2i32 cols[2];
    struct { i32 x11, x21, x12, x22; };
} AIL_Mat2i32;

typedef union AIL_Mat2u64 {
    u64 els[2][2];
    AIL_Vec2u64 cols[2];
    struct { u64 x11, x21, x12, x22; };
} AIL_Mat2u64;

typedef union AIL_Mat2i64 {
    i64 els[2][2];
    AIL_Vec2i64 cols[2];
    struct { i64 x11, x21, x12, x22; };
} AIL_Mat2i64;

typedef union AIL_Mat2f32 {
    f32 els[2][2];
    AIL_Vec2f32 cols[2];
    struct { f32 x11, x21, x12, x22; };
} AIL_Mat2f32;

typedef union AIL_Mat2f64 {
    f64 els[2][2];
    AIL_Vec2f64 cols[2];
    struct { f64 x11, x21, x12, x22; };
} AIL_Mat2f64;

typedef union AIL_Mat3u8 {
    u8 els[3][3];
    AIL_Vec3u8 cols[3];
    struct { u8 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3u8;

typedef union AIL_Mat3i8 {
    i8 els[3][3];
    AIL_Vec3i8 cols[3];
    struct { i8 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3i8;

typedef union AIL_Mat3u16 {
    u16 els[3][3];
    AIL_Vec3u16 cols[3];
    struct { u16 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3u16;

typedef union AIL_Mat3i16 {
    i16 els[3][3];
    AIL_Vec3i16 cols[3];
    struct { i16 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3i16;

typedef union AIL_Mat3u32 {
    u32 els[3][3];
    AIL_Vec3u32 cols[3];
    struct { u32 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3u32;

typedef union AIL_Mat3i32 {
    i32 els[3][3];
    AIL_Vec3i32 cols[3];
    struct { i32 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3i32;

typedef union AIL_Mat3u64 {
    u64 els[3][3];
    AIL_Vec3u64 cols[3];
    struct { u64 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3u64;

typedef union AIL_Mat3i64 {
    i64 els[3][3];
    AIL_Vec3i64 cols[3];
    struct { i64 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3i64;

typedef union AIL_Mat3f32 {
    f32 els[3][3];
    AIL_Vec3f32 cols[3];
    struct { f32 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3f32;

typedef union AIL_Mat3f64 {
    f64 els[3][3];
    AIL_Vec3f64 cols[3];
    struct { f64 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3f64;

typedef union AIL_Mat4u8 {
    u8 els[4][4];
    AIL_Vec4u8 cols[4];
    struct { u8 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4u8;

typedef union AIL_Mat4i8 {
    i8 els[4][4];
    AIL_Vec4i8 cols[4];
    struct { i8 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4i8;

typedef union AIL_Mat4u16 {
    u16 els[4][4];
    AIL_Vec4u16 cols[4];
    struct { u16 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4u16;

typedef union AIL_Mat4i16 {
    i16 els[4][4];
    AIL_Vec4i16 cols[4];
    struct { i16 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4i16;

typedef union AIL_Mat4u32 {
    u32 els[4][4];
    AIL_Vec4u32 cols[4];
    struct { u32 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4u32;

typedef union AIL_Mat4i32 {
    i32 els[4][4];
    AIL_Vec4i32 cols[4];
    struct { i32 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4i32;

typedef union AIL_Mat4u64 {
    u64 els[4][4];
    AIL_Vec4u64 cols[4];
    struct { u64 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4u64;

typedef union AIL_Mat4i64 {
    i64 els[4][4];
    AIL_Vec4i64 cols[4];
    struct { i64 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4i64;

typedef union AIL_Mat4f32 {
    f32 els[4][4];
    AIL_Vec4f32 cols[4];
    struct { f32 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4f32;

typedef union AIL_Mat4f64 {
    f64 els[4][4];
    AIL_Vec4f64 cols[4];
    struct { f64 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4f64;



/////////////////////////////////////
// //
//      Function Declarations      //
// //
/////////////////////////////////////


//////////////////////
//  Linear Algebra  //
//////////////////////

AIL_Vec2u8 ail_vec2u8(u8 x, u8 y);
AIL_Vec2i8 ail_vec2i8(i8 x, i8 y);
AIL_Vec2u16 ail_vec2u16(u16 x, u16 y);
AIL_Vec2i16 ail_vec2i16(i16 x, i16 y);
AIL_Vec2u32 ail_vec2u32(u32 x, u32 y);
AIL_Vec2i32 ail_vec2i32(i32 x, i32 y);
AIL_Vec2u64 ail_vec2u64(u64 x, u64 y);
AIL_Vec2i64 ail_vec2i64(i64 x, i64 y);
AIL_Vec2f32 ail_vec2f32(f32 x, f32 y);
AIL_Vec2f64 ail_vec2f64(f64 x, f64 y);
AIL_Vec3u8 ail_vec3u8(u8 x, u8 y, u8 z);
AIL_Vec3i8 ail_vec3i8(i8 x, i8 y, i8 z);
AIL_Vec3u16 ail_vec3u16(u16 x, u16 y, u16 z);
AIL_Vec3i16 ail_vec3i16(i16 x, i16 y, i16 z);
AIL_Vec3u32 ail_vec3u32(u32 x, u32 y, u32 z);
AIL_Vec3i32 ail_vec3i32(i32 x, i32 y, i32 z);
AIL_Vec3u64 ail_vec3u64(u64 x, u64 y, u64 z);
AIL_Vec3i64 ail_vec3i64(i64 x, i64 y, i64 z);
AIL_Vec3f32 ail_vec3f32(f32 x, f32 y, f32 z);
AIL_Vec3f64 ail_vec3f64(f64 x, f64 y, f64 z);
AIL_Vec4u8 ail_vec4u8(u8 x, u8 y, u8 z, u8 w);
AIL_Vec4i8 ail_vec4i8(i8 x, i8 y, i8 z, i8 w);
AIL_Vec4u16 ail_vec4u16(u16 x, u16 y, u16 z, u16 w);
AIL_Vec4i16 ail_vec4i16(i16 x, i16 y, i16 z, i16 w);
AIL_Vec4u32 ail_vec4u32(u32 x, u32 y, u32 z, u32 w);
AIL_Vec4i32 ail_vec4i32(i32 x, i32 y, i32 z, i32 w);
AIL_Vec4u64 ail_vec4u64(u64 x, u64 y, u64 z, u64 w);
AIL_Vec4i64 ail_vec4i64(i64 x, i64 y, i64 z, i64 w);
AIL_Vec4f32 ail_vec4f32(f32 x, f32 y, f32 z, f32 w);
AIL_Vec4f64 ail_vec4f64(f64 x, f64 y, f64 z, f64 w);

AIL_Vec2u8 ail_vec2u8_add(AIL_Vec2u8 a, AIL_Vec2u8 b);
AIL_Vec2i8 ail_vec2i8_add(AIL_Vec2i8 a, AIL_Vec2i8 b);
AIL_Vec2u16 ail_vec2u16_add(AIL_Vec2u16 a, AIL_Vec2u16 b);
AIL_Vec2i16 ail_vec2i16_add(AIL_Vec2i16 a, AIL_Vec2i16 b);
AIL_Vec2u32 ail_vec2u32_add(AIL_Vec2u32 a, AIL_Vec2u32 b);
AIL_Vec2i32 ail_vec2i32_add(AIL_Vec2i32 a, AIL_Vec2i32 b);
AIL_Vec2u64 ail_vec2u64_add(AIL_Vec2u64 a, AIL_Vec2u64 b);
AIL_Vec2i64 ail_vec2i64_add(AIL_Vec2i64 a, AIL_Vec2i64 b);
AIL_Vec2f32 ail_vec2f32_add(AIL_Vec2f32 a, AIL_Vec2f32 b);
AIL_Vec2f64 ail_vec2f64_add(AIL_Vec2f64 a, AIL_Vec2f64 b);
AIL_Vec3u8 ail_vec3u8_add(AIL_Vec3u8 a, AIL_Vec3u8 b);
AIL_Vec3i8 ail_vec3i8_add(AIL_Vec3i8 a, AIL_Vec3i8 b);
AIL_Vec3u16 ail_vec3u16_add(AIL_Vec3u16 a, AIL_Vec3u16 b);
AIL_Vec3i16 ail_vec3i16_add(AIL_Vec3i16 a, AIL_Vec3i16 b);
AIL_Vec3u32 ail_vec3u32_add(AIL_Vec3u32 a, AIL_Vec3u32 b);
AIL_Vec3i32 ail_vec3i32_add(AIL_Vec3i32 a, AIL_Vec3i32 b);
AIL_Vec3u64 ail_vec3u64_add(AIL_Vec3u64 a, AIL_Vec3u64 b);
AIL_Vec3i64 ail_vec3i64_add(AIL_Vec3i64 a, AIL_Vec3i64 b);
AIL_Vec3f32 ail_vec3f32_add(AIL_Vec3f32 a, AIL_Vec3f32 b);
AIL_Vec3f64 ail_vec3f64_add(AIL_Vec3f64 a, AIL_Vec3f64 b);
AIL_Vec4u8 ail_vec4u8_add(AIL_Vec4u8 a, AIL_Vec4u8 b);
AIL_Vec4i8 ail_vec4i8_add(AIL_Vec4i8 a, AIL_Vec4i8 b);
AIL_Vec4u16 ail_vec4u16_add(AIL_Vec4u16 a, AIL_Vec4u16 b);
AIL_Vec4i16 ail_vec4i16_add(AIL_Vec4i16 a, AIL_Vec4i16 b);
AIL_Vec4u32 ail_vec4u32_add(AIL_Vec4u32 a, AIL_Vec4u32 b);
AIL_Vec4i32 ail_vec4i32_add(AIL_Vec4i32 a, AIL_Vec4i32 b);
AIL_Vec4u64 ail_vec4u64_add(AIL_Vec4u64 a, AIL_Vec4u64 b);
AIL_Vec4i64 ail_vec4i64_add(AIL_Vec4i64 a, AIL_Vec4i64 b);
AIL_Vec4f32 ail_vec4f32_add(AIL_Vec4f32 a, AIL_Vec4f32 b);
AIL_Vec4f64 ail_vec4f64_add(AIL_Vec4f64 a, AIL_Vec4f64 b);

AIL_Vec2u8 ail_vec2u8_sub(AIL_Vec2u8 a, AIL_Vec2u8 b);
AIL_Vec2i8 ail_vec2i8_sub(AIL_Vec2i8 a, AIL_Vec2i8 b);
AIL_Vec2u16 ail_vec2u16_sub(AIL_Vec2u16 a, AIL_Vec2u16 b);
AIL_Vec2i16 ail_vec2i16_sub(AIL_Vec2i16 a, AIL_Vec2i16 b);
AIL_Vec2u32 ail_vec2u32_sub(AIL_Vec2u32 a, AIL_Vec2u32 b);
AIL_Vec2i32 ail_vec2i32_sub(AIL_Vec2i32 a, AIL_Vec2i32 b);
AIL_Vec2u64 ail_vec2u64_sub(AIL_Vec2u64 a, AIL_Vec2u64 b);
AIL_Vec2i64 ail_vec2i64_sub(AIL_Vec2i64 a, AIL_Vec2i64 b);
AIL_Vec2f32 ail_vec2f32_sub(AIL_Vec2f32 a, AIL_Vec2f32 b);
AIL_Vec2f64 ail_vec2f64_sub(AIL_Vec2f64 a, AIL_Vec2f64 b);
AIL_Vec3u8 ail_vec3u8_sub(AIL_Vec3u8 a, AIL_Vec3u8 b);
AIL_Vec3i8 ail_vec3i8_sub(AIL_Vec3i8 a, AIL_Vec3i8 b);
AIL_Vec3u16 ail_vec3u16_sub(AIL_Vec3u16 a, AIL_Vec3u16 b);
AIL_Vec3i16 ail_vec3i16_sub(AIL_Vec3i16 a, AIL_Vec3i16 b);
AIL_Vec3u32 ail_vec3u32_sub(AIL_Vec3u32 a, AIL_Vec3u32 b);
AIL_Vec3i32 ail_vec3i32_sub(AIL_Vec3i32 a, AIL_Vec3i32 b);
AIL_Vec3u64 ail_vec3u64_sub(AIL_Vec3u64 a, AIL_Vec3u64 b);
AIL_Vec3i64 ail_vec3i64_sub(AIL_Vec3i64 a, AIL_Vec3i64 b);
AIL_Vec3f32 ail_vec3f32_sub(AIL_Vec3f32 a, AIL_Vec3f32 b);
AIL_Vec3f64 ail_vec3f64_sub(AIL_Vec3f64 a, AIL_Vec3f64 b);
AIL_Vec4u8 ail_vec4u8_sub(AIL_Vec4u8 a, AIL_Vec4u8 b);
AIL_Vec4i8 ail_vec4i8_sub(AIL_Vec4i8 a, AIL_Vec4i8 b);
AIL_Vec4u16 ail_vec4u16_sub(AIL_Vec4u16 a, AIL_Vec4u16 b);
AIL_Vec4i16 ail_vec4i16_sub(AIL_Vec4i16 a, AIL_Vec4i16 b);
AIL_Vec4u32 ail_vec4u32_sub(AIL_Vec4u32 a, AIL_Vec4u32 b);
AIL_Vec4i32 ail_vec4i32_sub(AIL_Vec4i32 a, AIL_Vec4i32 b);
AIL_Vec4u64 ail_vec4u64_sub(AIL_Vec4u64 a, AIL_Vec4u64 b);
AIL_Vec4i64 ail_vec4i64_sub(AIL_Vec4i64 a, AIL_Vec4i64 b);
AIL_Vec4f32 ail_vec4f32_sub(AIL_Vec4f32 a, AIL_Vec4f32 b);
AIL_Vec4f64 ail_vec4f64_sub(AIL_Vec4f64 a, AIL_Vec4f64 b);

b32 ail_vec2u8_eq(AIL_Vec2u8 a, AIL_Vec2u8 b);
b32 ail_vec2i8_eq(AIL_Vec2i8 a, AIL_Vec2i8 b);
b32 ail_vec2u16_eq(AIL_Vec2u16 a, AIL_Vec2u16 b);
b32 ail_vec2i16_eq(AIL_Vec2i16 a, AIL_Vec2i16 b);
b32 ail_vec2u32_eq(AIL_Vec2u32 a, AIL_Vec2u32 b);
b32 ail_vec2i32_eq(AIL_Vec2i32 a, AIL_Vec2i32 b);
b32 ail_vec2u64_eq(AIL_Vec2u64 a, AIL_Vec2u64 b);
b32 ail_vec2i64_eq(AIL_Vec2i64 a, AIL_Vec2i64 b);
b32 ail_vec2f32_eq(AIL_Vec2f32 a, AIL_Vec2f32 b);
b32 ail_vec2f64_eq(AIL_Vec2f64 a, AIL_Vec2f64 b);
b32 ail_vec3u8_eq(AIL_Vec3u8 a, AIL_Vec3u8 b);
b32 ail_vec3i8_eq(AIL_Vec3i8 a, AIL_Vec3i8 b);
b32 ail_vec3u16_eq(AIL_Vec3u16 a, AIL_Vec3u16 b);
b32 ail_vec3i16_eq(AIL_Vec3i16 a, AIL_Vec3i16 b);
b32 ail_vec3u32_eq(AIL_Vec3u32 a, AIL_Vec3u32 b);
b32 ail_vec3i32_eq(AIL_Vec3i32 a, AIL_Vec3i32 b);
b32 ail_vec3u64_eq(AIL_Vec3u64 a, AIL_Vec3u64 b);
b32 ail_vec3i64_eq(AIL_Vec3i64 a, AIL_Vec3i64 b);
b32 ail_vec3f32_eq(AIL_Vec3f32 a, AIL_Vec3f32 b);
b32 ail_vec3f64_eq(AIL_Vec3f64 a, AIL_Vec3f64 b);
b32 ail_vec4u8_eq(AIL_Vec4u8 a, AIL_Vec4u8 b);
b32 ail_vec4i8_eq(AIL_Vec4i8 a, AIL_Vec4i8 b);
b32 ail_vec4u16_eq(AIL_Vec4u16 a, AIL_Vec4u16 b);
b32 ail_vec4i16_eq(AIL_Vec4i16 a, AIL_Vec4i16 b);
b32 ail_vec4u32_eq(AIL_Vec4u32 a, AIL_Vec4u32 b);
b32 ail_vec4i32_eq(AIL_Vec4i32 a, AIL_Vec4i32 b);
b32 ail_vec4u64_eq(AIL_Vec4u64 a, AIL_Vec4u64 b);
b32 ail_vec4i64_eq(AIL_Vec4i64 a, AIL_Vec4i64 b);
b32 ail_vec4f32_eq(AIL_Vec4f32 a, AIL_Vec4f32 b);
b32 ail_vec4f64_eq(AIL_Vec4f64 a, AIL_Vec4f64 b);

u8 ail_vec2u8_dot(AIL_Vec2u8 a, AIL_Vec2u8 b);
i8 ail_vec2i8_dot(AIL_Vec2i8 a, AIL_Vec2i8 b);
u16 ail_vec2u16_dot(AIL_Vec2u16 a, AIL_Vec2u16 b);
i16 ail_vec2i16_dot(AIL_Vec2i16 a, AIL_Vec2i16 b);
u32 ail_vec2u32_dot(AIL_Vec2u32 a, AIL_Vec2u32 b);
i32 ail_vec2i32_dot(AIL_Vec2i32 a, AIL_Vec2i32 b);
u64 ail_vec2u64_dot(AIL_Vec2u64 a, AIL_Vec2u64 b);
i64 ail_vec2i64_dot(AIL_Vec2i64 a, AIL_Vec2i64 b);
f32 ail_vec2f32_dot(AIL_Vec2f32 a, AIL_Vec2f32 b);
f64 ail_vec2f64_dot(AIL_Vec2f64 a, AIL_Vec2f64 b);
u8 ail_vec3u8_dot(AIL_Vec3u8 a, AIL_Vec3u8 b);
i8 ail_vec3i8_dot(AIL_Vec3i8 a, AIL_Vec3i8 b);
u16 ail_vec3u16_dot(AIL_Vec3u16 a, AIL_Vec3u16 b);
i16 ail_vec3i16_dot(AIL_Vec3i16 a, AIL_Vec3i16 b);
u32 ail_vec3u32_dot(AIL_Vec3u32 a, AIL_Vec3u32 b);
i32 ail_vec3i32_dot(AIL_Vec3i32 a, AIL_Vec3i32 b);
u64 ail_vec3u64_dot(AIL_Vec3u64 a, AIL_Vec3u64 b);
i64 ail_vec3i64_dot(AIL_Vec3i64 a, AIL_Vec3i64 b);
f32 ail_vec3f32_dot(AIL_Vec3f32 a, AIL_Vec3f32 b);
f64 ail_vec3f64_dot(AIL_Vec3f64 a, AIL_Vec3f64 b);
u8 ail_vec4u8_dot(AIL_Vec4u8 a, AIL_Vec4u8 b);
i8 ail_vec4i8_dot(AIL_Vec4i8 a, AIL_Vec4i8 b);
u16 ail_vec4u16_dot(AIL_Vec4u16 a, AIL_Vec4u16 b);
i16 ail_vec4i16_dot(AIL_Vec4i16 a, AIL_Vec4i16 b);
u32 ail_vec4u32_dot(AIL_Vec4u32 a, AIL_Vec4u32 b);
i32 ail_vec4i32_dot(AIL_Vec4i32 a, AIL_Vec4i32 b);
u64 ail_vec4u64_dot(AIL_Vec4u64 a, AIL_Vec4u64 b);
i64 ail_vec4i64_dot(AIL_Vec4i64 a, AIL_Vec4i64 b);
f32 ail_vec4f32_dot(AIL_Vec4f32 a, AIL_Vec4f32 b);
f64 ail_vec4f64_dot(AIL_Vec4f64 a, AIL_Vec4f64 b);

AIL_Mat2u8 ail_mat2u8_add(AIL_Mat2u8 a, AIL_Mat2u8 b);
AIL_Mat2i8 ail_mat2i8_add(AIL_Mat2i8 a, AIL_Mat2i8 b);
AIL_Mat2u16 ail_mat2u16_add(AIL_Mat2u16 a, AIL_Mat2u16 b);
AIL_Mat2i16 ail_mat2i16_add(AIL_Mat2i16 a, AIL_Mat2i16 b);
AIL_Mat2u32 ail_mat2u32_add(AIL_Mat2u32 a, AIL_Mat2u32 b);
AIL_Mat2i32 ail_mat2i32_add(AIL_Mat2i32 a, AIL_Mat2i32 b);
AIL_Mat2u64 ail_mat2u64_add(AIL_Mat2u64 a, AIL_Mat2u64 b);
AIL_Mat2i64 ail_mat2i64_add(AIL_Mat2i64 a, AIL_Mat2i64 b);
AIL_Mat2f32 ail_mat2f32_add(AIL_Mat2f32 a, AIL_Mat2f32 b);
AIL_Mat2f64 ail_mat2f64_add(AIL_Mat2f64 a, AIL_Mat2f64 b);
AIL_Mat3u8 ail_mat3u8_add(AIL_Mat3u8 a, AIL_Mat3u8 b);
AIL_Mat3i8 ail_mat3i8_add(AIL_Mat3i8 a, AIL_Mat3i8 b);
AIL_Mat3u16 ail_mat3u16_add(AIL_Mat3u16 a, AIL_Mat3u16 b);
AIL_Mat3i16 ail_mat3i16_add(AIL_Mat3i16 a, AIL_Mat3i16 b);
AIL_Mat3u32 ail_mat3u32_add(AIL_Mat3u32 a, AIL_Mat3u32 b);
AIL_Mat3i32 ail_mat3i32_add(AIL_Mat3i32 a, AIL_Mat3i32 b);
AIL_Mat3u64 ail_mat3u64_add(AIL_Mat3u64 a, AIL_Mat3u64 b);
AIL_Mat3i64 ail_mat3i64_add(AIL_Mat3i64 a, AIL_Mat3i64 b);
AIL_Mat3f32 ail_mat3f32_add(AIL_Mat3f32 a, AIL_Mat3f32 b);
AIL_Mat3f64 ail_mat3f64_add(AIL_Mat3f64 a, AIL_Mat3f64 b);
AIL_Mat4u8 ail_mat4u8_add(AIL_Mat4u8 a, AIL_Mat4u8 b);
AIL_Mat4i8 ail_mat4i8_add(AIL_Mat4i8 a, AIL_Mat4i8 b);
AIL_Mat4u16 ail_mat4u16_add(AIL_Mat4u16 a, AIL_Mat4u16 b);
AIL_Mat4i16 ail_mat4i16_add(AIL_Mat4i16 a, AIL_Mat4i16 b);
AIL_Mat4u32 ail_mat4u32_add(AIL_Mat4u32 a, AIL_Mat4u32 b);
AIL_Mat4i32 ail_mat4i32_add(AIL_Mat4i32 a, AIL_Mat4i32 b);
AIL_Mat4u64 ail_mat4u64_add(AIL_Mat4u64 a, AIL_Mat4u64 b);
AIL_Mat4i64 ail_mat4i64_add(AIL_Mat4i64 a, AIL_Mat4i64 b);
AIL_Mat4f32 ail_mat4f32_add(AIL_Mat4f32 a, AIL_Mat4f32 b);
AIL_Mat4f64 ail_mat4f64_add(AIL_Mat4f64 a, AIL_Mat4f64 b);

AIL_Mat2u8 ail_mat2u8_sub(AIL_Mat2u8 a, AIL_Mat2u8 b);
AIL_Mat2i8 ail_mat2i8_sub(AIL_Mat2i8 a, AIL_Mat2i8 b);
AIL_Mat2u16 ail_mat2u16_sub(AIL_Mat2u16 a, AIL_Mat2u16 b);
AIL_Mat2i16 ail_mat2i16_sub(AIL_Mat2i16 a, AIL_Mat2i16 b);
AIL_Mat2u32 ail_mat2u32_sub(AIL_Mat2u32 a, AIL_Mat2u32 b);
AIL_Mat2i32 ail_mat2i32_sub(AIL_Mat2i32 a, AIL_Mat2i32 b);
AIL_Mat2u64 ail_mat2u64_sub(AIL_Mat2u64 a, AIL_Mat2u64 b);
AIL_Mat2i64 ail_mat2i64_sub(AIL_Mat2i64 a, AIL_Mat2i64 b);
AIL_Mat2f32 ail_mat2f32_sub(AIL_Mat2f32 a, AIL_Mat2f32 b);
AIL_Mat2f64 ail_mat2f64_sub(AIL_Mat2f64 a, AIL_Mat2f64 b);
AIL_Mat3u8 ail_mat3u8_sub(AIL_Mat3u8 a, AIL_Mat3u8 b);
AIL_Mat3i8 ail_mat3i8_sub(AIL_Mat3i8 a, AIL_Mat3i8 b);
AIL_Mat3u16 ail_mat3u16_sub(AIL_Mat3u16 a, AIL_Mat3u16 b);
AIL_Mat3i16 ail_mat3i16_sub(AIL_Mat3i16 a, AIL_Mat3i16 b);
AIL_Mat3u32 ail_mat3u32_sub(AIL_Mat3u32 a, AIL_Mat3u32 b);
AIL_Mat3i32 ail_mat3i32_sub(AIL_Mat3i32 a, AIL_Mat3i32 b);
AIL_Mat3u64 ail_mat3u64_sub(AIL_Mat3u64 a, AIL_Mat3u64 b);
AIL_Mat3i64 ail_mat3i64_sub(AIL_Mat3i64 a, AIL_Mat3i64 b);
AIL_Mat3f32 ail_mat3f32_sub(AIL_Mat3f32 a, AIL_Mat3f32 b);
AIL_Mat3f64 ail_mat3f64_sub(AIL_Mat3f64 a, AIL_Mat3f64 b);
AIL_Mat4u8 ail_mat4u8_sub(AIL_Mat4u8 a, AIL_Mat4u8 b);
AIL_Mat4i8 ail_mat4i8_sub(AIL_Mat4i8 a, AIL_Mat4i8 b);
AIL_Mat4u16 ail_mat4u16_sub(AIL_Mat4u16 a, AIL_Mat4u16 b);
AIL_Mat4i16 ail_mat4i16_sub(AIL_Mat4i16 a, AIL_Mat4i16 b);
AIL_Mat4u32 ail_mat4u32_sub(AIL_Mat4u32 a, AIL_Mat4u32 b);
AIL_Mat4i32 ail_mat4i32_sub(AIL_Mat4i32 a, AIL_Mat4i32 b);
AIL_Mat4u64 ail_mat4u64_sub(AIL_Mat4u64 a, AIL_Mat4u64 b);
AIL_Mat4i64 ail_mat4i64_sub(AIL_Mat4i64 a, AIL_Mat4i64 b);
AIL_Mat4f32 ail_mat4f32_sub(AIL_Mat4f32 a, AIL_Mat4f32 b);
AIL_Mat4f64 ail_mat4f64_sub(AIL_Mat4f64 a, AIL_Mat4f64 b);

#endif // AIL_MATH_H_




//////////////////////////////
// //
//      Implementation      //
// //
//////////////////////////////
#if !defined(AIL_NO_MATH_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_MATH_IMPL_GUARD_
#define _AIL_MATH_IMPL_GUARD_


//////////////////////
//  Linear Algebra  //
//////////////////////

AIL_Vec2u8 ail_vec2u8(u8 x, u8 y)
{
    return (AIL_Vec2u8) { .x = x, .y = y };
}

AIL_Vec2i8 ail_vec2i8(i8 x, i8 y)
{
    return (AIL_Vec2i8) { .x = x, .y = y };
}

AIL_Vec2u16 ail_vec2u16(u16 x, u16 y)
{
    return (AIL_Vec2u16) { .x = x, .y = y };
}

AIL_Vec2i16 ail_vec2i16(i16 x, i16 y)
{
    return (AIL_Vec2i16) { .x = x, .y = y };
}

AIL_Vec2u32 ail_vec2u32(u32 x, u32 y)
{
    return (AIL_Vec2u32) { .x = x, .y = y };
}

AIL_Vec2i32 ail_vec2i32(i32 x, i32 y)
{
    return (AIL_Vec2i32) { .x = x, .y = y };
}

AIL_Vec2u64 ail_vec2u64(u64 x, u64 y)
{
    return (AIL_Vec2u64) { .x = x, .y = y };
}

AIL_Vec2i64 ail_vec2i64(i64 x, i64 y)
{
    return (AIL_Vec2i64) { .x = x, .y = y };
}

AIL_Vec2f32 ail_vec2f32(f32 x, f32 y)
{
    return (AIL_Vec2f32) { .x = x, .y = y };
}

AIL_Vec2f64 ail_vec2f64(f64 x, f64 y)
{
    return (AIL_Vec2f64) { .x = x, .y = y };
}

AIL_Vec3u8 ail_vec3u8(u8 x, u8 y, u8 z)
{
    return (AIL_Vec3u8) { .x = x, .y = y, .z = z };
}

AIL_Vec3i8 ail_vec3i8(i8 x, i8 y, i8 z)
{
    return (AIL_Vec3i8) { .x = x, .y = y, .z = z };
}

AIL_Vec3u16 ail_vec3u16(u16 x, u16 y, u16 z)
{
    return (AIL_Vec3u16) { .x = x, .y = y, .z = z };
}

AIL_Vec3i16 ail_vec3i16(i16 x, i16 y, i16 z)
{
    return (AIL_Vec3i16) { .x = x, .y = y, .z = z };
}

AIL_Vec3u32 ail_vec3u32(u32 x, u32 y, u32 z)
{
    return (AIL_Vec3u32) { .x = x, .y = y, .z = z };
}

AIL_Vec3i32 ail_vec3i32(i32 x, i32 y, i32 z)
{
    return (AIL_Vec3i32) { .x = x, .y = y, .z = z };
}

AIL_Vec3u64 ail_vec3u64(u64 x, u64 y, u64 z)
{
    return (AIL_Vec3u64) { .x = x, .y = y, .z = z };
}

AIL_Vec3i64 ail_vec3i64(i64 x, i64 y, i64 z)
{
    return (AIL_Vec3i64) { .x = x, .y = y, .z = z };
}

AIL_Vec3f32 ail_vec3f32(f32 x, f32 y, f32 z)
{
    return (AIL_Vec3f32) { .x = x, .y = y, .z = z };
}

AIL_Vec3f64 ail_vec3f64(f64 x, f64 y, f64 z)
{
    return (AIL_Vec3f64) { .x = x, .y = y, .z = z };
}

AIL_Vec4u8 ail_vec4u8(u8 x, u8 y, u8 z, u8 w)
{
    return (AIL_Vec4u8) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i8 ail_vec4i8(i8 x, i8 y, i8 z, i8 w)
{
    return (AIL_Vec4i8) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u16 ail_vec4u16(u16 x, u16 y, u16 z, u16 w)
{
    return (AIL_Vec4u16) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i16 ail_vec4i16(i16 x, i16 y, i16 z, i16 w)
{
    return (AIL_Vec4i16) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u32 ail_vec4u32(u32 x, u32 y, u32 z, u32 w)
{
    return (AIL_Vec4u32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i32 ail_vec4i32(i32 x, i32 y, i32 z, i32 w)
{
    return (AIL_Vec4i32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u64 ail_vec4u64(u64 x, u64 y, u64 z, u64 w)
{
    return (AIL_Vec4u64) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i64 ail_vec4i64(i64 x, i64 y, i64 z, i64 w)
{
    return (AIL_Vec4i64) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4f32 ail_vec4f32(f32 x, f32 y, f32 z, f32 w)
{
    return (AIL_Vec4f32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4f64 ail_vec4f64(f64 x, f64 y, f64 z, f64 w)
{
    return (AIL_Vec4f64) { .x = x, .y = y, .z = z, .w = w };
}


AIL_Vec2u8 ail_vec2u8_add(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return (AIL_Vec2u8) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2i8 ail_vec2i8_add(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return (AIL_Vec2i8) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2u16 ail_vec2u16_add(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return (AIL_Vec2u16) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2i16 ail_vec2i16_add(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return (AIL_Vec2i16) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2u32 ail_vec2u32_add(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return (AIL_Vec2u32) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2i32 ail_vec2i32_add(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return (AIL_Vec2i32) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2u64 ail_vec2u64_add(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return (AIL_Vec2u64) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2i64 ail_vec2i64_add(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return (AIL_Vec2i64) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2f32 ail_vec2f32_add(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return (AIL_Vec2f32) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2f64 ail_vec2f64_add(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return (AIL_Vec2f64) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec3u8 ail_vec3u8_add(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return (AIL_Vec3u8) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3i8 ail_vec3i8_add(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return (AIL_Vec3i8) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3u16 ail_vec3u16_add(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return (AIL_Vec3u16) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3i16 ail_vec3i16_add(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return (AIL_Vec3i16) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3u32 ail_vec3u32_add(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return (AIL_Vec3u32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3i32 ail_vec3i32_add(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return (AIL_Vec3i32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3u64 ail_vec3u64_add(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return (AIL_Vec3u64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3i64 ail_vec3i64_add(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return (AIL_Vec3i64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3f32 ail_vec3f32_add(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return (AIL_Vec3f32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3f64 ail_vec3f64_add(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return (AIL_Vec3f64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec4u8 ail_vec4u8_add(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return (AIL_Vec4u8) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4i8 ail_vec4i8_add(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return (AIL_Vec4i8) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4u16 ail_vec4u16_add(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return (AIL_Vec4u16) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4i16 ail_vec4i16_add(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return (AIL_Vec4i16) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4u32 ail_vec4u32_add(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return (AIL_Vec4u32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4i32 ail_vec4i32_add(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return (AIL_Vec4i32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4u64 ail_vec4u64_add(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return (AIL_Vec4u64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4i64 ail_vec4i64_add(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return (AIL_Vec4i64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4f32 ail_vec4f32_add(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return (AIL_Vec4f32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4f64 ail_vec4f64_add(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return (AIL_Vec4f64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}


AIL_Vec2u8 ail_vec2u8_sub(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return (AIL_Vec2u8) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2i8 ail_vec2i8_sub(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return (AIL_Vec2i8) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2u16 ail_vec2u16_sub(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return (AIL_Vec2u16) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2i16 ail_vec2i16_sub(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return (AIL_Vec2i16) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2u32 ail_vec2u32_sub(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return (AIL_Vec2u32) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2i32 ail_vec2i32_sub(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return (AIL_Vec2i32) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2u64 ail_vec2u64_sub(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return (AIL_Vec2u64) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2i64 ail_vec2i64_sub(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return (AIL_Vec2i64) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2f32 ail_vec2f32_sub(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return (AIL_Vec2f32) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2f64 ail_vec2f64_sub(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return (AIL_Vec2f64) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec3u8 ail_vec3u8_sub(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return (AIL_Vec3u8) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3i8 ail_vec3i8_sub(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return (AIL_Vec3i8) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3u16 ail_vec3u16_sub(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return (AIL_Vec3u16) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3i16 ail_vec3i16_sub(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return (AIL_Vec3i16) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3u32 ail_vec3u32_sub(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return (AIL_Vec3u32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3i32 ail_vec3i32_sub(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return (AIL_Vec3i32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3u64 ail_vec3u64_sub(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return (AIL_Vec3u64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3i64 ail_vec3i64_sub(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return (AIL_Vec3i64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3f32 ail_vec3f32_sub(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return (AIL_Vec3f32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3f64 ail_vec3f64_sub(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return (AIL_Vec3f64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec4u8 ail_vec4u8_sub(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return (AIL_Vec4u8) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4i8 ail_vec4i8_sub(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return (AIL_Vec4i8) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4u16 ail_vec4u16_sub(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return (AIL_Vec4u16) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4i16 ail_vec4i16_sub(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return (AIL_Vec4i16) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4u32 ail_vec4u32_sub(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return (AIL_Vec4u32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4i32 ail_vec4i32_sub(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return (AIL_Vec4i32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4u64 ail_vec4u64_sub(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return (AIL_Vec4u64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4i64 ail_vec4i64_sub(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return (AIL_Vec4i64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4f32 ail_vec4f32_sub(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return (AIL_Vec4f32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4f64 ail_vec4f64_sub(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return (AIL_Vec4f64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}


b32 ail_vec2u8_eq(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2i8_eq(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2u16_eq(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2i16_eq(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2u32_eq(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2i32_eq(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2u64_eq(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2i64_eq(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2f32_eq(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2f64_eq(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec3u8_eq(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3i8_eq(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3u16_eq(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3i16_eq(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3u32_eq(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3i32_eq(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3u64_eq(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3i64_eq(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3f32_eq(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3f64_eq(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec4u8_eq(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4i8_eq(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4u16_eq(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4i16_eq(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4u32_eq(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4i32_eq(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4u64_eq(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4i64_eq(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4f32_eq(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4f64_eq(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}


u8 ail_vec2u8_dot(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return  a.x * b.x + a.y * b.y;
}

i8 ail_vec2i8_dot(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return  a.x * b.x + a.y * b.y;
}

u16 ail_vec2u16_dot(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return  a.x * b.x + a.y * b.y;
}

i16 ail_vec2i16_dot(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return  a.x * b.x + a.y * b.y;
}

u32 ail_vec2u32_dot(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return  a.x * b.x + a.y * b.y;
}

i32 ail_vec2i32_dot(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return  a.x * b.x + a.y * b.y;
}

u64 ail_vec2u64_dot(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return  a.x * b.x + a.y * b.y;
}

i64 ail_vec2i64_dot(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return  a.x * b.x + a.y * b.y;
}

f32 ail_vec2f32_dot(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return  a.x * b.x + a.y * b.y;
}

f64 ail_vec2f64_dot(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return  a.x * b.x + a.y * b.y;
}

u8 ail_vec3u8_dot(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

i8 ail_vec3i8_dot(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

u16 ail_vec3u16_dot(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

i16 ail_vec3i16_dot(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

u32 ail_vec3u32_dot(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

i32 ail_vec3i32_dot(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

u64 ail_vec3u64_dot(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

i64 ail_vec3i64_dot(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

f32 ail_vec3f32_dot(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

f64 ail_vec3f64_dot(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

u8 ail_vec4u8_dot(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

i8 ail_vec4i8_dot(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

u16 ail_vec4u16_dot(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

i16 ail_vec4i16_dot(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

u32 ail_vec4u32_dot(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

i32 ail_vec4i32_dot(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

u64 ail_vec4u64_dot(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

i64 ail_vec4i64_dot(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

f32 ail_vec4f32_dot(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

f64 ail_vec4f64_dot(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


AIL_Mat2u8 ail_mat2u8_add(AIL_Mat2u8 a, AIL_Mat2u8 b)
{
    return (AIL_Mat2u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2i8 ail_mat2i8_add(AIL_Mat2i8 a, AIL_Mat2i8 b)
{
    return (AIL_Mat2i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2u16 ail_mat2u16_add(AIL_Mat2u16 a, AIL_Mat2u16 b)
{
    return (AIL_Mat2u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2i16 ail_mat2i16_add(AIL_Mat2i16 a, AIL_Mat2i16 b)
{
    return (AIL_Mat2i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2u32 ail_mat2u32_add(AIL_Mat2u32 a, AIL_Mat2u32 b)
{
    return (AIL_Mat2u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2i32 ail_mat2i32_add(AIL_Mat2i32 a, AIL_Mat2i32 b)
{
    return (AIL_Mat2i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2u64 ail_mat2u64_add(AIL_Mat2u64 a, AIL_Mat2u64 b)
{
    return (AIL_Mat2u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2i64 ail_mat2i64_add(AIL_Mat2i64 a, AIL_Mat2i64 b)
{
    return (AIL_Mat2i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2f32 ail_mat2f32_add(AIL_Mat2f32 a, AIL_Mat2f32 b)
{
    return (AIL_Mat2f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2f64 ail_mat2f64_add(AIL_Mat2f64 a, AIL_Mat2f64 b)
{
    return (AIL_Mat2f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat3u8 ail_mat3u8_add(AIL_Mat3u8 a, AIL_Mat3u8 b)
{
    return (AIL_Mat3u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3i8 ail_mat3i8_add(AIL_Mat3i8 a, AIL_Mat3i8 b)
{
    return (AIL_Mat3i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3u16 ail_mat3u16_add(AIL_Mat3u16 a, AIL_Mat3u16 b)
{
    return (AIL_Mat3u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3i16 ail_mat3i16_add(AIL_Mat3i16 a, AIL_Mat3i16 b)
{
    return (AIL_Mat3i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3u32 ail_mat3u32_add(AIL_Mat3u32 a, AIL_Mat3u32 b)
{
    return (AIL_Mat3u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3i32 ail_mat3i32_add(AIL_Mat3i32 a, AIL_Mat3i32 b)
{
    return (AIL_Mat3i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3u64 ail_mat3u64_add(AIL_Mat3u64 a, AIL_Mat3u64 b)
{
    return (AIL_Mat3u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3i64 ail_mat3i64_add(AIL_Mat3i64 a, AIL_Mat3i64 b)
{
    return (AIL_Mat3i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3f32 ail_mat3f32_add(AIL_Mat3f32 a, AIL_Mat3f32 b)
{
    return (AIL_Mat3f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3f64 ail_mat3f64_add(AIL_Mat3f64 a, AIL_Mat3f64 b)
{
    return (AIL_Mat3f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat4u8 ail_mat4u8_add(AIL_Mat4u8 a, AIL_Mat4u8 b)
{
    return (AIL_Mat4u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4i8 ail_mat4i8_add(AIL_Mat4i8 a, AIL_Mat4i8 b)
{
    return (AIL_Mat4i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4u16 ail_mat4u16_add(AIL_Mat4u16 a, AIL_Mat4u16 b)
{
    return (AIL_Mat4u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4i16 ail_mat4i16_add(AIL_Mat4i16 a, AIL_Mat4i16 b)
{
    return (AIL_Mat4i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4u32 ail_mat4u32_add(AIL_Mat4u32 a, AIL_Mat4u32 b)
{
    return (AIL_Mat4u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4i32 ail_mat4i32_add(AIL_Mat4i32 a, AIL_Mat4i32 b)
{
    return (AIL_Mat4i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4u64 ail_mat4u64_add(AIL_Mat4u64 a, AIL_Mat4u64 b)
{
    return (AIL_Mat4u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4i64 ail_mat4i64_add(AIL_Mat4i64 a, AIL_Mat4i64 b)
{
    return (AIL_Mat4i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4f32 ail_mat4f32_add(AIL_Mat4f32 a, AIL_Mat4f32 b)
{
    return (AIL_Mat4f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4f64 ail_mat4f64_add(AIL_Mat4f64 a, AIL_Mat4f64 b)
{
    return (AIL_Mat4f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}


AIL_Mat2u8 ail_mat2u8_sub(AIL_Mat2u8 a, AIL_Mat2u8 b)
{
    return (AIL_Mat2u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2i8 ail_mat2i8_sub(AIL_Mat2i8 a, AIL_Mat2i8 b)
{
    return (AIL_Mat2i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2u16 ail_mat2u16_sub(AIL_Mat2u16 a, AIL_Mat2u16 b)
{
    return (AIL_Mat2u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2i16 ail_mat2i16_sub(AIL_Mat2i16 a, AIL_Mat2i16 b)
{
    return (AIL_Mat2i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2u32 ail_mat2u32_sub(AIL_Mat2u32 a, AIL_Mat2u32 b)
{
    return (AIL_Mat2u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2i32 ail_mat2i32_sub(AIL_Mat2i32 a, AIL_Mat2i32 b)
{
    return (AIL_Mat2i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2u64 ail_mat2u64_sub(AIL_Mat2u64 a, AIL_Mat2u64 b)
{
    return (AIL_Mat2u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2i64 ail_mat2i64_sub(AIL_Mat2i64 a, AIL_Mat2i64 b)
{
    return (AIL_Mat2i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2f32 ail_mat2f32_sub(AIL_Mat2f32 a, AIL_Mat2f32 b)
{
    return (AIL_Mat2f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2f64 ail_mat2f64_sub(AIL_Mat2f64 a, AIL_Mat2f64 b)
{
    return (AIL_Mat2f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat3u8 ail_mat3u8_sub(AIL_Mat3u8 a, AIL_Mat3u8 b)
{
    return (AIL_Mat3u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3i8 ail_mat3i8_sub(AIL_Mat3i8 a, AIL_Mat3i8 b)
{
    return (AIL_Mat3i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3u16 ail_mat3u16_sub(AIL_Mat3u16 a, AIL_Mat3u16 b)
{
    return (AIL_Mat3u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3i16 ail_mat3i16_sub(AIL_Mat3i16 a, AIL_Mat3i16 b)
{
    return (AIL_Mat3i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3u32 ail_mat3u32_sub(AIL_Mat3u32 a, AIL_Mat3u32 b)
{
    return (AIL_Mat3u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3i32 ail_mat3i32_sub(AIL_Mat3i32 a, AIL_Mat3i32 b)
{
    return (AIL_Mat3i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3u64 ail_mat3u64_sub(AIL_Mat3u64 a, AIL_Mat3u64 b)
{
    return (AIL_Mat3u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3i64 ail_mat3i64_sub(AIL_Mat3i64 a, AIL_Mat3i64 b)
{
    return (AIL_Mat3i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3f32 ail_mat3f32_sub(AIL_Mat3f32 a, AIL_Mat3f32 b)
{
    return (AIL_Mat3f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3f64 ail_mat3f64_sub(AIL_Mat3f64 a, AIL_Mat3f64 b)
{
    return (AIL_Mat3f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat4u8 ail_mat4u8_sub(AIL_Mat4u8 a, AIL_Mat4u8 b)
{
    return (AIL_Mat4u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4i8 ail_mat4i8_sub(AIL_Mat4i8 a, AIL_Mat4i8 b)
{
    return (AIL_Mat4i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4u16 ail_mat4u16_sub(AIL_Mat4u16 a, AIL_Mat4u16 b)
{
    return (AIL_Mat4u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4i16 ail_mat4i16_sub(AIL_Mat4i16 a, AIL_Mat4i16 b)
{
    return (AIL_Mat4i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4u32 ail_mat4u32_sub(AIL_Mat4u32 a, AIL_Mat4u32 b)
{
    return (AIL_Mat4u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4i32 ail_mat4i32_sub(AIL_Mat4i32 a, AIL_Mat4i32 b)
{
    return (AIL_Mat4i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4u64 ail_mat4u64_sub(AIL_Mat4u64 a, AIL_Mat4u64 b)
{
    return (AIL_Mat4u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4i64 ail_mat4i64_sub(AIL_Mat4i64 a, AIL_Mat4i64 b)
{
    return (AIL_Mat4i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4f32 ail_mat4f32_sub(AIL_Mat4f32 a, AIL_Mat4f32 b)
{
    return (AIL_Mat4f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4f64 ail_mat4f64_sub(AIL_Mat4f64 a, AIL_Mat4f64 b)
{
    return (AIL_Mat4f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}




#endif // _AIL_MATH_IMPL_GUARD_
#endif // AIL_NO_MATH_IMPL





////////
// From ail_md.h
////////


// Memory Debugger
//
// Disable implementation by defining AIL_NO_MD_IMPL
//
// If AIL_MD_MEM_DEBUG is enabled, the memory debugging system will create macros that replace malloc, calloc, free and realloc
// and allows the system to keppt track of and report where memory is beeing allocated, how much and if the memory is beeing freed.
// This is very useful for finding memory leaks in large applications.
// The system can also over allocate memory and fill it with a magic number and can therfor detect if the application writes outside of the allocated memory.
//
// If AIL_MD_EXIT is defined, then exit(); will be replaced with a funtion that writes to NULL.
// This will make it trivial to find out where an application exits using any debugger.
//
// This code is adapted from Quelsolaar's Forge library (see License below)
// The original code was downloaded from https://gamepipeline.org/forge.html
// @Note: The source code did not provide the license itself. The following license is thus assumed to be correct,
// based on the comment on https://gamepipeline.org/index.html, saying that the code is licensed via the Free-BSD license.
//
// LICENSE of original code
/*
Copyright 2024 Eskil Steenberg. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//
// LICENSE regarding all modifications
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

#ifndef AIL_MD_H_
#define AIL_MD_H_


#ifndef AIL_MD_DEF
#ifdef  AIL_DEF
#define AIL_MD_DEF AIL_DEF
#else
#define AIL_MD_DEF
#endif // AIL_DEF
#endif // AIL_MD_DEF
#ifndef AIL_MD_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_MD_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_MD_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_MD_DEF_INLINE

#include <stdlib.h>
#include <stdio.h>

AIL_MD_DEF void ail_md_mem_init(void (*lock)(void *mutex), void (*unlock)(void *mutex), void *mutex); // Required for memory debugger to be thread safe
AIL_MD_DEF void *ail_md_malloc(u64 size, char *file, u32 line); // Replaces malloc and records the c file and line where it was called
AIL_MD_DEF void *ail_md_calloc(u64 nelem, u64 elsize, char *file, u32 line); // Replaces calloc and records the c file and line where it was called
AIL_MD_DEF void *ail_md_realloc(void *pointer, u64 size, char *file, u32 line); // Replaces realloc and records the c file and line where it was called
AIL_MD_DEF void ail_md_free(void *buf, char *file, u32 line); // Replaces free and records the c file and line where it was called
AIL_MD_DEF bool ail_md_comment(void *buf, char *comment); // add a comment to an allocation that can help identyfy its use.
AIL_MD_DEF void ail_md_print(u32 min_allocs); // Prints out a list of all allocations made, their location, how much memorey each has allocated, freed, and how many allocations have been made. The min_allocs parameter can be set to avoid printing any allocations that have been made fewer times then min_allocs
AIL_MD_DEF void ail_md_reset(void); // ail_md_reset allows you to clear all memory stored in the debugging system if you only want to record allocations after a specific point in your code
AIL_MD_DEF u64 ail_md_consumption(void); // add up all memory consumed by mallocsd and reallocs coverd by the memory debugger .
AIL_MD_DEF bool ail_md_query(void *pointer, u32 *line, char **file, u64 *size); // query the size and place of allocation of a pointer
AIL_MD_DEF bool ail_md_test(void *pointer, u64 size, bool ignore_not_found); // query if a bit of memory is safe to access.
AIL_MD_DEF bool ail_md_mem(); //ail_md_mem checks if any of the bounds of any allocation has been over written and reports where to standard out. The function returns true if any error was found
AIL_MD_DEF void exit_crash(u32 i); // finction guaranteed to crash (Writes to NULL).

AIL_MD_DEF void *ail_md_mem_fopen(const char *file_name, const char *mode, char *file, u32 line);
AIL_MD_DEF void ail_md_mem_fclose(void *f, char *file, u32 line);

#endif // AIL_MD_H_

#if !defined(AIL_NO_MD_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_MD_IMPL_GUARD_
#define _AIL_MD_IMPL_GUARD_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define AIL_MD_MEM_INTERNAL
#define AIL_MD_NO_AIL_MD

AIL_MD_DEF void ail_md_print(u32 min_allocs);

#define AIL_MD_OVER_ALLOC 256
#define AIL_MD_MAGIC_NUM 132
typedef struct{
    u32 size;
    void *buf;
    char *comment;
}STMemAllocBuf;

typedef struct{
    u32 line;
    char file[256];
    STMemAllocBuf *allocs;
    u32 alloc_count;
    u64 alloc_alocated;
    u64 size;
    u64 alocated;
    u64 freed;
}STMemAllocLine;

STMemAllocLine ail_md_alloc_lines[1024];
u32 ail_md_alloc_line_count = 0;

#define AIL_MD_FREE_POINTER_BUFFER_SIZE 1024

typedef struct {
    u32 alloc_line;
    char alloc_file[256];
    u32 free_line;
    char free_file[256];
    u64 size;
    void *pointer;
    bool realloc;
} STMemFreeBuf;

STMemFreeBuf ail_md_freed_memory[AIL_MD_FREE_POINTER_BUFFER_SIZE];
u32 ail_md_freed_memory_current = 0;
u32 ail_md_freed_memory_count = 0;

void *ail_md_alloc_mutex = NULL;
void (*ail_md_alloc_mutex_lock)(void *mutex) = NULL;
void (*ail_md_alloc_mutex_unlock)(void *mutex) = NULL;

void ail_md_mem_init(void (*lock)(void *mutex), void (*unlock)(void *mutex), void *mutex)
{
    ail_md_alloc_mutex = mutex;
    ail_md_alloc_mutex_lock = lock;
    ail_md_alloc_mutex_unlock = unlock;
}


void *ail_md_mem_fopen(const char *file_name, const char *mode, char *file, u32 line)
{
    (void)file;
    (void)line;
    FILE *f;
    f = fopen(file_name, mode);
    return f;
}

void ail_md_mem_fclose(void *f, char *file, u32 line)
{
    (void)file;
    (void)line;
    fclose((FILE*)f);
}


bool ail_md_mem()
{
    bool output = false;
    u32 i, j, k;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    for(i = 0; i < ail_md_alloc_line_count; i++)
    {
        for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
        {
            u8 *buf;
            u64 size;
            buf = ail_md_alloc_lines[i].allocs[j].buf;
            size = ail_md_alloc_lines[i].allocs[j].size;
            for(k = 0; k < AIL_MD_OVER_ALLOC; k++)
                if(buf[size + k] != AIL_MD_MAGIC_NUM)
                    break;
            if(k < AIL_MD_OVER_ALLOC)
            {
                if(ail_md_alloc_lines[i].allocs[j].comment == NULL)
                    printf("MEM ERROR: Overshoot at line %u in file %s\n", ail_md_alloc_lines[i].line, ail_md_alloc_lines[i].file);
                else
                    printf("MEM ERROR: Overshoot at line %u in file %s /* %s */\n", ail_md_alloc_lines[i].line, ail_md_alloc_lines[i].file, ail_md_alloc_lines[i].allocs[j].comment);
                {
                    u32 *X = NULL;
                    X[0] = 0;
                }
                output = true;
            }
        }
        // if (output) exit(0);
    }
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    return output;
}

void ail_md_add(void *pointer, u64 size, char *file, u32 line)
{
    u32 i, j;
    for(i = 0; i < AIL_MD_OVER_ALLOC; i++)
        ((u8 *)pointer)[size + i] = AIL_MD_MAGIC_NUM;

    for(i = 0; i < ail_md_alloc_line_count; i++)
    {
        if(line == ail_md_alloc_lines[i].line)
        {
            for(j = 0; file[j] != 0 && file[j] == ail_md_alloc_lines[i].file[j] ; j++);
            if(file[j] == ail_md_alloc_lines[i].file[j])
                break;
        }
    }
    if(i < ail_md_alloc_line_count)
    {
        if(ail_md_alloc_lines[i].alloc_alocated == ail_md_alloc_lines[i].alloc_count)
        {
            ail_md_alloc_lines[i].alloc_alocated += 1024;
            ail_md_alloc_lines[i].allocs = realloc(ail_md_alloc_lines[i].allocs, (sizeof *ail_md_alloc_lines[i].allocs) * ail_md_alloc_lines[i].alloc_alocated);
            if(ail_md_alloc_lines[i].allocs == NULL)
            {
                printf("MEM ERROR: Realloc returns NULL when trying to allocate %zu bytes at line %u in file %s\n", size, line, file);
                exit(0);
            }
        }
        ail_md_alloc_lines[i].allocs[ail_md_alloc_lines[i].alloc_count].size = size;
        ail_md_alloc_lines[i].allocs[ail_md_alloc_lines[i].alloc_count].comment = NULL;
        ail_md_alloc_lines[i].allocs[ail_md_alloc_lines[i].alloc_count++].buf = pointer;
        ail_md_alloc_lines[i].size += size;
        ail_md_alloc_lines[i].alocated++;
    }else
    {
        if(i < 1024)
        {
            ail_md_alloc_lines[i].line = line;
            for(j = 0; j < 255 && file[j] != 0; j++)
                ail_md_alloc_lines[i].file[j] = file[j];
            ail_md_alloc_lines[i].file[j] = 0;
            ail_md_alloc_lines[i].alloc_alocated = 256;
            ail_md_alloc_lines[i].allocs = malloc((sizeof *ail_md_alloc_lines[i].allocs) * ail_md_alloc_lines[i].alloc_alocated);
            ail_md_alloc_lines[i].allocs[0].size = size;
            ail_md_alloc_lines[i].allocs[0].buf = pointer;
            ail_md_alloc_lines[i].allocs[0].comment = NULL;
            ail_md_alloc_lines[i].alloc_count = 1;
            ail_md_alloc_lines[i].size = size;
            ail_md_alloc_lines[i].freed = 0;
            ail_md_alloc_lines[i].alocated++;
            ail_md_alloc_line_count++;
        }
    }
    for(i = 0; i < ail_md_freed_memory_count; i++)
    {
        if(pointer == ail_md_freed_memory[i].pointer)
        {
            ail_md_freed_memory[i] = ail_md_freed_memory[--ail_md_freed_memory_count];
            break;
        }
    }
}

void *ail_md_malloc(u64 size, char *file, u32 line)
{
    void *pointer;
    u32 i;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    ail_md_mem();
    pointer = malloc(size + AIL_MD_OVER_ALLOC);

#ifdef AIL_MD_MEM_PRINT
    printf("Malloc %6llu bytes at pointer %p at %s line %u\n", size, pointer, file, line);
#endif
    if(pointer == NULL)
    {
        printf("MEM ERROR: Malloc returns NULL when trying to allocate %zu bytes at line %u in file %s\n", size, line, file);
        if(ail_md_alloc_mutex != NULL)
            ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
        ail_md_print(0);
        exit(0);
    }
    for(i = 0; i < size + AIL_MD_OVER_ALLOC; i++)
         ((u8 *)pointer)[i] = AIL_MD_MAGIC_NUM + 1;
    ail_md_add(pointer, size, file, line);

    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    return pointer;
}

void *ail_md_calloc(u64 nelem, u64 elsize, char *file, u32 line)
{
    void *pointer;
    u32 i;
    u64 size = nelem * elsize;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    ail_md_mem();
    pointer = malloc(size + AIL_MD_OVER_ALLOC);

#ifdef AIL_MD_MEM_PRINT
    printf("Calloc %zu bytes at pointer %p at %s line %u\n", size, pointer, file, line);
#endif
    if(pointer == NULL)
    {
        printf("MEM ERROR: Calloc returns NULL when trying to allocate %zu bytes at line %u in file %s\n", size, line, file);
        if(ail_md_alloc_mutex != NULL)
            ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
        ail_md_print(0);
        exit(0);
    }
    for(i = 0; i < size; i++)
         ((u8 *)pointer)[i] = 0;
    for(i = size; i < size + AIL_MD_OVER_ALLOC; i++)
        ((u8 *)pointer)[i] = AIL_MD_MAGIC_NUM + 1;
    ail_md_add(pointer, size, file, line);

    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    return pointer;
}


bool ail_md_remove(void *buf, char *file, u32 line, bool realloc, u64 *size)
{
    STMemFreeBuf  *f;
    u32 i, j, k;

    f = &ail_md_freed_memory[ail_md_freed_memory_current];
    ail_md_freed_memory_current = (ail_md_freed_memory_current + 1) % AIL_MD_FREE_POINTER_BUFFER_SIZE;
    if(ail_md_freed_memory_current > ail_md_freed_memory_count)
        ail_md_freed_memory_count = ail_md_freed_memory_current;
    for(i = 0; i < 255 && file[i] != 0; i++)
        f->free_file[i] = file[i];
    f->free_file[i] = 0;
    f->free_line = line;
    f->realloc = realloc;
    f->pointer = buf;

    for(i = 0; i < ail_md_alloc_line_count; i++)
    {
        for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
        {
            if(ail_md_alloc_lines[i].allocs[j].buf == buf)
            {
                for(k = 0; k < AIL_MD_OVER_ALLOC; k++)
                    if(((u8 *)buf)[ail_md_alloc_lines[i].allocs[j].size + k] != AIL_MD_MAGIC_NUM)
                        break;
                if(k < AIL_MD_OVER_ALLOC)
                {
                    u32 *a = NULL;
                    printf("MEM ERROR: Overshoot at line %u in file %s\n", ail_md_alloc_lines[i].line, ail_md_alloc_lines[i].file);
                    exit(0);
                    a[0] = 0;
                }
                for(k = 0; k < ail_md_alloc_lines[i].allocs[j].size; k++)
                    ((u8 *)buf)[k] = 255;
                f->alloc_line = ail_md_alloc_lines[i].line;
                for(k = 0; k < 255 && ail_md_alloc_lines[i].file[k] != 0; k++)
                    f->alloc_file[k] = ail_md_alloc_lines[i].file[k];
                f->alloc_file[k] = 0;
                f->size = ail_md_alloc_lines[i].allocs[j].size;
                *size = ail_md_alloc_lines[i].allocs[j].size;
                ail_md_alloc_lines[i].size -= ail_md_alloc_lines[i].allocs[j].size;
                ail_md_alloc_lines[i].allocs[j] = ail_md_alloc_lines[i].allocs[--ail_md_alloc_lines[i].alloc_count];
                ail_md_alloc_lines[i].freed++;
                return true;
            }
        }
    }
    for(i = 0; i < ail_md_freed_memory_count; i++)
    {
        if(f != &ail_md_freed_memory[i] && buf == ail_md_freed_memory[i].pointer)
        {
            if(f->realloc)
                printf("MEM ERROR: Pointer %p in file is freed twice! if was freed one line %u in %s, was reallocated to %zu bytes long one line %u in file %s\n", f->pointer, f->free_line, f->free_file, f->size, f->alloc_line, f->alloc_file);
            else
                printf("MEM ERROR: Pointer %p in file is freed twice! if was freed one line %u in %s, was allocated to %zu bytes long one line %u in file %s\n", f->pointer, f->free_line, f->free_file, f->size, f->alloc_line, f->alloc_file);

            return false;
        }
    }
    return true;
}



void ail_md_free(void *buf, char *file, u32 line)
{
    // STMemFreeBuf b;
    u64 size = 0;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    if(!ail_md_remove(buf, file, line, false, &size))
    {
        u32 *X = NULL;
        X[0] = 0;
    }


#ifdef AIL_MD_MEM_PRINT
    printf("Free   %6llu bytes at pointer %p at %s line %u\n", size, buf, file, line);
#endif

    free(buf);
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
}

bool ail_md_comment(void *buf, char *comment)
{
    u32 i, j;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    for(i = 0; i < ail_md_alloc_line_count; i++)
    {
        for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
        {
            if(ail_md_alloc_lines[i].allocs[j].buf == buf)
            {
                ail_md_alloc_lines[i].allocs[j].comment = comment;
                return true;
            }
        }
    }
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    return false;
}


void *ail_md_realloc(void *pointer, u64 size, char *file, u32 line)
{
    u64 i, j = 0, k, move;
    void *pointer2;

    if(pointer == NULL)
        return ail_md_malloc(size, file, line);

    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    for(i = 0; i < ail_md_alloc_line_count; i++)
    {
        for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
            if(ail_md_alloc_lines[i].allocs[j].buf == pointer)
                break;
        if(j < ail_md_alloc_lines[i].alloc_count)
            break;
    }
    if(i == ail_md_alloc_line_count)
    {
        printf("AIL memory debugger error. Trying to reallocate pointer %p in %s line %u. Pointer is not allocated\n", pointer, file, line);
        for(i = 0; i < ail_md_alloc_line_count; i++)
        {
            for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
            {
                u32 *buf;
                buf = ail_md_alloc_lines[i].allocs[j].buf;
                for(k = 0; k < ail_md_alloc_lines[i].allocs[j].size; k++)
                {
                    if(&buf[k] == pointer)
                    {
                        printf("Trying to reallocate pointer %zu bytes (out of %u) in to allocation made in %s on line %u.\n", k, ail_md_alloc_lines[i].allocs[j].size, ail_md_alloc_lines[i].file, ail_md_alloc_lines[i].line);
                    }
                }
            }
        }
        exit(0);
    }
    move = ail_md_alloc_lines[i].allocs[j].size;

    if(move > size)
        move = size;

    pointer2 = malloc(size + AIL_MD_OVER_ALLOC);
    if(pointer2 == NULL)
    {
        printf("MEM ERROR: Realloc returns NULL when trying to allocate %zu bytes at line %u in file %s\n", size, line, file);
        if(ail_md_alloc_mutex != NULL)
            ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
        ail_md_print(0);
        exit(0);
    }
    for(i = 0; i < size + AIL_MD_OVER_ALLOC; i++)
         ((u8 *)pointer2)[i] = AIL_MD_MAGIC_NUM;
    memcpy(pointer2, pointer, move);

    ail_md_add(pointer2, size, file, line);
    move = 0;
    ail_md_remove(pointer, file, line, true, &move);
#ifdef AIL_MD_MEM_PRINT
    printf("Relloc %6llu bytes at pointer %p to %zu bytes at pointer %p at %s line %u\n", move, pointer, size, pointer2, file, line);
#endif
    free(pointer);

    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    return pointer2;
}

void ail_md_print(u32 min_allocs)
{
    u32 i, j;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    printf("Memory repport:\n----------------------------------------------\n");
    for(i = 0; i < ail_md_alloc_line_count; i++)
    {
        if(min_allocs < ail_md_alloc_lines[i].alocated - ail_md_alloc_lines[i].freed)
        {
            printf("%s line: %u\n",ail_md_alloc_lines[i].file, ail_md_alloc_lines[i].line);
            printf(" - Bytes allocated: %zu\n - Allocations: %zu\n - Frees: %zu\n\n", ail_md_alloc_lines[i].size, ail_md_alloc_lines[i].alocated, ail_md_alloc_lines[i].freed);
            for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
                if(ail_md_alloc_lines[i].allocs[j].comment != NULL)
                    printf("\t\t comment %p : %s\n", ail_md_alloc_lines[i].allocs[j].buf, ail_md_alloc_lines[i].allocs[j].comment);
        }
    }
    printf("----------------------------------------------\n");
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
}


u64 ail_md_footprint(u32 min_allocs)
{
    (void)min_allocs;
    u32 i;
    u64 size = 0;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    for(i = 0; i < ail_md_alloc_line_count; i++)
        size += ail_md_alloc_lines[i].size;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    return size;
}

bool ail_md_query(void *pointer, u32 *line, char **file, u64 *size)
{
    u32 i, j;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    for(i = 0; i < ail_md_alloc_line_count; i++)
    {
        for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
        {
            if(ail_md_alloc_lines[i].allocs[j].buf == pointer)
            {
                if(line != NULL)
                    *line = ail_md_alloc_lines[i].line;
                if(file != NULL)
                    *file = ail_md_alloc_lines[i].file;
                if(size != NULL)
                    *size = ail_md_alloc_lines[i].allocs[j].size;
                if(ail_md_alloc_mutex != NULL)
                    ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
                return true;
            }
        }
    }
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    return false;
}

bool ail_md_test(void *pointer, u64 size, bool ignore_not_found)
{
    u32 i, j;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    for(i = 0; i < ail_md_alloc_line_count; i++)
    {
        for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
        {
            if(ail_md_alloc_lines[i].allocs[j].buf >= pointer && ((u8 *)ail_md_alloc_lines[i].allocs[j].buf) + ail_md_alloc_lines[i].allocs[j].size <= (u8*)pointer)
            {
                if(((u8 *)ail_md_alloc_lines[i].allocs[j].buf) + ail_md_alloc_lines[i].allocs[j].size < ((u8 *)pointer) + size)
                {
                    printf("MEM ERROR: Not enough memory to access pointer %p, %zu bytes missing\n", pointer, (u64)(((u8 *)ail_md_alloc_lines[i].allocs[j].buf) + ail_md_alloc_lines[i].allocs[j].size) - (u64)(((u8 *)pointer) + size));
                    if(ail_md_alloc_mutex != NULL)
                        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
                    return true;
                }else
                {
                    if(ail_md_alloc_mutex != NULL)
                        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
                    return false;
                }
            }
        }
    }
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    if(ignore_not_found)
        return false;

    for(i = 0; i < ail_md_freed_memory_count; i++)
    {
        if(pointer >= ail_md_freed_memory[i].pointer && ((u8 *)ail_md_freed_memory[i].pointer) + ail_md_freed_memory[i].size >= ((u8 *)pointer) + size)
        {
            printf("MEM ERROR: Pointer %p was freed on line %u in file %s\n", pointer, ail_md_freed_memory[i].free_line, ail_md_freed_memory[i].free_file);
        }
    }

    printf("MEM ERROR: No matching memory for pointer %p found!\n", pointer);
    return true;
}


u64 ail_md_consumption()
{
    u32 i;
    u64 sum = 0;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
    for(i = 0; i < ail_md_alloc_line_count; i++)
        sum += ail_md_alloc_lines[i].size;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
    return sum;
}

void ail_md_reset()
{
    u32 i;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
#ifdef AIL_MD_MEM_PRINT
    printf("Memmory reset --------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
#endif
    for(i = 0; i < ail_md_alloc_line_count; i++)
        free(ail_md_alloc_lines[i].allocs);
    ail_md_alloc_line_count = 0;
    if(ail_md_alloc_mutex != NULL)
        ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
}

void exit_crash(u32 i)
{
    (void)i;
    u32 *a = NULL;
    a[0] = 0;
}

#endif // _AIL_MD_IMPL_GUARD_
#endif // AIL_NO_MD_IMPL

#ifdef AIL_MD_MEM_DEBUG
#define malloc(n)     ail_md_malloc(n, __FILE__, __LINE__)     // Replaces malloc
#define calloc(n, m)  ail_md_calloc(n, m, __FILE__, __LINE__)  // Replaces calloc
#define realloc(n, m) ail_md_realloc(n, m, __FILE__, __LINE__) // Replaces realloc
#define free(n)       ail_md_free(n, __FILE__, __LINE__)       // Replaces free
#define fopen(n, m)   ail_md_mem_fopen(n, m, __FILE__, __LINE__)
#define fclose(n)     ail_md_mem_fclose(n, __FILE__, __LINE__)
#endif

#ifdef AIL_MD_EXIT
#define exit(n) exit_crash(n) // overwriting exit(n) with a function guarantueed to crash
#endif





////////
// From ail_pm.h
////////


// Pattern Matching
//
// This implementation was lightly inspired by the tiny-regex-c library here: https://github.com/kokke/tiny-regex-c
//
// Currently, regex and glob patterns are supported (see list of supported syntax below)
//
// To match a pattern, you first need to compile it with `ail_pm_compile`
// Then you can match strings against the pattern with `ail_pm_match_greedy` or `ail_pm_match_lazy`
// @TODO: For now, lazy matching is not implemented yet
//
//  *** REGEX Support ***
//  * '.'        Dot, matches any character
//  * '^'        Start anchor, matches beginning of string
//  * '$'        End anchor, matches end of string
//  * '*'        Asterisk, match zero or more
//  * '+'        Plus, match one or more
//  * '?'        Question, match zero or one
//  * '[abc]'    Character class, match if one of {'a', 'b', 'c'}
//  * '[^abc]'   Inverted class, match if NOT one of {'a', 'b', 'c'}
//  * '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
//  * '\s'       Whitespace, \t \f \r \n \v and spaces
//  * '\S'       Non-whitespace
//  * '\w'       Alphanumeric, [a-zA-Z0-9_]
//  * '\W'       Non-alphanumeric
//  * '\d'       Digits, [0-9]
//  * '\D'       Non-digits
//
//  *** GLOB Support ***
//  * '*'        Asterisk, matches any character zero or more times
//  * '?'        Question, matches one or zero characters
//  * '[abc]'    Character class, match if one of {'a', 'b', 'c'}
//  * '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
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

#ifndef AIL_PM_H_
#define AIL_PM_H_

#include "ail_sv.h"

#ifndef AIL_PM_DEF
#   ifdef  AIL_DEF
#       define AIL_PM_DEF AIL_DEF
#   else
#       define AIL_PM_DEF
#   endif // AIL_DEF
#endif // AIL_PM_DEF

#ifndef AIL_PM_DEF_INLINE
#   ifdef  AIL_DEF_INLINE
#       define AIL_PM_DEF_INLINE AIL_DEF_INLINE
#   else
#       define AIL_PM_DEF_INLINE static inline
#   endif // AIL_DEF_INLINE
#endif // AIL_PM_DEF_INLINE


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
AIL_SLICE_INIT(AIL_PM_Range);

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
        AIL_SLICE(char) cs;
        AIL_SLICE(AIL_PM_Range) rs;
    };
} AIL_PM_El;
AIL_DA_INIT(AIL_PM_El);
AIL_SLICE_INIT(AIL_PM_El);

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


global AIL_Allocator ail_pm_tmp_allocator = {
    .data  = NULL,
    .alloc = &ail_default_alloc,
};


AIL_PM_DEF const char* ail_pm_exp_to_str(AIL_PM_Exp_Type type);
AIL_PM_DEF u32   ail_pm_err_to_str(AIL_PM_Err err, char *buf, u32 buflen);
AIL_PM_DEF u32   ail_pm_el_to_str (AIL_PM_El  el,  char *buf, u32 buflen);
AIL_PM_DEF u32   ail_pm_pattern_to_str(AIL_PM_Pattern pattern, char *buf, u32 buflen);

AIL_PM_DEF AIL_PM_Comp_Res ail_pm_compile_a(const char *p, u32 plen, AIL_PM_Exp_Type exp_type, AIL_Allocator allocator);
AIL_PM_DEF void         ail_pm_free_a(AIL_PM_Pattern pattern, AIL_Allocator allocator);
AIL_PM_DEF AIL_PM_Match ail_pm_match (AIL_PM_Pattern pattern, const char *s, u32 slen);
AIL_PM_DEF AIL_PM_Match ail_pm_match_lazy  (AIL_PM_Pattern pattern, const char *s, u32 len);
AIL_PM_DEF AIL_PM_Match ail_pm_match_greedy(AIL_PM_Pattern pattern, const char *s, u32 len);
AIL_PM_DEF b32          ail_pm_matches(AIL_PM_Pattern pattern, const char *s, u32 slen);
AIL_PM_DEF_INLINE b32   ail_pm_match_eq(AIL_PM_Match a, AIL_PM_Match b);
#define ail_pm_compile(p, plen, exp_type) ail_pm_compile_a(p, plen, exp_type, ail_default_allocator)
#define ail_pm_free(pattern)              ail_pm_free_a(pattern, ail_default_allocator)

AIL_PM_DEF AIL_PM_Comp_Res ail_pm_compile_sv_a(AIL_SV pattern, AIL_PM_Exp_Type type, AIL_Allocator allocator);
AIL_PM_DEF AIL_PM_Match ail_pm_match_greedy_sv(AIL_PM_Pattern pattern, AIL_SV sv);
AIL_PM_DEF AIL_PM_Match ail_pm_match_lazy_sv  (AIL_PM_Pattern pattern, AIL_SV sv);
AIL_PM_DEF AIL_PM_Match ail_pm_match_sv  (AIL_PM_Pattern pattern, AIL_SV sv);
AIL_PM_DEF b32          ail_pm_matches_sv(AIL_PM_Pattern pattern, AIL_SV sv);
#define ail_pm_compile_sv(pattern, type) ail_pm_compile_sv_a(pattern, type, ail_default_allocator)

AIL_PM_DEF AIL_PM_Comp_Char_Res _ail_pm_comp_range_char(const char *p, u32 plen, u32 *idx);
AIL_PM_DEF AIL_PM_Comp_El_Res   _ail_pm_comp_range(const char *p, u32 plen, u32 *idx, AIL_Allocator allocator);
AIL_PM_DEF b32 _ail_pm_match_el(AIL_PM_El el, char c);
AIL_PM_DEF u32 _ail_pm_match_immediate_greedy(AIL_PM_El *els, u32 ellen, const char *s, u32 slen);

#endif // AIL_PM_H_


////////////////////
// IMPLEMENTATION //
////////////////////

#if !defined(AIL_NO_PM_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_PM_IMPL_GUARD_
#define _AIL_PM_IMPL_GUARD_

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
            AIL_ASSERT(el.cs.len > 0);
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
    for (u32 i = 0; i < AIL_ARRLEN(ail_pm_attr_strs); i++) {
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
            el.rs   = ail_slice_from_da_t(AIL_PM_Range, ranges);
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
            el.cs   = ail_slice_from_da_t(char, chars);
        }
    }

    *idx = i;
    if (i < plen) { // No missing brackets
        AIL_ASSERT(p[*idx++] == ']');
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

AIL_PM_Comp_Res ail_pm_compile_sv_a(AIL_SV pattern, AIL_PM_Exp_Type type, AIL_Allocator allocator)
{
    return ail_pm_compile_a(pattern.str, pattern.len, type, allocator);
}

void ail_pm_free_a(AIL_PM_Pattern pattern, AIL_Allocator allocator)
{
    AIL_CALL_FREE(allocator, pattern.els);
}

b32 _ail_pm_match_el(AIL_PM_El el, char c)
{
    switch (el.type) {
        case AIL_PM_EL_CHAR:         return el.c == c;
        case AIL_PM_EL_RANGE:        return el.r.start <= c && c <= el.r.end;
        case AIL_PM_EL_ONE_OF_CHAR:  return ail_sv_find_char(ail_sv_from_parts(el.cs.data, el.cs.len), c) >= 0;
        case AIL_PM_EL_ONE_OF_RANGE:
            for (u32 i = 0; i < el.rs.len; i++) {
                AIL_PM_Range r = el.rs.data[i];
                if (r.start <= c && c <= r.end) return true;
            }
            return false;
        case AIL_PM_EL_ANY:          return c != '\n' && c != '\r';
        case AIL_PM_EL_DIGIT:        return ail_sv_is_digit(c);
        case AIL_PM_EL_ALPHA:        return ail_sv_is_alpha(c);
        case AIL_PM_EL_ALPHANUM:     return ail_sv_is_alpha(c) || ail_sv_is_digit(c);
        case AIL_PM_EL_WHITESPACE:   return ail_sv_is_space(c);
        case AIL_PM_EL_COUNT:        AIL_UNREACHABLE();
    }
    return false;
}

u32 _ail_pm_match_immediate_greedy(AIL_PM_El *els, u32 ellen, const char *s, u32 slen)
{
    u32 *stack = AIL_CALL_ALLOC(ail_pm_tmp_allocator, ellen*sizeof(u32));
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
        AIL_ASSERT(el_idx < ellen);
        stack[el_idx++] = n;
    }
done:
    AIL_CALL_FREE(ail_pm_tmp_allocator, stack);
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

AIL_PM_Match ail_pm_match_sv(AIL_PM_Pattern pattern, AIL_SV sv)
{
    return ail_pm_match(pattern, sv.str, sv.len);
}

AIL_PM_Match ail_pm_match_greedy_sv(AIL_PM_Pattern pattern, AIL_SV sv)
{
    return ail_pm_match_greedy(pattern, sv.str, sv.len);
}

AIL_PM_Match ail_pm_match_lazy_sv(AIL_PM_Pattern pattern, AIL_SV sv)
{
    return ail_pm_match_lazy(pattern, sv.str, sv.len);
}

b32 ail_pm_matches(AIL_PM_Pattern pattern, const char *s, u32 slen)
{
    return ail_pm_match(pattern, s, slen).len > 0;
}

b32 ail_pm_matches_sv(AIL_PM_Pattern pattern, AIL_SV sv)
{
    return ail_pm_match(pattern, sv.str, sv.len).len > 0;
}

b32 ail_pm_match_eq(AIL_PM_Match a, AIL_PM_Match b)
{
    AIL_STATIC_ASSERT(sizeof(AIL_PM_Match) == 8);
    return a.len == b.len && (!a.len || a.idx == b.idx);
}

#endif // _AIL_PM_IMPL_GUARD_
#endif // AIL_NO_PM_IMPL





////////
// From ail_ring.h
////////


// Simple Buffer
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

#ifndef AIL_RING_H_
#define AIL_RING_H_


#ifndef AIL_RING_ASSERT
#define AIL_RING_ASSERT AIL_ASSERT
#endif

#ifndef AIL_RING_DEF
#ifdef  AIL_DEF
#define AIL_RING_DEF AIL_DEF
#else
#define AIL_RING_DEF
#endif // AIL_DEF
#endif // AIL_RING_DEF
#ifndef AIL_RING_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_RING_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_RING_DEF_INLINE static inline // AIL_DEF_INLINE
#endif // AIL_DEF_INLINE
#endif // AIL_RING_DEF_INLINE

#if !defined(AIL_RING_FREE) && !defined(AIL_RING_MALLOC)
#if  defined(AIL_MALLOC) &&  defined(AIL_FREE)
#define AIL_RING_MALLOC AIL_MALLOC
#define AIL_RING_FREE   AIL_FREE
#else
#include <stdlib.h>
#define AIL_RING_FREE(ptr)   free(ptr)
#define AIL_RING_MALLOC(sze) malloc(sze)
#endif
#elif !defined(AIL_RING_FREE) || !defined(AIL_RING_MALLOC)
#error "You must define both AIL_RING_MALLOC and AIL_RING_FREE, or neither."
#endif // AIL_RING_FREE / AIL_RING_MALLOC

// @Study whether we should use memcpy
// #ifndef AIL_RING_MEMCPY
// #ifdef AIL_MEMCPY
// #define AIL_RING_MEMCPY AIL_MEMCPY
// #elif
// #include <string.h>
// #define AIL_RING_MEMCPY(dst, src, len) memcpy(dst, src, len)
// #endif
// #endif

typedef struct AIL_RingBuffer {
    u64 start;
    u64 end;
    u64 buffer_size;
    u8 *data;
} AIL_RingBuffer;

AIL_RING_DEF_INLINE AIL_RingBuffer ail_ring_from_data(u8 *buffer, u64 buffer_size);
AIL_RING_DEF_INLINE AIL_RingBuffer ail_ring_new(u64 buffer_size);
AIL_RING_DEF_INLINE void ail_buf_free(AIL_RingBuffer rb);
AIL_RING_DEF u64  ail_ring_len      (AIL_RingBuffer  rb);
AIL_RING_DEF void ail_ring_pop      (AIL_RingBuffer *rb);
AIL_RING_DEF void ail_ring_popn     (AIL_RingBuffer *rb, u64 n);
AIL_RING_DEF u8   ail_ring_peek     (AIL_RingBuffer  rb);
AIL_RING_DEF u8   ail_ring_peek_at  (AIL_RingBuffer  rb, u64 offset);
AIL_RING_DEF u16  ail_ring_peek2msb (AIL_RingBuffer  rb);
AIL_RING_DEF u16  ail_ring_peek2lsb (AIL_RingBuffer  rb);
AIL_RING_DEF u32  ail_ring_peek4msb (AIL_RingBuffer  rb);
AIL_RING_DEF u32  ail_ring_peek4lsb (AIL_RingBuffer  rb);
AIL_RING_DEF u64  ail_ring_peek8msb (AIL_RingBuffer  rb);
AIL_RING_DEF u64  ail_ring_peek8lsb (AIL_RingBuffer  rb);
AIL_RING_DEF void ail_ring_peekn    (AIL_RingBuffer  rb, u64 n, u8 *buf);
AIL_RING_DEF u8    ail_ring_read    (AIL_RingBuffer *rb);
AIL_RING_DEF u16  ail_ring_read2msb (AIL_RingBuffer *rb);
AIL_RING_DEF u16  ail_ring_read2lsb (AIL_RingBuffer *rb);
AIL_RING_DEF u32  ail_ring_read4msb (AIL_RingBuffer *rb);
AIL_RING_DEF u32  ail_ring_read4lsb (AIL_RingBuffer *rb);
AIL_RING_DEF u64  ail_ring_read8msb (AIL_RingBuffer *rb);
AIL_RING_DEF u64  ail_ring_read8lsb (AIL_RingBuffer *rb);
AIL_RING_DEF void ail_ring_readn    (AIL_RingBuffer *rb, u64 n, u8 *buf);
AIL_RING_DEF void ail_ring_write_at (AIL_RingBuffer *rb, u64 offset, u8 x); // @Note: Does not modify rb->end
AIL_RING_DEF void ail_ring_write1   (AIL_RingBuffer *rb, u8 x);
AIL_RING_DEF void ail_ring_readn    (AIL_RingBuffer *rb, u64 n, u8 *buf);
AIL_RING_DEF void ail_ring_write2msb(AIL_RingBuffer *rb, u16 x);
AIL_RING_DEF void ail_ring_write2lsb(AIL_RingBuffer *rb, u16 x);
AIL_RING_DEF void ail_ring_write4msb(AIL_RingBuffer *rb, u32 x);
AIL_RING_DEF void ail_ring_write4lsb(AIL_RingBuffer *rb, u32 x);
AIL_RING_DEF void ail_ring_write8msb(AIL_RingBuffer *rb, u64 x);
AIL_RING_DEF void ail_ring_write8lsb(AIL_RingBuffer *rb, u64 x);
AIL_RING_DEF void ail_ring_writen   (AIL_RingBuffer *rb, u64 n, u8 *buf);

#endif // AIL_RING_H_


#if !defined(AIL_NO_RING_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_RING_IMPL_GUARD_
#define _AIL_RING_IMPL_GUARD_

AIL_RingBuffer ail_ring_from_data(u8 *buffer, u64 buffer_size)
{
    return (AIL_RingBuffer) {
        .data = buffer,
        .buffer_size = buffer_size,
    };
}

AIL_RingBuffer ail_ring_new(u64 buffer_size)
{
    return (AIL_RingBuffer) {
        .data = AIL_RING_MALLOC(buffer_size),
        .buffer_size = buffer_size,
    };
}

void ail_buf_free(AIL_RingBuffer rb)
{
    AIL_RING_FREE((void *)rb.data);
}

u64 ail_ring_len(AIL_RingBuffer rb)
{
    bool wrapped = rb.end < rb.start;
    return (!wrapped)*(rb.end - rb.start) + (wrapped)*(rb.end + rb.buffer_size - rb.start);
}

// Pops first element or does nothing if buffer is empty
void ail_ring_pop(AIL_RingBuffer *rb)
{
    if (rb->end != rb->start) rb->start = (rb->start + 1) % rb->buffer_size;
}

void ail_ring_popn(AIL_RingBuffer *rb, u64 n)
{
    if (ail_ring_len(*rb) < n) rb->start = rb->end = 0;
    else rb->start = (rb->start + n) % rb->buffer_size;
}

// Returns the next byte or 0 if the buffer's length is 0
u8 ail_ring_peek(AIL_RingBuffer rb)
{
    return (rb.end != rb.start)*rb.data[rb.start];
}

u8 ail_ring_peek_at(AIL_RingBuffer rb, u64 offset)
{
    return (ail_ring_len(rb) > offset)*rb.data[(rb.start + offset) % rb.buffer_size];
}

u16 ail_ring_peek2msb(AIL_RingBuffer rb)
{
    return ((u16)ail_ring_peek_at(rb, 0) << 8) | ((u16)ail_ring_peek_at(rb, 1));
}

u16 ail_ring_peek2lsb(AIL_RingBuffer rb)
{
    return ((u16)ail_ring_peek_at(rb, 1) << 8) | ((u16)ail_ring_peek_at(rb, 0));
}

u32 ail_ring_peek4msb(AIL_RingBuffer rb)
{
    return ((u32)ail_ring_peek_at(rb, 0) << 24) | ((u32)ail_ring_peek_at(rb, 1) << 16) | ((u32)ail_ring_peek_at(rb, 2) << 8) | ((u32)ail_ring_peek_at(rb, 3));
}

u32 ail_ring_peek4lsb(AIL_RingBuffer rb)
{
    return ((u32)ail_ring_peek_at(rb, 3) << 24) | ((u32)ail_ring_peek_at(rb, 2) << 16) | ((u32)ail_ring_peek_at(rb, 1) << 8) | ((u32)ail_ring_peek_at(rb, 0));
}

u64 ail_ring_peek8msb(AIL_RingBuffer rb)
{
    return ((u64)ail_ring_peek_at(rb, 0) << 7*8) | ((u64)ail_ring_peek_at(rb, 1) << 6*8) | ((u64)ail_ring_peek_at(rb, 2) << 5*8) | ((u64)ail_ring_peek_at(rb, 3) << 4*8) |
           ((u64)ail_ring_peek_at(rb, 4) << 3*8) | ((u64)ail_ring_peek_at(rb, 5) << 2*8) | ((u64)ail_ring_peek_at(rb, 6) << 1*8) | ((u64)ail_ring_peek_at(rb, 7) << 0*8);
}

u64 ail_ring_peek8lsb(AIL_RingBuffer rb)
{
    return ((u64)ail_ring_peek_at(rb, 7) << 7*8) | ((u64)ail_ring_peek_at(rb, 6) << 6*8) | ((u64)ail_ring_peek_at(rb, 5) << 5*8) | ((u64)ail_ring_peek_at(rb, 4) << 4*8) |
           ((u64)ail_ring_peek_at(rb, 3) << 3*8) | ((u64)ail_ring_peek_at(rb, 2) << 2*8) | ((u64)ail_ring_peek_at(rb, 1) << 1*8) | ((u64)ail_ring_peek_at(rb, 0) << 0*8);
}

void ail_ring_peekn(AIL_RingBuffer rb, u64 n, u8 *buf)
{
    // @TODO: Use AIL_RING_MEMCPY instead maybe?
    for (u64 i = 0, j = rb.start; i < n; i++, j = (j+1)%rb.buffer_size) {
        buf[i] = rb.data[j];
        // @Safety @Performance
        // @Note: The more safe version would set the value to 0 if it's out of bounds
        // buf[i] = rb.data[j]*(j < rb.end || (rb.end < rb.start && rb.start <= j));
    }
}

u8 ail_ring_read(AIL_RingBuffer *rb)
{
    u8 res = ail_ring_peek(*rb);
    ail_ring_popn(rb, 1);
    return res;
}

u16 ail_ring_read2msb(AIL_RingBuffer *rb)
{
    u16 res = ail_ring_peek2msb(*rb);
    ail_ring_popn(rb, 2);
    return res;
}

u16 ail_ring_read2lsb(AIL_RingBuffer *rb)
{
    u16 res = ail_ring_peek2lsb(*rb);
    ail_ring_popn(rb, 2);
    return res;
}

u32 ail_ring_read4msb(AIL_RingBuffer *rb)
{
    u32 res = ail_ring_peek4msb(*rb);
    ail_ring_popn(rb, 4);
    return res;
}

u32 ail_ring_read4lsb(AIL_RingBuffer *rb)
{
    u32 res = ail_ring_peek4lsb(*rb);
    ail_ring_popn(rb, 4);
    return res;
}

u64 ail_ring_read8msb(AIL_RingBuffer *rb)
{
    u64 res = ail_ring_peek8msb(*rb);
    ail_ring_popn(rb, 8);
    return res;
}

u64 ail_ring_read8lsb(AIL_RingBuffer *rb)
{
    u64 res = ail_ring_peek8lsb(*rb);
    ail_ring_popn(rb, 8);
    return res;
}

void ail_ring_readn(AIL_RingBuffer *rb, u64 n, u8 *buf)
{
    ail_ring_peekn(*rb, n, buf);
    ail_ring_popn(rb, n);
}

void ail_ring_write_at(AIL_RingBuffer *rb, u64 offset, u8 x)
{
    rb->data[(rb->end + offset)%rb->buffer_size] = x;
    AIL_RING_ASSERT((rb->end < rb->start) == (rb->end + offset < rb->start));
}

void ail_ring_write1(AIL_RingBuffer *rb, u8 x)
{
    ail_ring_write_at(rb, 0, x);
    rb->end = (rb->end + 1)%rb->buffer_size;
}

void ail_ring_write2msb(AIL_RingBuffer *rb, u16 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 0*8));
    rb->end = (rb->end + 2)%rb->buffer_size;
}

void ail_ring_write2lsb(AIL_RingBuffer *rb, u16 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 0*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 1*8));
    rb->end = (rb->end + 2)%rb->buffer_size;
}

void ail_ring_write4msb(AIL_RingBuffer *rb, u32 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 3*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 2*8));
    ail_ring_write_at(rb, 2, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 3, (u8)(x >> 0*8));
    rb->end = (rb->end + 4)%rb->buffer_size;
}

void ail_ring_write4lsb(AIL_RingBuffer *rb, u32 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 0*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 2, (u8)(x >> 2*8));
    ail_ring_write_at(rb, 3, (u8)(x >> 3*8));
    rb->end = (rb->end + 4)%rb->buffer_size;
}

void ail_ring_write8msb(AIL_RingBuffer *rb, u64 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 7*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 6*8));
    ail_ring_write_at(rb, 2, (u8)(x >> 5*8));
    ail_ring_write_at(rb, 3, (u8)(x >> 4*8));
    ail_ring_write_at(rb, 4, (u8)(x >> 3*8));
    ail_ring_write_at(rb, 5, (u8)(x >> 2*8));
    ail_ring_write_at(rb, 6, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 7, (u8)(x >> 0*8));
    rb->end = (rb->end + 8)%rb->buffer_size;
}

void ail_ring_write8lsb(AIL_RingBuffer *rb, u64 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 0*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 2, (u8)(x >> 2*8));
    ail_ring_write_at(rb, 3, (u8)(x >> 3*8));
    ail_ring_write_at(rb, 4, (u8)(x >> 4*8));
    ail_ring_write_at(rb, 5, (u8)(x >> 5*8));
    ail_ring_write_at(rb, 6, (u8)(x >> 6*8));
    ail_ring_write_at(rb, 7, (u8)(x >> 7*8));
    rb->end = (rb->end + 8)%rb->buffer_size;
}

void ail_ring_writen(AIL_RingBuffer *rb, u64 n, u8 *buf)
{
    for (u64 i = 0; i < n; i++, rb->end = (rb->end+1)%rb->buffer_size) {
        rb->data[rb->end] = buf[i];

        u8 tmp = (rb->end+1)%rb->buffer_size;
        AIL_RING_ASSERT(tmp != rb->start);
    }
}


#endif // _AIL_RING_IMPL_GUARD_
#endif // AIL_NO_RING_IMPL





////////
// From ail_sv.h
////////


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
#define ail_str_new_sv(sv)        ail_str_new_sv_a(sv,    ail_default_allocator)
#define ail_str_new_cstr(str)     ail_str_new_cstr_a(str, ail_default_allocator)
#define ail_str_new_da(str)       ail_str_new_da_a(str,   ail_default_allocator)
#define ail_str_new_unsigned(num) ail_str_new_unsigned_a(num, ail_default_allocator)
#define ail_str_new_signed(num)   ail_str_new_signed_a(num,   ail_default_allocator)
#define ail_str_new_float(num)    ail_str_new_float_a(num,    ail_default_allocator)

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

AIL_SV_DEF_INLINE AIL_SB ail_sb_from_parts(char *data, u64 len, u64 cap, AIL_Allocator allocator);
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
#define ail_sv_join(list, n, joiner)     ail_sv_join_a(list, n, joiner, ail_default_allocator)
#define ail_sv_rev_join(list, n, joiner) ail_sv_rev_join_a(list, n, joiner, ail_default_allocator)
#define ail_sv_join_da(list, joiner)     ail_sv_join_a((list).data, (list).len, joiner, ail_default_allocator)
#define ail_sv_rev_join_da(list, joiner) ail_sv_rev_join_da_a((list).data, (list).len, joiner, ail_default_allocator)

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
AIL_SV_DEF AIL_Str ail_sv_concat2_full_a(char *str1, u64 len1, char *str2, u64 len2, AIL_Allocator allocator);
#define ail_sv_concat2_full(str1, len1, str2, len2) ail_sv_concat2_full_a(str1, len1, str2, len2, ail_default_allocator)
#define ail_sv_concat2_a(sv1, sv2, allocator) ail_sv_concat2_full_a((sv1).str, (sv1).len, (sv2).str, (sv2).len, allocator)
#define ail_sv_concat2(sv1, sv2) ail_sv_concat2_full_a((sv1).str, (sv1).len, (sv2).str, (sv2).len, ail_default_allocator)

// Concatenate an arbitrary amount of String-Views to a single String
// @Important: To avoid memory leaks, make sure to free the underlying string
// @Note: `sv_count` refers to the amount of string-length pairs that follow. So you must supply 2*n further arguments after n
AIL_SV_DEF AIL_Str ail_sv_concat_full_a(AIL_Allocator allocator, u32 sv_count, ...);
#define ail_sv_concat_full(str1, len1, str2, len2, ...) ail_sv_concat_full_a(ail_default_allocator, 2 + (AIL_VA_LEN(__VA_ARGS__)/2), (str1), (len1), (str2), (len2), __VA_ARGS__)

AIL_SV_DEF AIL_Str ail_sv_concat_list_full_a(AIL_Allocator allocator, u32 sv_count, AIL_SV *svs);
#define ail_sv_concat_list_full(sv_count, svs) ail_sv_concat_list_full_a(ail_default_allocator, (sv_count), (svs))
#define ail_sv_concat_list_a(allocator, sv_list) ail_sv_concat_list_full_a((allocator), (sv_list).len, (sv_list).data)
#define ail_sv_concat_list(sv_list) ail_sv_concat_list_full_a(ail_default_allocator, (sv_list).len, (sv_list).data)

AIL_SV_DEF AIL_Str ail_sv_concat_a(AIL_Allocator allocator, u32 sv_count, ...);
#define ail_sv_concat(sv1, sv2, ...) ail_sv_concat_a(ail_default_allocator, 2 + AIL_VA_LEN(__VA_ARGS__), (sv1), (sv2), __VA_ARGS__)

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

#if !defined(AIL_NO_SV_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_SV_IMPL_GUARD_
#define _AIL_SV_IMPL_GUARD_
#include <stdarg.h> // For va_<>

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

AIL_SB ail_sb_from_parts(char *data, u64 len, u64 cap, AIL_Allocator allocator)
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
    if (str.len < 2048) AIL_NEXT_2POWER_POS(str.len, cap);
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
    for (u64 i = 0; i < alen; i++) {
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
    for (u64 i = 0; i < prefix.len; i++) {
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
    for (u64 i = suffix.len; i > 0; i--) {
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

AIL_Str ail_sv_concat2_full_a(char *astr, u64 alen, char *bstr, u64 blen, AIL_Allocator allocator)
{
    char *s = AIL_CALL_ALLOC(allocator, alen + blen + 1);
    memcpy(&s[0],     astr, alen);
    memcpy(&s[alen], bstr, blen);
    s[alen + blen] = 0;
    return ail_str_from_parts(s, alen + blen);
}

AIL_Str ail_sv_concat_list_full_a(AIL_Allocator allocator, u32 sv_count, AIL_SV *svs)
{
    u64 size = 0;
    for (u32 i = 0; i < sv_count; i++) {
        size += svs[i].len;
    }
    char *s = AIL_CALL_ALLOC(allocator, size + 1);
    u64 filled_count = 0;
    for (u32 i = 0; i < sv_count; i++) {
        memcpy(s + filled_count, svs[i].str, svs[i].len);
        filled_count += svs[i].len;
    }
    AIL_ASSERT(filled_count == size);
    s[filled_count] = 0;
    return ail_str_from_parts(s, filled_count);
}

AIL_Str ail_sv_concat_full_a(AIL_Allocator allocator, u32 sv_count, ...)
{
    va_list args;
    va_start(args, sv_count);
    u64 size = 0;
    for (u32 i = 0; i < sv_count; i++) {
        va_arg(args, char*);
        size += va_arg(args, u64);
    }
    va_end(args);
    char *s = AIL_CALL_ALLOC(allocator, size + 1);
    u64 filled_count = 0;
    va_start(args, sv_count);
    for (u32 i = 0; i < sv_count; i++) {
        char *str = va_arg(args, char*);
        u64   len = va_arg(args, u64);
        memcpy(s + filled_count, str, len);
        filled_count += len;
    }
    va_end(args);
    AIL_ASSERT(filled_count == size);
    s[filled_count] = 0;
    return ail_str_from_parts(s, filled_count);
}

AIL_Str ail_sv_concat_a(AIL_Allocator allocator, u32 sv_count, ...)
{
    va_list args;
    va_start(args, sv_count);
    u64 size = 0;
    for (u32 i = 0; i < sv_count; i++) {
        AIL_SV sv = va_arg(args, AIL_SV);
        size += sv.len;
    }
    va_end(args);
    char *s = AIL_CALL_ALLOC(allocator, size + 1);
    u64 filled_count = 0;
    va_start(args, sv_count);
    for (u32 i = 0; i < sv_count; i++) {
        AIL_SV sv = va_arg(args, AIL_SV);
        memcpy(s + filled_count, sv.str, sv.len);
        filled_count += sv.len;
    }
    va_end(args);
    AIL_ASSERT(filled_count == size);
    s[filled_count] = 0;
    return ail_str_from_parts(s, filled_count);
}

AIL_Str ail_sv_replace_a(AIL_SV sv, AIL_SV to_replace, AIL_SV replace_with, AIL_Allocator allocator)
{
    AIL_DA(AIL_SV) list = ail_sv_split_a(sv, to_replace, false, allocator);
    AIL_Str out = ail_sv_join_a(list.data, list.len, replace_with, allocator);
    ail_da_free(&list);
    return out;
}

#endif // _AIL_SV_IMPL_GUARD_
#endif // AIL_NO_SV_IMPL





////////
// From ail_time.h
////////


// Time-related functions
//
// LICENSE
/*
Copyright (c) 2024 Lily Val Richter

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

#ifndef AIL_TIME_H_
#define AIL_TIME_H_


#include <time.h> // For clock

#ifndef AIL_TIME_DEF
#ifdef  AIL_DEF
#define AIL_TIME_DEF AIL_DEF
#else
#define AIL_TIME_DEF
#endif // AIL_DEF
#endif // AIL_TIME_DEF
#ifndef AIL_TIME_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_TIME_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_TIME_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_TIME_DEF_INLINE

// Sleep `msecs` milliseconds
AIL_TIME_DEF void ail_time_sleep(u64 msecs);

// Time in seconds here
AIL_TIME_DEF_INLINE f64 ail_time_clock_start(void);
AIL_TIME_DEF_INLINE f64 ail_time_clock_elapsed(f64 start);

#endif // AIL_TIME_H_


#if !defined(AIL_NO_TIME_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_TIME_GUARD_
#define _AIL_TIME_GUARD_

#if AIL_OS_WIN32
#   include <windows.h> // For Sleep
#   define AIL_TIME_FLAG_WINSLEEP
#elif _POSIX_C_SOURCE >= 199309L
#   define AIL_TIME_FLAG_NANOSLEEP
#   include <time.h>    // for nanosleep
#else
#   define AIL_TIME_FLAG_USLEEP
#   include <unistd.h>  // for usleep
#endif

void ail_time_sleep(u64 msecs)
{
#if defined(AIL_TIME_FLAG_NANOSLEEP)
    struct timespec ts;
    ts.tv_sec = msecs / 1000;
    ts.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep(&ts, NULL);
#elif defined(AIL_TIME_FLAG_WINSLEEP)
    Sleep(msecs);
#elif defined(AIL_TIME_FLAG_USLEEP)
    if (msecs >= 1000) sleep(msecs / 1000);
    usleep((msecs % 1000) * 1000);
#else
#    error "Missing feature flag not implemented yet"
#endif
}

f64 ail_time_clock_start(void)
{
#if AIL_OS_WIN32
    return (f64)timeGetTime() / 1000;
#elif AIL_OS_POSIX
    struct timespec ts = {0};
    int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    AIL_ASSERT(ret == 0);
    return (f64)ts.tv_sec + ts.tv_nsec*1e-9;
#else
    return (f64)clock() / CLOCKS_PER_SEC;
#endif
}

f64 ail_time_clock_elapsed(f64 start)
{
    return ail_time_clock_start() - start;
}


#endif // _AIL_TIME_GUARD_
#endif // AIL_NO_TIME_IMPL
