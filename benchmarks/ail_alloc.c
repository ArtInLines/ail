#define AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_IMPL
#define AIL_TIME_IMPL
#include "../ail_alloc.h"
#include "../ail_time.h"
#include <float.h>

static inline double fixed_size_allocs(AIL_Allocator *a, u64 el_size, u64 n)
{
    f64 start = ail_time_clock_start();
    for (u64 i = 0; i < n; i++) {
        void *el = a->alloc(a->data, el_size);
        AIL_UNUSED(el);
    }
    return ail_time_clock_elapsed(start);
}

static inline double fixed_size_allocs_imm_frees(AIL_Allocator *a, u64 el_size, u64 n)
{
    f64 start = ail_time_clock_start();
    for (u64 i = 0; i < n; i++) {
        void *el = a->alloc(a->data, el_size);
        a->free_one(a->data, el);
    }
    return ail_time_clock_elapsed(start);
}

static inline double fixed_size_allocs_arb_frees(AIL_Allocator *a, u64 el_size, u64 n)
{
    f64 start = ail_time_clock_start();
    void *ptrs[1024];
    for (u64 i = 0; i < n/1024 + ((n%1024) > 0); i++) {
        u64 max_j = AIL_MIN(1024, n - i*1024);
        for (u64 j = 0; j < 1024 && i*1024 + j < n; j++) {
            ptrs[j] = a->alloc(a->data, el_size);
        }
        // Randomly ordered Frees:
        for (u64 j = 500;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 0;    j < 10    && j < max_j; j++)    a->free_one(a->data, ptrs[j]);
        for (u64 j = 153;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 10;   j < 100   && j < max_j; j += 2) a->free_one(a->data, ptrs[j]);
        for (u64 j = 502;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 11;   j < 100   && j < max_j; j += 2) a->free_one(a->data, ptrs[j]);
        for (u64 j = 501;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 505;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 150;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 504;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 154;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 1023; j >= 1000 && j < max_j; j--)    a->free_one(a->data, ptrs[j]);
        for (u64 j = 507;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 156;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 158;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 503;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 151;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 506;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 149;  j >= 100  && j < max_j; j--)    a->free_one(a->data, ptrs[j]);
        for (u64 j = 155;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 508;  j < 1000  && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 152;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
        for (u64 j = 157;  j < 500   && j < max_j; j += 9) a->free_one(a->data, ptrs[j]);
    }
    return ail_time_clock_elapsed(start);
}

static inline double steadily_increasing_reallocs(AIL_Allocator *a, u64 el_size, u64 n)
{
    f64 start = ail_time_clock_start();
    void *el = a->alloc(a->data, el_size);
    for (u64 i = 1; i < n; i++) {
        el = a->re_alloc(a->data, el, i*el_size);
        *(u8 *)el = (u8)i;
    }
    AIL_UNUSED(el);
    return ail_time_clock_elapsed(start);
}

// @TODO: Calculate median & mode instead of min
#define ITER(...) do {                         \
        f64 res = 0;                           \
        total = ail_time_clock_start();        \
        min = DBL_MAX;                         \
        avg = 0;                               \
        for (u64 i = 0; i < (n); i++) {        \
            res = __VA_ARGS__;                 \
            if (res < min) min = res;          \
            avg += res;                        \
        }                                      \
        avg = avg/(n);                         \
        total = ail_time_clock_elapsed(total); \
    } while(0)

#define PAGER(...) ITER(__VA_ARGS__)

#define STD(...) ITER(__VA_ARGS__)

#define BUFFER(...) do {                                                       \
        u8 *back_buffer = ail_alloc_page_alloc(ail_alloc_pager.data, mem_max); \
        AIL_Allocator buffer = ail_alloc_buffer_new(mem_max, back_buffer);     \
        ITER(__VA_ARGS__; buffer.free_all(buffer.data));                       \
        ail_alloc_page_free(ail_alloc_pager.data, back_buffer);                \
    } while(0)

#define RING(...) do {                                                         \
        u8 *back_buffer = ail_alloc_page_alloc(ail_alloc_pager.data, mem_max); \
        AIL_Allocator ring = ail_alloc_ring_new(mem_max, back_buffer);         \
        ITER(__VA_ARGS__; ring.free_all(ring.data));                           \
        ail_alloc_page_free(ail_alloc_pager.data, back_buffer);                \
    } while(0)

#define ARENA(...) do {                                                         \
        AIL_Allocator arena = ail_alloc_arena_new(start_cap, &ail_alloc_pager); \
        ITER(__VA_ARGS__; arena.free_all(arena.data));                \
        ail_alloc_pager.free_one(ail_alloc_pager.data, arena.data);             \
    } while(0)

// @Note: requires variable el_size to be set
#define POOL(...) do {                                                                         \
        AIL_Allocator pool = ail_alloc_pool_new(start_cap/el_size, el_size, &ail_alloc_pager); \
        ITER(__VA_ARGS__; pool.free_all(pool.data));                                           \
        ail_alloc_pager.free_one(ail_alloc_pager.data, pool.data);                             \
    } while(0)

#define FREELIST(...) do {                                                      \
        AIL_Allocator fl = ail_alloc_freelist_new(start_cap, &ail_alloc_pager); \
        ITER(__VA_ARGS__; fl.free_all(fl.data));                                \
        ail_alloc_pager.free_one(ail_alloc_pager.data, fl.data);                \
    } while(0)

#define OUTPUT(name) printf("  %-8s - min: %6.3fms, avg: %6.3fms, total: %6.3fs\n", (name), min*1000, avg*1000, total)

#define ALLOCATORS                        \
    X("Std",    STD,    &ail_alloc_std)   \
    X("Pager",  PAGER,  &ail_alloc_pager) \
    X("Buffer", BUFFER, &buffer)          \
    X("Ring",   RING,   &ring)            \
    X("Arena",  ARENA,  &arena)           \
    X("Pool",   POOL,   &pool)            \
//    X("Freelist", FREELIST, &fl)          \

#define ALLOCATORS_WO_PAGER               \
    X("Arena",  ARENA,  &arena)           \
    X("Std",    STD,    &ail_alloc_std)   \
    X("Buffer", BUFFER, &buffer)          \
    X("Ring",   RING,   &ring)            \
    X("Pool",   POOL,   &pool)            \
//    X("Freelist", FREELIST, &fl)          \

#define PSEUDO_GROWING_ALLOCATORS         \
    X("Std",    STD,    &ail_alloc_std)   \
    X("Pager",  PAGER,  &ail_alloc_pager) \
    X("Ring",   RING,   &ring)            \
    X("Arena",  ARENA,  &arena)           \
    X("Pool",   POOL,   &pool)            \
//    X("Freelist", FREELIST, &fl)          \

#define GROWING_ALLOCATORS                \
    X("Arena",  ARENA,  &arena)           \
    X("Std",    STD,    &ail_alloc_std)   \
    X("Pager",  PAGER,  &ail_alloc_pager) \
    X("Pool",   POOL,   &pool)            \
//    X("Freelist", FREELIST, &fl)          \

#define ARB_SIZE_ALLOCATORS               \
    X("Std",    STD,    &ail_alloc_std)   \
    X("Buffer", BUFFER, &buffer)          \
    X("Ring",   RING,   &ring)            \
    X("Arena",  ARENA,  &arena)           \
//    X("Freelist", FREELIST, &fl)          \

#define PSEUDO_GROWING_ARB_SIZE_ALLOCATORS \
    X("Std",    STD,    &ail_alloc_std)    \
    X("Ring",   RING,   &ring)             \
    X("Arena",  ARENA,  &arena)            \
//    X("Freelist", FREELIST, &fl)          \

#define GROWING_ARB_SIZE_ALLOCATORS        \
    X("Std",    STD,    &ail_alloc_std)    \
    X("Arena",  ARENA,  &arena)            \
//    X("Freelist", FREELIST, &fl)          \

int main(void)
{
    u64 start_cap = AIL_ALLOC_PAGE_SIZE;
    u64 mem_max   = AIL_ALLOC_PAGE_SIZE*64;
    u64 n         = 1000;
    f64 min, avg, total;
    printf("Every test is iterated %lld times...\n", n);
    { // Fixed Size Allocs with randomly ordered frees
        printf("------\n");
        u64 el_size = 64;
        u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
        AIL_ASSERT(el_size * el_count <= mem_max);
        printf("%lld fixed-size allocations (with randomly ordered frees) of size %lld:\n", el_count, el_size);
        #define X(name, macro, allocator) macro(fixed_size_allocs_arb_frees(allocator, el_size, el_count)); OUTPUT(name);
            ALLOCATORS_WO_PAGER
        #undef X
    }
    { // Fixed Size Allocs with immediate frees
        printf("------\n");
        u64 el_size  = 1024;
        u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
        AIL_ASSERT(el_size * el_count <= mem_max);
        printf("%lld fixed-size allocations (with immediate frees) of size %lld:\n", el_count, el_size);
        #define X(name, macro, allocator) macro(fixed_size_allocs_imm_frees(allocator, el_size, el_count)); OUTPUT(name);
            ALLOCATORS
        #undef X
    }
    { // Fixed Size Allocs with immediate frees
        printf("------\n");
        u64 el_size  = 15;
        u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
        AIL_ASSERT(el_size * el_count <= mem_max);
        printf("%lld fixed-size allocations (with immediate frees) of size %lld:\n", el_count, el_size);
        #define X(name, macro, allocator) macro(fixed_size_allocs_imm_frees(allocator, el_size, el_count)); OUTPUT(name);
            ALLOCATORS_WO_PAGER
        #undef X
    }
    { // Steadily increasing Reallocations
        printf("------\n");
        u64 el_size = 32;
        u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
        AIL_ASSERT(el_size * el_count <= mem_max);
        printf("%lld steadily increasing reallocations of initial size %lld:\n", el_count, el_size);
        #define X(name, macro, allocator) macro(steadily_increasing_reallocs(allocator, el_size, el_count)); OUTPUT(name);
            GROWING_ARB_SIZE_ALLOCATORS
        #undef X
    }
}