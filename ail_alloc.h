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

#define AIL_ALLOC_IMPL
#ifdef AIL_ALLOC_IMPL
#ifndef _AIL_ALLOC_IMPL_GUARD_
#define _AIL_ALLOC_IMPL_GUARD_

#include <stdlib.h> // For std_allocator and memset

// @TODO: Provide reserve/committ capacities for allocators instead of just one capacity
// @TODO: Implement Page Allocations for OSes other than WINDOWS and UNIX
// @TODO: Check whether page calloc actually requires a memset or whether they are required to return zero-pages already
#if defined(_WIN32)
#include <Windows.h> // For VirtualAlloc, VirtualFree
#else
#include <sys/mman.h> // For mmap, munmap
#endif

// For tracing memory
#ifndef AIL_ALLOC_PRINT_MEM
#define _AIL_ALLOC_LOG(...) do { if (0) printf(__VA_ARGS__); } while(0)
#else
#define _AIL_ALLOC_LOG(...) do { AIL_DBG_PRINT("Memory Trace: " __VA_ARGS__); AIL_DBG_PRINT("\n"); } while(0)
#endif // AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_LOG_ALLOC(allocator, ptr, size)          _AIL_ALLOC_LOG("Malloc   %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CALLOC(allocator, ptr, size)         _AIL_ALLOC_LOG("Calloc   %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_REALLOC(allocator, nptr, optr, size) _AIL_ALLOC_LOG("Relloc   %4llu bytes from %p to %p in '" allocator "' allocator", (size), (optr), (nptr));
#define AIL_ALLOC_LOG_FREE(allocator, ptr, size)           _AIL_ALLOC_LOG("Free     %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CLEAR_ALL(allocator, size)           _AIL_ALLOC_LOG("ClearAll %4llu bytes in '" allocator "' allocator", (size));
#define AIL_ALLOC_LOG_FREE_ALL(allocator, size)            _AIL_ALLOC_LOG("FreeAll  %4llu bytes in '" allocator "' allocator", (size));
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
                AIL_ALLOC_LOG_FREE(allocator, optr, size);         \
                break;                                             \
            case AIL_MEM_CLEAR_ALL:                                \
                AIL_ALLOC_LOG_CLEAR_ALL(allocator, size);          \
                break;                                             \
            case AIL_MEM_FREE_ALL:                                 \
                AIL_ALLOC_LOG_FREE_ALL(allocator, size);           \
                break;                                             \
        }                                                          \
    } while(0)

#define AIL_ALLOC_GET_LAST_REGION(listPtr) _ail_alloc_get_last_region_( \
        (listPtr),                                                      \
        AIL_OFFSETOF(listPtr, region_head),                             \
        AIL_OFFSETOF(&(listPtr)->region_head, region_next)              \
    )
#define AIL_ALLOC_REGION_OF(listPtr, ptr) _ail_alloc_region_of_( \
        (u8 *)(listPtr),                                         \
        AIL_OFFSETOF(listPtr, region_head),                      \
        AIL_OFFSETOF(&(listPtr)->region_head, region_next),      \
        AIL_OFFSETOF(&(listPtr)->region_head, mem),              \
        AIL_OFFSETOF(&(listPtr)->region_head, region_size),      \
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
            if (false && AIL_ALLOC_CAN_MERGE_REGIONS(last_region_ptr_var, _ail_alloc_new_region_ptr_var_)) {                                  \
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
#if AIL_IS_2POWER(AIL_ALLOC_ALIGNMENT) == true
    // size % alignment but faster, bc alignment is a power of two
    mod = size & (AIL_ALLOC_ALIGNMENT - 1);
#else
    mod = size % AIL_ALLOC_ALIGNMENT;
#endif
    return size + (mod > 0)*(AIL_ALLOC_ALIGNMENT - mod);
}

u64 ail_alloc_align_forward(u64 n, u64 alignment)
{
    AIL_ASSERT(alignment > 0 && AIL_IS_2POWER(alignment));
    return ail_alloc_align_backward(n + (alignment - 1), alignment);
}

u64 ail_alloc_align_backward(u64 n, u64 alignment)
{
    AIL_ASSERT(alignment > 0 && AIL_IS_2POWER(alignment));
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
        case AIL_MEM_CLEAR_ALL: break;
        case AIL_MEM_FREE_ALL:  break;
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
static void __ail_alloc_page_unused__(void)
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
    ptr   = (u8 *)ptr - sizeof(AIL_Alloc_Page_Header);
    size += sizeof(AIL_Alloc_Page_Header);
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
    ((AIL_Alloc_Page_Header *)ptr)->size = aligned_size - sizeof(AIL_Alloc_Page_Header);
    return (u8 *)ptr + sizeof(AIL_Alloc_Page_Header);
}

void *ail_alloc_page_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{
    AIL_UNUSED(data);
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
            AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
            u64 old_size = header->size;
            u64 aligned_size = ail_alloc_align_forward(size + sizeof(AIL_Alloc_Page_Header), AIL_ALLOC_PAGE_SIZE);
            if (aligned_size <= old_size + sizeof(AIL_Alloc_Page_Header)) {
                u64 sz_diff = aligned_size - old_size;
                if (sz_diff >= AIL_ALLOC_PAGE_SIZE) {
                    _ail_alloc_internal_free_pages_((void *)ail_alloc_align_forward((u64)((u8 *)old_ptr + size), AIL_ALLOC_PAGE_SIZE), sz_diff);
                }
                header->size = aligned_size - sizeof(AIL_Alloc_Page_Header);
                res = old_ptr;
            } else {
                // @TODO: VirtualAlloc can take the previous pointer to potentially just increase the size
                // @TODO: mmap has some kind of hint system, that probably does more or less the same
                AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
                _ail_alloc_internal_free_pages_(old_ptr, header->size);
                res = _ail_alloc_page_internal_alloc_(size);
            }
        } break;
        case AIL_MEM_FREE: {
            AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
            size = header->size;
            _ail_alloc_internal_free_pages_(old_ptr, header->size);
        } break;
        case AIL_MEM_CLEAR_ALL: break;
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
        case AIL_MEM_FREE: break;
        case AIL_MEM_CLEAR_ALL:
        case AIL_MEM_FREE_ALL: buffer->idx = 0;
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
        case AIL_MEM_FREE: break;
        case AIL_MEM_CLEAR_ALL:
        case AIL_MEM_FREE_ALL: ring->idx = 0;
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
    AIL_ALLOC_FIND_REGION(region, last_region, (region->used + size + header_size <= region->region_size));
    if (AIL_UNLIKELY(!region)) {
        AIL_ALLOC_NEW_REGION(Arena, *arena, region, last_region, size,
            region->used = 0;
        );
    }
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
        case AIL_MEM_FREE: {
            AIL_Alloc_Arena_Region *region = AIL_ALLOC_REGION_OF(arena, old_ptr);
            if (!region) { // @Note Bounds checking failed -> crash in debug mode and just ignore it otherwise
                AIL_UNREACHABLE();
                goto done;
            }
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
    AIL_ALLOC_LOG("arena", mode, ptr, size, old_ptr);
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
    AIL_UNUSED(size);
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
    AIL_ALLOC_LOG("pool", mode, ptr, size, old_ptr);
    return ptr;
}


///////////////
// Free-List //
///////////////

void _ail_alloc_freelist_internal_clear_region_(AIL_Alloc_Freelist_Region *region)
{
    AIL_Alloc_Freelist_Free_Node *head = (AIL_Alloc_Freelist_Free_Node *)region->mem;
    head->size          = region->region_size;
    head->next          = NULL;
    region->head        = head;
    region->region_used = 0;
}

void *_ail_alloc_freelist_internal_alloc_(AIL_Alloc_Freelist *fl, u64 size)
{
    AIL_Alloc_Freelist_Region *prev_region, *region = &fl->region_head;
    AIL_Alloc_Freelist_Free_Node *node, *prev = NULL;
    AIL_Alloc_Freelist_Header header = {
        .pad  = ail_alloc_size_aligned_pad(sizeof(AIL_Alloc_Freelist_Header) + size),
        .size = size,
    };
    u64 req_size = sizeof(AIL_Alloc_Freelist_Header) + size + header.pad;
    while (region) {
        node = region->head;
        while (node && node->size < req_size) {
            prev = node;
            node = node->next;
        }
        if (node) break;
        prev        = NULL;
        prev_region = region;
        region      = region->region_next;
    }
    if (!node) {
        _AIL_ALLOC_NEW_REGION_(Freelist, *fl, region, size);
        if (!region) return NULL;
        prev_region->region_next = region;
        _ail_alloc_freelist_internal_clear_region_(region);
        node = region->head;
    }

    AIL_Alloc_Freelist_Free_Node *next;
    if (node->size - req_size > AIL_MAX(sizeof(AIL_Alloc_Freelist_Free_Node), sizeof(AIL_Alloc_Freelist_Header) + AIL_ALLOC_ALIGNMENT)) {
        next = (AIL_Alloc_Freelist_Free_Node *)((u8 *)node + req_size);
        next->next = node->next;
        next->size = node->size - req_size;
    } else {
        next = node->next;
    }
    if (prev) prev->next   = next;
    else      region->head = next;

    region->region_used += req_size;
    *(AIL_Alloc_Freelist_Header *)node = header;
    return (u8 *)node + sizeof(AIL_Alloc_Freelist_Header);
}

void _ail_alloc_freelist_internal_free_(AIL_Alloc_Freelist *fl, u8 *old_ptr)
{
    // @TODO
    // @TODO: Full Bounds checking?
    if (!old_ptr) return;
    AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Freelist_Header);
    u64 block_size = header->size + header->pad + sizeof(AIL_Alloc_Freelist_Header); // @TODO: block_size needed for logging

    AIL_Alloc_Freelist_Region *region = &fl->region_head;
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
                AIL_ASSERT(region->region_used >= block_size);
                region->region_used -= block_size;
                return;
            } else if ((u8 *)node + node->size == old_ptr) {
                node->size += block_size;
                if ((u8 *)node + node->size == (u8 *)node->next) {
                    node->size += node->next->size;
                    node->next  = node->next->next;
                }
                AIL_ASSERT(region->region_used >= block_size);
                region->region_used -= block_size;
                return;
            }
            node = node->next;
        }
        if (old_ptr < region->mem + region->region_size) {
            AIL_Alloc_Freelist_Free_Node *new_node = (AIL_Alloc_Freelist_Free_Node *)old_ptr;
            new_node->next = NULL;
            new_node->size = block_size;
            if (prev) prev->next   = new_node;
            else      region->head = new_node;
            AIL_ASSERT(region->region_used >= block_size);
            region->region_used -= block_size;
            return;
        }
        region = region->region_next;
    }
    AIL_UNREACHABLE(); // Out-of-Bounds
}

AIL_Allocator ail_alloc_freelist_new(u64 cap, AIL_Allocator *backing_allocator)
{
    u8 *mem = (u8 *)AIL_CALL_ALLOC(*backing_allocator, cap);
    AIL_Alloc_Freelist *fl = (AIL_Alloc_Freelist *)mem;
    fl->region_head.region_size = cap - sizeof(AIL_Alloc_Freelist);
    fl->backing_allocator = backing_allocator;
    _ail_alloc_freelist_internal_clear_region_(&fl->region_head); // Sets all other parameters of fl
    return (AIL_Allocator) {
        .data       = fl,
        .alloc      = &ail_alloc_freelist_alloc,
    };
}

void *ail_alloc_freelist_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{

    void *ptr = NULL;
    AIL_Alloc_Freelist *fl = (AIL_Alloc_Freelist *)data;
    AIL_Alloc_Freelist_Region *region = &fl->region_head;
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
                AIL_Alloc_Freelist_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Freelist_Header);
                if (size <= header->size + header->pad) {
                    u64 new_pad  = header->size + header->pad - size;
                    header->size = size;
                    header->pad  = new_pad;
                    ptr = old_ptr;
                } else {
                    ptr = _ail_alloc_freelist_internal_alloc_(fl, size);
                    if (ptr) memcpy(ptr, old_ptr, size);
                    _ail_alloc_freelist_internal_free_(fl, (u8 *)old_ptr);
                }
            }
        } break;
        case AIL_MEM_FREE: {
            _ail_alloc_freelist_internal_free_(fl, (u8 *)old_ptr);
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
            AIL_Alloc_Freelist_Region *next;
            // u64 total = fl->region_head.region_used;
            while (region) {
                // total += region->region_used;
                next   = region->region_next;
                AIL_CALL_FREE(*fl->backing_allocator, region);
                region = next;
            }
            _ail_alloc_freelist_internal_clear_region_(&fl->region_head);
        } break;
    }
    AIL_ALLOC_LOG("freelist", mode, ptr, size, old_ptr);
    return ptr;
}


#endif // _AIL_ALLOC_IMPL_GUARD_
#endif // AIL_ALLOC_IMPL
