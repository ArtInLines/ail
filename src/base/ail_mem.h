/*
*** Memory ***
*
* Define AIL_NO_ALLOC_IMPL in some file, to not include any implementations
* Define AIL_ALLOC_ALIGNMENT to change the alignment used by all custom allocators
* Define AIL_ALLOC_PRINT_MEM to track (all) allocations
*
* Implementation of the Arena Allocator was inspired by tsoding's arena library (https://github.com/tsoding/arena/)
* and by gingerBill's blog post on Arena Allocators (https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/)
*
* @TODO: Add ail_mem_copy, ail_mem_set
* @TODO: Add documentation explaining the different available Allocators
* @TODO: Add a way to only track allocations of a single allocator / allocator-type (?)
* @TODO: Add a way to track amount of used memory
* @TODO: Change the semantics of alloc/calloc to mean that if `old_ptr` is unequal to 0, it should try to allocate there (like mmap & VirtualAlloc do too)
*/


#ifndef _AIL_ALLOC_H_
#define _AIL_ALLOC_H_

#include "ail_base.h"
#include "ail_arr.h"
#include "ail_base_math.h"

#ifndef AIL_ALLOC_ALIGNMENT
#   define AIL_ALLOC_ALIGNMENT 8 // Reasonable default for all 64bit machines
#endif // AIL_ALLOC_ALIGNMENT
#if !ail_is_2power(AIL_ALLOC_ALIGNMENT)
#   warning "AIL_ALLOC_ALIGNMENT should best be set to a power of two for almost all computer architectures"
#endif

#define AIL_ALLOC_GET_HEADER(ptr, headerType) ((headerType *)(ptr) - 1)

// @Impl: sizes of allocations stored in headers never include the size of the header itself

typedef struct AIL_Alloc_Size_Header {
    u64 size;
} AIL_Alloc_Size_Header;

#define AIL_ALLOC_INIT_ALLOCATOR(name, region_params, params) \
    AIL_WARN_PUSH                                             \
    AIL_WARN_DISABLE(AIL_WARN_ZERO_LENGTH_ARRAY)              \
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
    } AIL_Alloc_##name;                                       \
    AIL_WARN_POP

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
internal u64 ail_alloc_align_size(u64 size);
// @Note: Alignment must be greater than 0 and a power of 2
internal u64 ail_alloc_align_forward(u64 n, u64 alignment);
// @Note: Alignment must be greater than 0 and a power of 2
internal u64 ail_alloc_align_backward(u64 n, u64 alignment);
// Returns padding from aligning to AIL_ALLOC_ALIGNMENT
internal u64 ail_alloc_size_aligned_pad(u64 size);
internal u64 ail_alloc_size_aligned_backward_pad(u64 size, u64 alignment);
internal u64 ail_alloc_size_aligned_forward_pad(u64 size, u64 alignment);


//////////////
// Std Allocator
// C's stdlib allocator
// @Note: free_all is a no-op
//////////////
global AIL_Allocator_Func ail_alloc_std_alloc;
inline void __ail_alloc_std_unused__(void);

//////////////
// Page Allocator
// Allocates always complete pages
// Especially useful for use as a backing allocator
// @Note: When allocating in Page-Sizes, make sure to subtract sizeof(AIL_Alloc_Page_Header) from the size to allocate
// @Note: free_all is a no-op
//////////////
global AIL_Allocator_Func ail_alloc_page_alloc;
inline void __ail_alloc_page_unused__(void);

//////////////
// Buffer Allocator
// Does not have a backing allocator and can thus not grow
// Especially useful for using stack-allocated memory
// @Note: free is a no-op, use free_all instead
//////////////
internal AIL_Allocator ail_alloc_buffer_new(u64 n, u8 *mem);
internal AIL_Allocator_Func ail_alloc_buffer_alloc;

//////////////
// Ring-Buffer Allocator
// Does not have a backing allocator and can thus not grow
// Same as the Buffer Allocator, except it wraps around allocating from the start again
//////////////
internal AIL_Allocator ail_alloc_ring_new(u64 n, u8 *mem);
internal AIL_Allocator_Func ail_alloc_ring_alloc;

//////////////
// Arena Allocator
// Last-In-First-Out kind of allocator (i.e. free only frees memory, when it was the memory that was allocated last)
// Recommended to call free_all to prevent leaking memory for too long
//////////////
internal AIL_Allocator ail_alloc_arena_new(u64 cap, AIL_Allocator *backing_allocator);
internal AIL_Allocator_Func ail_alloc_arena_alloc;

//////////////
// Pool Allocator
// Allocates only fixed-size chunks
// Allows freeing any chunk at any time
// @TODO: Allow allocating several chunks at once (make sure that all of those chunks get freed at once too)
//////////////
internal AIL_Allocator ail_alloc_pool_new(u64 bucket_amount, u64 el_size, AIL_Allocator *backing_allocator);
internal AIL_Allocator_Func ail_alloc_pool_alloc;

//////////////
// Free-List Allocator
// Allows allocating arbitrarily sized regions
// Stores a linked-list internally to keep track of allocations
//////////////
internal AIL_Allocator ail_alloc_freelist_new(u64 cap, AIL_Allocator *backing_allocator);
internal AIL_Allocator_Func ail_alloc_freelist_alloc;

#endif // _AIL_ALLOC_H_


#if !defined(AIL_NO_ALLOC_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_ALLOC_IMPL_GUARD_
#define _AIL_ALLOC_IMPL_GUARD_
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNREACHABLE_CODE)

#include <stdlib.h> // For std_allocator

// @TODO: Provide reserve/committ capacities for allocators instead of just one capacity
// @TODO: Implement Page Allocations for OSes other than WINDOWS and UNIX
#if AIL_OS_WIN
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_ALL)
#include <Windows.h> // For VirtualAlloc, VirtualFree
AIL_WARN_POP
#else
#include <sys/mman.h> // For mmap, munmap
#endif

// For tracing memory
#ifndef AIL_ALLOC_PRINT_MEM
#define _AIL_ALLOC_LOG(...) do { if (0) printf(__VA_ARGS__); } while(0)
#else
#define _AIL_ALLOC_LOG(...) do { _AIL_DBG_PRINT_("Memory Trace: " __VA_ARGS__); _AIL_DBG_PRINT_("\n"); } while(0)
#endif // AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_LOG_ALLOC(allocator, ptr, size)          _AIL_ALLOC_LOG("Malloc   %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CALLOC(allocator, ptr, size)         _AIL_ALLOC_LOG("Calloc   %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_REALLOC(allocator, nptr, optr, size) _AIL_ALLOC_LOG("Relloc   %4llu bytes from %p to %p in '" allocator "' allocator", (size), (optr), (nptr));
#define AIL_ALLOC_LOG_SHRINK(allocator, ptr, osize, nsize) _AIL_ALLOC_LOG("Shrink   %4llu bytes at %p %4llu bytes '" allocator "' allocator", (osize), (ptr), (nsize));
#define AIL_ALLOC_LOG_FREE(allocator, ptr, size)           _AIL_ALLOC_LOG("Free     %4llu bytes at %p in '" allocator "' allocator", (size), (ptr));
#define AIL_ALLOC_LOG_CLEAR_ALL(allocator, size)           _AIL_ALLOC_LOG("ClearAll %4llu bytes in '" allocator "' allocator", (size));
#define AIL_ALLOC_LOG_FREE_ALL(allocator, size)            _AIL_ALLOC_LOG("FreeAll  %4llu bytes in '" allocator "' allocator", (size));
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
    for (AIL_CONCAT(AIL_Alloc_, allocatorName, _Region) *it_name = first_region_ptr, *_ail_alloc_for_each_next_region_; it_name != NULL; it_name = _ail_alloc_for_each_next_region_) { \
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
        AIL_CONCAT(AIL_Alloc_, allocatorName, _Region) *_ail_alloc_new_region_ptr_var_;                                              \
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
        ail_mem_copy(res, old_ptr, header->size);
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
            if (ptr) ail_mem_set(ptr, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            if (!old_ptr) {
                ptr = _ail_alloc_buffer_internal_alloc_(buffer, mem, size);
            } else {
                u64 max_old_size = AIL_MIN(size, (u64)mem + buffer->idx - (u64)ptr);
                ptr = _ail_alloc_buffer_internal_alloc_(buffer, mem, size);
                ail_mem_copy(ptr, old_ptr, max_old_size);
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
            if (ptr) ail_mem_set(ptr, 0, size);
        } break;
        case AIL_MEM_REALLOC: {
            if (!old_ptr) {
                ptr = _ail_alloc_ring_internal_alloc_(ring, mem, size);
            } else {
                u64 max_old_size = AIL_MIN(size, (u64)mem + ring->idx - (u64)old_ptr);
                ptr = _ail_alloc_ring_internal_alloc_(ring, mem, size);
                ail_mem_copy(ptr, old_ptr, max_old_size); // @Bug: ail_mem_copy might not work correctly, if the new poiner wrapped around and its region overlaps with the old region
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
    AIL_Alloc_Arena_Region *last_region = NULL, *region = &arena->region_head;
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
            if (nptr != optr) ail_mem_copy(nptr, optr, old_size);
        } else {
            AIL_ALLOC_GET_HEADER(optr, AIL_Alloc_Arena_Header)->size = size;
        }
    } else {
        // @Note: This leaks memory, since the old allocation cannot be freed in such a simple arena
        nptr = _ail_alloc_arena_internal_alloc_(arena, header_size, size);
        if (nptr) ail_mem_copy(nptr, optr, old_size);
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
            if (ptr) ail_mem_set(ptr, 0, AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Arena_Header)->size);
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
            old_size = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Arena_Header)->size;
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
    AIL_Alloc_Pool_Region *last_region = NULL, *region = &pool->region_head;
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
            if (ptr) ail_mem_set(ptr, 0, pool->bucket_size);
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
    printf("\033[42m| %lld, %lld \033[0m", id, size);
}

void _ail_alloc_freelist_block_print_(AIL_Alloc_Freelist_Header header)
{
    printf("\033[41m| %lld ", header.size);
    AIL_ASSERT(header.pad < AIL_ALLOC_ALIGNMENT);
    if (header.pad) printf("+ %lld ", header.pad);
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

    printf("\033[48;5;0m%lld/%lld used: ", region->region_used, region->region_size);
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
    AIL_Alloc_Freelist_Region *prev_region = NULL, *region = &fl->region_head;
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
            if (ptr) ail_mem_set(ptr, 0, size);
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
                        ail_mem_copy(ptr, old_ptr, sz);
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

AIL_WARN_POP
#endif // _AIL_ALLOC_IMPL_GUARD_
#endif // AIL_NO_ALLOC_IMPL
