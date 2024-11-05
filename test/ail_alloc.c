#include "./test_assert.h"
#define AIL_ALLOC_IMPL
#include "../ail_alloc.h"

static inline u64 sum(u64 n)
{
    return n*(n+1)/2;
}

bool test_sum(u64 *arr, u64 n)
{
    u64 s = 0;
    for (u64 i = 0; i < n; i++) {
        ASSERT(arr[i] == i);
        s += arr[i];
    }
    ASSERT(s == sum(n-1));
    return true;
}

bool test_ptrs_sum(u64 **arr, u64 n)
{
    u64 s = 0;
    for (u64 i = 0; i < n; i++) {
        ASSERT(*arr[i] == i);
        s += *arr[i];
    }
    ASSERT(s == sum(n-1));
    return true;
}

bool test_alloc_single(AIL_Allocator *allocator, u64 bytes)
{
    u64 n = bytes/sizeof(u64);
    u64 **ptrs = malloc(n * sizeof(void*));
    for (u64 i = 0; i < n; i++) {
        ptrs[i] = AIL_CALL_ALLOC(*allocator, sizeof(u64));
        ASSERT(ptrs[i] != NULL);
        *ptrs[i] = i;
    }
    ASSERT(test_ptrs_sum(ptrs, n));
    free(ptrs);
    return true;
}

bool test_alloc(AIL_Allocator *allocator, u64 bytes)
{
    u64 n = bytes/2/sizeof(u64);
    // Test allocations for single elements
    if (!test_alloc_single(allocator, bytes/2)) return false;
    // Test allocations for lists of elements
    u64 *vals = AIL_CALL_ALLOC(*allocator, n * sizeof(u64));
    ASSERT(vals != NULL);
    for (u64 i = 0; i < n; i++) {
        vals[i] = i;
    }
    ASSERT(test_sum(vals, n));

    return true;
}

bool test_calloc_single(AIL_Allocator *allocator, u64 bytes)
{
    // Test allocations for single elements
    u64 n = bytes/sizeof(u64);
    u64 **ptrs = malloc(n * sizeof(void*));
    for (u64 i = 0; i < n; i++) {
        ptrs[i] = AIL_CALL_CALLOC(*allocator, 1, sizeof(u64));
        ASSERT(ptrs[i] != NULL);
        ASSERT(*ptrs[i] == 0);
        *ptrs[i] = i;
    }
    ASSERT(test_ptrs_sum(ptrs, n));
    free(ptrs);
    return true;
}

bool test_calloc(AIL_Allocator *allocator, u64 bytes)
{
    u64 n = bytes/2/sizeof(u64);
    if (!test_calloc_single(allocator, bytes/2)) return false;
    // Test allocations for lists of elements
    u64 *vals = AIL_CALL_CALLOC(*allocator, n, sizeof(u64));
    ASSERT(vals != NULL);
    for (u64 i = 0; i < n; i++) {
        ASSERT(vals[i] == 0);
        vals[i] = i;
    }
    ASSERT(test_sum(vals, n));

    return true;
}

bool test_realloc(AIL_Allocator *allocator, u64 bytes)
{
    u64 total = bytes/sizeof(u64);
    // Test reallocating NULL
    u64 n = total/4;
    u64 *vals = AIL_CALL_REALLOC(*allocator, NULL, n*sizeof(u64));
    ASSERT(vals != NULL);
    for (u64 i = 0; i < n; i++) {
        vals[i] = i;
    }
    ASSERT(test_sum(vals, n));

    // Test reallocating other kinds of pointers
    n = total - n;
    u64 init_size = n/3;
    u64 smol_size = init_size/2;
    u64 big_size  = n - init_size - smol_size;
    ASSERT(big_size > init_size);
    u64 *initial = AIL_CALL_ALLOC(*allocator, init_size*sizeof(u64));
    ASSERT(initial != NULL);
    for (u64 i = 0; i < init_size; i++) initial[i] = i;
    u64 *smaller = AIL_CALL_REALLOC(*allocator, initial, smol_size*sizeof(u64));
    ASSERT(smaller != NULL);
    ASSERT(test_sum(smaller, smol_size));
    u64 *bigger = AIL_CALL_REALLOC(*allocator, smaller, big_size*sizeof(u64));
    ASSERT(bigger != NULL);
    ASSERT(test_sum(bigger, smol_size));
    for (u64 i = 0; i < big_size; i++) bigger[i] = i;
    ASSERT(test_sum(bigger, big_size));
    return true;
}

// Assumes that allocator has AIL_ALLOC_PAGE memory space at the beginning
bool general_test(AIL_Allocator allocator, const char *name, bool lists_allocable, bool extendable)
{
    bool res = true;
    bool tmp;
    if (!lists_allocable) {
        tmp = test_alloc_single(&allocator, AIL_ALLOC_PAGE_SIZE/4);
        res = res && tmp;
        if (tmp)  printf("\033[032mSimple   allocations      work for %s allocator :)\033[0m\n", name);
        else      printf("\033[031mSimple   allocations      fail for %s allocator :(\033[0m\n", name);
        tmp = test_calloc_single(&allocator, AIL_ALLOC_PAGE_SIZE/4);
        res = res && tmp;
        if (tmp) printf("\033[032mSimple   zero-allocations work for %s allocator :)\033[0m\n", name);
        else     printf("\033[031mSimple   zero-allocations fail for %s allocator :(\033[0m\n", name);
        if (extendable) {
            tmp = test_alloc_single(&allocator, 2*AIL_ALLOC_PAGE_SIZE);
            res = res && tmp;
            if (tmp) printf("\033[032mExtended allocations      work for %s allocator :)\033[0m\n", name);
            else     printf("\033[031mExtended allocations      fail for %s allocator :(\033[0m\n", name);
            tmp = test_calloc_single(&allocator, 2*AIL_ALLOC_PAGE_SIZE);
            res = res && tmp;
            if (tmp) printf("\033[032mExtended zero-allocations work for %s allocator :)\033[0m\n", name);
            else     printf("\033[031mExtended zero-allocations fail for %s allocator :(\033[0m\n", name);
        }
    } else {
        tmp = test_alloc(&allocator, AIL_ALLOC_PAGE_SIZE/6);
        res = res && tmp;
        if (tmp) printf("\033[032mSimple   allocations      work for %s allocator :)\033[0m\n", name);
        else     printf("\033[031mSimple   allocations      fail for %s allocator :(\033[0m\n", name);
        tmp = test_calloc(&allocator, AIL_ALLOC_PAGE_SIZE/6);
        res = res && tmp;
        if (tmp) printf("\033[032mSimple   zero-allocations work for %s allocator :)\033[0m\n", name);
        else     printf("\033[031mSimple   zero-allocations fail for %s allocator :(\033[0m\n", name);
        tmp = test_realloc(&allocator, AIL_ALLOC_PAGE_SIZE/6);
        res = res && tmp;
        if (tmp) printf("\033[032mSimple   re-allocations   work for %s allocator :)\033[0m\n", name);
        else     printf("\033[031mSimple   re-allocations   fail for %s allocator :(\033[0m\n", name);
        if (extendable) {
            tmp = test_alloc(&allocator, 2*AIL_ALLOC_PAGE_SIZE);
            res = res && tmp;
            if (tmp) printf("\033[032mExtended allocations      work for %s allocator :)\033[0m\n", name);
            else     printf("\033[031mExtended allocations      fail for %s allocator :(\033[0m\n", name);
            tmp = test_calloc(&allocator, 2*AIL_ALLOC_PAGE_SIZE);
            res = res && tmp;
            if (tmp) printf("\033[032mExtended zero-allocations work for %s allocator :)\033[0m\n", name);
            else     printf("\033[031mExtended zero-allocations fail for %s allocator :(\033[0m\n", name);
            tmp = test_realloc(&allocator, 2*AIL_ALLOC_PAGE_SIZE);
            res = res && tmp;
            if (tmp) printf("\033[032mExtended re-allocations   work for %s allocator :)\033[0m\n", name);
            else     printf("\033[031mExtended re-allocations   fail for %s allocator :(\033[0m\n", name);
        }
    }
    return res;
}

bool aligning_test(void)
{
    for (u64 i = 0; i < 4; i++) {
        for (u64 j = 0; j < AIL_ALLOC_ALIGNMENT; j++) {
            u64 x = i*AIL_ALLOC_ALIGNMENT + j;
            u64 pad = ail_alloc_size_aligned_pad(x);
            u64 out = ail_alloc_align_size(x);
            if (j==0) {
                ASSERT(pad == 0);
                ASSERT(out == i*AIL_ALLOC_ALIGNMENT);
            }
            else {
                ASSERT(pad == AIL_ALLOC_ALIGNMENT - j);
                ASSERT(out == (i+1)*AIL_ALLOC_ALIGNMENT);
            }
        }
    }
    for (u64 alidx = 2; alidx < 8; alidx++) {
        u64 alignment = 1 << alidx;
        for (u64 i = 0; i < 4; i++) {
            for (u64 j = 0; j < alignment; j++) {
                u64 x = i*alignment + j;
                u64 forward  = ail_alloc_align_forward(x, alignment);
                u64 backward = ail_alloc_align_backward(x, alignment);
                u64 fpad = ail_alloc_size_aligned_forward_pad(x, alignment);
                u64 bpad = ail_alloc_size_aligned_backward_pad(x, alignment);
                // printf("bpad: %zd, j: %zd, x: %zd\n", bpad, j, x);
                ASSERT(bpad == j);
                if (j==0) {
                    ASSERT(forward == i*alignment);
                    ASSERT(forward == backward);
                    ASSERT(fpad == 0);
                } else {
                    ASSERT(forward  == (i+1)*alignment);
                    ASSERT(backward == i*alignment);
                    ASSERT(fpad == alignment - j);
                }
            }
        }
    }
    return true;
}

bool test_pager(void)
{
    u64 first_size   = 16;
    u64 first_size2  = 64;
    u64 second_size2 = AIL_ALLOC_PAGE_SIZE/2;
    u64 second_size  = 2*AIL_ALLOC_PAGE_SIZE/3;
    void *first  = AIL_CALL_ALLOC(ail_alloc_pager, first_size);
    AIL_Alloc_Page_Header *fh = AIL_ALLOC_GET_HEADER(first, AIL_Alloc_Page_Header);
    ASSERT(fh->size >= first_size);
    void *second = AIL_CALL_ALLOC(ail_alloc_pager, second_size);
    AIL_Alloc_Page_Header *sh = AIL_ALLOC_GET_HEADER(second, AIL_Alloc_Page_Header);
    ASSERT(sh->size >= second_size);

    void *first2 = AIL_CALL_REALLOC(ail_alloc_pager, first, first_size2);
    ASSERT(first2 == first);
    ASSERT(fh->size >= first_size2);
    first = AIL_CALL_REALLOC(ail_alloc_pager, first2, first_size);
    ASSERT(first == first2);
    ASSERT(fh->size >= first_size);

    void *second2 = AIL_CALL_REALLOC(ail_alloc_pager, second, second_size2);
    ASSERT(second == second2);
    ASSERT(sh->size >= second_size2);
    second = AIL_CALL_REALLOC(ail_alloc_pager, second2, second_size);
    ASSERT(second == second2);
    ASSERT(sh->size >= second_size);

    u64 zero_size = 512;
    u8 *zero = AIL_CALL_ALLOC(ail_alloc_pager, zero_size);
    for (u64 i = 0; i < zero_size; i++) ASSERT(zero[i] == 0);

    AIL_CALL_FREE(ail_alloc_pager, first);
    AIL_CALL_FREE(ail_alloc_pager, second);
    AIL_CALL_FREE(ail_alloc_pager, zero);
    return true;
}

bool test_buffer(void)
{
    u64 n = AIL_ALLOC_PAGE_SIZE - sizeof(AIL_Alloc_Page_Header);
    u8 *buffer = AIL_CALL_ALLOC(ail_alloc_pager, n);
    AIL_Allocator b = ail_alloc_buffer_new(n, buffer);
    ASSERT(((AIL_Alloc_Buffer *)b.data)->size == n - sizeof(AIL_Alloc_Buffer));

    // Malloc
    u8 *p = AIL_CALL_ALLOC(b, n/4);
    for (u64 i = 0; i < n/4; i++) p[i] = (u8)i;
    ASSERT(((AIL_Alloc_Buffer *)b.data)->idx  == n/4);
    for (u64 i = 0; i < n/4; i++) ASSERT(p[i] == (u8)i);
    // Free is no-op
    AIL_CALL_FREE(b, p);
    u8 *tmp = AIL_CALL_ALLOC(b, n/10);
    ASSERT(tmp > p);
    for (u64 i = 0; i < n/4; i++) ASSERT(p[i] == (u8)i);
    // Realloc
    u8 *q = AIL_CALL_REALLOC(b, p, n/6);
    ASSERT(q > p);
    for (u64 i = 0; i < n/6; i++) ASSERT(q[i] == (u8)i);
    // Calloc
    u8 *r = AIL_CALL_CALLOC(b, n/6, 1);
    for (u64 i = 0; i < n/6; i++) ASSERT(r[i] == 0);
    // Free all
    AIL_CALL_FREE_ALL(b);
    ASSERT(((AIL_Alloc_Buffer *)b.data)->idx  == 0);
    u8 *s = AIL_CALL_ALLOC(b, n/4);
    ASSERT(p == s);

    AIL_CALL_FREE(ail_alloc_pager, buffer);
    return true;
}

int main(void)
{
    { // Test Alignment utilities
        if (aligning_test()) printf("\033[032mAlignment Utilities work correctly :)\033[0m\n");
        else                 printf("\033[031mAlignment Utilities fail :(\033[0m\n");
    }
    printf("------\n");
    { // Test Pager
        if (test_pager()) printf("\033[032mPage Allocator works correctly :)\033[0m\n");
        else              printf("\033[031mPage Allocator fails :(\033[0m\n");
    }
    printf("------\n");
    { // Test Buffer Allocator
        u8 backing_buffer[AIL_ALLOC_PAGE_SIZE];
        AIL_Allocator buffer = ail_alloc_buffer_new(AIL_ALLOC_PAGE_SIZE, backing_buffer);
        bool res = general_test(buffer, "Buffer", true, false);
        res = res && test_buffer();
        if (res) printf("\033[032mBuffer Allocator works correctly :)\033[0m\n");
        else     printf("\033[031mBuffer Allocator fails :( \033[0m\n");
    }
    printf("------\n");
    { // Test Ring Allocator
        u8 backing_buffer[AIL_ALLOC_PAGE_SIZE];
        AIL_Allocator ring = ail_alloc_ring_new(AIL_ALLOC_PAGE_SIZE, backing_buffer);
        bool res = general_test(ring, "Ring", true, false);
        // res = res && test_ring();
        if (res) printf("\033[032mRing Allocator works correctly :)\033[0m\n");
        else     printf("\033[031mRing Allocator fails :( \033[0m\n");
    }
    printf("------\n");
    { // Test Arena Allocator
        AIL_Allocator arena = ail_alloc_arena_new(AIL_ALLOC_PAGE_SIZE, &ail_alloc_pager);
        bool res = general_test(arena, "Arena", true, true);
        // res = res && test_arena();
        if (res) printf("\033[032mArena Allocator works correctly :)\033[0m\n");
        else     printf("\033[031mArena Allocator fails :( \033[0m\n");
    }
    printf("------\n");
    { // Test Pool Allocator
        AIL_Allocator pool = ail_alloc_pool_new(AIL_ALLOC_PAGE_SIZE/sizeof(u64), sizeof(u64), &ail_alloc_pager);
        bool res = general_test(pool, "Pool", false, true);
        // res = res && test_pool();
        if (res) printf("\033[032mPool Allocator works correctly :)\033[0m\n");
        else     printf("\033[031mPool Allocator fails :( \033[0m\n");
    }
    printf("------\n");
    { // Test Freelist Allocator
        AIL_Allocator freelist = ail_alloc_freelist_new(AIL_ALLOC_PAGE_SIZE, &ail_alloc_pager);
        bool res = general_test(freelist, "Freelist", true, true);
        // res = res && test_freelist();
        if (res) printf("\033[032mFreelist Allocator works correctly :)\033[0m\n");
        else     printf("\033[031mFreelist Allocator fails :( \033[0m\n");
    }
    printf("------\n");
    printf("\033[032mTested all allocators\033[0m\n");
    return 0;
}
