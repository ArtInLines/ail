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

// @TODO: Growable allocators should check whether regions can be coalesced (i.e. current region + size == next region)

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
    AIL_Alloc_Arena_Region start;  // @Note: Code assumes that Region starts right before the rest of the data
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

typedef struct AIL_Alloc_Freelist_Free_Node {
    u64 size;
    struct AIL_Alloc_Freelist_Free_Node *next;
} AIL_Alloc_Freelist_Free_Node;

typedef struct AIL_Alloc_Freelist_Header {
    u64 size;
    u64 pad;
} AIL_Alloc_Freelist_Header;

typedef struct AIL_Alloc_Freelist_Region {
    u8 *mem;
    u64 size;
    u64 used;
    AIL_Alloc_Freelist_Free_Node *head;
    struct AIL_Alloc_Freelist_Region *next;
} AIL_Alloc_Freelist_Region;

typedef struct AIL_Alloc_Freelist {
    AIL_Allocator *backing_allocator;
    AIL_Alloc_Freelist_Region start;
} AIL_Alloc_Freelist;


//////////////
// Utility Functions
//////////////

// Aligns size to AIL_ALLOC_ALIGNMENT
AIL_ALLOC_DEF size_t ail_alloc_align_size(size_t size);
// @Note: Alignment must be greater than 0 and a power of 2
AIL_ALLOC_DEF size_t ail_alloc_align_forward(size_t n, size_t alignment);
// @Note: Alignment must be greater than 0 and a power of 2
AIL_ALLOC_DEF size_t ail_alloc_align_backward(size_t n, size_t alignment);
// Returns padding from aligning to AIL_ALLOC_ALIGNMENT
AIL_ALLOC_DEF size_t ail_alloc_size_aligned_pad(size_t size);
AIL_ALLOC_DEF size_t ail_alloc_size_aligned_backward_pad(size_t size, size_t alignment);
AIL_ALLOC_DEF size_t ail_alloc_size_aligned_forward_pad(size_t size, size_t alignment);

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
AIL_ALLOC_DEF void ail_alloc_arena_free_all_keep_regions(void *data);

//////////////
// Pool Allocator
// Allocates only fixed-size chunks
// Allows freeing any chunk at any time
// @TODO: Allow allocating several chunks at once (make sure that all of those chunks get freed at once too)
//////////////
AIL_ALLOC_DEF AIL_Allocator ail_alloc_pool_new(u64 bucket_amount, u64 el_size, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_pool_alloc;
AIL_ALLOC_DEF void ail_alloc_pool_free_all_keep_regions(void *data);

//////////////
// Free-List Allocator
// Allows allocating arbitrarily sized regions
// Stores a linked-list internally to keep track of allocations
//////////////
AIL_ALLOC_DEF AIL_Allocator ail_alloc_freelist_new(u64 cap, AIL_Allocator *backing_allocator);
AIL_ALLOC_DEF AIL_Allocator_Func ail_alloc_freelist_alloc;
AIL_ALLOC_DEF void ail_alloc_freelist_free_all_keep_regions(void *data);


#endif // AIL_ALLOC_H_

#define AIL_ALLOC_IMPL
#ifdef AIL_ALLOC_IMPL
#ifndef _AIL_ALLOC_IMPL_GUARD_
#define _AIL_ALLOC_IMPL_GUARD_

#include <stdlib.h> // For std_allocator and memset

// @TODO: Provide reserve/commited capacities for allocators instead of just one capacity
// @TODO: Linked-List Region allocators should merge regions together if a new region is right next to the last region
// @TODO: Implement Page Allocations for OSes other than WINDOWS and UNIX
// @Performance: Check whether page calloc actually requires a memset or whether they are required to return zero-pages already
#if defined(_WIN32)
#include <Windows.h>
// #include <memoryapi.h> // For VirtualAlloc
#else
#include <sys/mman.h>
#endif

// For tracing memory
#ifndef AIL_ALLOC_PRINT_MEM
#define _AIL_ALLOC_LOG(...) do { if (0) printf(__VA_ARGS__); } while(0)
#else
#define _AIL_ALLOC_LOG(...) do { AIL_DBG_PRINT("Memory Trace: " __VA_ARGS__); AIL_DBG_PRINT("\n"); } while(0)
#endif // AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_LOG_ALLOC(allocator, ptr, size)          _AIL_ALLOC_LOG("Malloc  %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CALLOC(allocator, ptr, size)         _AIL_ALLOC_LOG("Calloc  %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_REALLOC(allocator, nptr, optr, size) _AIL_ALLOC_LOG("Relloc  %4llu bytes from %p to %p in '" allocator "' allocator", (size), (optr), (nptr));
#define AIL_ALLOC_LOG_FREE(allocator, ptr, size)           _AIL_ALLOC_LOG("Free    %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_FREE_ALL(allocator, size)            _AIL_ALLOC_LOG("FreeAll %4llu bytes in '" allocator "' allocator", (size));
#define AIL_ALLOC_LOG(allocator, mode, ptr, size, optr) do {       \
        switch(mode) {                                             \
            case AIL_MEM_ALLOC:                                    \
                AIL_ALLOC_LOG_ALLOC(allocator, ptr, size);         \
                break;                                             \
            case AIL_MEM_CALLOC:                                   \
                AIL_ALLOC_LOG_CALLOC(allocator, ptr, size);        \
                break;                                             \
            case AIL_MEM_REALLOC:                                  \
                AIL_ALLOC_LOG_REALLOC(allocator, ptr, optr, size); \
                break;                                             \
            case AIL_MEM_FREE:                                     \
                AIL_ALLOC_LOG_FREE(allocator, ptr, size);          \
                break;                                             \
            case AIL_MEM_FREE_ALL:                                 \
                AIL_ALLOC_LOG_FREE_ALL(allocator, size);           \
                break;                                             \
        }                                                          \
    } while(0)

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

size_t ail_alloc_size_aligned_pad(size_t size)
{
    return ail_alloc_align_size(size) - size;
}

size_t ail_alloc_size_aligned_forward_pad(size_t size, size_t alignment)
{
    return ail_alloc_align_forward(size, alignment) - size;
}

size_t ail_alloc_size_aligned_backward_pad(size_t size, size_t alignment)
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

void* ail_alloc_std_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr, u64 old_size)
{
    AIL_UNUSED(data); AIL_UNUSED(size); AIL_UNUSED(old_size);
    void *res = NULL;
    switch (mode) {
        case AIL_MEM_ALLOC:    res = malloc(size); break;
        case AIL_MEM_CALLOC:   res = calloc(size, 1); break;
        case AIL_MEM_REALLOC:  res = realloc(old_ptr, size); break;
        case AIL_MEM_FREE:     free(old_ptr); break;
        case AIL_MEM_FREE_ALL: break;
    }
    AIL_ALLOC_LOG("std", mode, res, size, old_ptr);
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
static inline void _ail_alloc_internal_free_pages_(void *ptr, u64 size)
{
#if defined(_WIN32)
    VirtualFree(ptr, size, MEM_DECOMMIT);
#else
    munmap(ptr, size);
#endif
}

static void* _ail_alloc_page_internal_alloc_(u64 size)
{
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

void *ail_alloc_page_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr, u64 old_size)
{
    AIL_UNUSED(data); AIL_UNUSED(old_size);
    void *res = NULL;
    switch (mode) {
        case AIL_MEM_ALLOC: {
            res = _ail_alloc_page_internal_alloc_(size);
        } break;
        case AIL_MEM_CALLOC: {
            res = _ail_alloc_page_internal_alloc_(size);
            memset(res, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            // @TODO: Track some metadata in the data pointer to allow checking the old size before reallocating
            AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
            u64 old_size = header->size;
            if (size <= old_size) {
                u64 sz_diff = old_size - size;
                u64 aligned_sz_diff = ail_alloc_align_forward(sz_diff, AIL_ALLOC_PAGE_SIZE);
                if (aligned_sz_diff >= AIL_ALLOC_PAGE_SIZE) {
                    _ail_alloc_internal_free_pages_((void *)ail_alloc_align_forward((size_t)((char *)old_ptr + size), AIL_ALLOC_PAGE_SIZE), aligned_sz_diff);
                }
                // header->size = size;
                res = old_ptr;
            } else {
                // @TODO: VirtualAlloc can take the previous pointer to potentially just increase the size
                // @TODO: mmap has some kind of hint system, that probably does more or less the same
                AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
                _ail_alloc_internal_free_pages_((void*)header, ail_alloc_align_forward(header->size, AIL_ALLOC_PAGE_SIZE));
                res = _ail_alloc_page_internal_alloc_(size);
            }
        } break;
        case AIL_MEM_FREE: {
            AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
            _ail_alloc_internal_free_pages_((void *)header, ail_alloc_align_forward(header->size, AIL_ALLOC_PAGE_SIZE));
        } break;
        case AIL_MEM_FREE_ALL: break;
    }
    AIL_ALLOC_LOG("page", mode, res, size, old_ptr);
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

AIL_DEF_INLINE void* _ail_alloc_buffer_internal_alloc_(AIL_Alloc_Buffer *buffer, u8 *mem, u64 size)
{
    void *ptr = NULL;
    if (AIL_LIKELY(size + buffer->idx < buffer->size)) {
        ptr = &mem[buffer->idx];
        buffer->idx += size;
    }
    return ptr;
}

void *ail_alloc_buffer_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr, u64 old_size)
{
    void *ptr = NULL;
    AIL_Alloc_Buffer *buffer = data;
    u8 *mem = (u8 *)&buffer[1];
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_buffer_internal_alloc_(buffer, mem, size);
        } break;
        case AIL_MEM_CALLOC: {
            void *ptr = _ail_alloc_buffer_internal_alloc_(data, mem, size);
            if (ptr) memset(ptr, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            if (!old_ptr) {
                ptr = _ail_alloc_buffer_internal_alloc_(data, mem, size);
            } else {
                u64 max_old_size = AIL_MIN(size, (u64)mem + buffer->idx - (u64)ptr);
                ptr = _ail_alloc_buffer_internal_alloc_(data, mem, size);
                memcpy(ptr, old_ptr, max_old_size);
            }
        } break;
        case AIL_MEM_FREE: {
            if (AIL_UNLIKELY(old_size && (u8 *)old_ptr + old_size == mem + buffer->idx)) buffer->idx -= old_size;
        } break;
        case AIL_MEM_FREE_ALL: {
            buffer->idx = 0;
        } break;
    }
    AIL_ALLOC_LOG("buffer", mode, ptr, size, old_ptr);
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

void* _ail_alloc_ring_internal_alloc_(AIL_Alloc_Ring *ring, u8 *mem, size_t size)
{
    void *ptr = NULL;
    if (AIL_UNLIKELY(size + ring->idx >= ring->size)) ring->idx = 0;
    ptr = &mem[ring->idx];
    ring->idx += size;
    return ptr;
}

void *ail_alloc_ring_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr, u64 old_size)
{
    AIL_UNUSED(old_size);
    void *ptr = NULL;
    AIL_Alloc_Ring *ring = data;
    u8 *mem = (u8 *)&ring[1];
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_ring_internal_alloc_(ring, mem, size);
        } break;
        case AIL_MEM_CALLOC: {
            ptr = _ail_alloc_ring_internal_alloc_(data, mem, size);
            if (ptr) memset(ptr, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            if (!old_ptr) {
                ptr = _ail_alloc_ring_internal_alloc_(data, mem, size);
            } else {
                u64 max_old_size = AIL_MIN(size, (u64)mem + ring->idx - (u64)old_ptr);
                ptr = _ail_alloc_ring_internal_alloc_(data, mem, size);
                memcpy(ptr, old_ptr, max_old_size); // @Bug: memcpy might not work correctly, if the new poiner wrapped around and its region overlaps with the old region
            }
        } break;
        case AIL_MEM_FREE: break;
        case AIL_MEM_FREE_ALL: {
            ring->idx = 0;
        } break;
    }
    AIL_ALLOC_LOG("ring", mode, ptr, size, old_ptr);
    return ptr;
}


///////////
// Arena //
///////////

AIL_Allocator ail_alloc_arena_new(u64 cap, AIL_Allocator *backing_allocator)
{
    AIL_ASSERT(cap > sizeof(AIL_Alloc_Arena));
    u8 *mem = AIL_CALL_ALLOC(*backing_allocator, cap);
    AIL_ASSERT(mem != NULL);

    AIL_Alloc_Arena *arena = (AIL_Alloc_Arena *)mem;
    arena->start.idx  = 0;
    arena->start.size = cap - ail_alloc_align_size(sizeof(AIL_Alloc_Arena));
    arena->start.next = NULL;
    arena->backing_allocator = backing_allocator;
    return (AIL_Allocator) {
        .data       = arena,
        .alloc      = &ail_alloc_arena_alloc,
    };
}

AIL_Alloc_Arena_Region *_ail_alloc_internal_new_region_(AIL_Alloc_Arena *arena, u64 old_region_size, size_t min_new_size)
{
    u64 region_size = AIL_MAX(old_region_size, min_new_size);
    AIL_Alloc_Arena_Region *new_region = AIL_CALL_ALLOC(*arena->backing_allocator, region_size + ail_alloc_align_size(sizeof(AIL_Alloc_Arena)));
    new_region->size = region_size;
    return new_region;
}

void* _ail_alloc_arena_internal_alloc_(AIL_Alloc_Arena *arena, u64 header_size, u64 size)
{
    bool too_big;
    AIL_Alloc_Arena_Region *region = &arena->start;
    while ((too_big = (region->idx + size + header_size > region->size)) && region->next) region = region->next;
    if (AIL_UNLIKELY(too_big)) {
        AIL_Alloc_Arena_Region *new_region = _ail_alloc_internal_new_region_(arena, region->size, size);
        if (AIL_UNLIKELY(!new_region)) {
            AIL_ALLOC_LOG_ALLOC("arena", NULL, size);
            return NULL;
        }
        if (region == &arena->start) {
            region->next = new_region;
        } else {
            new_region->next = region;
            arena->start.next = new_region;
        }
        region = new_region;
    }
    u8 *mem = (u8 *)&region[1];
    //u8 *mem = &((u8 *)region)[ail_alloc_align_size(sizeof(AIL_Alloc_Arena))];
    AIL_Alloc_Arena_Header *header = (AIL_Alloc_Arena_Header *) &mem[region->idx];
    header->size = size;
    void *ptr    = &mem[region->idx + header_size];
    region->idx  += size + header_size;
    return ptr;
}

void *_ail_alloc_arena_internal_realloc_(AIL_Alloc_Arena *arena, u64 header_size, void *old_ptr, size_t size)
{
    if (!old_ptr) {
        return _ail_alloc_arena_internal_alloc_(arena, header_size, size);
    }
    AIL_Alloc_Arena_Region *region = &arena->start;
    u8 *mem = (u8 *)&region[1];
    while ((u8 *)old_ptr < mem || (u8 *)old_ptr > mem + region->idx) {
        if (!region->next) {
            // Bounds check failure -> crash in debug mode and return null otherwise
            AIL_UNREACHABLE();
            return NULL;
        }
        region = region->next;
        mem    = (u8 *)&region[1];
    }
    u64 old_size  = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Arena_Header)->size;
    void *new_ptr = old_ptr;
    if (old_size >= size || (u8 *)old_ptr + old_size == mem + region->idx) {
        // If size didn't increase, or old_ptr points at the last allocation, no memory needs to be moved
        // Unless the new size doesn't fit into this region, that is
        if ((u8 *)old_ptr + size > mem + region->size) {
            size = ail_alloc_align_size(size);
            region->idx = (u8 *)old_ptr - sizeof(AIL_Alloc_Arena_Header) - mem; // Free memory from previous region
            region->next = _ail_alloc_internal_new_region_(arena, region->size, size);
            region = region->next;
            if (!region) {
                return NULL;
            }
            u64 header_size = ail_alloc_align_size(sizeof(AIL_Alloc_Arena_Header));
            u8 *mem = (u8 *)&region[1];
            //u8 *mem = &((u8 *)region)[ail_alloc_align_size(sizeof(AIL_Alloc_Arena))];
            ((AIL_Alloc_Arena_Header *)mem)->size = size;
            new_ptr = &mem[header_size];
            region->idx += size + header_size;
        } else {
            AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Arena_Header)->size = size; // Potentially shrink used region
            new_ptr = old_ptr;
        }
    } else {
        new_ptr = _ail_alloc_arena_internal_alloc_(arena, header_size, size);
        if (new_ptr) memcpy(new_ptr, old_ptr, old_size);
    }
    return new_ptr;
}

void* ail_alloc_arena_alloc(void *data, AIL_Allocator_Mode mode, _AIL_ALLOCATOR_SIZE_TYPE size, void *old_ptr, _AIL_ALLOCATOR_SIZE_TYPE old_size)
{
    AIL_UNUSED(old_size);
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
            if (ptr) {
                memset(ptr, 0, AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size);
            }
        } break;
        case AIL_MEM_REALLOC: {
            ptr = _ail_alloc_arena_internal_realloc_(arena, header_size, old_ptr, size);
        } break;
        case AIL_MEM_FREE: {
            AIL_Alloc_Arena_Region *region = &arena->start;
            u8 *mem = (u8 *)&region[1];
            while ((u8 *)old_ptr < mem || (u8 *)old_ptr > mem + region->idx) {
                if (!region->next) {
                    // Bounds checking failed -> crash in debug mode and just ignore it otherwise
                    AIL_UNREACHABLE();
                    goto done;
                }
                region = region->next;
                mem = (u8 *)&region[1];
            }
            // Free element, if it was the last one allocated
            size_t old_size = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Arena_Header)->size;
            if ((u8 *)old_ptr + old_size == mem + region->idx) region->idx -= old_size + sizeof(AIL_Alloc_Arena_Header);
        } break;
        case AIL_MEM_FREE_ALL: {
            AIL_Alloc_Arena_Region *region = &arena->start;
            AIL_Alloc_Arena_Region *next;
            u64 size    = region->idx;
            region->idx = 0;
            region      = region->next;
            while (region) {
                size  += region->idx;
                region->idx = 0;
                next   = region->next;
                AIL_CALL_FREE(*arena->backing_allocator, region);
                region = next;
            }
        } break;
    }
done:
    AIL_ALLOC_LOG("arena", mode, ptr, size, old_ptr);
    return ptr;
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

void _ail_alloc_pool_internal_free_region_(AIL_Alloc_Pool_Region *region, u64 bucket_amount, u64 bucket_size)
{
    region->head = NULL;
    for (u64 i = 0; i < bucket_amount; i++) {
        AIL_Allloc_Pool_Free_Node *node = (AIL_Allloc_Pool_Free_Node *)(region->buf + i*bucket_size);
        node->next   = region->head;
        region->head = node;
    }
}

void* _ail_alloc_pool_internal_alloc_(AIL_Alloc_Pool *pool, u64 header_size)
{
    AIL_Alloc_Pool_Region *region = &pool->start;
    AIL_Alloc_Pool_Region *prev   = NULL;
    while (region) {
        if (AIL_LIKELY(region->head)) {
            void *ptr = region->head;
            region->head = region->head->next;
            return ptr;
        }
        prev   = region;
        region = region->next;
    }
    // No memory left -> need to allocate new region via backing allocator
    AIL_Alloc_Pool_Region *next = AIL_CALL_ALLOC(*pool->backing_allocator, header_size + pool->bucket_amount*pool->bucket_size);
    next->buf    = &((u8 *)next)[header_size];
    next->next   = NULL;
    _ail_alloc_pool_internal_free_region_(next, pool->bucket_amount, pool->bucket_size);
    void *ptr    = next->head;
    next->head   = next->head->next;
    AIL_ASSERT(prev != NULL);
    prev->next = next;
    return ptr;
}

AIL_Allocator ail_alloc_pool_new(u64 bucket_amount, u64 el_size, AIL_Allocator *backing_allocator)
{
    u64 aligned_header_size = ail_alloc_align_size(sizeof(AIL_Alloc_Pool));
    u64 bucket_size         = ail_alloc_align_size(AIL_MAX(el_size, sizeof(AIL_Allloc_Pool_Free_Node)));
    AIL_Alloc_Pool *pool    = AIL_CALL_ALLOC(*backing_allocator, aligned_header_size + bucket_amount*bucket_size);
    pool->start.buf         = &((u8 *)pool)[aligned_header_size];
    pool->start.next        = NULL;
    pool->bucket_size       = bucket_size;
    pool->bucket_amount     = bucket_amount;
    pool->backing_allocator = backing_allocator;
    AIL_ASSERT(pool->start.buf != NULL);
    _ail_alloc_pool_internal_free_region_(&pool->start, bucket_amount, bucket_size); // Set up free-list
    return (AIL_Allocator) {
        .data       = pool,
        .alloc      = &ail_alloc_pool_alloc,
    };
}

void* ail_alloc_pool_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr, u64 old_size)
{
    AIL_UNUSED(size); AIL_UNUSED(old_size);
    AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
    u64 header_size = ail_alloc_align_size(sizeof(AIL_Alloc_Pool_Region));
    void *ptr = NULL;
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_pool_internal_alloc_(pool, header_size);
        } break;
        case AIL_MEM_CALLOC: {
            ptr = _ail_alloc_pool_internal_alloc_(pool, header_size);
            if (ptr) memset(ptr, 0, pool->bucket_size);
        } break;
        case AIL_MEM_REALLOC: {
            // Since all buckets are the same size, reallocating for more space doesn't make sense and becomes a no-op
            AIL_ASSERT(size <= pool->bucket_size);
            ptr = old_ptr;
        } break;
        case AIL_MEM_FREE: {
            if (AIL_LIKELY(old_ptr != NULL)) {
                AIL_Alloc_Pool_Region *region = &pool->start;
                while (region->buf > (u8 *)old_ptr || region->buf + pool->bucket_amount*pool->bucket_size < (u8 *)old_ptr) {
                    AIL_ASSERT(region->next); // Out-of-Bounds
                    region = region->next;
                }
                AIL_Allloc_Pool_Free_Node *node = old_ptr;
                node->next   = region->head;
                region->head = node;
            }
        } break;
        case AIL_MEM_FREE_ALL: {
            // Remove regions after the start region
            AIL_Alloc_Pool_Region *region = pool->start.next;
            AIL_Alloc_Pool_Region *next;
            while (region) {
                next = region->next;
                AIL_CALL_FREE(*pool->backing_allocator, region);
                region = next;
            }
            // Reset free-list for initial region
            _ail_alloc_pool_internal_free_region_(&pool->start, pool->bucket_amount, pool->bucket_size);
        } break;
    }
    AIL_ALLOC_LOG("pool", mode, ptr, size, old_ptr);
    return ptr;
}

void ail_alloc_pool_free_all_keep_regions(void *data)
{
    // Reset free-list for each region
    AIL_Alloc_Pool *pool = (AIL_Alloc_Pool *)data;
    AIL_Alloc_Pool_Region *region = &pool->start;
    while (region) {
        _ail_alloc_pool_internal_free_region_(region, pool->bucket_amount, pool->bucket_size);
        region = region->next;
    }
    AIL_ALLOC_LOG_FREE_ALL("pool", pool->bucket_amount * pool->bucket_size);
}


///////////////
// Free-List //
///////////////

void _ail_alloc_freelist_internal_free_region_(AIL_Alloc_Freelist_Region *region)
{
    AIL_Alloc_Freelist_Free_Node *head = (AIL_Alloc_Freelist_Free_Node *)region->mem;
    head->size   = region->size;
    head->next   = NULL;
    region->head = head;
    region->used = 0;
}

void *_ail_alloc_freelist_internal_alloc_(AIL_Alloc_Freelist *fl, AIL_Alloc_Freelist_Region *region, size_t size)
{
    AIL_Alloc_Freelist_Region *prev_region = NULL;
    AIL_Alloc_Freelist_Free_Node *prev     = NULL;
    AIL_Alloc_Freelist_Free_Node *node     = region->head;

    AIL_Alloc_Freelist_Header header = {
        .pad  = ail_alloc_size_aligned_pad(sizeof(AIL_Alloc_Freelist_Header) + size),
        .size = size,
    };
    u64 req_size = sizeof(AIL_Alloc_Freelist_Header) + size + header.pad;
    while (region) {
        while (node && req_size > node->size) {
            prev = node;
            node = node->next;
        }
        if (node) break;
        prev_region = region;
        region      = region->next;
    }
    if (AIL_UNLIKELY(node == NULL)) {
        u64 cap    = ail_alloc_align_size(AIL_MAX(fl->start.size + sizeof(AIL_Alloc_Freelist_Region), size));
        AIL_Alloc_Freelist_Region *next = AIL_CALL_ALLOC(*fl->backing_allocator, cap);
        next->mem  = (u8 *)&next[1];
        next->size = cap - sizeof(AIL_Alloc_Freelist_Region);
        _ail_alloc_freelist_internal_free_region_(next);
        if (prev_region) prev_region->next = next;
        else             fl->start.next    = next;
        region = next;
        node   = region->head;
    }

    AIL_Alloc_Freelist_Free_Node *next;
    if (node->size - req_size > AIL_MAX(sizeof(AIL_Alloc_Freelist_Free_Node), sizeof(AIL_Alloc_Freelist_Header))) {
        next = (AIL_Alloc_Freelist_Free_Node *)((u8 *)node + req_size);
        next->next = node->next;
        next->size = node->size - req_size;
    } else {
        next = node->next;
    }
    if (prev) prev->next  = next;
    else     region->head = next;

    region->used += req_size;
    // printf("Used: %lld out of %lld\n", fl->used, fl->size);
    *(AIL_Alloc_Freelist_Header *)node = header;
    void *ptr = (u8 *)node + sizeof(AIL_Alloc_Freelist_Header);
    return ptr;
}

void _ail_alloc_freelist_internal_free_(AIL_Alloc_Freelist *fl, u8 *old_ptr)
{
    // @TODO: Full Bounds checking?
    if (!old_ptr) return;
    AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Freelist_Header);
    u64 block_size = header->size + header->pad + sizeof(AIL_Alloc_Freelist_Header);
    AIL_ALLOC_LOG_FREE("freelist", old_ptr, header->size);

    AIL_Alloc_Freelist_Region *region = &fl->start;
    while (region) {
        AIL_Alloc_Freelist_Free_Node *prev = NULL;
        AIL_Alloc_Freelist_Free_Node *node = region->head;
        while (node) {
            if (old_ptr < (u8 *)node) {
                AIL_Alloc_Freelist_Free_Node *new_node = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Freelist_Free_Node);
                new_node->next = node;
                new_node->size = block_size;
                if (prev) prev->next   = new_node;
                else      region->head = new_node;
                if ((u8 *)new_node + block_size == (u8 *)node) {
                    new_node->size += node->size;
                    new_node->next  = node->next;
                }
                AIL_ASSERT(region->used >= block_size);
                region->used -= block_size;
                return;
            } else if ((u8 *)node + node->size == old_ptr) {
                node->size += block_size;
                if ((u8 *)node + node->size == (u8 *)node->next) {
                    node->size += node->next->size;
                    node->next  = node->next->next;
                }
                AIL_ASSERT(region->used >= block_size);
                region->used -= block_size;
                return;
            }
            node = node->next;
        }
        if (old_ptr < region->mem + region->size) {
            AIL_Alloc_Freelist_Free_Node *new_node = (AIL_Alloc_Freelist_Free_Node *)old_ptr;
            new_node->next = NULL;
            new_node->size = block_size;
            if (prev) prev->next   = new_node;
            else      region->head = new_node;
            AIL_ASSERT(region->used >= block_size);
            region->used -= block_size;
            return;
        }
        printf("Switching region in free\n");
        region = region->next;
    }
    AIL_UNREACHABLE(); // Out-of-Bounds
}

AIL_Allocator ail_alloc_freelist_new(u64 cap, AIL_Allocator *backing_allocator)
{
    u8 *mem = AIL_CALL_ALLOC(*backing_allocator, cap);
    AIL_Alloc_Freelist *fl = (AIL_Alloc_Freelist *)mem;
    fl->start.mem  = (u8 *)&fl[1];
    fl->start.size = cap - sizeof(AIL_Alloc_Freelist);
    fl->backing_allocator = backing_allocator;
    _ail_alloc_freelist_internal_free_region_(&fl->start); // Sets all other parameters of fl
    return (AIL_Allocator) {
        .data       = fl,
        .alloc      = &ail_alloc_freelist_alloc,
    };
}

void *ail_alloc_freelist_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr, u64 old_size)
{
    AIL_UNUSED(old_size);
    void *ptr = NULL;
    AIL_Alloc_Freelist *fl = data;
    AIL_Alloc_Freelist_Region *region = &fl->start;
    switch (mode) {
        case AIL_MEM_ALLOC: {
            ptr = _ail_alloc_freelist_internal_alloc_(fl, region, size);
        } break;
        case AIL_MEM_CALLOC: {
            ptr = _ail_alloc_freelist_internal_alloc_(fl, region, size);
            if (ptr) memset(ptr, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            if (!old_ptr) {
                ptr = _ail_alloc_freelist_internal_alloc_(fl, region, size);
            } else {
                AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Freelist_Header);
                if (size <= header->size + header->pad) {
                    u64 new_pad  = header->size + header->pad - size;
                    header->size = size;
                    header->pad  = new_pad;
                    ptr = old_ptr;
                } else {
                    ptr = _ail_alloc_freelist_internal_alloc_(fl, region, size);
                    if (ptr) memcpy(ptr, old_ptr, size);
                    _ail_alloc_freelist_internal_free_(fl, old_ptr);
                }
            }
        } break;
        case AIL_MEM_FREE: {
            _ail_alloc_freelist_internal_free_(fl, old_ptr);
        } break;
        case AIL_MEM_FREE_ALL: {
            AIL_Alloc_Freelist_Region *next;
            u64 total = fl->start.used;
            while (region) {
                total += region->used;
                next   = region->next;
                AIL_CALL_FREE(*fl->backing_allocator, region);
                region = next;
            }
            _ail_alloc_freelist_internal_free_region_(&fl->start);
        } break;
    }
    AIL_ALLOC_LOG("freelist", mode, ptr, size, old_ptr);
    return ptr;
}

void ail_alloc_freelist_free_all_keep_regions(void *data)
{
    AIL_Alloc_Freelist *fl = data;
    AIL_Alloc_Freelist_Region *region = &fl->start;
    u64 total = 0;
    while (region) {
        total += region->used;
        _ail_alloc_freelist_internal_free_region_(region);
        region = region->next;
    }
    AIL_ALLOC_LOG_FREE_ALL("freelist", total);
}


#endif // _AIL_ALLOC_IMPL_GUARD_
#endif // AIL_ALLOC_IMPL
