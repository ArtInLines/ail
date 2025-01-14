/*
*** Memory ***
*
*/

#ifndef _AIL_MEM_H_
#define _AIL_MEM_H_

#include "ail_base.h"

AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

internal void ail_mem_set(void *dst, u8 value, u64 size);
internal void ail_mem_copy(void *dst, void *src, u64 size);

AIL_WARN_POP
#endif // _AIL_MEM_H_


#if !defined(AIL_NO_MEM_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_MEM_IMPL_GUARD_
#define _AIL_MEM_IMPL_GUARD_
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

// @TODO: Replace with custom implementations
#include <stdlib.h>
#include <string.h>

void ail_mem_set(void *dst, u8 value, u64 size)
{
	memset(dst, value, size);
}

void ail_mem_copy(void *dst, void *src, u64 size)
{
	memmove(dst, src, size);
}

AIL_WARN_POP
#endif // _AIL_MEM_IMPL_GUARD_
#endif // AIL_NO_MEM_IMPL
