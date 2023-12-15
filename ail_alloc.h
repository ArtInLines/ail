// Different types of Allocators
//
// Define AIL_ALLOC_IMPL in some file, to include the function bodies
// Drop-in replacement for malloc/free via the global ail_alloc_ctx variable
// Call ail_alloc_ctx_alloc, ail_alloc_ctx_free, ail_alloc_ctx_free_all to use whichever allocator is currently set as the context
// Use AIL_ALLOC_SWITCH_CTX and AIL_ALLOC_SWITCH_BACK_CTX to change the context (back)

#ifndef AIL_ALLOC_H_
#define AIL_ALLOC_H_

#ifndef AIL_TYPES_IMPL
#define AIL_TYPES_IMPL
#endif // AIL_TYPES_IMPL
#include "ail.h"

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

typedef struct {
	void *data;
	void *(*alloc)(void*, size_t);
	void *(*realloc)(void*, void*, size_t);
    void  (*free)(void*, void*);
    void  (*freeAll)(void*);
} AIL_Alloc_Allocator;

// @TODO: This allocator isn't really an arena -> need another name
typedef struct {
	size_t cap;
	size_t idx;
	void  *next;
	AIL_Alloc_Allocator allocator;
} Alloc_Arena_Header;

typedef size_t AIL_Alloc_Arena_El_Header;

AIL_ALLOC_DEF void *ail_alloc_ctx_alloc(size_t size);
AIL_ALLOC_DEF void *ail_alloc_ctx_calloc(size_t nelem, size_t elsize);
AIL_ALLOC_DEF void *ail_alloc_ctx_realloc(void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_ctx_free(void *ptr);
AIL_ALLOC_DEF void ail_alloc_ctx_free_all();
AIL_ALLOC_DEF void *ail_alloc_std_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_std_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_std_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_std_free_all(void *data);
AIL_ALLOC_DEF void *ail_alloc_arena_next_block(AIL_Alloc_Allocator allocator, size_t size, bool growable);
AIL_ALLOC_DEF void *ail_alloc_arena_alloc(void *data, size_t size);
AIL_ALLOC_DEF void *ail_alloc_arena_realloc(void *data, void *ptr, size_t size);
AIL_ALLOC_DEF void ail_alloc_arena_free(void *data, void *ptr);
AIL_ALLOC_DEF void ail_alloc_arena_free_all(void *data);

static AIL_Alloc_Allocator ail_alloc_std = {
	.data    = NULL,
	.alloc   = ail_alloc_std_alloc,
	.realloc = ail_alloc_std_realloc,
	.free    = ail_alloc_std_free,
	.freeAll = ail_alloc_std_free_all,
};

static AIL_Alloc_Allocator *ail_alloc_ctx = &ail_alloc_std;
static AIL_Alloc_Allocator *_ail_alloc_prev_ctx_ = NULL;
#define AIL_ALLOC_SWITCH_CTX(allocator) { _ail_alloc_prev_ctx_ = ail_alloc_ctx; ail_alloc_ctx = &(allocator); }
#define AIL_ALLOC_SWITCH_BACK_CTX() ail_alloc_ctx = _ail_alloc_prev_ctx_
#define AIL_ALLOC_GET_HEADER(strct, data) ((strct *)(((char *)(data))-sizeof(strct)))


#endif // AIL_ALLOC_H_

#ifdef AIL_ALLOC_IMPL
#ifndef _AIL_ALLOC_IMPL_GUARD_
#define _AIL_ALLOC_IMPL_GUARD_

#include <stdlib.h> // For std_allocator


/////////////
// Context //
/////////////

AIL_ALLOC_DEF void *ail_alloc_ctx_alloc(size_t size)
{
	return ail_alloc_ctx->alloc(ail_alloc_ctx->data, size);
}

AIL_ALLOC_DEF void *ail_alloc_ctx_calloc(size_t nelem, size_t elsize)
{
	return ail_alloc_ctx_alloc(nelem * elsize);
}

AIL_ALLOC_DEF void *ail_alloc_ctx_realloc(void *ptr, size_t size)
{
	return ail_alloc_ctx->realloc(ail_alloc_ctx->data, ptr, size);
}

AIL_ALLOC_DEF void ail_alloc_ctx_free(void *ptr)
{
	ail_alloc_ctx->free(ail_alloc_ctx->data, ptr);
}

AIL_ALLOC_DEF void ail_alloc_ctx_free_all()
{
	ail_alloc_ctx->freeAll(ail_alloc_ctx->data);
}


/////////
// Std //
/////////

AIL_ALLOC_DEF void *ail_alloc_std_alloc(void *data, size_t size)
{
	(void)data;
	return malloc(size);
}

AIL_ALLOC_DEF void *ail_alloc_std_realloc(void *data, void *ptr, size_t size)
{
	(void)data;
	return realloc(ptr, size);
}

AIL_ALLOC_DEF void ail_alloc_std_free(void *data, void *ptr)
{
	(void)data;
	free(ptr);
}

AIL_ALLOC_DEF void ail_alloc_std_free_all(void *data)
{
	(void)data;
}


///////////
// Arena //
///////////

AIL_ALLOC_DEF AIL_Alloc_Allocator ail_alloc_arena_init(AIL_Alloc_Allocator allocator, size_t initialSize, bool growable)
{
	return (AIL_Alloc_Allocator) {
		.data    = ail_alloc_arena_next_block(allocator, initialSize, growable),
		.alloc   = ail_alloc_arena_alloc,
		.realloc = ail_alloc_arena_realloc,
		.free    = ail_alloc_arena_free,
		.freeAll = ail_alloc_arena_free_all,
	};
}

AIL_ALLOC_DEF void *ail_alloc_arena_next_block(AIL_Alloc_Allocator allocator, size_t size, bool growable)
{
	void *data = allocator.alloc(allocator.data, sizeof(Alloc_Arena_Header) + size);
	*((Alloc_Arena_Header *)data) = (Alloc_Arena_Header) {
		.cap  = size,
		.idx  = 0,
		.next = NULL,
		.allocator = growable ? allocator : (AIL_Alloc_Allocator){0},
	};
	return ((char *)(data)) + sizeof(Alloc_Arena_Header);
}

AIL_ALLOC_DEF void *ail_alloc_arena_alloc(void *data, size_t size)
{
	Alloc_Arena_Header *header = AIL_ALLOC_GET_HEADER(Alloc_Arena_Header, data);
	if (AIL_UNLIKELY(header->idx + size + sizeof(AIL_Alloc_Arena_El_Header) >= header->cap)) {
		if (header->allocator.alloc) {
			if (!header->next) {
				header->next = ail_alloc_arena_next_block(header->allocator, AIL_MAX(header->cap, 2*size), true);
			}
			return ail_alloc_arena_alloc(header->next, size);
		} else {
			ail_alloc_arena_free_all(data);
		}
	}
	void *out = &((char *)data)[header->idx + sizeof(AIL_Alloc_Arena_El_Header)];
	header->idx += size + sizeof(AIL_Alloc_Arena_El_Header);
	return out;
}

AIL_ALLOC_DEF void *ail_alloc_arena_realloc(void *data, void *ptr, size_t size)
{
	if (!ptr) return ail_alloc_arena_alloc(data, size);
	Alloc_Arena_Header    *header = AIL_ALLOC_GET_HEADER(Alloc_Arena_Header, data);
	AIL_Alloc_Arena_El_Header *elSize = AIL_ALLOC_GET_HEADER(AIL_Alloc_Arena_El_Header, ptr);
	while (AIL_UNLIKELY(((char *)(data)) + header->cap < ((char *)(ptr)))) header = AIL_ALLOC_GET_HEADER(Alloc_Arena_Header, data);
	if (header->idx == (size_t)(ptr) + *elSize) {
		header->idx = (size_t)(ptr) + size;
		*elSize     = size;
		return ptr;
	} else {
		void *newptr = ail_alloc_arena_alloc(data, size);
		memcpy(ptr, newptr, *elSize);
		return newptr;
	}
}

AIL_ALLOC_DEF void ail_alloc_arena_free(void *data, void *ptr)
{
	(void)data;
	(void)ptr;
}

AIL_ALLOC_DEF void ail_alloc_arena_free_all(void *data)
{
	Alloc_Arena_Header *header = AIL_ALLOC_GET_HEADER(Alloc_Arena_Header, data);
	header->idx = 0;
	if (AIL_UNLIKELY(header->next)) ail_alloc_arena_free_all(header->next);
	// @Decide: Should we memset previously allocated region to 0?
}

#endif // _AIL_ALLOC_IMPL_GUARD_
#endif // AIL_ALLOC_IMPL