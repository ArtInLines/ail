/*
*** Memory ***
*
*/

#ifndef _AIL_MEM_H_
#define _AIL_MEM_H_

#include "ail_base.h"

void ail_mem_set(void *dst, u8 value, u64 size);
void ail_mem_copy(void *dst, void *src, u64 size);

#endif // _AIL_MEM_H_


#if !defined(AIL_NO_MEM_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_MEM_IMPL_GUARD_
#define _AIL_MEM_IMPL_GUARD_

// @TODO: Replace with custom implementations
#include <stdlib.h>
#include <string.h>

void ail_mem_set(void *dst, u8 value, u64 size)
{
	memset(dst, value, size);
}

void ail_mem_copy(void *dst, void *src, u64 size)
{
	memcpy(dst, src, size);
}

#endif // _AIL_MEM_IMPL_GUARD_
#endif // AIL_NO_MEM_IMPL
