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

bool test_alloc(AIL_Allocator *allocator, u64 bytes)
{
	// Test allocations for single elements
	u64 n = bytes/2/sizeof(u64);
	u64 **ptrs = malloc(n * sizeof(void*));
	for (u64 i = 0; i < n; i++) {
		ptrs[i] = allocator->alloc(allocator->data, sizeof(u64));
		ASSERT(ptrs[i] != NULL);
		*ptrs[i] = i;
	}
	ASSERT(test_ptrs_sum(ptrs, n));
	free(ptrs);

	// Test allocations for lists of elements
	u64 *vals = allocator->alloc(allocator->data, n * sizeof(u64));
	for (u64 i = 0; i < n; i++) {
		vals[i] = i;
	}
	ASSERT(test_sum(vals, n));

	return true;
}

bool test_calloc(AIL_Allocator *allocator, u64 bytes)
{
	// Test allocations for single elements
	u64 n = bytes/2/sizeof(u64);
	u64 **ptrs = malloc(n * sizeof(void*));
	for (u64 i = 0; i < n; i++) {
		ptrs[i] = allocator->zero_alloc(allocator->data, 1, sizeof(u64));
		ASSERT(ptrs[i] != NULL);
		ASSERT(*ptrs[i] == 0);
		*ptrs[i] = i;
	}
	ASSERT(test_ptrs_sum(ptrs, n));
	free(ptrs);

	// Test allocations for lists of elements
	u64 *vals = allocator->zero_alloc(allocator->data, n, sizeof(u64));
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
	u64 *vals = allocator->re_alloc(allocator->data, NULL, n*sizeof(u64));
	for (u64 i = 0; i < n; i++) {
		vals[i] = i;
	}
	ASSERT(test_sum(vals, n));

	// Test reallocating other kinds of pointers
	n = total - n;
	u64 init_size = n/3;
	u64 smol_size = init_size/2;
	u64 big_size  = n - init_size - smol_size;
	AIL_STATIC_ASSERT(big_size > init_size);
	u64 *initial = allocator->alloc(allocator->data, init_size*sizeof(u64));
	for (u64 i = 0; i < init_size; i++) initial[i] = i;
	u64 *smaller = allocator->re_alloc(allocator->data, initial, smol_size*sizeof(u64));
	ASSERT(test_sum(smaller, smol_size));
	u64 *bigger = allocator->re_alloc(allocator->data, smaller, big_size*sizeof(u64));
	ASSERT(test_sum(bigger, smol_size));
	for (u64 i = 0; i < big_size; i++) bigger[i] = i;
	ASSERT(test_sum(bigger, big_size));
	return true;
}

// Assumes that allocator has AIL_ALLOC_PAGE memory space at the beginning
void test(AIL_Allocator allocator, const char *name, bool extendable)
{
    if (test_alloc(&allocator, AIL_ALLOC_PAGE_SIZE/4))   printf("\033[032mSimple allocations work for %s allocator :)\033[0m\n", name);
    else                                                 printf("\033[031mSimple allocations fail for %s allocator :(\033[0m\n", name);
    if (test_calloc(&allocator, AIL_ALLOC_PAGE_SIZE/4))  printf("\033[032mSimple zero-allocations work for %s allocator :)\033[0m\n", name);
    else                                  			     printf("\033[031mSimple zero-allocations fail for %s allocator :(\033[0m\n", name);
    if (test_realloc(&allocator, AIL_ALLOC_PAGE_SIZE/4)) printf("\033[032mSimple re-allocations work for %s allocator :)\033[0m\n", name);
    else                                                 printf("\033[031mSimple allocations fail for %s allocator :(\033[0m\n", name);

	if (extendable) {
		if (test_alloc(&allocator, 2*AIL_ALLOC_PAGE_SIZE))   printf("\033[032mSimple allocations work for %s allocator :)\033[0m\n", name);
		else                                                 printf("\033[031mSimple allocations fail for %s allocator :(\033[0m\n", name);
		if (test_calloc(&allocator, 2*AIL_ALLOC_PAGE_SIZE))  printf("\033[032mSimple zero-allocations work for %s allocator :)\033[0m\n", name);
		else                                  			     printf("\033[031mSimple zero-allocations fail for %s allocator :(\033[0m\n", name);
		if (test_realloc(&allocator, 2*AIL_ALLOC_PAGE_SIZE)) printf("\033[032mSimple re-allocations work for %s allocator :)\033[0m\n", name);
		else                                                 printf("\033[031mSimple allocations fail for %s allocator :(\033[0m\n", name);
	}
}

int main(void)
{
	{ // Test Buffer Allocator
		u8 backing_buffer[AIL_ALLOC_PAGE_SIZE];
		AIL_Allocator buffer = ail_alloc_buffer_new(AIL_ALLOC_PAGE_SIZE, backing_buffer);
		test(buffer, "buffer", false);
	}
	{ // Test Ring Allocator
		u8 backing_buffer[AIL_ALLOC_PAGE_SIZE];
		AIL_Allocator ring = ail_alloc_ring_new(AIL_ALLOC_PAGE_SIZE, backing_buffer);
		test(ring, "ring", false);
	}
	{ // Test Arena Allocator
		AIL_Allocator arena = ail_alloc_arena_new(AIL_ALLOC_PAGE_SIZE, &ail_alloc_pager);
		test(arena, "arena", true);
	}
	{ // Test Pool Allocator
		AIL_Allocator pool = ail_alloc_pool_new(AIL_ALLOC_PAGE_SIZE/sizeof(u64), sizeof(u64), &ail_alloc_pager);
		test(pool, "pool", true);
	}
	// { // Test Freelist Allocator
	// 	AIL_Allocator freelist = ail_alloc_freelist_new(AIL_ALLOC_PAGE_SIZE, &ail_alloc_pager);
	// 	test(freelist, "freelist", true);
	// }
	return 0;
}