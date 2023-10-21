// Different types of Allocators
//
// Define AIL_ALLOC_IMPL in some file, to include the function bodies
// Drop-in replacement for malloc/free via the global alloc_ctx variable
// Call alloc_ctxAlloc, alloc_ctxFree, alloc_ctxFreeAll to use whichever allocator is currently set as the context
// Use ALLOC_SWITCH_CTX and ALLOC_SWITCH_BACK_CTX to change the context (back)

#ifndef AIL_ALLOC_H_
#define AIL_ALLOC_H_

#ifndef AIL_TYPES_IMPL
#define AIL_TYPES_IMPL
#endif // AIL_TYPES_IMPL
#include "ail.h"

#define ALLOC_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#define ALLOC_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#define ALLOC_MAX(a, b)      (((a) > (b)) ? (a) : (b))

typedef struct {
	void *data;
	void *(*alloc)(void*, size_t);
	void *(*realloc)(void*, void*, size_t);
    void  (*free)(void*, void*);
    void  (*freeAll)(void*);
} Alloc_Allocator;

// @TODO: This allocator isn't really an arena -> need another name
typedef struct {
	size_t cap;
	size_t idx;
	void  *next;
	Alloc_Allocator allocator;
} Alloc_Arena_Header;

typedef size_t Alloc_Arena_El_Header;

void *alloc_ctxAlloc(size_t size);
void *alloc_ctxCalloc(size_t nelem, size_t elsize);
void *alloc_ctxRealloc(void *ptr, size_t size);
void alloc_ctxFree(void *ptr);
void alloc_ctxFreeAll();
void *alloc_stdAlloc(void *data, size_t size);
void *alloc_stdRealloc(void *data, void *ptr, size_t size);
void alloc_stdFree(void *data, void *ptr);
void alloc_stdFreeAll(void *data);
void *alloc_arenaNextBlock(Alloc_Allocator allocator, size_t size, bool growable);
void *alloc_arenaAlloc(void *data, size_t size);
void *alloc_arenaRealloc(void *data, void *ptr, size_t size);
void alloc_arenaFree(void *data, void *ptr);
void alloc_arenaFreeAll(void *data);

static Alloc_Allocator alloc_std = {
	.data    = NULL,
	.alloc   = alloc_stdAlloc,
	.realloc = alloc_stdRealloc,
	.free    = alloc_stdFree,
	.freeAll = alloc_stdFreeAll,
};

static Alloc_Allocator *alloc_ctx = &alloc_std;
static Alloc_Allocator *_alloc_prev_ctx_ = NULL;
#define ALLOC_SWITCH_CTX(allocator) { _alloc_prev_ctx_ = alloc_ctx; alloc_ctx = &(allocator); }
#define ALLOC_SWITCH_BACK_CTX() alloc_ctx = _alloc_prev_ctx_
#define ALLOC_GET_HEADER(strct, data) ((strct *)(((char *)(data))-sizeof(strct)))


#endif // AIL_ALLOC_H_

#ifdef AIL_ALLOC_IMPL
#ifndef _AIL_ALLOC_IMPL_GUARD_
#define _AIL_ALLOC_IMPL_GUARD_

#include <stdlib.h> // For std_allocator


/////////////
// Context //
/////////////

void *alloc_ctxAlloc(size_t size)
{
	return alloc_ctx->alloc(alloc_ctx->data, size);
}

void *alloc_ctxCalloc(size_t nelem, size_t elsize)
{
	return alloc_ctxAlloc(nelem * elsize);
}

void *alloc_ctxRealloc(void *ptr, size_t size)
{
	return alloc_ctx->realloc(alloc_ctx->data, ptr, size);
}

void alloc_ctxFree(void *ptr)
{
	alloc_ctx->free(alloc_ctx->data, ptr);
}

void alloc_ctxFreeAll()
{
	alloc_ctx->freeAll(alloc_ctx->data);
}


/////////
// Std //
/////////

void *alloc_stdAlloc(void *data, size_t size)
{
	(void)data;
	return malloc(size);
}

void *alloc_stdRealloc(void *data, void *ptr, size_t size)
{
	(void)data;
	return realloc(ptr, size);
}

void alloc_stdFree(void *data, void *ptr)
{
	(void)data;
	free(ptr);
}

void alloc_stdFreeAll(void *data)
{
	(void)data;
}


///////////
// Arena //
///////////

Alloc_Allocator alloc_arenaInit(Alloc_Allocator allocator, size_t initialSize, bool growable)
{
	return (Alloc_Allocator) {
		.data    = alloc_arenaNextBlock(allocator, initialSize, growable),
		.alloc   = alloc_arenaAlloc,
		.realloc = alloc_arenaRealloc,
		.free    = alloc_arenaFree,
		.freeAll = alloc_arenaFreeAll,
	};
}

void *alloc_arenaNextBlock(Alloc_Allocator allocator, size_t size, bool growable)
{
	void *data = allocator.alloc(allocator.data, sizeof(Alloc_Arena_Header) + size);
	*((Alloc_Arena_Header *)data) = (Alloc_Arena_Header) {
		.cap  = size,
		.idx  = 0,
		.next = NULL,
		.allocator = growable ? allocator : (Alloc_Allocator){0},
	};
	return ((char *)(data)) + sizeof(Alloc_Arena_Header);
}

void *alloc_arenaAlloc(void *data, size_t size)
{
	Alloc_Arena_Header *header = ALLOC_GET_HEADER(Alloc_Arena_Header, data);
	if (ALLOC_UNLIKELY(header->idx + size + sizeof(Alloc_Arena_El_Header) >= header->cap)) {
		if (header->allocator.alloc) {
			if (!header->next) {
				header->next = alloc_arenaNextBlock(header->allocator, ALLOC_MAX(header->cap, 2*size), true);
			}
			return alloc_arenaAlloc(header->next, size);
		} else {
			alloc_arenaFreeAll(data);
		}
	}
	void *out = &((char *)data)[header->idx + sizeof(Alloc_Arena_El_Header)];
	header->idx += size + sizeof(Alloc_Arena_El_Header);
	return out;
}

void *alloc_arenaRealloc(void *data, void *ptr, size_t size)
{
	if (!ptr) return alloc_arenaAlloc(data, size);
	Alloc_Arena_Header    *header = ALLOC_GET_HEADER(Alloc_Arena_Header, data);
	Alloc_Arena_El_Header *elSize = ALLOC_GET_HEADER(Alloc_Arena_El_Header, ptr);
	while (ALLOC_UNLIKELY(((char *)(data)) + header->cap < ((char *)(ptr)))) header = ALLOC_GET_HEADER(Alloc_Arena_Header, data);
	if (header->idx == (size_t)(ptr) + *elSize) {
		header->idx = (size_t)(ptr) + size;
		*elSize     = size;
		return ptr;
	} else {
		void *newptr = alloc_arenaAlloc(data, size);
		memcpy(ptr, newptr, *elSize);
		return newptr;
	}
}

void alloc_arenaFree(void *data, void *ptr)
{
	(void)data;
	(void)ptr;
}

void alloc_arenaFreeAll(void *data)
{
	Alloc_Arena_Header *header = ALLOC_GET_HEADER(Alloc_Arena_Header, data);
	header->idx = 0;
	if (ALLOC_UNLIKELY(header->next)) alloc_arenaFreeAll(header->next);
	// @Decide: Should we memset previously allocated region to 0?
}

#endif // _AIL_ALLOC_IMPL_GUARD_
#endif // AIL_ALLOC_IMPL