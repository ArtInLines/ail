/*
* This header contains general utilities used throughout the other ail.h libraries
*
* By default this file includes all implementation alongside the declarations. To not include any implementation, define the following before including this file
  * AIL_NO_IMPL:        include no implementations from any of the ail_* libraries included
  * AIL_NO_ENDIAN_IMPL: include no implementation for endianness
  * AIL_NO_ALLOC_IMPL:  include no implementation for allocators
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
* Heavily inspired by (and adapted code of) the following:
  * https://sourceforge.net/p/predef/wiki/Home/
  * https://github.com/nemequ/hedley
  * http://web.archive.org/web/20160306052035/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
  * https://github.com/bjoernknafla/poc
* Each of the following macros is defined to be either 1 or 0 (except AIL_LANG_STANDARD)
* Architecture Size (aka size of pointers):
  * AIL_32BIT
  * AIL_64BIT
* Operating System (several can be true at once):
  * AIL_OS_WIN
  * AIL_OS_LINUX
  * AIL_OS_UNIX
  * AIL_OS_POSIX
  * AIL_OS_GNU
  * AIL_OS_MAC
  * AIL_OS_IOS
  * AIL_OS_ANDROID
  * AIL_OS_WASM
  * AIL_OS_BSD
  * AIL_OS_MINGW
  * AIL_OS_CYGWIN
  * AIL_OS_SPARC
* CPU Architecture (at most one can be true):
  * AIL_ARCH_X86   - https://en.wikipedia.org/wiki/X86
  * AIL_ARCH_ARM   - https://en.wikipedia.org/wiki/ARM_architecture_family
  * AIL_ARCH_MIPS  - https://en.wikipedia.org/wiki/MIPS_architecture
  * AIL_ARCH_RISCV - https://en.wikipedia.org/wiki/RISC-V
  * AIL_ARCH_SPARC - https://en.wikipedia.org/wiki/SPARC
  * AIL_ARCH_PPC   - https://en.wikipedia.org/wiki/PowerPC
* Compiler (at most one is true):
  * AIL_COMP_CLANG        - https://clang.llvm.org/
  * AIL_COMP_MSVC         - https://learn.microsoft.com/en-us/cpp/build/reference/compiling-a-c-cpp-program
  * AIL_COMP_GCC          - https://gcc.gnu.org/
  * AIL_COMP_TCC          - https://www.bellard.org/tcc/
  * AIL_COMP_PELLES       - http://www.smorgasbordet.com/pellesc/
  * AIL_COMP_DIGITAL_MARS - https://digitalmars.com/
  * AIL_COMP_INTEL        - https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html
  * AIL_COMP_EMSCRIPTEN   - https://emscripten.org/
* Language (exactly one is true):
  * AIL_LANG_C
  * AIL_LANG_CPP
  * AIL_LANG_OBJC
* Language Standard: AIL_LANG_STANDARD
  * AIL_LANG_STANDARD is defined to be the year of the standard's release (as a number)
  * i.e. to test for C11+:
    * #if AIL_LANG_C && AIL_LANG_STANDARD >= 2011
  * to test for C++17+:
    * #if AIL_LANG_CPP && AIL_LANG_STANDARD >= 2017
*
*
*** Endianness ***
* Endianness describes the order of bytes in a word/double-word.
* The Enum AIL_Endian contains the different types of endianness recognized by ail.h
* Detecting the platform's endianness cannot be done portably & reliably at compile-time (see https://sourceforge.net/p/predef/wiki/Endianness/)
* The following functions are provided to work with different endiannesses:
  * ail_endian: Detect the platform's endianness
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
  * AIL_HAS_WARNING(warning):         Check whether the provided warning is active. The warning must be one of the AIL_Warn_Kind enum
  *
  * AIL_WARN_PUSH: Store the current warning level (presumably to change it temporarily)
  * AIL_WARN_POP:  Reset the latest saved warning level
  * AIL_WARN_ENABLE(warning):  Enable warning for the specified warning
  * AIL_WARN_DISABLE(warning): Disable warning for the specified warning
  * AIL_WARN_ERROR(wanring):   Set the specified warning as an error
  * AIL_Warn_Kind is an enum containing all the warnings you can specify in macros like AIL_WARN_ENABLE
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
// Platform Identification Macros
/////////////////////////

#include <stdint.h> // For INTPTR_MAX, INT64_MAX, INT32_MAX
#if INTPTR_MAX == INT32_MAX
#   define AIL_32BIT 1
#   define AIL_64BIT 0
#elif INTPTR_MAX == INT64_MAX
#   define AIL_32BIT 0
#   define AIL_64BIT 1
#else
#   error "Compiling on a platform that is neither 32 nor 64 bit. Please change ail.h if you wish to compile on such a platform"
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

#if defined(__OBJC__)
#   define AIL_LANG_OBJC 1
#   define AIL_LANG_CPP  0
#   define AIL_LANG_C    0
    // @Note: I do not know of any way to get the version for Objective-C from within C
#   define AIL_LANG_STANDARD 1
#elif defined(__cplusplus)
#   define AIL_LANG_OBJC 0
#   define AIL_LANG_CPP  1
#   define AIL_LANG_C    0
#   if __cplusplus == 1
#       define AIL_LANG_STANDARD 1985
#   elif __cplusplus == 199711L
#       define AIL_LANG_STANDARD 1998
#   elif __cplusplus == 201103L
#       define AIL_LANG_STANDARD 2011
#   elif __cplusplus == 201402L
#       define AIL_LANG_STANDARD 2014
#   elif __cplusplus == 201703L
#       define AIL_LANG_STANDARD 2017
#   elif __cplusplus == 202002L
#       define AIL_LANG_STANDARD 2020
#   elif __cplusplus >= 202302L
#       define AIL_LANG_STANDARD 2023
#   else
#       define AIL_LANG_STANDARD 1989
#   endif
#else
#   define AIL_LANG_OBJC 0
#   define AIL_LANG_CPP 0
#   define AIL_LANG_C   1
#   if __STDC_VERSION__ == 199901L
#       define AIL_LANG_STANDARD 1999
#   elif __STDC_VERSION__ == 201112L
#       define AIL_LANG_STANDARD 2011
#   elif __STDC_VERSION__ == 201710L
#       define AIL_LANG_STANDARD 2017
#   elif __STDC_VERSION__ >= 202311L
#       define AIL_LANG_STANDARD 2023
#   else
#       define AIL_LANG_STANDARD 1989
#   endif
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
#	define AIL_OS_WIN 1
#else
#   define AIL_OS_WIN 0
#endif
#if defined(__linux__) || defined(__linux)
#	define AIL_OS_LINUX 1
#else
#   define AIL_OS_LINUX 0
#endif
#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
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
// @Note: There are many more target informations that could be retrieved from TargetConditionals.h
// See the following for more info:
// - https://epir.at/2019/10/30/api-availability-and-target-conditionals/
// - https://github.com/xybp888/iOS-SDKs/blob/master/iPhoneOS13.0.sdk/usr/include/TargetConditionals.h
#   include <TargetConditionals.h>
#   if TARGET_OS_IPHONE // iOS, tvOS, or watchOS device
#       define AIL_OS_MAC 0
#       define AIL_OS_IOS 1
#   elif TARGET_OS_MAC
#       define AIL_OS_MAC 1
#       define AIL_OS_IOS 0
#   else
#      define AIL_OS_MAC 0
#      define AIL_OS_IOS 0
#   endif
#else
#   define AIL_OS_MAC 0
#   define AIL_OS_IOS 0
#endif
#if defined(__ANDROID__)
#	define AIL_OS_ANDROID 1
#else
#   define AIL_OS_ANDROID 0
#endif
#if defined(__wasm__) || defined(__wasm32__) || defined(__wasm64__)
#	define AIL_OS_WASM 1
#else
#   define AIL_OS_WASM 0
#endif
#if defined(BSD)
#	define AIL_OS_BSD 1
#else
#   define AIL_OS_BSD 0
#endif
#if defined(__MINGW32__) || defined(__MINGW64__)
#	define AIL_OS_MINGW 1
#else
#   define AIL_OS_MINGW 0
#endif
#if defined(__CYGWIN__)
#	define AIL_OS_CYGWIN 1
#else
#   define AIL_OS_CYGWIN 0
#endif
#if defined(__sparc__) || defined(__sparc)
#	define AIL_OS_SPARC   1
#   define AIL_ARCH_SPARC 1
#else
#   define AIL_OS_SPARC   0
#   define AIL_ARCH_SPARC 0
#endif

#if defined(__i386) || defined(_M_IX86) || defined(_X86_) || defined(__X86__) || defined(__I86__) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || defined(_M_AMD64) || defined(_M_X64)
#	define AIL_ARCH_X86 1
#else
#   define AIL_ARCH_X86 0
#endif
#if defined(__arm__) || defined(_M_ARM) || defined(_ARM) || defined(__aarch64__)
#	define AIL_ARCH_ARM 1
#else
#   define AIL_ARCH_ARM 0
#endif
#if defined(__mips) || defined(__mips__) || defined(__MIPS__)
#	define AIL_ARCH_MIPS 1
#else
#   define AIL_ARCH_MIPS 0
#endif
#if defined(__riscv) || defined(__riscv64)
#	define AIL_ARCH_RISCV 1
#else
#   define AIL_ARCH_RISCV 0
#endif
#if defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__PPC) || defined(__PPC__) || defined(__PPC64__) || defined(__ppc__) || defined(__ppc64__) || defined(__POWERPC__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64)
#	define AIL_ARCH_PPC 1
#else
#   define AIL_ARCH_PPC 0
#endif



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

#define internal static
#define persist  static
#define global   static



/////////////////////////
// Endianness
// Disable Implementation with `AIL_NO_ENDIAN_IMPL`
/////////////////////////
typedef enum AIL_Endian {
    AIL_ENDIAN_UNKNOWN = 0,
    AIL_ENDIAN_BIG,          // Most-Significant Byte first
    AIL_ENDIAN_LITTLE,       // Least-Significant Byte first
    AIL_ENDIAN_BIG_WORD,     // Big-Endian for Words, Little-Endian for Double-Words
    AIL_ENDIAN_LITTLE_WORD,  // Little-Endian for Words, Big-Endian for Double-Words
    AIL_ENDIAN_COUNT,
} AIL_Endian;
AIL_DEF AIL_Endian ail_endian(void);


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

// Most of the following macros that deal with providing a unified interface for compiler extensions across compilers
// This was to a large part adapted from Hedley: https://github.com/nemequ/hedley
// The original Hedley project supports even more compilers as well as older C versions
#define _AIL_VERSION_ENCODE_(major, minor, patch) (((major) * 1000000) + ((minor) * 1000) + (patch))
#define _AIL_VERSION_CHECK_(version, major, minor, patch) ((version) >= _AIL_VERSION_ENCODE_(major, minor, patch))
// _AIL_VERSION_GCC_
#if AIL_COMP_GCC && defined(__GNUC__)
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

// See also: https://gcc.gnu.org/onlinedocs/cpp/Pragmas.html
#define AIL_DO_PRAGMA_(x) _Pragma(#x)
#define AIL_DO_PRAGMA(x) AIL_DO_PRAGMA_(x)

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

// AIL_WARN_ENABLE
#define AIL_WARN_ENABLE(warning_name)  AIL_EXPAND(AIL_CONCAT(_AIL_WARN_ENABLE_,  warning_name))
#define AIL_WARN_DISABLE(warning_name) AIL_EXPAND(AIL_CONCAT(_AIL_WARN_DISABLE_, warning_name))
#define AIL_WARN_ERROR(warning_name)   AIL_EXPAND(AIL_CONCAT(_AIL_WARN_ERROR_,   warning_name))

// #if AIL_COMP_CLANG
// #   define AIL_WARN_ENABLE(warning_name) AIL_DO_PRAGMA(clang diagnostic warning AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name))
// #elif AIL_COMP_GCC
// #   define AIL_WARN_ENABLE(warning_name) AIL_DO_PRAGMA(GCC diagnostic warning AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name))
// #elif AIL_COMP_MSVC
// #   define AIL_WARN_ENABLE(warning_name) AIL_DO_PRAGMA(warning(AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #elif AIL_COMP_INTEL
// #   define AIL_WARN_ENABLE(warning_name) AIL_DO_PRAGMA(warning(AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #elif AIL_COMP_PELLES
// #   define AIL_WARN_ENABLE(warning_name) AIL_DO_PRAGMA(warn(AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #   define AIL_WARN_ENABLE(warning_name)
// #endif
// // AIL_WARN_DISABLE
// #if AIL_COMP_CLANG
// #   define AIL_WARN_DISABLE(warning_name) AIL_DO_PRAGMA(clang diagnostic ignored AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name))
// #elif AIL_COMP_GCC
// #   define AIL_WARN_DISABLE(warning_name) AIL_DO_PRAGMA(GCC diagnostic ignored AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name))
// #elif AIL_COMP_MSVC
// #   define AIL_WARN_DISABLE(warning_name) AIL_DO_PRAGMA(warning(disable:AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #elif AIL_COMP_INTEL
// #   define AIL_WARN_DISABLE(warning_name) AIL_DO_PRAGMA(warning(disable:AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #elif AIL_COMP_PELLES
// #   define AIL_WARN_DISABLE(warning_name) AIL_DO_PRAGMA(warn(disable:AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #   define AIL_WARN_DISABLE(warning_name)
// #endif
// // AIL_WARN_ERROR
// #if AIL_COMP_CLANG
// #   define AIL_WARN_ERROR(warning_name) AIL_DO_PRAGMA(clang diagnostic error AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name))
// #elif AIL_COMP_GCC
// #   define AIL_WARN_ERROR(warning_name) AIL_DO_PRAGMA(GCC diagnostic error AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name))
// #elif AIL_COMP_MSVC
// #   define AIL_WARN_ERROR(warning_name) AIL_DO_PRAGMA(warning(error:AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #elif AIL_COMP_INTEL
// #   define AIL_WARN_ERROR(warning_name) AIL_DO_PRAGMA(warning(error:AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #elif AIL_COMP_PELLES
// #   define AIL_WARN_ERROR(warning_name) AIL_DO_PRAGMA(warn(error:AIL_EXPAND(_AIL_WARN_KIND_NAME_##warning_name)))
// #   define AIL_WARN_ERROR(warning_name)
// #endif

// AIL_Warn_Kinds
typedef enum AIL_Warn_Kinds {
    AIL_WARN_NONE,
    AIL_WARN_ALL,
    AIL_WARN_EXHAUSTIVE_SWITCH,
    AIL_WARN_UNSAFE_CONVERSION,
    AIL_WARN_MISSING_ARGLIST,
    AIL_WARN_IFDEF_0_MACRO,
    AIL_WARN_SECTION_NAME_TOO_LONG,
    AIL_WARN_PRINTF_FORMTAT,
    AIL_WARN_SIGN_EXTENDED_CONVERSION,
    AIL_WARN_STRING_CAST,
    AIL_WARN_UNBALANCED_PUSH_POP,
    AIL_WARN_DEPRECATED,
    AIL_WARN_UNKNOWN_PRAGMA,
    AIL_WARN_UNKNOWN_MACRO,
    AIL_WARN_UNUSED_FUNCTION,
    AIL_WARN_UNUSED_VARIABLE,
    AIL_WARN_ZERO_LENGTH_ARRAY,
    AIL_WARN_CAST_QUAL,
    AIL_WARN_IMPLICIT_INT,
    AIL_WARN_UNREACHABLE_CODE,
    AIL_WARN_COUNT,
} AIL_Warn_Kinds;

#if AIL_COMP_CLANG
#   define _AIL_WARN_ENABLE_AIL_WARN_NONE
#   define _AIL_WARN_DISABLE_AIL_WARN_NONE
#   define _AIL_WARN_ERROR_AIL_WARN_NONE
#   define _AIL_WARN_ENABLE_AIL_WARN_ALL
#   define _AIL_WARN_DISABLE_AIL_WARN_ALL
#   define _AIL_WARN_ERROR_AIL_WARN_ALL
#   define _AIL_WARN_ENABLE_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_DISABLE_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_ERROR_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_ENABLE_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_DISABLE_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_ERROR_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_ENABLE_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_DISABLE_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_ERROR_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_ENABLE_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_DISABLE_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_ERROR_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_ENABLE_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_DISABLE_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_ERROR_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_ENABLE_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_DISABLE_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_ERROR_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_ENABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_DISABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_ERROR_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_ENABLE_AIL_WARN_STRING_CAST
#   define _AIL_WARN_DISABLE_AIL_WARN_STRING_CAST
#   define _AIL_WARN_ERROR_AIL_WARN_STRING_CAST
#   define _AIL_WARN_ENABLE_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_DISABLE_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_ERROR_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_ENABLE_AIL_WARN_DEPRECATED _Pragma("clang diagnostic warning \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_DEPRECATED _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_ERROR_AIL_WARN_DEPRECATED _Pragma("clang diagnostic error \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("clang diagnostic warning \"-Wunknown-pragmas\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_PRAGMA _Pragma("clang diagnostic error \"-Wunknown-pragmas\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("clang diagnostic warning \"-Wunused-function\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("clang diagnostic ignored \"-Wunused-function\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_FUNCTION _Pragma("clang diagnostic error \"-Wunused-function\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("clang diagnostic warning \"-Wunused-variable\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("clang diagnostic ignored \"-Wunused-variable\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_VARIABLE _Pragma("clang diagnostic error \"-Wunused-variable\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("clang diagnostic warning \"-Wzero-length-array\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("clang diagnostic ignored \"-Wzero-length-array\"")
#   define _AIL_WARN_ERROR_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("clang diagnostic error \"-Wzero-length-array\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_CAST_QUAL _Pragma("clang diagnostic warning \"-Wcast-qual\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_CAST_QUAL _Pragma("clang diagnostic ignored \"-Wcast-qual\"")
#   define _AIL_WARN_ERROR_AIL_WARN_CAST_QUAL _Pragma("clang diagnostic error \"-Wcast-qual\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_DISABLE_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_ERROR_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_ENABLE_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_DISABLE_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_ERROR_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_ENABLE_AIL_WARN_COUNT
#   define _AIL_WARN_DISABLE_AIL_WARN_COUNT
#   define _AIL_WARN_ERROR_AIL_WARN_COUNT
#elif AIL_COMP_GCC
#   define _AIL_WARN_ENABLE_AIL_WARN_NONE
#   define _AIL_WARN_DISABLE_AIL_WARN_NONE
#   define _AIL_WARN_ERROR_AIL_WARN_NONE
#   define _AIL_WARN_ENABLE_AIL_WARN_ALL
#   define _AIL_WARN_DISABLE_AIL_WARN_ALL
#   define _AIL_WARN_ERROR_AIL_WARN_ALL
#   define _AIL_WARN_ENABLE_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_DISABLE_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_ERROR_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_ENABLE_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_DISABLE_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_ERROR_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_ENABLE_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_DISABLE_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_ERROR_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_ENABLE_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_DISABLE_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_ERROR_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_ENABLE_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_DISABLE_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_ERROR_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_ENABLE_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_DISABLE_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_ERROR_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_ENABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_DISABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_ERROR_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_ENABLE_AIL_WARN_STRING_CAST
#   define _AIL_WARN_DISABLE_AIL_WARN_STRING_CAST
#   define _AIL_WARN_ERROR_AIL_WARN_STRING_CAST
#   define _AIL_WARN_ENABLE_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_DISABLE_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_ERROR_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_ENABLE_AIL_WARN_DEPRECATED _Pragma("GCC diagnostic warning \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_DEPRECATED _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_ERROR_AIL_WARN_DEPRECATED _Pragma("GCC diagnostic error \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("GCC diagnostic warning \"-Wunknown-pragmas\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("GCC diagnostic ignored \"-Wunknown-pragmas\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_PRAGMA _Pragma("GCC diagnostic error \"-Wunknown-pragmas\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("GCC diagnostic warning \"-Wunused-function\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_FUNCTION _Pragma("GCC diagnostic error \"-Wunused-function\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("GCC diagnostic warning \"-Wunused-variable\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_VARIABLE _Pragma("GCC diagnostic error \"-Wunused-variable\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("GCC diagnostic warning \"-Wpedantic\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("GCC diagnostic ignored \"-Wpedantic\"")
#   define _AIL_WARN_ERROR_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("GCC diagnostic error \"-Wpedantic\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_CAST_QUAL _Pragma("GCC diagnostic warning \"-Wcast-qual\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_CAST_QUAL _Pragma("GCC diagnostic ignored \"-Wcast-qual\"")
#   define _AIL_WARN_ERROR_AIL_WARN_CAST_QUAL _Pragma("GCC diagnostic error \"-Wcast-qual\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_DISABLE_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_ERROR_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_ENABLE_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_DISABLE_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_ERROR_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_ENABLE_AIL_WARN_COUNT
#   define _AIL_WARN_DISABLE_AIL_WARN_COUNT
#   define _AIL_WARN_ERROR_AIL_WARN_COUNT
#elif AIL_COMP_MSVC || AIL_COMP_INTEL
#   define _AIL_WARN_ENABLE_AIL_WARN_NONE
#   define _AIL_WARN_DISABLE_AIL_WARN_NONE
#   define _AIL_WARN_ERROR_AIL_WARN_NONE
#   define _AIL_WARN_ENABLE_AIL_WARN_ALL _Pragma("warning(push, 4)")
#   define _AIL_WARN_DISABLE_AIL_WARN_ALL _Pragma("warning(push, 0)")
#   define _AIL_WARN_ERROR_AIL_WARN_ALL _Pragma("warning(error: 4061 4062 5262 4191 4242 4254 4287 4388 5219 4546 4574 4619 4668 4767 4774 4777 4826 4905 4906 5031 5032 4996 4505 5245 5264 4200 5266 4431)")
#   define _AIL_WARN_ENABLE_AIL_WARN_EXHAUSTIVE_SWITCH _Pragma("warning(4061 4062 5262)")
#   define _AIL_WARN_DISABLE_AIL_WARN_EXHAUSTIVE_SWITCH _Pragma("warning(disable:4061 4062 5262)")
#   define _AIL_WARN_ERROR_AIL_WARN_EXHAUSTIVE_SWITCH _Pragma("warning(error:4061 4062 5262)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNSAFE_CONVERSION _Pragma("warning(4191 4242 4254 4287 4388 5219)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNSAFE_CONVERSION _Pragma("warning(disable:4191 4242 4254 4287 4388 5219)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNSAFE_CONVERSION _Pragma("warning(error:4191 4242 4254 4287 4388 5219)")
#   define _AIL_WARN_ENABLE_AIL_WARN_MISSING_ARGLIST _Pragma("warning(4546)")
#   define _AIL_WARN_DISABLE_AIL_WARN_MISSING_ARGLIST _Pragma("warning(disable:4546)")
#   define _AIL_WARN_ERROR_AIL_WARN_MISSING_ARGLIST _Pragma("warning(error:4546)")
#   define _AIL_WARN_ENABLE_AIL_WARN_IFDEF_0_MACRO _Pragma("warning(4574)")
#   define _AIL_WARN_DISABLE_AIL_WARN_IFDEF_0_MACRO _Pragma("warning(disable:4574)")
#   define _AIL_WARN_ERROR_AIL_WARN_IFDEF_0_MACRO _Pragma("warning(error:4574)")
#   define _AIL_WARN_ENABLE_AIL_WARN_SECTION_NAME_TOO_LONG _Pragma("warning(4767)")
#   define _AIL_WARN_DISABLE_AIL_WARN_SECTION_NAME_TOO_LONG _Pragma("warning(disable:4767)")
#   define _AIL_WARN_ERROR_AIL_WARN_SECTION_NAME_TOO_LONG _Pragma("warning(error:4767)")
#   define _AIL_WARN_ENABLE_AIL_WARN_PRINTF_FORMTAT _Pragma("warning(4774 4777)")
#   define _AIL_WARN_DISABLE_AIL_WARN_PRINTF_FORMTAT _Pragma("warning(disable:4774 4777)")
#   define _AIL_WARN_ERROR_AIL_WARN_PRINTF_FORMTAT _Pragma("warning(error:4774 4777)")
#   define _AIL_WARN_ENABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION _Pragma("warning(4826)")
#   define _AIL_WARN_DISABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION _Pragma("warning(disable:4826)")
#   define _AIL_WARN_ERROR_AIL_WARN_SIGN_EXTENDED_CONVERSION _Pragma("warning(error:4826)")
#   define _AIL_WARN_ENABLE_AIL_WARN_STRING_CAST _Pragma("warning(4905 4906)")
#   define _AIL_WARN_DISABLE_AIL_WARN_STRING_CAST _Pragma("warning(disable:4905 4906)")
#   define _AIL_WARN_ERROR_AIL_WARN_STRING_CAST _Pragma("warning(error:4905 4906)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNBALANCED_PUSH_POP _Pragma("warning(5031 5032)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNBALANCED_PUSH_POP _Pragma("warning(disable:5031 5032)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNBALANCED_PUSH_POP _Pragma("warning(error:5031 5032)")
#   define _AIL_WARN_ENABLE_AIL_WARN_DEPRECATED _Pragma("warning(4996)")
#   define _AIL_WARN_DISABLE_AIL_WARN_DEPRECATED _Pragma("warning(disable:4996)")
#   define _AIL_WARN_ERROR_AIL_WARN_DEPRECATED _Pragma("warning(error:4996)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("warning(4619)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("warning(disable:4619)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_PRAGMA _Pragma("warning(error:4619)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_MACRO _Pragma("warning(4668)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_MACRO _Pragma("warning(disable:4668)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_MACRO _Pragma("warning(error:4668)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("warning(4505 5245)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("warning(disable:4505 5245)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_FUNCTION _Pragma("warning(error:4505 5245)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("warning(5264)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("warning(disable:5264)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_VARIABLE _Pragma("warning(error:5264)")
#   define _AIL_WARN_ENABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("warning(4200)")
#   define _AIL_WARN_DISABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("warning(disable:4200)")
#   define _AIL_WARN_ERROR_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("warning(error:4200)")
#   define _AIL_WARN_ENABLE_AIL_WARN_CAST_QUAL _Pragma("warning(5266)")
#   define _AIL_WARN_DISABLE_AIL_WARN_CAST_QUAL _Pragma("warning(disable:5266)")
#   define _AIL_WARN_ERROR_AIL_WARN_CAST_QUAL _Pragma("warning(error:5266)")
#   define _AIL_WARN_ENABLE_AIL_WARN_IMPLICIT_INT _Pragma("warning(4431)")
#   define _AIL_WARN_DISABLE_AIL_WARN_IMPLICIT_INT _Pragma("warning(disable:4431)")
#   define _AIL_WARN_ERROR_AIL_WARN_IMPLICIT_INT _Pragma("warning(error:4431)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNREACHABLE_CODE _Pragma("warning(4702)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNREACHABLE_CODE _Pragma("warning(disable:4702)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNREACHABLE_CODE _Pragma("warning(error:4702)")
#   define _AIL_WARN_ENABLE_AIL_WARN_COUNT
#   define _AIL_WARN_DISABLE_AIL_WARN_COUNT
#   define _AIL_WARN_ERROR_AIL_WARN_COUNT
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
#define AIL_CALL_CALLOC(allocator, ...) AIL_VFUNC(_AIL_CALL_CALLOC, allocator, __VA_ARGS__)

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



#if !defined(AIL_NO_ENDIAN_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_ENDIAN_GUARD_
#define _AIL_ENDIAN_GUARD_
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

AIL_Endian ail_endian(void)
{
    persist AIL_Endian res;
    if (res) return res;
    union {
        u32 val;
        u8 data[4];
    } num;
    num.data[0] = 0x00;
    num.data[1] = 0x01;
    num.data[2] = 0x02;
    num.data[3] = 0x03;
    AIL_STATIC_ASSERT(AIL_ENDIAN_COUNT == 5);
    switch (num.val) {
        case 0x00010203: res = AIL_ENDIAN_BIG;         break;
        case 0x03020100: res = AIL_ENDIAN_LITTLE;      break;
        case 0x02030001: res = AIL_ENDIAN_BIG_WORD;    break;
        case 0x01000302: res = AIL_ENDIAN_LITTLE_WORD; break;
        default:         res = AIL_ENDIAN_UNKNOWN;
    }
    return res;
}

AIL_WARN_POP
#endif // _AIL_ENDIAN_GUARD_
#endif // AIL_NO_ENDIAN_IMPL


#if !defined(AIL_NO_ALLOC_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_ALLOCATOR_GUARD_
#define _AIL_ALLOCATOR_GUARD_
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNREACHABLE_CODE)
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
AIL_WARN_POP

global AIL_Allocator ail_default_allocator = {
    .data  = NULL,
    .alloc = &ail_default_alloc,
};

// Function just exists to suppress of potential "unused ail_default_allocator" warning
AIL_DEF void __ail_default_allocator_unused__(void)
{
    AIL_UNUSED(ail_default_allocator);
}

AIL_WARN_POP
#endif // _AIL_ALLOCATOR_GUARD_
#endif // AIL_NO_ALLOC_IMPL
