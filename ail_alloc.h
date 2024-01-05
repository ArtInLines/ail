// Different types of Allocators
//
// Define AIL_ALLOC_IMPL in some file, to include the function bodies
// Define AIL_ALLOC_ALIGNMENT to change the alignment used by all custom allocators
// Define AIL_ALLOC_PRINT_MEM to track allocations
//
// @TODO: Add some way to drop-in replace C malloc calls without having to change the code
//
// Implementation of the Arena Allocator was inspired by tsoding's arena library (https://github.com/tsoding/arena/)
// and by gingerBill's blog post on Arena Allocators (https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/)
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


#ifndef AIL_ALLOC_H_
#define AIL_ALLOC_H_

#define AIL_TYPES_IMPL
#define AIL_ALLOCATOR_IMPL
#include "ail.h"

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

typedef struct AIL_Alloc_Arena {
	u8 *buf;
	u64 cap;
	u64 len;
} AIL_Alloc_Arena;

typedef struct AIL_Alloc_Arena_Header {
	u64 size;
} AIL_Alloc_Arena_Header;

AIL_ALLOC_DEF size_t  align_size(size_t size);

AIL_ALLOC_DEF void *ail_alloc_std_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_std_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_std_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_std_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_std_free_all(void *data);

AIL_ALLOC_DEF AIL_Allocator ail_alloc_arena_new(u64 cap, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF void *ail_alloc_arena_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_arena_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_arena_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_arena_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_arena_free_all(void *data);

#endif // AIL_ALLOC_H_

#define AIL_ALLOC_IMPL
#ifdef AIL_ALLOC_IMPL
#ifndef _AIL_ALLOC_IMPL_GUARD_
#define _AIL_ALLOC_IMPL_GUARD_

#include <stdlib.h> // For std_allocator and memset

// For tracing memory
#ifndef AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_LOG(...) do { if (0) printf(__VA_ARGS__); } while(0)
#else
#define AIL_ALLOC_LOG(...) do { AIL_DBG_PRINT("AIL_Alloc Mem Trace: " __VA_ARGS__); AIL_DBG_PRINT("\n"); } while(0)
#endif // AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_LOG_ALLOC(allocator, ptr, size)           AIL_ALLOC_LOG("Allocate   %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CALLOC(allocator, ptr, nelem, elsize) AIL_ALLOC_LOG("0-Allocate %4llu elements of size %4llu at %p in '" allocator "' allocator", (nelem), (elsize), (ptr));
#define AIL_ALLOC_LOG_REALLOC(allocator, nptr, optr, size)  AIL_ALLOC_LOG("Reallocate from %p to %4llu bytes at %p in '" allocator "' allocator", (optr), (size), (nptr));
#define AIL_ALLOC_LOG_FREE(allocator, ptr, size)            AIL_ALLOC_LOG("Free       %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_FREE_ALL(allocator, size)             AIL_ALLOC_LOG("Free all   %4llu bytes in '" allocator "' allocator", (size));

AIL_ALLOC_DEF size_t align_size(size_t size)
{
	size_t mod;
#if AIL_IS_2POWER(AIL_ALLOC_ALIGNMENT) == true
	// size % alignment but faster, bc alignment is a power of two
	mod = size & (AIL_ALLOC_ALIGNMENT - 1);
#else
	mod = size % AIL_ALLOC_ALIGNMENT;
#endif
	return size + (mod > 0)*(AIL_ALLOC_ALIGNMENT - mod);
}

/////////
// Std //
/////////
static AIL_Allocator ail_alloc_std = {
	.data       = NULL,
	.alloc      = &ail_alloc_std_alloc,
	.zero_alloc = &ail_alloc_std_calloc,
	.re_alloc   = &ail_alloc_std_realloc,
	.free_one   = &ail_alloc_std_free,
	.free_all   = &ail_alloc_std_free_all,
};

AIL_ALLOC_DEF void *ail_alloc_std_alloc(void *data, size_t size)
{
	AIL_UNUSED(data);
	void *ptr = malloc(size);
	AIL_ALLOC_LOG_ALLOC("std", ptr, size);
	return ptr;
}

AIL_ALLOC_DEF void *ail_alloc_std_calloc(void *data, size_t nelem, size_t elsize)
{
	AIL_UNUSED(data);
	void *ptr = calloc(nelem, elsize);
	AIL_ALLOC_LOG_CALLOC("std", ptr, nelem, elsize);
	return ptr;
}

AIL_ALLOC_DEF void *ail_alloc_std_realloc(void *data, void *ptr, size_t size)
{
	AIL_UNUSED(data);
	void *out = realloc(ptr, size);
	AIL_ALLOC_LOG_REALLOC("std", out, ptr, size);
	return out;
}

AIL_ALLOC_DEF void ail_alloc_std_free(void *data, void *ptr)
{
	AIL_UNUSED(data);
	AIL_ALLOC_LOG_FREE("std", ptr, (size_t)0);
	free(ptr);
}

AIL_ALLOC_DEF void ail_alloc_std_free_all(void *data)
{
	AIL_ALLOC_LOG_FREE_ALL("std", (size_t)0);
	AIL_UNUSED(data);
}


///////////
// Arena //
///////////

AIL_ALLOC_DEF AIL_Allocator ail_alloc_arena_new(u64 cap, AIL_Allocator *backing_allocator)
{
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)backing_allocator->alloc(backing_allocator->data, sizeof(AIL_Alloc_Arena));
	arena->buf = (u8 *)backing_allocator->alloc(backing_allocator->data, cap);
	arena->cap = cap;
	arena->len = 0;
	AIL_ASSERT(arena->buf != NULL);
	return (AIL_Allocator) {
		.data       = arena,
		.alloc      = &ail_alloc_arena_alloc,
		.zero_alloc = &ail_alloc_arena_calloc,
		.re_alloc   = &ail_alloc_arena_realloc,
		.free_one   = &ail_alloc_arena_free,
		.free_all   = &ail_alloc_arena_free_all,
	};
}

AIL_ALLOC_DEF void *ail_alloc_arena_alloc(void *data, size_t size)
{
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
	u64 header_size = align_size(sizeof(AIL_Alloc_Arena_Header));
	    size        = align_size(size);
	if (arena->len + size + header_size <= arena->cap) {
		AIL_Alloc_Arena_Header *header = (AIL_Alloc_Arena_Header *) &arena->buf[arena->len];
		header->size = size;
		void *ptr    = &arena->buf[arena->len + header_size];
		arena->len  += size + header_size;
		AIL_ALLOC_LOG_ALLOC("arena", ptr, size);
		AIL_ALLOC_LOG("Current size: %llu", arena->len);
		return ptr;
	} else {
		AIL_ALLOC_LOG_ALLOC("arena", NULL, (size_t)0);
		AIL_ASSERT(false);
		return NULL;
	}
}

AIL_ALLOC_DEF void *ail_alloc_arena_calloc(void *data, size_t nelem, size_t elsize)
{
	void *ptr = ail_alloc_arena_alloc(data, nelem * elsize);
	if (ptr) {
		u64 size  = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size;
		memset(ptr, (size_t)0, size);
	}
	AIL_ALLOC_LOG_CALLOC("arena", ptr, nelem, elsize);
	return ptr;
}

AIL_ALLOC_DEF void *ail_alloc_arena_realloc(void *data, void *ptr, size_t size)
{
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
	size_t old_size = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size;
	if (old_size >= size || (u8 *)ptr + old_size == arena->buf + arena->len) {
		// If size didn't increase, or ptr points at the last allocation, no memory needs to be moved
		AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size = size; // Potentially shrink used region
		AIL_ALLOC_LOG_REALLOC("arena", ptr, ptr, size);
		return ptr;
	} else {
		void *new_ptr = ail_alloc_arena_alloc(data, size);
		if (new_ptr) memcpy(new_ptr, ptr, old_size);
		AIL_ALLOC_LOG_REALLOC("arena", new_ptr, ptr, size);
		return new_ptr;
	}
}

AIL_ALLOC_DEF void ail_alloc_arena_free(void *data, void *ptr)
{
	AIL_UNUSED(data);
	AIL_UNUSED(ptr);
	AIL_ALLOC_LOG_FREE("arena", ptr, (size_t)0);
}

AIL_ALLOC_DEF void ail_alloc_arena_free_all(void *data)
{
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
	AIL_ALLOC_LOG_FREE_ALL("arena", arena->len);
	AIL_ALLOC_LOG("Current size: %4llu", arena->len);
	arena->len = 0;
	AIL_ALLOC_LOG("Current size: %4llu", arena->len);
}


#endif // _AIL_ALLOC_IMPL_GUARD_
#endif // AIL_ALLOC_IMPL