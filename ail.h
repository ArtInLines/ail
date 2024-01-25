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

// AIL_DEF and AIL_DEF_INLINE don't actually effect any of the functions in this file, as only macros are defined here
// They do however serve as defaults for the other ail headers
#ifndef AIL_DEF
#define AIL_DEF
#endif // AIL_DEF
#ifndef AIL_DEF_INLINE
#define AIL_DEF_INLINE static inline
#endif // AIL_DEF_INLINE

#ifdef _MSC_VER
#define AIL_UNUSED(v)  (void)(v)
#else
#define AIL_UNUSED(v)  (void)sizeof(v)
#endif

// @TODO: Add support for short names via `AIL_SHORT_NAMES` macro

// Implement all functionalities with `#define AIL_ALL_IMPL`
#ifdef  AIL_ALL_IMPL
#define AIL_TYPES_IMPL
#define AIL_DA_IMPL
#define AIL_ALLOC_IMPL
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
// Clang: __clang__
// MSVC:  _MSC_VER
// gcc:   __GNUC__
// MinGW: __MINGW32__ (defined on 32- and 64-bit version, use __MINGW64__ for detecting 64-bit version only)
// TinyC: __TINYC__

// To detect standard
// __STDC__                    - C89 or higher Standard
// __STDC_VERSION__ == 199901L - C99 Standard
// __STDC_VERSION__ == 201112L - C11 Standard
// __cplusplus                 - C++ Standard

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
// x86       -64                         - __x86_64 || __x86_64__
// AMD64     - __amd64__ || _M_AMD64
// ARM       - __arm__ || _M_ARM
// ARM64     - __aarch64__
// Intel x86 - __i386 || _M_IX86 || _X86_
// MIPS      - __mips || __mips__


/////////////////////////
// Custom Utility Macros
// always enabled
/////////////////////////
#ifndef _AIL_UTIL_GUARD_
#define _AIL_UTIL_GUARD_

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

#define AIL_STRINGIZE2(x) #x
#define AIL_STRINGIZE(x) AIL_STRINGIZE2(x)
#define AIL_STR_LINE AIL_STRINGIZE(__LINE__)

#define AIL_MAX(a, b) ((a > b) ? a : b)
#define AIL_MIN(a, b) ((a < b) ? a : b)
#define AIL_CLAMP(x, min, max) ((x) > (max) ? (max) : (x) < (min) ? (min) : (x))

#ifdef AIL_TYPEOF
	#define AIL_SWAP(x, y) do { __typeof__(x) _swap_tmp_ = x; x = y; y = _swap_tmp_; } while(0)
#else
	#define AIL_SWAP(x, y) do { x ^= y; y ^= x; x ^= y; } while(0)
#endif

// AIL_LERP(AIL_LERP(x, min, max), min, max) = x
#define AIL_LERP(t, min, max) ((min) + (t)*((max) - (min)))
#define AIL_REV_LERP(x, min, max) ((x) - (min)) / ((max) - (min))

#define AIL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#define AIL_LIKELY(expr)   __builtin_expect(!!(expr), 1)

#define AIL_DBG_EXIT() do { int *X = 0; *X = 0; exit(1); } while(0)
#define AIL_ASSERT(expr) do { if (!(expr)) { AIL_DBG_PRINT("Assertion failed in " __FILE__ ":" AIL_STR_LINE "\n    with expression 'AIL_ASSERT(" #expr ")'"); AIL_DBG_EXIT(); } } while(0)
#define AIL_PANIC(...) do { AIL_DBG_PRINT(__VA_ARGS__); AIL_DBG_PRINT("\n"); AIL_DBG_EXIT(); } while(0)
#define AIL_TODO() do { AIL_DBG_PRINT("Hit TODO in " __FILE__ ":" AIL_STR_LINE "\n"); AIL_DBG_EXIT(); } while(0)
#define AIL_UNREACHABLE() do { AIL_DBG_PRINT("Reached an unreachable place in " __FILE__ ":" AIL_STR_LINE "\n"); AIL_DBG_EXIT(); } while(0)
// @Bug: Static Assert is currently broken
// #define AIL_STATIC_ASSERT_MSG(expr, msg) do { extern int __attribute__((error("assertion failure: '" #msg "' in " __FILE__ ":" AIL_STR_LINE))) compile_time_check(); ((expr)?0:compile_time_check()),(void)0; } while(0)
// #define AIL_STATIC_ASSERT(expr) AIL_STATIC_ASSERT_MSG(expr, #expr);

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

#endif // _AIL_UTIL_GUARD_


/////////////////////////
// General Allocator Interface
// enable with `#define AIL_ALLOCATOR_IMPL`
// automatically enabled if dynamic arrays are enabled
/////////////////////////
#if defined(AIL_ALLOCATOR_IMPL) || defined(AIL_DA_IMPL)
#ifndef _AIL_ALLOCATOR_GUARD_
#define _AIL_ALLOCATOR_GUARD_

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

void* ail_default_malloc(void *data, size_t size)
{
	(void)data;
	return AIL_MALLOC(size);
}
void* ail_default_calloc(void* data, size_t nelem, size_t elsize)
{
	(void)data;
	return AIL_CALLOC(nelem, elsize);
}
void* ail_default_realloc(void* data, void* ptr, size_t size)
{
	(void)data;
	return AIL_REALLOC(ptr, size);
}
void ail_default_free(void* data, void* ptr)
{
	(void)data;
	AIL_FREE(ptr);
}
void ail_default_free_all(void* data)
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

#endif // _AIL_ALLOCATOR_GUARD_
#endif // AIL_ALLOCATOR_IMPL


/////////////////////////
// Dynamic Array Implementation
// enable with `#define AIL_DA_IMPL`
// You can also define `AIL_DA_INIT_CAP` to set a different initial capacity for dynamic arrays
// This implementation is heavily inspired by:
// - nob.h (https://github.com/tsoding/musializer/blob/master/src/nob.h)
// - stb_ds.h (https://github.com/nothings/stb/blob/master/stb_ds.h)
/////////////////////////
#ifdef  AIL_DA_IMPL
#ifndef _AIL_DA_GUARD_
#define _AIL_DA_GUARD_

#ifndef AIL_DA_PRINT
#include <stdio.h>
#define AIL_DA_PRINT printf
#endif

#ifndef AIL_DA_INIT_CAP
#define AIL_DA_INIT_CAP 256
#endif

#define AIL_DA_INIT(T) typedef struct AIL_DA_##T { T *data; unsigned int len; unsigned int cap; AIL_Allocator *allocator; } AIL_DA_##T
#define AIL_DA(T) AIL_DA_##T

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

// #define AIL_DA(T) AIL_DA // To allow adding the element T in array definitions - serves only as documentation

#define ail_da_from_parts(T, d, l, c, alPtr) (AIL_DA(T)) { .data = (d),                                     .len = (l), .cap = (c), .allocator = (alPtr) }
#define ail_da_new_with_alloc(T, c, alPtr)   (AIL_DA(T)) { .data = (alPtr)->alloc((alPtr)->data, sizeof(T) * (c)),         .len = 0,   .cap = (c), .allocator = (alPtr) }
#define ail_da_new_zero_alloc(T, c, alPtr)   (AIL_DA(T)) { .data = (alPtr)->zero_alloc((alPtr)->data, sizeof(T) * (c)),    .len = 0,   .cap = (c), .allocator = (alPtr) }
#define ail_da_new_with_cap(T, c)            (AIL_DA(T)) { .data = AIL_MALLOC(sizeof(T) * (c)),             .len = 0,   .cap = (c), .allocator = &ail_default_allocator }
#define ail_da_new(T)                        (AIL_DA(T)) { .data = AIL_MALLOC(sizeof(T) * AIL_DA_INIT_CAP), .len = 0,   .cap = AIL_DA_INIT_CAP, .allocator = &ail_default_allocator }
#define ail_da_new_empty(T)                  (AIL_DA(T)) { .data = NULL,                                    .len = 0,   .cap = 0,   .allocator = &ail_default_allocator }
#define ail_da_new_zero_init(T, c)           (AIL_DA(T)) { .data = AIL_CALLOC(c, sizeof(T)),                .len = 0,   .cap = (c), .allocator = &ail_default_allocator }
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

#define ail_da_maybe_grow(daPtr, n) {                                      \
		if ((daPtr)->len + (n) > (daPtr)->cap)                             \
		ail_da_resize(daPtr, AIL_MAX(2*(daPtr)->cap, (daPtr)->cap + (n))); \
	}

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

#endif // _AIL_DA_GUARD_
#endif // AIL_DA_IMPL