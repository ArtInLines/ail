// This header contains general utilities used throughout the other ail.h libraries
//

#ifndef AIL_H_
#define AIL_H_

#if   !defined(AIL_MALLOC) && !defined(AIL_REALLOC) && !defined(AIL_FREE)
#include <stdlib.h>
#define AIL_MALLOC(sz)       malloc(sz)
#define AIL_REALLOC(ptr, sz) realloc(ptr, sz)
#define AIL_FREE(ptr)        free(ptr)
#elif !defined(AIL_MALLOC) || !defined(AIL_REALLOC) || !defined(AIL_FREE)
#error "You must define AIL_MALLOC, AIL_REALLOC and AIL_FREE or none of them. You cannot only define one or two of them."
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
#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define i8  int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
#define f32 float
#define f64 double

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
#define AIL_CLAMP(x, min, max) (x) > (max) ? (max) : (x) < (min) ? (min) : (x)
#define AIL_LERP(t, min, max) (min) + (t)*((max) - (min))
#define AIL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#define AIL_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#define AIL_SWAP(x, y) do { __typeof__(x) _swap_tmp_ = x; x = y; y = _swap_tmp_; } while (0)
#define AIL_PANIC(...) do { AIL_DBG_PRINT(__VA_ARGS__); AIL_DBG_PRINT("\n"); exit(1); } while (0)
#define AIL_TODO() do { AIL_DBG_PRINT("Hit TODO in " __FILE__ ":" AIL_STR_LINE "\n"); exit(1); } while(0)
#define AIL_UNREACHABLE() do { AIL_DBG_PRINT("Reached an unreachable place in " __FILE__ ":" AIL_STR_LINE "\n"); exit(1); } while(0)
#define AIL_STATIC_ASSERT_MSG(expr, msg) { extern int __attribute__((error("assertion failure: '" #msg "' in " __FILE__ ":" AIL_STR_LINE))) compile_time_check(); ((expr)?0:compile_time_check()),(void)0; }
#define AIL_STATIC_ASSERT(expr) AIL_STATIC_ASSERT_MSG(expr, #expr);


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

typedef struct AIL_DA {
	char  *data;      // The list of elements, stored as char* instead of void* to avoid endless casting in macros
	size_t elLen;	  // The length of a single element
	unsigned int len; // The amount of elements currently stored in the array
	unsigned int cap; // The currently allocated capacity (in amount of elements) for the array
} AIL_DA;

#define ail_da_new(type) (AIL_DA) { .data = AIL_MALLOC(AIL_DA_INIT_CAP * sizeof(type)), .elLen = sizeof(type), .len = 0, .cap = AIL_DA_INIT_CAP }
#define ail_da_with_cap(type, c) (AIL_DA) { .data = AIL_MALLOC((c) * sizeof(type)), .elLen = sizeof(type), .len = 0, .cap = (c) }
#define ail_da_new_empty(type) (AIL_DA) { .data = NULL, .elLen = sizeof(type), .len = 0, .cap = 0 }
#define ail_da_set(daPtr, idx, elem) {                                                                    \
		for (unsigned int _ail_da_set_i_ = 0; _ail_da_set_i_ < (daPtr)->elLen; _ail_da_set_i_++) {        \
			(daPtr)->data[(idx)*(daPtr)->elLen + _ail_da_set_i_] = ((elem) << (8*_ail_da_set_i_)) & 0xff; \
		}                                                                                                 \
	}
#define ail_da_setn(daPtr, idx, elems, n) {                                                \
		for (unsigned int _ail_da_setn_i_ = 0; _ail_da_setn_i_ < (n); _ail_da_setn_i_++) { \
			ail_da_set(daPtr, (idx) + _ail_da_setn_i_, (elems)[_ail_da_setn_i_])           \
		}                                                                                  \
	}
#define ail_da_grow(daPtr, newCap) {                                         \
		(daPtr)->cap = (newCap);                                             \
		(daPtr)->data = AIL_REALLOC((daPtr)->data, (newCap)*(daPtr)->elLen); \
	}
#define ail_da_maybe_grow(daPtr, n) {                                    \
		if ((daPtr)->len + (n) > (daPtr)->cap)                           \
		ail_da_grow(daPtr, AIL_MAX(2*(daPtr)->cap, (daPtr)->cap + (n))); \
	}
#define ail_da_push(daPtr, elem) {             \
		ail_da_maybe_grow(daPtr, 1);           \
		ail_da_set(daPtr, (daPtr)->len, elem); \
		(daPtr)->len++;						   \
	}
#define ail_da_pushn(daPtr, elems, n) {                                                       \
		ail_da_maybe_grow(daPtr, n);                                                          \
		AIL_MEMCPY((daPtr)->data + (daPtr)->elLen*(daPtr)->len, (elems), (n)*(daPtr)->elLen); \
		(daPtr)->len += (n);                                                                  \
	}
#define ail_da_grow_with_gap(daPtr, gapStart, gapLen, newCap) {                                                                                                       \
		(daPtr)->cap = (newCap);                                                                                                                                      \
		char *_ail_da_gwg_ptr_ = AIL_MALLOC((newCap)*(daPtr)->elLen);                                                                                                 \
		AIL_ASSERT(_ail_da_gwg_ptr_ != NULL);                                                                                                                         \
		AIL_MEMCPY(_ail_da_gwg_ptr_, (daPtr)->data, (gapStart)*(daPtr)->elLen);                                                                                       \
		AIL_MEMCPY(_ail_da_gwg_ptr_ + ((gapStart) + (gapLen))*(daPtr)->elLen, (daPtr)->data + (gapStart)*(daPtr)->elLen, ((daPtr)->len - (gapStart))*(daPtr)->elLen); \
		AIL_FREE((daPtr)->data);                                                                                                                                      \
		(daPtr)->data = _ail_da_gwg_ptr_;                                                                                                                             \
		(daPtr)->len += (gapLen);                                                                                                                                     \
	}
#define ail_da_maybe_grow_with_gap(daPtr, idx, n) {                                                               \
		if ((daPtr)->len + (n) > (daPtr)->cap) {                                                                  \
			ail_da_grow_with_gap((daPtr), (idx), (n), AIL_MAX(2*(daPtr)->cap, (daPtr)->cap + (n)));               \
		} else {                                                                                                  \
			for (unsigned int _ail_da_i_ = 1; _ail_da_i_ <= ((daPtr)->len - (idx))*(daPtr)->elLen; _ail_da_i_++) { \
				char _ail_da_el_ = (daPtr)->data[(daPtr)->len*(daPtr)->elLen - _ail_da_i_];                       \
				(daPtr)->data[((daPtr)->len + (n))*(daPtr)->elLen - _ail_da_i_] = _ail_da_el_;                    \
			}                                                                                                     \
			(daPtr)->len += (n);																	              \
		}                                                                                                         \
	}
#define ail_da_insert(daPtr, idx, elem) {          \
		ail_da_maybe_grow_with_gap(daPtr, idx, 1); \
		ail_da_set(daPtr, idx, elem);              \
	}
#define ail_da_insertn(daPtr, idx, elems, n) {     \
		ail_da_maybe_grow_with_gap(daPtr, idx, n); \
		ail_da_setn(daPtr, idx, elems, n);         \
	}
// @TODO: Add macros to remove, join, ...

#endif // _AIL_DA_GUARD_
#endif // AIL_DA_IMPL



#endif // AIL_H_