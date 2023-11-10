// This header contains general utilities used throughout the other ail.h libraries
//

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


// @TODO: Add support for short names via `AIL_SHORT_NAMES` macro

// Implement all functionalities with `#define AIL_ALL_IMPL`
#ifdef  AIL_ALL_IMPL
#define AIL_TYPES_IMPL
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
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;

#endif // _AIL_TYPES_GUARD_
#endif // AIL_TYPES_IMPL


/////////////////////////
// Custom Utility Macros
// always enabled
/////////////////////////
#ifndef AIL_DBG_PRINT
#include <stdio.h>
#define AIL_DBG_PRINT puts
#endif // AIL_DBG_PRINT

#ifndef AIL_ASSERT
#include <assert.h>
#define AIL_ASSERT assert
#endif // AIL_ASSERT

#define AIL_STRINGIZE2(x) #x
#define AIL_STRINGIZE(x) AIL_STRINGIZE2(x)
#define AIL_STR_LINE AIL_STRINGIZE(__LINE__)
#define AIL_MAX(a, b) ((a > b) ? a : b)
#define AIL_MIN(a, b) ((a < b) ? a : b)
#define AIL_CLAMP(x, min, max) ((x) > (max) ? (max) : (x) < (min) ? (min) : (x))
#define AIL_LERP(t, min, max) ((min) + (t)*((max) - (min)))
#define AIL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#define AIL_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#define AIL_SWAP(x, y) do { __typeof__(x) _swap_tmp_ = x; x = y; y = _swap_tmp_; } while(0)
#define AIL_DBG_EXIT() do { *(char *)0 = 0; exit(1); } while(0)
#define AIL_PANIC(...) do { AIL_DBG_PRINT(__VA_ARGS__); AIL_DBG_PRINT("\n"); AIL_DBG_EXIT(); } while(0)
#define AIL_TODO() do { AIL_DBG_PRINT("Hit TODO in " __FILE__ ":" AIL_STR_LINE "\n"); AIL_DBG_EXIT(); } while(0)
#define AIL_UNREACHABLE() do { AIL_DBG_PRINT("Reached an unreachable place in " __FILE__ ":" AIL_STR_LINE "\n"); AIL_DBG_EXIT(); } while(0)
#define AIL_STATIC_ASSERT_MSG(expr, msg) do { extern int __attribute__((error("assertion failure: '" #msg "' in " __FILE__ ":" AIL_STR_LINE))) compile_time_check(); ((expr)?0:compile_time_check()),(void)0; } while(0)
#define AIL_STATIC_ASSERT(expr) AIL_STATIC_ASSERT_MSG(expr, #expr);

#define AIL_IS_2POWER(x) x && !(x & (x - 1))
#define AIL_NEXT_2POWER(x, out) do {                                                                                                       \
        out = x;                                                                                                                           \
        out--;                                                                                                                             \
        for (size_t _ail_next_2power_shift_ = 1; _ail_next_2power_shift_ < 8 * sizeof(x); _ail_next_2power_shift_ += _ail_next_2power_shift_) \
            out |= out >> _ail_next_2power_shift_;                                                                                         \
        out++;                                                                                                                             \
        out += (out==0);                                                                                                                   \
    } while(0)


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

#ifndef AIL_DA_INIT_CAP
#define AIL_DA_INIT_CAP 256
#endif

#define AIL_DA_INIT(T) typedef struct AIL_DA_##T { T *data; unsigned int len; unsigned int cap; } AIL_DA_##T
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
#endif

// #define AIL_DA(T) AIL_DA // To allow adding the element T in array definitions - serves only as documentation

#define ail_da_new(T)                 (AIL_DA(T)) { .data = AIL_MALLOC(AIL_DA_INIT_CAP * sizeof(T)), .len = 0, .cap = AIL_DA_INIT_CAP }
#define ail_da_with_cap(T, c)         (AIL_DA(T)) { .data = AIL_MALLOC((c) * sizeof(T)), .len = 0, .cap = (c) }
#define ail_da_new_empty(T)           (AIL_DA(T)) { .data = NULL, .len = 0, .cap = 0 }
#define ail_da_new_zero_init(T, c)    (AIL_DA(T)) { .data = AIL_CALLOC(c, sizeof(T)), .len = 0, .cap = (c) }
#define ail_da_from_parts(T, d, l, c) (AIL_DA(T)) { .data = (d), .len = (l), .cap = (c) }
#define ail_da_free(da) AIL_FREE(da.data)

#define ail_da_setn(daPtr, idx, elems, n) do {                                             \
		for (unsigned int _ail_da_setn_i_ = 0; _ail_da_setn_i_ < (n); _ail_da_setn_i_++) { \
			(daPtr)->data[(idx) + _ail_da_setn_i_] = (elems)[_ail_da_setn_i_];             \
		}                                                                                  \
	} while(0)

#define ail_da_grow(daPtr, newCap) do {                        					       \
		(daPtr)->data = AIL_REALLOC((daPtr)->data, sizeof(*((daPtr)->data))*(newCap)); \
	} while(0)

#define ail_da_maybe_grow(daPtr, n) {                                    \
		if ((daPtr)->len + (n) > (daPtr)->cap)                           \
		ail_da_grow(daPtr, AIL_MAX(2*(daPtr)->cap, (daPtr)->cap + (n))); \
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
		(daPtr)->data = AIL_MALLOC((elSize)*(newCap));                                                                                     \
		AIL_ASSERT((daPtr)->data != NULL);                                                                                                 \
		AIL_MEMCPY((daPtr)->data, _ail_da_gwg_ptr_, (elSize)*(gapStart));                                                                  \
		AIL_MEMCPY(&(daPtr)->data[((gapStart) + (gapLen))], &_ail_da_gwg_ptr_[(elSize)*(gapStart)], (elSize)*((daPtr)->len - (gapStart))); \
		AIL_FREE(_ail_da_gwg_ptr_);                                                                                                        \
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



#endif // AIL_H_