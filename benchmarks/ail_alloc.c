// #define AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_IMPL
#define AIL_BENCH_IMPL
#define AIL_BENCH_PROFILE
#include "../ail_alloc.h"
#include "../ail_bench.h"
#include <float.h>

static inline void fixed_size_allocs(AIL_Allocator *a, u64 el_size, u64 n)
{
    for (u64 i = 0; i < n; i++) {
        void *el = AIL_CALL_ALLOC(*a, el_size);
        AIL_UNUSED(el);
    }
}

static inline void fixed_size_allocs_imm_frees(AIL_Allocator *a, u64 el_size, u64 n)
{
    for (u64 i = 0; i < n; i++) {
        void *el = AIL_CALL_ALLOC(*a, el_size);
        AIL_CALL_FREE(*a, el);
    }
}

static inline void fixed_size_allocs_arb_frees(AIL_Allocator *a, u64 el_size, u64 n)
{
    void *ptrs[1024];
    for (u64 i = 0; i < n/1024 + ((n%1024) > 0); i++) {
        u64 max_j = AIL_MIN(1024, n - i*1024);
        for (u64 j = 0; j < 1024 && i*1024 + j < n; j++) {
            ptrs[j] = AIL_CALL_ALLOC(*a, el_size);
        }
        // Randomly ordered Frees:
        for (u64 j = 500;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 0;    j < 10    && j < max_j; j++)    AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 153;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 10;   j < 100   && j < max_j; j += 2) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 502;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 11;   j < 100   && j < max_j; j += 2) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 501;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 505;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 150;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 504;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 154;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 1023; j >= 1000 && j < max_j; j--)    AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 507;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 156;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 158;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 503;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 151;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 506;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 149;  j >= 100  && j < max_j; j--)    AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 155;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 508;  j < 1000  && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 152;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
        for (u64 j = 157;  j < 500   && j < max_j; j += 9) AIL_CALL_FREE(*a, ptrs[j]);
    }
}

static inline void steadily_increasing_reallocs(AIL_Allocator *a, u64 el_size, u64 n)
{
    void *el = AIL_CALL_ALLOC(*a, el_size);
    for (u64 i = 1; i < n; i++) {
        el = AIL_CALL_REALLOC(*a, el, i*el_size);
        *(u8 *)el = (u8)i;
    }
    AIL_UNUSED(el);
}

#define ITER(alloc_name, n, ...) do { \
        for (u64 i = 0; i < (n); i++) {          \
            AIL_BENCH_PROFILE_START(alloc_name); \
            __VA_ARGS__;                         \
            AIL_BENCH_PROFILE_END(alloc_name);   \
        }                                        \
    } while(0)

#define PAGER(alloc_name, n, ...) ITER(alloc_name, n, __VA_ARGS__)

#define STD(alloc_name, n, ...) ITER(alloc_name, n, __VA_ARGS__)

#define BUFFER(alloc_name, n, ...) do {                                    \
        u8 *back_buffer = AIL_CALL_ALLOC(ail_alloc_pager, mem_max);        \
        AIL_Allocator buffer = ail_alloc_buffer_new(mem_max, back_buffer); \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(buffer));       \
        AIL_CALL_FREE(ail_alloc_pager, back_buffer);                       \
    } while(0)

#define RING(alloc_name, n, ...) do {                                  \
        u8 *back_buffer = AIL_CALL_ALLOC(ail_alloc_pager, mem_max);    \
        AIL_Allocator ring = ail_alloc_ring_new(mem_max, back_buffer); \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(ring));     \
        AIL_CALL_FREE(ail_alloc_pager, back_buffer);                   \
    } while(0)

#define ARENA(alloc_name, n, ...) do {                                          \
        AIL_Allocator arena = ail_alloc_arena_new(start_cap, &ail_alloc_pager); \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(arena));             \
        AIL_CALL_FREE(ail_alloc_pager, arena.data);                             \
    } while(0)

// @Note: requires variable el_size to be set
#define POOL(alloc_name, n, ...) do {                                                          \
        AIL_Allocator pool = ail_alloc_pool_new(start_cap/el_size, el_size, &ail_alloc_pager); \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(pool));                             \
        AIL_CALL_FREE(ail_alloc_pager, pool.data);                                             \
    } while(0)

#define FREELIST(alloc_name, n, ...) do {                                       \
        AIL_Allocator fl = ail_alloc_freelist_new(start_cap, &ail_alloc_pager); \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(fl));                \
        AIL_CALL_FREE(ail_alloc_pager, fl.data);                                \
    } while(0)

#define ALLOCATORS                         \
    X(Pager,  PAGER,  &ail_alloc_pager)    \
    X(Std,    STD,    &ail_alloc_std)      \
    X(Buffer, BUFFER, &buffer)             \
    X(Ring,   RING,   &ring)               \
    X(Arena,  ARENA,  &arena)              \
    X(Pool,   POOL,   &pool)               \
//    X(Freelist, FREELIST, &fl)           \

#define ALLOCATORS_WO_PAGER                \
    X(Std,    STD,    &ail_alloc_std)      \
    X(Arena,  ARENA,  &arena)              \
    X(Buffer, BUFFER, &buffer)             \
    X(Ring,   RING,   &ring)               \
    X(Pool,   POOL,   &pool)               \
//    X(Freelist, FREELIST, &fl)           \

#define PSEUDO_GROWING_ALLOCATORS          \
    X(Pager,  PAGER,  &ail_alloc_pager)    \
    X(Std,    STD,    &ail_alloc_std)      \
    X(Ring,   RING,   &ring)               \
    X(Arena,  ARENA,  &arena)              \
    X(Pool,   POOL,   &pool)               \
//    X(Freelist, FREELIST, &fl)           \

#define GROWING_ALLOCATORS                 \
    X(Pager,  PAGER,  &ail_alloc_pager)    \
    X(Arena,  ARENA,  &arena)              \
    X(Std,    STD,    &ail_alloc_std)      \
    X(Pool,   POOL,   &pool)               \
//    X(Freelist, FREELIST, &fl)           \

#define ARB_SIZE_ALLOCATORS                \
    X(Std,    STD,    &ail_alloc_std)      \
    X(Buffer, BUFFER, &buffer)             \
    X(Ring,   RING,   &ring)               \
    X(Arena,  ARENA,  &arena)              \
//    X(Freelist, FREELIST, &fl)           \

#define PSEUDO_GROWING_ARB_SIZE_ALLOCATORS \
    X(Std,    STD,    &ail_alloc_std)      \
    X(Ring,   RING,   &ring)               \
    X(Arena,  ARENA,  &arena)              \
//    X(Freelist, FREELIST, &fl)           \

#define GROWING_ARB_SIZE_ALLOCATORS        \
    X(Std,    STD,    &ail_alloc_std)      \
    X(Arena,  ARENA,  &arena)              \
//    X(Freelist, FREELIST, &fl)           \

int main(void)
{
    u64 start_cap = AIL_ALLOC_PAGE_SIZE - sizeof(AIL_Alloc_Page_Header);
    u64 mem_max   = (AIL_ALLOC_PAGE_SIZE - sizeof(AIL_Alloc_Page_Header))*64;
    u64 n         = 1000;
    printf("Every test is iterated %lld times...\n", n);
    { // Fixed Size Allocs with randomly ordered frees
        printf("------\n");
        ail_bench_begin_profile();
        u64 el_size = 64;
        u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
        AIL_ASSERT(el_size * el_count <= mem_max);
        printf("%lld fixed-size allocations (with randomly ordered frees) of size %lld:\n", el_count, el_size);
        #define X(name, macro, allocator) macro(name, n, fixed_size_allocs_arb_frees(allocator, el_size, el_count));
            ALLOCATORS_WO_PAGER
        #undef X
        ail_bench_end_and_print_profile(true);
    }
    { // Fixed Size Allocs with immediate frees
        printf("------\n");
        ail_bench_begin_profile();
        u64 el_size  = 1024;
        u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
        AIL_ASSERT(el_size * el_count <= mem_max);
        printf("%lld fixed-size allocations (with immediate frees) of size %lld:\n", el_count, el_size);
        #define X(name, macro, allocator) macro(name, n, fixed_size_allocs_imm_frees(allocator, el_size, el_count));
            ALLOCATORS
        #undef X
        ail_bench_end_and_print_profile(true);
    }
    { // Fixed Size Allocs with immediate frees
        printf("------\n");
        ail_bench_begin_profile();
        u64 el_size  = 15;
        u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
        AIL_ASSERT(el_size * el_count <= mem_max);
        printf("%lld fixed-size allocations (with immediate frees) of size %lld:\n", el_count, el_size);
        #define X(name, macro, allocator) macro(name, n, fixed_size_allocs_imm_frees(allocator, el_size, el_count));
            ALLOCATORS_WO_PAGER
        #undef X
        ail_bench_end_and_print_profile(true);
    }
    // { // Steadily increasing Reallocations
    //     printf("------\n");
    //     ail_bench_begin_profile();
    //     u64 el_size = 32;
    //     u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
    //     AIL_ASSERT(el_size * el_count <= mem_max);
    //     printf("%lld steadily increasing reallocations of initial size %lld:\n", el_count, el_size);
    //     #define X(name, macro, allocator) macro(name, n, steadily_increasing_reallocs(allocator, el_size, el_count));
    //         GROWING_ARB_SIZE_ALLOCATORS
    //     #undef X
    //     ail_bench_end_and_print_profile(true);
    // }
    AIL_BENCH_END_OF_COMPILATION_UNIT();
}
