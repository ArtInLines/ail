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
// @TODO: Add documentation explaining the different available Allocators
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

typedef struct AIL_Alloc_Size_Header {
	u64 size;
} AIL_Alloc_Size_Header;

typedef AIL_Alloc_Size_Header AIL_Alloc_Page_Header;

typedef struct AIL_Alloc_Buffer {
	u64 size;  // @Note: Does not include the 16 bytes of the buffer's header itself
	u64 idx;
} AIL_Alloc_Buffer;

typedef AIL_Alloc_Buffer AIL_Alloc_Ring;

typedef struct AIL_Alloc_Arena_Region {
	u64 size;  // @Note: Does not include the 24 bytes of the region's header itself
	u64 idx;
	struct AIL_Alloc_Arena_Region *next;
} AIL_Alloc_Arena_Region;

typedef struct AIL_Alloc_Arena {
	AIL_Allocator *backing_allocator;
	AIL_Alloc_Arena_Region start;     // Code assumes that Region starts right before the rest of the data
} AIL_Alloc_Arena;

typedef AIL_Alloc_Size_Header AIL_Alloc_Arena_Header;

typedef struct AIL_Allloc_Pool_Free_Node {
	struct AIL_Allloc_Pool_Free_Node *next;
} AIL_Allloc_Pool_Free_Node;

typedef struct AIL_Alloc_Pool_Region {
	u8 *buf;
	AIL_Allloc_Pool_Free_Node *head;
	struct AIL_Alloc_Pool_Region *next;
} AIL_Alloc_Pool_Region;

typedef struct AIL_Alloc_Pool {
	u64 bucket_amount;
	u64 bucket_size;
	AIL_Alloc_Pool_Region start;
	AIL_Allocator *backing_allocator;
} AIL_Alloc_Pool;

typedef struct AIL_Alloc_Size_Header AIL_Alloc_Pool_Header;

//////////////
// Utility Functions
//////////////

// Aligns size to AIL_ALLOC_ALIGNMENT
AIL_ALLOC_DEF size_t ail_alloc_align_size(size_t size);
// @Note: Alignment must be greater than 0 and a power of 2
AIL_ALLOC_DEF size_t ail_alloc_align_forward(size_t n, size_t alignment);
// @Note: Alignment must be greater than 0 and a power of 2
AIL_ALLOC_DEF size_t ail_alloc_align_backward(size_t n, size_t alignment);


//////////////
// Std Allocator
// C's stdlib allocator
// @Note: free_all is a no-op
//////////////

AIL_ALLOC_DEF void *ail_alloc_std_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_std_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_std_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_std_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_std_free_all(void *data);


//////////////
// Page Allocator
// Allocates always complete pages
// Especially useful for use as a backing allocator
// @Note: free_all is a no-op
//////////////

AIL_ALLOC_DEF void *ail_alloc_page_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_page_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_page_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_page_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_page_free_all(void *data);


//////////////
// Buffer Allocator
// Does not have a backing allocator and can thus not grow
// Especially useful for using stack-allocated memory
// @Note: free is a no-op, use free_all instead
//////////////

AIL_ALLOC_DEF AIL_Allocator ail_alloc_buffer_new(u64 n, u8 *mem);
AIL_ALLOC_DEF void *ail_alloc_buffer_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_buffer_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_buffer_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_buffer_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_buffer_free_all(void *data);


//////////////
// Ring-Buffer Allocator
// Does not have a backing allocator and can thus not grow
// Same as the Buffer Allocator, except it wraps around allocating from the start again
//////////////

AIL_ALLOC_DEF AIL_Allocator ail_alloc_ring_new(u64 n, u8 *mem);
AIL_ALLOC_DEF void *ail_alloc_ring_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_ring_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_ring_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_ring_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_ring_free_all(void *data);


//////////////
// Arena Allocator
// Last-In-First-Out kind of allocator (i.e. free only frees memory, when it was the memory that was allocated last)
// Recommended to call free_all to prevent leaking memory for too long
//////////////

AIL_ALLOC_DEF AIL_Allocator ail_alloc_arena_new(u64 cap, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF void *ail_alloc_arena_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_arena_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_arena_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_arena_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_arena_free_all(void *data);
AIL_ALLOC_DEF void ail_alloc_arena_free_all_keep_regions(void *data);


//////////////
// Pool Allocator
// Allocates only fixed-size chunks
// Allows freeing any chunk at any time
// @TODO: Allow allocating several chunks at once (make sure that all of those chunks get freed at once too)
//////////////

AIL_ALLOC_DEF AIL_Allocator ail_alloc_pool_new(u64 bucket_amount, u64 el_size, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF void *ail_alloc_pool_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_pool_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_pool_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_pool_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_pool_free_region(AIL_Alloc_Pool_Region *region, u64 bucket_amount, u64 bucket_size);
AIL_ALLOC_DEF void ail_alloc_pool_free_all(void *data);
AIL_ALLOC_DEF void ail_alloc_pool_free_all_keep_regions(void *data);


//////////////
// Free-List Allocator
// Allows allocating arbitrarily sized regions
// Stores a linked-list internally to keep track of allocations
//////////////

AIL_ALLOC_DEF AIL_Allocator ail_alloc_freelist_new(u64 cap, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF void *ail_alloc_freelist_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_freelist_calloc(void *data, size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_freelist_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_freelist_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_freelist_free_all(void *data);


#endif // AIL_ALLOC_H_

#define AIL_ALLOC_IMPL
#ifdef AIL_ALLOC_IMPL
#ifndef _AIL_ALLOC_IMPL_GUARD_
#define _AIL_ALLOC_IMPL_GUARD_

#include <stdlib.h> // For std_allocator and memset

// @TODO: Implement Page Allocations for OSes other than WINDOWS and UNIX
#if defined(_WIN32)
#include <Windows.h> // For VirtualAlloc
#else
#include <sys/mman.h>
#endif

// For tracing memory
#ifndef AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_LOG(...) do { if (0) printf(__VA_ARGS__); } while(0)
#else
#define AIL_ALLOC_LOG(...) do { AIL_DBG_PRINT("Memory Trace: " __VA_ARGS__); AIL_DBG_PRINT("\n"); } while(0)
#endif // AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_LOG_ALLOC(allocator, ptr, size)           AIL_ALLOC_LOG("Malloc  %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CALLOC(allocator, ptr, nelem, elsize) AIL_ALLOC_LOG("Calloc  %4llu elements of size %4llu at %p in '" allocator "' allocator", (nelem), (elsize), (ptr));
#define AIL_ALLOC_LOG_REALLOC(allocator, nptr, optr, size)  AIL_ALLOC_LOG("Relloc  %4llu bytes from %p to %p in '" allocator "' allocator", (size), (optr), (nptr));
#define AIL_ALLOC_LOG_FREE(allocator, ptr, size)            AIL_ALLOC_LOG("Free    %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_FREE_ALL(allocator, size)             AIL_ALLOC_LOG("FreeAll %4llu bytes in '" allocator "' allocator", (size));

size_t ail_alloc_align_size(size_t size)
{
	// @Performance: ail_alloc_align_forward should be doing the same but faster, right?
	size_t mod;
#if AIL_IS_2POWER(AIL_ALLOC_ALIGNMENT) == true
	// size % alignment but faster, bc alignment is a power of two
	mod = size & (AIL_ALLOC_ALIGNMENT - 1);
#else
	mod = size % AIL_ALLOC_ALIGNMENT;
#endif
	return size + (mod > 0)*(AIL_ALLOC_ALIGNMENT - mod);
}

size_t ail_alloc_align_forward(size_t n, size_t alignment)
{
	AIL_ASSERT(alignment > 0 && AIL_IS_2POWER(alignment));
	return ail_alloc_align_backward(n + (alignment - 1), alignment);
}

size_t ail_alloc_align_backward(size_t n, size_t alignment)
{
	AIL_ASSERT(alignment > 0 && AIL_IS_2POWER(alignment));
	return n & ~(alignment - 1);
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

// This function only exists to suppress the "unused ail_alloc_std" warning
void __ail_alloc_std_unused__(void)
{
	AIL_UNUSED(ail_alloc_std);
}

void *ail_alloc_std_alloc(void *data, size_t size)
{
	AIL_UNUSED(data);
	void *ptr = malloc(size);
	AIL_ALLOC_LOG_ALLOC("std", ptr, size);
	return ptr;
}

void *ail_alloc_std_calloc(void *data, size_t nelem, size_t elsize)
{
	AIL_UNUSED(data);
	void *ptr = calloc(nelem, elsize);
	AIL_ALLOC_LOG_CALLOC("std", ptr, nelem, elsize);
	return ptr;
}

void *ail_alloc_std_realloc(void *data, void *ptr, size_t size)
{
	AIL_UNUSED(data);
	void *out = realloc(ptr, size);
	AIL_ALLOC_LOG_REALLOC("std", out, ptr, size);
	return out;
}

void ail_alloc_std_free(void *data, void *ptr)
{
	AIL_UNUSED(data);
	AIL_ALLOC_LOG_FREE("std", ptr, (size_t)0);
	free(ptr);
}

void ail_alloc_std_free_all(void *data)
{
	AIL_ALLOC_LOG_FREE_ALL("std", (size_t)0);
	AIL_UNUSED(data);
}


///////////
// Pager //
///////////

static AIL_Allocator ail_alloc_pager = {
	.data       = NULL,
	.alloc      = &ail_alloc_page_alloc,
	.zero_alloc = &ail_alloc_page_calloc,
	.re_alloc   = &ail_alloc_page_realloc,
	.free_one   = &ail_alloc_page_free,
	.free_all   = &ail_alloc_page_free_all,
};

// This function only exists to suppress the "unused ail_alloc_std" warning
void __ail_alloc_page_unused__(void)
{
	AIL_UNUSED(ail_alloc_pager);
}

// @TODO: Add page size of 64 * 1024 for WASM
// Sizes taken from Zig's page_size implementation (https://ziglang.org/documentation/master/std/src/std/mem.zig.html)
#if defined(__APPLE__)
#define AIL_ALLOC_PAGE_SIZE 16*1024
#elif defined(__sparc__) || defined(__sparc)
#define AIL_ALLOC_PAGE_SIZE 8*1024
#else
#define AIL_ALLOC_PAGE_SIZE 4*1024
#endif

// Assumes that size is page-size-aligned
static inline void ail_alloc_internal_free_pages(void *ptr, u64 size)
{
#if defined(_WIN32)
	VirtualFree(ptr, size, MEM_DECOMMIT);
#else
	munmap(ptr, size);
#endif
}

void *ail_alloc_page_alloc(void *data, size_t size)
{
	AIL_UNUSED(data);
	u64 aligned_size = ail_alloc_align_forward(size + sizeof(AIL_Alloc_Page_Header), AIL_ALLOC_PAGE_SIZE);
	size = aligned_size - sizeof(AIL_Alloc_Page_Header);
#if defined(_WIN32)
	void *ptr = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	void *ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
#endif
	((AIL_Alloc_Page_Header *)ptr)->size = aligned_size;
	return (char *)ptr + sizeof(AIL_Alloc_Page_Header);
}

void *ail_alloc_page_calloc(void *data, size_t nelem, size_t elsize)
{
	AIL_UNUSED(data);
	size_t size = nelem*elsize;
	void *ptr   = ail_alloc_page_alloc(data, size);
	memset(ptr, 0, size);
	return ptr;
}

void *ail_alloc_page_realloc(void *data, void *ptr, size_t size)
{
	// @TODO: Track some metadata in the data pointer to allow checking the old size before reallocating
	AIL_UNUSED(data);
	AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Page_Header);
	u64 old_size = header->size;
	if (size <= old_size) {
		u64 sz_diff = old_size - size;
		u64 aligned_sz_diff = ail_alloc_align_forward(sz_diff, AIL_ALLOC_PAGE_SIZE);
		if (aligned_sz_diff >= AIL_ALLOC_PAGE_SIZE) {
			ail_alloc_internal_free_pages((void *)ail_alloc_align_forward((size_t)((char *)ptr + size), AIL_ALLOC_PAGE_SIZE), aligned_sz_diff);
		}
		header->size = size;
		return ptr;
	} else {
		// @TODO: VirtualAlloc can take the previous pointer to potentially just increase the size
		// @TODO: mmap has some kind of hint system, that probably does more or less the same
		ail_alloc_page_free(data, ptr);
		return ail_alloc_page_alloc(data, size);
	}
}

void ail_alloc_page_free(void *data, void *ptr)
{
	AIL_UNUSED(data);
	AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Page_Header);
	ail_alloc_internal_free_pages((void *)header, ail_alloc_align_forward(header->size, AIL_ALLOC_PAGE_SIZE));
}

void ail_alloc_page_free_all(void *data)
{
	AIL_UNUSED(data);
	// @Decide: Should we track all allocated pages to allow freeing all at once?
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
		.zero_alloc = &ail_alloc_buffer_calloc,
		.re_alloc   = &ail_alloc_buffer_realloc,
		.free_one   = &ail_alloc_buffer_free,
		.free_all   = &ail_alloc_buffer_free_all,
	};
}

void *ail_alloc_buffer_alloc(void *data, size_t size)
{
	void *ptr = NULL;
	AIL_Alloc_Buffer *buffer = data;
	u8 *mem = (u8 *)&buffer[1];
	if (AIL_LIKELY(size + buffer->idx < buffer->size)) {
		ptr = &mem[buffer->idx];
		buffer->idx += size;
	}
	AIL_ALLOC_LOG_ALLOC("buffer", ptr, size);
	return ptr;
}

void *ail_alloc_buffer_calloc(void *data, size_t nelem, size_t elsize)
{
	u64  size = nelem * elsize;
	void *ptr = ail_alloc_buffer_alloc(data, size);
	if (ptr) memset(ptr, 0, size);
	AIL_ALLOC_LOG_CALLOC("buffer", ptr, nelem, elsize);
	return ptr;
}

void *ail_alloc_buffer_realloc(void *data, void *ptr, size_t size)
{
	if (!ptr) {
		void *out = ail_alloc_buffer_alloc(data, size);
		AIL_ALLOC_LOG_REALLOC("buffer", out, ptr, size);
		return out;
	}
	AIL_Alloc_Buffer *buffer = data;
	u8 *mem = (u8 *)&buffer[1];
	u64 max_old_size = AIL_MIN(size, (u64)mem + buffer->idx - (u64)ptr);
	void *out = ail_alloc_buffer_alloc(data, size);
	memcpy(out, ptr, max_old_size);
	AIL_ALLOC_LOG_REALLOC("buffer", out, ptr, size);
	return out;
}

void ail_alloc_buffer_free(void *data, void *ptr)
{
	AIL_UNUSED(data);
	AIL_ALLOC_LOG_FREE("buffer", ptr, (size_t)0);
}

void ail_alloc_buffer_free_all(void *data)
{
	AIL_Alloc_Buffer *buffer = data;
	AIL_ALLOC_LOG_FREE_ALL("buffer", buffer->idx);
	buffer->idx = 0;
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
		.data       = ring,
		.alloc      = &ail_alloc_ring_alloc,
		.zero_alloc = &ail_alloc_ring_calloc,
		.re_alloc   = &ail_alloc_ring_realloc,
		.free_one   = &ail_alloc_ring_free,
		.free_all   = &ail_alloc_ring_free_all,
	};
}

void *ail_alloc_ring_alloc(void *data, size_t size)
{
	void *ptr = NULL;
	AIL_Alloc_Ring *ring = data;
	u8 *mem = (u8 *)&ring[1];
	AIL_ASSERT(size <= ring->size);
	if (AIL_UNLIKELY(size + ring->idx >= ring->size)) ring->idx = 0;
	ptr = &mem[ring->idx];
	ring->idx += size;
	AIL_ALLOC_LOG_ALLOC("ring", ptr, size);
	return ptr;
}

void *ail_alloc_ring_calloc(void *data, size_t nelem, size_t elsize)
{
	u64  size = nelem * elsize;
	void *ptr = ail_alloc_ring_alloc(data, size);
	if (ptr) memset(ptr, 0, size);
	AIL_ALLOC_LOG_CALLOC("ring", ptr, nelem, elsize);
	return ptr;
}

void *ail_alloc_ring_realloc(void *data, void *ptr, size_t size)
{
	if (!ptr) {
		void *out = ail_alloc_ring_alloc(data, size);
		AIL_ALLOC_LOG_REALLOC("ring", out, ptr, size);
		return out;
	}
	AIL_Alloc_Ring *ring = data;
	u8 *mem = (u8 *)&ring[1];
	u64 max_old_size = AIL_MIN(size, (u64)mem + ring->idx - (u64)ptr);
	void *out = ail_alloc_ring_alloc(data, size);
	memcpy(out, ptr, max_old_size); // @Bug: memcpy might not work correctly, if the new poiner wrapped around and its region overlaps with the old region
	AIL_ALLOC_LOG_REALLOC("ring", out, ptr, size);
	return out;
}

void ail_alloc_ring_free(void *data, void *ptr)
{
	AIL_UNUSED(data);
	AIL_ALLOC_LOG_FREE("ring", ptr, (size_t)0);
}

void ail_alloc_ring_free_all(void *data)
{
	AIL_Alloc_Ring *ring = data;
	AIL_ALLOC_LOG_FREE_ALL("ring", ring->idx);
	ring->idx = 0;
}


///////////
// Arena //
///////////

AIL_Allocator ail_alloc_arena_new(u64 cap, AIL_Allocator *backing_allocator)
{
	AIL_ASSERT(cap > sizeof(AIL_Alloc_Arena));
	u8 *mem = backing_allocator->alloc(backing_allocator->data, cap);
	AIL_ASSERT(mem != NULL);

	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)mem;
	arena->start.idx  = 0;
	arena->start.size = cap - sizeof(AIL_Alloc_Arena);
	arena->start.next = NULL;
	arena->backing_allocator = backing_allocator;
	return (AIL_Allocator) {
		.data       = arena,
		.alloc      = &ail_alloc_arena_alloc,
		.zero_alloc = &ail_alloc_arena_calloc,
		.re_alloc   = &ail_alloc_arena_realloc,
		.free_one   = &ail_alloc_arena_free,
		.free_all   = &ail_alloc_arena_free_all_keep_regions,
	};
}

void *ail_alloc_arena_alloc(void *data, size_t size)
{
	bool too_big;
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
	u64 header_size = ail_alloc_align_size(sizeof(AIL_Alloc_Arena_Header));
	    size        = ail_alloc_align_size(size);
	AIL_Alloc_Arena_Region *region = &arena->start;
	while ((too_big = (region->idx + size + header_size > region->size)) && region->next) region = region->next;
	if (AIL_UNLIKELY(too_big)) {
		u64 region_size = AIL_MAX(region->size, size);
		region->next = arena->backing_allocator->alloc(arena->backing_allocator->data, region_size + sizeof(AIL_Alloc_Arena));
		if (!region->next) {
			AIL_ALLOC_LOG_ALLOC("arena", NULL, size);
			return NULL;
		}
		region = region->next;
		region->size = region_size;
	}
	u8 *mem = (u8 *)(&region[1]);
	AIL_Alloc_Arena_Header *header = (AIL_Alloc_Arena_Header *) &mem[region->idx];
	header->size = size;
	void *ptr    = &mem[region->idx + header_size];
	region->idx  += size + header_size;
	AIL_ALLOC_LOG_ALLOC("arena", ptr, size);
	return ptr;
}

void *ail_alloc_arena_calloc(void *data, size_t nelem, size_t elsize)
{
	void *ptr = ail_alloc_arena_alloc(data, nelem * elsize);
	if (ptr) {
		u64 size = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size;
		memset(ptr, 0, size);
	}
	AIL_ALLOC_LOG_CALLOC("arena", ptr, nelem, elsize);
	return ptr;
}

void *ail_alloc_arena_realloc(void *data, void *ptr, size_t size)
{
	if (!ptr) {
		void *out = ail_alloc_arena_alloc(data, size);
		AIL_ALLOC_LOG_REALLOC("arena", out, ptr, size);
		return out;
	}
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
	AIL_Alloc_Arena_Region *region = &arena->start;
	u8 *mem = (u8 *)&region[1];
	while ((u8 *)ptr < mem || (u8 *)ptr > mem + region->idx) {
		if (!region->next) {
			// Bounds check failure -> crash in debug mode and return null otherwise
			AIL_ALLOC_LOG_REALLOC("arena", NULL, ptr, size);
			AIL_UNREACHABLE();
			return NULL;
		}
		region = region->next;
		mem    = (u8 *)&region[1];
	}
	size_t old_size = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size;
	if (old_size >= size || (u8 *)ptr + old_size == mem + region->idx) {
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

void ail_alloc_arena_free(void *data, void *ptr)
{
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
	AIL_Alloc_Arena_Region *region = &arena->start;
	u8 *mem = (u8 *)&region[1];
	while ((u8 *)ptr < mem || (u8 *)ptr > mem + region->idx) {
		if (!region->next) {
			// Bounds checking failed -> crash in debug mode and just ignore it otherwise
			AIL_ALLOC_LOG_FREE("arena", ptr, (size_t)0);
			AIL_UNREACHABLE();
			return;
		}
		region = region->next;
		mem = (u8 *)&region[1];
	}
	// Free element, if it was the last one allocated
	size_t old_size = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size;
	if ((u8 *)ptr + old_size == mem + region->idx) region->idx -= old_size + sizeof(AIL_Alloc_Arena_Header);
	AIL_ALLOC_LOG_FREE("arena", ptr, (size_t)0);
}

void ail_alloc_arena_free_all(void *data)
{
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
	AIL_Alloc_Arena_Region *region = &arena->start;
	AIL_Alloc_Arena_Region *next;
	u64 size = region->idx;
	region->idx = 0;
	region   = region->next;
	while (region) {
		size  += region->idx;
		region->idx = 0;
		next   = region->next;
		arena->backing_allocator->free_one(arena->backing_allocator->data, region);
		region = next;
	}
	AIL_ALLOC_LOG_FREE_ALL("arena", size);
}

void ail_alloc_arena_free_all_keep_regions(void *data)
{
	u64 size = 0;
	AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)data;
	AIL_Alloc_Arena_Region *region = &arena->start;
	do {
		size += region->idx;
		region->idx = 0;
		region = region->next;
	} while (region);
	AIL_ALLOC_LOG_FREE_ALL("arena", size);
}


//////////
// Pool //
//////////

AIL_Allocator ail_alloc_pool_new(u64 bucket_amount, u64 el_size, AIL_Allocator *backing_allocator)
{
	u64 aligned_header_size = ail_alloc_align_size(sizeof(AIL_Alloc_Pool));
	u64 bucket_size         = ail_alloc_align_size(el_size + sizeof(AIL_Allloc_Pool_Free_Node));
	AIL_Alloc_Pool *pool    = (AIL_Alloc_Pool *)backing_allocator->alloc(backing_allocator->data, aligned_header_size + bucket_amount*bucket_size);
	pool->start.buf         = &((u8 *)pool)[aligned_header_size];
	pool->start.next        = NULL;
	pool->bucket_size       = bucket_size;
	pool->bucket_amount     = bucket_amount;
	pool->backing_allocator = backing_allocator;
	AIL_ASSERT(pool->start.buf != NULL);
	ail_alloc_pool_free_region(&pool->start, bucket_amount, bucket_size); // Set up free-list
	return (AIL_Allocator) {
		.data       = pool,
		.alloc      = &ail_alloc_pool_alloc,
		.zero_alloc = &ail_alloc_pool_calloc,
		.re_alloc   = &ail_alloc_pool_realloc,
		.free_one   = &ail_alloc_pool_free,
		.free_all   = &ail_alloc_pool_free_all,
	};
}

void *ail_alloc_pool_alloc(void *data, size_t size)
{
	AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
	AIL_Alloc_Pool_Region *region = &pool->start;
	AIL_Alloc_Pool_Region *prev   = NULL;
	while (region) {
		if (AIL_LIKELY(region->head)) {
			void *out = region->head;
			region->head = region->head->next;
			AIL_ALLOC_LOG_ALLOC("pool", out, size);
			return out;
		}
		prev   = region;
		region = region->next;
	}
	// No memory left -> need to allocate new region via backing allocator
	u64 aligned_header_size = ail_alloc_align_size(sizeof(AIL_Alloc_Pool_Region));
	AIL_Alloc_Pool_Region *next = (AIL_Alloc_Pool_Region *)pool->backing_allocator->alloc(pool->backing_allocator->data, aligned_header_size + pool->bucket_amount*pool->bucket_size);
	next->buf    = &((u8 *)next)[aligned_header_size];
	next->next   = NULL;
	ail_alloc_pool_free_region(next, pool->bucket_amount, pool->bucket_size);
	void *out    = next->head;
	next->head   = next->head->next;
	AIL_ASSERT(prev != NULL);
	prev->next = next;
	AIL_ALLOC_LOG_ALLOC("pool", out, size);
	return out;
}

void *ail_alloc_pool_calloc(void *data, size_t nelem, size_t elsize)
{
	AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
	void *ptr = ail_alloc_pool_alloc(data, nelem * elsize);
	if (ptr) memset(ptr, 0, pool->bucket_size);
	AIL_ALLOC_LOG_CALLOC("pool", ptr, nelem, elsize);
	return ptr;
}

void *ail_alloc_pool_realloc(void *data, void *ptr, size_t size)
{
	AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
	AIL_ASSERT(size <= pool->bucket_size);
	AIL_ALLOC_LOG_REALLOC("pool", ptr, ptr, size);
	// Since all buckets are the same size, reallocating for more space doesn't make sense and becomes a no-op
	return ptr;
}

void ail_alloc_pool_free(void *data, void *ptr)
{
	if (AIL_UNLIKELY(ptr == NULL)) return;
	AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
	AIL_Alloc_Pool_Region *region = &pool->start;
	while (region->buf > (u8 *)ptr || region->buf + pool->bucket_amount*pool->bucket_size < (u8 *)ptr) {
		AIL_ASSERT(region->next); // Out-of-Bounds
		region = region->next;
	}
	AIL_Allloc_Pool_Free_Node *node = (AIL_Allloc_Pool_Free_Node *)ptr;
	node->next   = region->head;
	region->head = node;
	AIL_ALLOC_LOG_FREE("pool", ptr, pool->bucket_size);
}

void ail_alloc_pool_free_region(AIL_Alloc_Pool_Region *region, u64 bucket_amount, u64 bucket_size)
{
	region->head = NULL;
	for (u64 i = 0; i < bucket_amount; i++) {
		AIL_Allloc_Pool_Free_Node *node = (AIL_Allloc_Pool_Free_Node *)(region->buf + i*bucket_size);
		node->next   = region->head;
		region->head = node;
	}
}

void ail_alloc_pool_free_all(void *data)
{
	AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
	// Remove regions after the start region
	AIL_Alloc_Pool_Region *region = pool->start.next;
	AIL_Alloc_Pool_Region *next;
	while (region) {
		next = region->next;
		pool->backing_allocator->free_one(pool->backing_allocator->data, region);
		region = next;
	}
	// Reset free-list for initial region
	ail_alloc_pool_free_region(&pool->start, pool->bucket_amount, pool->bucket_size);
	AIL_ALLOC_LOG_FREE_ALL("pool", pool->bucket_amount * pool->bucket_size);
}

void ail_alloc_pool_free_all_keep_regions(void *data)
{
	// Reset free-list for each region
	AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
	AIL_Alloc_Pool_Region *region = &pool->start;
	while (region) {
		ail_alloc_pool_free_region(region, pool->bucket_amount, pool->bucket_size);
		region = region->next;
	}
	AIL_ALLOC_LOG_FREE_ALL("pool", pool->bucket_amount * pool->bucket_size);
}


///////////////
// Free-List //
///////////////


typedef struct AIL_Alloc_Freelist_Free_Node {
	u64 size;
	struct AIL_Alloc_Freelist_Free_Node *next;
} AIL_Alloc_Freelist_Free_Node;

typedef struct AIL_Alloc_Freelist_Header {
	u64 size;
	u64 pad;
} AIL_Alloc_Freelist_Header;

typedef struct AIL_Alloc_Freelist {
	u8 *mem;
	u64 size;
	u64 used;
	AIL_Allocator *backing_allocator;
	AIL_Alloc_Freelist_Free_Node *head;
} AIL_Alloc_Freelist;

AIL_Allocator ail_alloc_freelist_new(u64 cap, AIL_Allocator *backing_allocator)
{
	u8 *mem = backing_allocator->alloc(backing_allocator->data, cap);
	AIL_Alloc_Freelist *fl = (AIL_Alloc_Freelist *)mem;
	fl->mem  = (u8 *)&fl[1];
	fl->size = cap - sizeof(AIL_Alloc_Freelist);
	fl->backing_allocator = backing_allocator;
	ail_alloc_freelist_free_all(fl); // Sets all other parameters of fl

	return (AIL_Allocator) {
		.data       = fl,
		.alloc      = &ail_alloc_freelist_alloc,
		.zero_alloc = &ail_alloc_freelist_calloc,
		.re_alloc   = &ail_alloc_freelist_realloc,
		.free_one   = &ail_alloc_freelist_free,
		.free_all   = &ail_alloc_freelist_free_all,
	};
}

void *ail_alloc_freelist_alloc(void *data, size_t size)
{
	AIL_Alloc_Freelist *fl = data;
	AIL_Alloc_Freelist_Free_Node *prev = NULL;
	AIL_Alloc_Freelist_Free_Node *node = fl->head;

	AIL_Alloc_Freelist_Header header = {
		.pad  = ail_alloc_align_size(sizeof(AIL_Alloc_Freelist_Header) + size),
		.size = size,
	};
	u64 req_size = sizeof(AIL_Alloc_Freelist_Header) + size + header.pad;
	while (node && req_size > node->size) {
		prev = node;
		node = node->next;
	}
	if (AIL_UNLIKELY(node == NULL)) {
		AIL_TODO();
		// @TODO: Allocate next region from backing allocator
	}

	AIL_Alloc_Freelist_Free_Node *next;
	if (node->size - req_size > AIL_MAX(sizeof(AIL_Alloc_Freelist_Free_Node), sizeof(AIL_Alloc_Freelist_Header))) {
		next = (AIL_Alloc_Freelist_Free_Node *)((u8 *)node + req_size);
		next->next = node->next;
		next->size = node->size - req_size;
	} else {
		next = node->next;
	}
	prev->next = next;

	fl->used += req_size;
	*(AIL_Alloc_Freelist_Header *)node = header;
	void *ptr = (u8 *)node + sizeof(AIL_Alloc_Freelist_Header);
	AIL_ALLOC_LOG_ALLOC("freelist", ptr, size);
	return ptr;
}

void *ail_alloc_freelist_calloc(void *data, size_t nelem, size_t elsize)
{
	u64 size = nelem * elsize;
	u8 *ptr  = ail_alloc_freelist_alloc(data, size);
	memset(ptr, 0, size);
	AIL_ALLOC_LOG_CALLOC("freelist", ptr, nelem, elsize);
	return ptr;
}

void *ail_alloc_freelist_realloc(void *data, void *ptr, size_t size)
{
	AIL_Alloc_Freelist *fl = data;
	AIL_UNUSED(fl);
	AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Freelist_Header);
	if (size <= header->size) {
		AIL_ALLOC_LOG_REALLOC("freelist", ptr, ptr, size);
		return ptr;
	}
	ail_alloc_freelist_free(data, ptr);
	void *nptr = ail_alloc_freelist_alloc(data, size);
	// The old data cannot be overwritten by a single alloc (assuming that sizeof(AIL_Alloc_Freelist_Header) >= sizeof(AIL_Alloc_Freelist_Free_Node))
	// it should thus be safe to allocate the new region after freeing the old region
	AIL_STATIC_ASSERT(sizeof(AIL_Alloc_Freelist_Header) >= sizeof(AIL_Alloc_Freelist_Free_Node));
	memcpy(nptr, ptr, size); // @Bug: memcpy might not work correctly, if the old and new region overlap
	AIL_ALLOC_LOG_REALLOC("freelist", nptr, ptr, size);
	return nptr;
}

void ail_alloc_freelist_free(void *data, void *ptr)
{
	// @TODO: Bounds checking?
	if (!ptr) return;
	AIL_Alloc_Freelist *fl = data;
	AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Freelist_Header);
	u64 block_size = header->size + header->pad + sizeof(AIL_Alloc_Freelist_Header);
	fl->used -= block_size;

	// @TODO: Implementation not done and potentially wrong
	AIL_Alloc_Freelist_Free_Node *prev = NULL;
	AIL_Alloc_Freelist_Free_Node *node = fl->head;
	while (node) {
		if ((u8 *)ptr < (u8 *)node) {
			AIL_Alloc_Freelist_Free_Node *new_node = (AIL_Alloc_Freelist_Free_Node *)ptr;
			new_node->next = node;
			new_node->size = block_size;
			if (AIL_LIKELY(prev)) prev->next = new_node;
			else                  fl->head   = new_node;
			if ((u8 *)new_node + new_node->size == (u8 *)node) {
				new_node->size += node->size;
				new_node->next  = node->next;
			}
			break;
		} else if ((u8 *)node + node->size == (u8 *)ptr) {
			node->size += block_size;
			if ((u8 *)node + node->size == (u8 *)node->next) {
				node->size += node->next->size;
				node->next  = node->next->next;
			}
			break;
		}
		node = node->next;
	}
}

void ail_alloc_freelist_free_all(void *data)
{
	// @TODO: Deal with having several regions allocated from backing allocator
	AIL_Alloc_Freelist *fl = data;
	AIL_Alloc_Freelist_Free_Node *head = (AIL_Alloc_Freelist_Free_Node *)fl->mem;
	head->size = fl->size;
	head->next = NULL;
	fl->head   = head;
	fl->used   = 0;
}



#endif // _AIL_ALLOC_IMPL_GUARD_
#endif // AIL_ALLOC_IMPL