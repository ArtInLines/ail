// #define AIL_ALLOC_PRINT_MEM
#define AIL_ALLOC_IMPL
#define AIL_BENCH_IMPL
#define AIL_BENCH_PROFILE
#include "../ail_alloc.h"
#include "../ail_bench.h"
#include <float.h>

static u64 size_to_page_count(u64 size)
{
    AIL_STATIC_ASSERT(AIL_IS_2POWER(AIL_ALLOC_PAGE_SIZE));
    return ail_alloc_align_forward(size, AIL_ALLOC_PAGE_SIZE) / AIL_ALLOC_PAGE_SIZE;
}

typedef struct CountingPager {
    u64 cur_page_count;
    u64 max_page_count;
} CountingPager;

static u64 get_alloc_size_in_pages(void *ptr)
{
    return size_to_page_count(AIL_ALLOC_GET_HEADER(ptr, AIL_Alloc_Page_Header)->size);
}

static void *counting_pager_alloc(void *data, AIL_Allocator_Mode mode, u64 size, void *old_ptr)
{
    CountingPager *pager = (CountingPager *)data;
    void *res = NULL;
    switch (mode) {
        case AIL_MEM_ALLOC: {
            res = _ail_alloc_page_internal_alloc_(size);
            pager->cur_page_count += get_alloc_size_in_pages(res);
        } break;
        case AIL_MEM_CALLOC: {
            res = _ail_alloc_page_internal_alloc_(size);
            memset(res, 0, size);
            pager->cur_page_count += get_alloc_size_in_pages(res);
        } break;
        case AIL_MEM_REALLOC: {
            AIL_ASSERT(pager->cur_page_count > 0);
            pager->cur_page_count -= get_alloc_size_in_pages(old_ptr);
            res = _ail_alloc_page_internal_realloc_(old_ptr, size);
            pager->cur_page_count += get_alloc_size_in_pages(res);
        } break;
        case AIL_MEM_FREE: {
            AIL_ASSERT(pager->cur_page_count > 0);
            pager->cur_page_count -= get_alloc_size_in_pages(old_ptr);
            AIL_Alloc_Page_Header *header = AIL_ALLOC_GET_HEADER(old_ptr, AIL_Alloc_Page_Header);
            size = header->size;
            _ail_alloc_internal_free_pages_(old_ptr, header->size);
        } break;
        case AIL_MEM_CLEAR_ALL:
        case AIL_MEM_FREE_ALL:
            pager->cur_page_count = 0;
    }
    pager->max_page_count = AIL_MAX(pager->max_page_count, pager->cur_page_count);
    return res;
}

static u64 counting_pager_get_max_and_reset(AIL_Allocator *allocator)
{
    CountingPager *pager  = allocator->data;
    u64 max = pager->max_page_count;
    pager->cur_page_count = 0;
    pager->max_page_count = 0;
    return max;
}

static AIL_Allocator counting_pager_new(void)
{
    CountingPager *data  = AIL_CALL_ALLOC(ail_alloc_pager, sizeof(CountingPager));
    data->cur_page_count = 0;
    data->max_page_count = 0;
    return (AIL_Allocator) {
        .data  = data,
        .alloc = &counting_pager_alloc,
    };
}

typedef struct SizeAnchor {
    const char *label;
    u64 size;
} SizeAnchor;

static u64 global_max_page_idx;
static SizeAnchor global_max_page_sizes[16];
AIL_Allocator global_pager;

static inline u32 u32_str_len(u32 x) {
    u32 len = 1;
    while (x > 9) {
        x /= 10;
        len++;
    }
    return len;
}

static void print_and_clear_max_page_sizes(void)
{
    printf("Maximum Memory used:\n");
    u32 max_name_len = 0;
    u32 max_size_len = 0;
    for (u32 i = 0; i < AIL_ARRLEN(global_max_page_sizes); i++) {
        SizeAnchor anchor = global_max_page_sizes[i];
        if (anchor.size) {
            max_name_len = AIL_MAX(max_name_len, strlen(anchor.label));
            max_size_len = AIL_MAX(max_size_len, u32_str_len(anchor.size));
        }
    }
    for (u32 i = 0; i < AIL_ARRLEN(global_max_page_sizes); i++) {
        SizeAnchor *anchor = &global_max_page_sizes[i];
        if (anchor->size) {
            printf("  %s:", anchor->label);
            u32 len_diff = max_name_len - strlen(anchor->label);
            while (len_diff--) printf(" ");
            len_diff = max_size_len - u32_str_len(max_size_len);
            while (len_diff--) printf(" ");
            printf("%llu pages\n", anchor->size);
            anchor->size  = 0;
            anchor->label = NULL;
        }
    }
    global_max_page_idx = 0;
}

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

#define ITER(alloc_name, n, ...) do {            \
        for (u64 i = 0; i < (n); i++) {          \
            AIL_BENCH_PROFILE_START(alloc_name); \
            __VA_ARGS__;                         \
            AIL_BENCH_PROFILE_END(alloc_name);   \
        }                                        \
    } while(0)

#define PAGER(alloc_name, n, ...) do {                                                  \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(global_pager));              \
        u64 size = counting_pager_get_max_and_reset(&global_pager);                     \
        global_max_page_sizes[global_max_page_idx].size    = size;                      \
        global_max_page_sizes[global_max_page_idx++].label = AIL_STRINGIZE(alloc_name); \
    } while(0)

#define STD(alloc_name, n, ...) ITER(alloc_name, n, __VA_ARGS__)

#define BUFFER(alloc_name, n, ...) do {                                                 \
        u8 *back_buffer = AIL_CALL_ALLOC(ail_alloc_pager, mem_max);                     \
        AIL_Allocator buffer = ail_alloc_buffer_new(mem_max, back_buffer);              \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(buffer));                    \
        global_max_page_sizes[global_max_page_idx].size = size_to_page_count(mem_max);  \
        global_max_page_sizes[global_max_page_idx++].label = AIL_STRINGIZE(alloc_name); \
        AIL_CALL_FREE(ail_alloc_pager, back_buffer);                                    \
    } while(0)

#define RING(alloc_name, n, ...) do {                                                   \
        u8 *back_buffer = AIL_CALL_ALLOC(ail_alloc_pager, mem_max);                     \
        AIL_Allocator ring = ail_alloc_ring_new(mem_max, back_buffer);                  \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(ring));                      \
        global_max_page_sizes[global_max_page_idx].size = size_to_page_count(mem_max);  \
        global_max_page_sizes[global_max_page_idx++].label = AIL_STRINGIZE(alloc_name); \
        AIL_CALL_FREE(ail_alloc_pager, back_buffer);                                    \
    } while(0)

#define ARENA(alloc_name, n, ...) do {                                                  \
        AIL_Allocator arena = ail_alloc_arena_new(start_cap, &global_pager);            \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(arena));                     \
        AIL_CALL_FREE(global_pager, arena.data);                                        \
        u64 size = counting_pager_get_max_and_reset(&global_pager);                     \
        global_max_page_sizes[global_max_page_idx].size    = size;                      \
        global_max_page_sizes[global_max_page_idx++].label = AIL_STRINGIZE(alloc_name); \
    } while(0)

// @Note: requires variable el_size to be set
#define POOL(alloc_name, n, ...) do {                                                       \
        AIL_Allocator pool = ail_alloc_pool_new(start_cap/el_size, el_size, &global_pager); \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(pool));                          \
        AIL_CALL_FREE(global_pager, pool.data);                                             \
        u64 size = counting_pager_get_max_and_reset(&global_pager);                         \
        global_max_page_sizes[global_max_page_idx].size    = size;                          \
        global_max_page_sizes[global_max_page_idx++].label = AIL_STRINGIZE(alloc_name);     \
    } while(0)

#define FREELIST(alloc_name, n, ...) do {                                               \
        AIL_Allocator fl = ail_alloc_freelist_new(start_cap, &global_pager);            \
        ITER(alloc_name, n, __VA_ARGS__; AIL_CALL_FREE_ALL(fl));                        \
        AIL_CALL_FREE(global_pager, fl.data);                                           \
        u64 size = counting_pager_get_max_and_reset(&global_pager);                     \
        global_max_page_sizes[global_max_page_idx].size    = size;                      \
        global_max_page_sizes[global_max_page_idx++].label = AIL_STRINGIZE(alloc_name); \
    } while(0)

#define ALLOCATORS                      \
    X(Pager,    PAGER,  &global_pager)  \
    X(Std,      STD,    &ail_alloc_std) \
    X(Buffer,   BUFFER, &buffer)        \
    X(Ring,     RING,   &ring)          \
    X(Arena,    ARENA,  &arena)         \
    X(Pool,     POOL,   &pool)          \
    X(Freelist, FREELIST, &fl)          \

#define ALLOCATORS_WO_PAGER             \
    X(Std,      STD,    &ail_alloc_std) \
    X(Arena,    ARENA,  &arena)         \
    X(Buffer,   BUFFER, &buffer)        \
    X(Ring,     RING,   &ring)          \
    X(Pool,     POOL,   &pool)          \
    X(Freelist, FREELIST, &fl)          \

#define PSEUDO_GROWING_ALLOCATORS       \
    X(Pager,    PAGER,  &global_pager)  \
    X(Std,      STD,    &ail_alloc_std) \
    X(Ring,     RING,   &ring)          \
    X(Arena,    ARENA,  &arena)         \
    X(Pool,     POOL,   &pool)          \
    X(Freelist, FREELIST, &fl)          \

#define GROWING_ALLOCATORS              \
    X(Pager,    PAGER,  &global_pager)  \
    X(Arena,    ARENA,  &arena)         \
    X(Std,      STD,    &ail_alloc_std) \
    X(Pool,     POOL,   &pool)          \
    X(Freelist, FREELIST, &fl)          \

#define ARB_SIZE_ALLOCATORS             \
    X(Std,      STD,    &ail_alloc_std) \
    X(Buffer,   BUFFER, &buffer)        \
    X(Ring,     RING,   &ring)          \
    X(Arena,    ARENA,  &arena)         \
    X(Freelist, FREELIST, &fl)          \

#define PSEUDO_GROWING_ARB_SIZE_ALLOCATORS \
    X(Std,      STD,    &ail_alloc_std)    \
    X(Ring,     RING,   &ring)             \
    X(Arena,    ARENA,  &arena)            \
    X(Freelist, FREELIST, &fl)             \

#define GROWING_ARB_SIZE_ALLOCATORS     \
    X(Std,      STD,    &ail_alloc_std) \
    X(Arena,    ARENA,  &arena)         \
    X(Freelist, FREELIST, &fl)          \


// @TODO: Track required memory too
int main(void)
{
    global_pager = counting_pager_new();
    u64 start_cap = AIL_ALLOC_PAGE_SIZE - sizeof(AIL_Alloc_Page_Header);
    u64 mem_max   = (AIL_ALLOC_PAGE_SIZE - sizeof(AIL_Alloc_Page_Header))*64;
    u64 n         = 100;
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
        print_and_clear_max_page_sizes();
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
        print_and_clear_max_page_sizes();
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
        print_and_clear_max_page_sizes();
        ail_bench_end_and_print_profile(true);
    }
    { // Steadily increasing Reallocations
        printf("------\n");
        ail_bench_begin_profile();
        u64 el_size = 256;
        u64 el_count = mem_max/(el_size + 16); // +16 for some header sizes
        AIL_ASSERT(el_size * el_count <= mem_max);
        printf("%lld steadily increasing reallocations of initial size %lld:\n", el_count, el_size);
        #define X(name, macro, allocator) macro(name, n, steadily_increasing_reallocs(allocator, el_size, el_count));
            GROWING_ARB_SIZE_ALLOCATORS
        #undef X
        print_and_clear_max_page_sizes();
        ail_bench_end_and_print_profile(true);
    }
    AIL_BENCH_END_OF_COMPILATION_UNIT();
}
