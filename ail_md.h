// Memory Debugger
//
// Enable implementation by defining AIL_MD_IMPL
//
// If AIL_MD_MEM_DEBUG is enabled, the memory debugging system will create macros that replace malloc, calloc, free and realloc
// and allows the system to keppt track of and report where memory is beeing allocated, how much and if the memory is beeing freed.
// This is very useful for finding memory leaks in large applications.
// The system can also over allocate memory and fill it with a magic number and can therfor detect if the application writes outside of the allocated memory.
//
// If AIL_MD_EXIT is defined, then exit(); will be replaced with a funtion that writes to NULL.
// This will make it trivial to find out where an application exits using any debugger.
//
// This code is adapted from Quelsolaar's Forge library (see License below)
// The original code was downloaded from https://gamepipeline.org/forge.html
// @Note: The source code did not provide the license itself. The following license is thus assumed to be correct,
// based on the comment on https://gamepipeline.org/index.html, saying that the code is licensed via the Free-BSD license.
//
// LICENSE of original code
/*
Copyright 2024 Eskil Steenberg. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//
// LICENSE regarding all modifications
/*
Copyright (c) 2024 Val Richter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef AIL_MD_H_
#define AIL_MD_H_

#define AIL_TYPES_IMPL
#include "ail.h"

#ifndef AIL_MD_DEF
#ifdef  AIL_DEF
#define AIL_MD_DEF AIL_DEF
#else
#define AIL_MD_DEF
#endif // AIL_DEF
#endif // AIL_MD_DEF
#ifndef AIL_MD_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_MD_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_MD_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_MD_DEF_INLINE

#include <stdlib.h>
#include <stdio.h>

AIL_MD_DEF void ail_md_mem_init(void (*lock)(void *mutex), void (*unlock)(void *mutex), void *mutex); // Required for memory debugger to be thread safe
AIL_MD_DEF void *ail_md_malloc(u64 size, char *file, u32 line); // Replaces malloc and records the c file and line where it was called
AIL_MD_DEF void *ail_md_calloc(u64 nelem, u64 elsize, char *file, u32 line); // Replaces calloc and records the c file and line where it was called
AIL_MD_DEF void *ail_md_realloc(void *pointer, u64 size, char *file, u32 line); // Replaces realloc and records the c file and line where it was called
AIL_MD_DEF void ail_md_free(void *buf, char *file, u32 line); // Replaces free and records the c file and line where it was called
AIL_MD_DEF bool ail_md_comment(void *buf, char *comment); // add a comment to an allocation that can help identyfy its use.
AIL_MD_DEF void ail_md_print(u32 min_allocs); // Prints out a list of all allocations made, their location, how much memorey each has allocated, freed, and how many allocations have been made. The min_allocs parameter can be set to avoid printing any allocations that have been made fewer times then min_allocs
AIL_MD_DEF void ail_md_reset(void); // ail_md_reset allows you to clear all memory stored in the debugging system if you only want to record allocations after a specific point in your code
AIL_MD_DEF u64 ail_md_consumption(void); // add up all memory consumed by mallocsd and reallocs coverd by the memory debugger .
AIL_MD_DEF bool ail_md_query(void *pointer, u32 *line, char **file, u64 *size); // query the size and place of allocation of a pointer
AIL_MD_DEF bool ail_md_test(void *pointer, u64 size, bool ignore_not_found); // query if a bit of memory is safe to access.
AIL_MD_DEF bool ail_md_mem(); //ail_md_mem checks if any of the bounds of any allocation has been over written and reports where to standard out. The function returns true if any error was found
AIL_MD_DEF void exit_crash(u32 i); // finction guaranteed to crash (Writes to NULL).

AIL_MD_DEF void *ail_md_mem_fopen(const char *file_name, const char *mode, char *file, u32 line);
AIL_MD_DEF void ail_md_mem_fclose(void *f, char *file, u32 line);

#endif // AIL_MD_H_

#ifdef AIL_MD_IMPL
#ifndef _AIL_MD_IMPL_GUARD_
#define _AIL_MD_IMPL_GUARD_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define AIL_MD_MEM_INTERNAL
#define AIL_MD_NO_AIL_MD

AIL_MD_DEF void ail_md_print(u32 min_allocs);

#define AIL_MD_OVER_ALLOC 256
#define AIL_MD_MAGIC_NUM 132
typedef struct{
	u32 size;
	void *buf;
	char *comment;
}STMemAllocBuf;

typedef struct{
	u32 line;
	char file[256];
	STMemAllocBuf *allocs;
	u32 alloc_count;
	u64 alloc_alocated;
	u64 size;
	u64 alocated;
	u64 freed;
}STMemAllocLine;

STMemAllocLine ail_md_alloc_lines[1024];
u32 ail_md_alloc_line_count = 0;

#define AIL_MD_FREE_POINTER_BUFFER_SIZE 1024

typedef struct {
	u32 alloc_line;
	char alloc_file[256];
	u32 free_line;
	char free_file[256];
	u64 size;
	void *pointer;
	bool realloc;
} STMemFreeBuf;

STMemFreeBuf ail_md_freed_memory[AIL_MD_FREE_POINTER_BUFFER_SIZE];
u32 ail_md_freed_memory_current = 0;
u32 ail_md_freed_memory_count = 0;

void *ail_md_alloc_mutex = NULL;
void (*ail_md_alloc_mutex_lock)(void *mutex) = NULL;
void (*ail_md_alloc_mutex_unlock)(void *mutex) = NULL;

void ail_md_mem_init(void (*lock)(void *mutex), void (*unlock)(void *mutex), void *mutex)
{
	ail_md_alloc_mutex = mutex;
	ail_md_alloc_mutex_lock = lock;
	ail_md_alloc_mutex_unlock = unlock;
}


void *ail_md_mem_fopen(const char *file_name, const char *mode, char *file, u32 line)
{
	(void)file;
	(void)line;
	FILE *f;
	f = fopen(file_name, mode);
	return f;
}

void ail_md_mem_fclose(void *f, char *file, u32 line)
{
	(void)file;
	(void)line;
	fclose(f);
}


bool ail_md_mem()
{
	bool output = false;
	u32 i, j, k;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	for(i = 0; i < ail_md_alloc_line_count; i++)
	{
		for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
		{
			u8 *buf;
			u64 size;
			buf = ail_md_alloc_lines[i].allocs[j].buf;
			size = ail_md_alloc_lines[i].allocs[j].size;
			for(k = 0; k < AIL_MD_OVER_ALLOC; k++)
				if(buf[size + k] != AIL_MD_MAGIC_NUM)
					break;
			if(k < AIL_MD_OVER_ALLOC)
			{
				if(ail_md_alloc_lines[i].allocs[j].comment == NULL)
					printf("MEM ERROR: Overshoot at line %u in file %s\n", ail_md_alloc_lines[i].line, ail_md_alloc_lines[i].file);
				else
					printf("MEM ERROR: Overshoot at line %u in file %s /* %s */\n", ail_md_alloc_lines[i].line, ail_md_alloc_lines[i].file, ail_md_alloc_lines[i].allocs[j].comment);
				{
					u32 *X = NULL;
					X[0] = 0;
				}
				output = true;
			}
		}
		// if(output)
		// 	exit(0);
	}
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	return output;
}

void ail_md_add(void *pointer, u64 size, char *file, u32 line)
{
	u32 i, j;
	for(i = 0; i < AIL_MD_OVER_ALLOC; i++)
		((u8 *)pointer)[size + i] = AIL_MD_MAGIC_NUM;

	for(i = 0; i < ail_md_alloc_line_count; i++)
	{
		if(line == ail_md_alloc_lines[i].line)
		{
			for(j = 0; file[j] != 0 && file[j] == ail_md_alloc_lines[i].file[j] ; j++);
			if(file[j] == ail_md_alloc_lines[i].file[j])
				break;
		}
	}
	if(i < ail_md_alloc_line_count)
	{
		if(ail_md_alloc_lines[i].alloc_alocated == ail_md_alloc_lines[i].alloc_count)
		{
			ail_md_alloc_lines[i].alloc_alocated += 1024;
			ail_md_alloc_lines[i].allocs = realloc(ail_md_alloc_lines[i].allocs, (sizeof *ail_md_alloc_lines[i].allocs) * ail_md_alloc_lines[i].alloc_alocated);
			if(ail_md_alloc_lines[i].allocs == NULL)
			{
				printf("MEM ERROR: Realloc returns NULL when trying to allocate %llu bytes at line %u in file %s\n", size, line, file);
				exit(0);
			}
		}
		ail_md_alloc_lines[i].allocs[ail_md_alloc_lines[i].alloc_count].size = size;
		ail_md_alloc_lines[i].allocs[ail_md_alloc_lines[i].alloc_count].comment = NULL;
		ail_md_alloc_lines[i].allocs[ail_md_alloc_lines[i].alloc_count++].buf = pointer;
		ail_md_alloc_lines[i].size += size;
		ail_md_alloc_lines[i].alocated++;
	}else
	{
		if(i < 1024)
		{
			ail_md_alloc_lines[i].line = line;
			for(j = 0; j < 255 && file[j] != 0; j++)
				ail_md_alloc_lines[i].file[j] = file[j];
			ail_md_alloc_lines[i].file[j] = 0;
			ail_md_alloc_lines[i].alloc_alocated = 256;
			ail_md_alloc_lines[i].allocs = malloc((sizeof *ail_md_alloc_lines[i].allocs) * ail_md_alloc_lines[i].alloc_alocated);
			ail_md_alloc_lines[i].allocs[0].size = size;
			ail_md_alloc_lines[i].allocs[0].buf = pointer;
			ail_md_alloc_lines[i].allocs[0].comment = NULL;
			ail_md_alloc_lines[i].alloc_count = 1;
			ail_md_alloc_lines[i].size = size;
			ail_md_alloc_lines[i].freed = 0;
			ail_md_alloc_lines[i].alocated++;
			ail_md_alloc_line_count++;
		}
	}
	for(i = 0; i < ail_md_freed_memory_count; i++)
	{
		if(pointer == ail_md_freed_memory[i].pointer)
		{
			ail_md_freed_memory[i] = ail_md_freed_memory[--ail_md_freed_memory_count];
			break;
		}
	}
}

void *ail_md_malloc(u64 size, char *file, u32 line)
{
	void *pointer;
	u32 i;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	ail_md_mem();
	pointer = malloc(size + AIL_MD_OVER_ALLOC);

#ifdef AIL_MD_MEM_PRINT
	printf("Malloc %6llu bytes at pointer %p at %s line %u\n", size, pointer, file, line);
#endif
	if(pointer == NULL)
	{
		printf("MEM ERROR: Malloc returns NULL when trying to allocate %llu bytes at line %u in file %s\n", size, line, file);
		if(ail_md_alloc_mutex != NULL)
			ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
		ail_md_print(0);
		exit(0);
	}
	for(i = 0; i < size + AIL_MD_OVER_ALLOC; i++)
 		((u8 *)pointer)[i] = AIL_MD_MAGIC_NUM + 1;
	ail_md_add(pointer, size, file, line);

	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	return pointer;
}

void *ail_md_calloc(u64 nelem, u64 elsize, char *file, u32 line)
{
	void *pointer;
	u32 i;
	u64 size = nelem * elsize;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	ail_md_mem();
	pointer = malloc(size + AIL_MD_OVER_ALLOC);

#ifdef AIL_MD_MEM_PRINT
	printf("Calloc %llu bytes at pointer %p at %s line %u\n", size, pointer, file, line);
#endif
	if(pointer == NULL)
	{
		printf("MEM ERROR: Calloc returns NULL when trying to allocate %llu bytes at line %u in file %s\n", size, line, file);
		if(ail_md_alloc_mutex != NULL)
			ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
		ail_md_print(0);
		exit(0);
	}
	for(i = 0; i < size; i++)
 		((u8 *)pointer)[i] = 0;
	for(i = size; i < size + AIL_MD_OVER_ALLOC; i++)
		((u8 *)pointer)[i] = AIL_MD_MAGIC_NUM + 1;
	ail_md_add(pointer, size, file, line);

	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	return pointer;
}


bool ail_md_remove(void *buf, char *file, u32 line, bool realloc, u64 *size)
{
	STMemFreeBuf  *f;
	u32 i, j, k;

	f = &ail_md_freed_memory[ail_md_freed_memory_current];
	ail_md_freed_memory_current = (ail_md_freed_memory_current + 1) % AIL_MD_FREE_POINTER_BUFFER_SIZE;
	if(ail_md_freed_memory_current > ail_md_freed_memory_count)
		ail_md_freed_memory_count = ail_md_freed_memory_current;
	for(i = 0; i < 255 && file[i] != 0; i++)
		f->free_file[i] = file[i];
	f->free_file[i] = 0;
	f->free_line = line;
	f->realloc = realloc;
	f->pointer = buf;

	for(i = 0; i < ail_md_alloc_line_count; i++)
	{
		for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
		{
			if(ail_md_alloc_lines[i].allocs[j].buf == buf)
			{
				for(k = 0; k < AIL_MD_OVER_ALLOC; k++)
					if(((u8 *)buf)[ail_md_alloc_lines[i].allocs[j].size + k] != AIL_MD_MAGIC_NUM)
						break;
				if(k < AIL_MD_OVER_ALLOC)
				{
					u32 *a = NULL;
					printf("MEM ERROR: Overshoot at line %u in file %s\n", ail_md_alloc_lines[i].line, ail_md_alloc_lines[i].file);
					exit(0);
					a[0] = 0;
				}
				for(k = 0; k < ail_md_alloc_lines[i].allocs[j].size; k++)
					((u8 *)buf)[k] = 255;
				f->alloc_line = ail_md_alloc_lines[i].line;
				for(k = 0; k < 255 && ail_md_alloc_lines[i].file[k] != 0; k++)
					f->alloc_file[k] = ail_md_alloc_lines[i].file[k];
				f->alloc_file[k] = 0;
				f->size = ail_md_alloc_lines[i].allocs[j].size;
				*size = ail_md_alloc_lines[i].allocs[j].size;
				ail_md_alloc_lines[i].size -= ail_md_alloc_lines[i].allocs[j].size;
				ail_md_alloc_lines[i].allocs[j] = ail_md_alloc_lines[i].allocs[--ail_md_alloc_lines[i].alloc_count];
				ail_md_alloc_lines[i].freed++;
				return true;
			}
		}
	}
	for(i = 0; i < ail_md_freed_memory_count; i++)
	{
		if(f != &ail_md_freed_memory[i] && buf == ail_md_freed_memory[i].pointer)
		{
			if(f->realloc)
				printf("MEM ERROR: Pointer %p in file is freed twice! if was freed one line %u in %s, was reallocated to %llu bytes long one line %u in file %s\n", f->pointer, f->free_line, f->free_file, f->size, f->alloc_line, f->alloc_file);
			else
				printf("MEM ERROR: Pointer %p in file is freed twice! if was freed one line %u in %s, was allocated to %llu bytes long one line %u in file %s\n", f->pointer, f->free_line, f->free_file, f->size, f->alloc_line, f->alloc_file);

			return false;
		}
	}
	return true;
}



void ail_md_free(void *buf, char *file, u32 line)
{
	// STMemFreeBuf b;
	u64 size = 0;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	if(!ail_md_remove(buf, file, line, false, &size))
	{
		u32 *X = NULL;
		X[0] = 0;
	}


#ifdef AIL_MD_MEM_PRINT
	printf("Free   %6llu bytes at pointer %p at %s line %u\n", size, buf, file, line);
#endif

	free(buf);
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
}

bool ail_md_comment(void *buf, char *comment)
{
	u32 i, j;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	for(i = 0; i < ail_md_alloc_line_count; i++)
	{
		for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
		{
			if(ail_md_alloc_lines[i].allocs[j].buf == buf)
			{
				ail_md_alloc_lines[i].allocs[j].comment = comment;
				return true;
			}
		}
	}
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	return false;
}


void *ail_md_realloc(void *pointer, u64 size, char *file, u32 line)
{
	u64 i, j = 0, k, move;
	void *pointer2;

	if(pointer == NULL)
		return ail_md_malloc(size, file, line);

	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	for(i = 0; i < ail_md_alloc_line_count; i++)
	{
		for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
			if(ail_md_alloc_lines[i].allocs[j].buf == pointer)
				break;
		if(j < ail_md_alloc_lines[i].alloc_count)
			break;
	}
	if(i == ail_md_alloc_line_count)
	{
		printf("AIL memory debugger error. Trying to reallocate pointer %p in %s line %u. Pointer is not allocated\n", pointer, file, line);
		for(i = 0; i < ail_md_alloc_line_count; i++)
		{
			for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
			{
				u32 *buf;
				buf = ail_md_alloc_lines[i].allocs[j].buf;
				for(k = 0; k < ail_md_alloc_lines[i].allocs[j].size; k++)
				{
					if(&buf[k] == pointer)
					{
						printf("Trying to reallocate pointer %llu bytes (out of %u) in to allocation made in %s on line %u.\n", k, ail_md_alloc_lines[i].allocs[j].size, ail_md_alloc_lines[i].file, ail_md_alloc_lines[i].line);
					}
				}
			}
		}
		exit(0);
	}
	move = ail_md_alloc_lines[i].allocs[j].size;

	if(move > size)
		move = size;

	pointer2 = malloc(size + AIL_MD_OVER_ALLOC);
	if(pointer2 == NULL)
	{
		printf("MEM ERROR: Realloc returns NULL when trying to allocate %llu bytes at line %u in file %s\n", size, line, file);
		if(ail_md_alloc_mutex != NULL)
			ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
		ail_md_print(0);
		exit(0);
	}
	for(i = 0; i < size + AIL_MD_OVER_ALLOC; i++)
 		((u8 *)pointer2)[i] = AIL_MD_MAGIC_NUM;
	memcpy(pointer2, pointer, move);

	ail_md_add(pointer2, size, file, line);
	move = 0;
	ail_md_remove(pointer, file, line, true, &move);
#ifdef AIL_MD_MEM_PRINT
	printf("Relloc %6llu bytes at pointer %p to %llu bytes at pointer %p at %s line %u\n", move, pointer, size, pointer2, file, line);
#endif
	free(pointer);

	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	return pointer2;
}

void ail_md_print(u32 min_allocs)
{
	u32 i, j;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	printf("Memory repport:\n----------------------------------------------\n");
	for(i = 0; i < ail_md_alloc_line_count; i++)
	{
		if(min_allocs < ail_md_alloc_lines[i].alocated - ail_md_alloc_lines[i].freed)
		{
			printf("%s line: %u\n",ail_md_alloc_lines[i].file, ail_md_alloc_lines[i].line);
			printf(" - Bytes allocated: %llu\n - Allocations: %llu\n - Frees: %llu\n\n", ail_md_alloc_lines[i].size, ail_md_alloc_lines[i].alocated, ail_md_alloc_lines[i].freed);
			for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
				if(ail_md_alloc_lines[i].allocs[j].comment != NULL)
					printf("\t\t comment %p : %s\n", ail_md_alloc_lines[i].allocs[j].buf, ail_md_alloc_lines[i].allocs[j].comment);
		}
	}
	printf("----------------------------------------------\n");
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
}


u64 ail_md_footprint(u32 min_allocs)
{
	(void)min_allocs;
	u32 i;
	u64 size = 0;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	for(i = 0; i < ail_md_alloc_line_count; i++)
		size += ail_md_alloc_lines[i].size;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	return size;
}

bool ail_md_query(void *pointer, u32 *line, char **file, u64 *size)
{
	u32 i, j;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	for(i = 0; i < ail_md_alloc_line_count; i++)
	{
		for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
		{
			if(ail_md_alloc_lines[i].allocs[j].buf == pointer)
			{
				if(line != NULL)
					*line = ail_md_alloc_lines[i].line;
				if(file != NULL)
					*file = ail_md_alloc_lines[i].file;
				if(size != NULL)
					*size = ail_md_alloc_lines[i].allocs[j].size;
				if(ail_md_alloc_mutex != NULL)
					ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
				return true;
			}
		}
	}
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	return false;
}

bool ail_md_test(void *pointer, u64 size, bool ignore_not_found)
{
	u32 i, j;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	for(i = 0; i < ail_md_alloc_line_count; i++)
	{
		for(j = 0; j < ail_md_alloc_lines[i].alloc_count; j++)
		{
			if(ail_md_alloc_lines[i].allocs[j].buf >= pointer && ((u8 *)ail_md_alloc_lines[i].allocs[j].buf) + ail_md_alloc_lines[i].allocs[j].size <= (u8*)pointer)
			{
				if(((u8 *)ail_md_alloc_lines[i].allocs[j].buf) + ail_md_alloc_lines[i].allocs[j].size < ((u8 *)pointer) + size)
				{
					printf("MEM ERROR: Not enough memory to access pointer %p, %llu bytes missing\n", pointer, (u64)(((u8 *)ail_md_alloc_lines[i].allocs[j].buf) + ail_md_alloc_lines[i].allocs[j].size) - (u64)(((u8 *)pointer) + size));
					if(ail_md_alloc_mutex != NULL)
						ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
					return true;
				}else
				{
					if(ail_md_alloc_mutex != NULL)
						ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
					return false;
				}
			}
		}
	}
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	if(ignore_not_found)
		return false;

	for(i = 0; i < ail_md_freed_memory_count; i++)
	{
		if(pointer >= ail_md_freed_memory[i].pointer && ((u8 *)ail_md_freed_memory[i].pointer) + ail_md_freed_memory[i].size >= ((u8 *)pointer) + size)
		{
			printf("MEM ERROR: Pointer %p was freed on line %u in file %s\n", pointer, ail_md_freed_memory[i].free_line, ail_md_freed_memory[i].free_file);
		}
	}

	printf("MEM ERROR: No matching memory for pointer %p found!\n", pointer);
	return true;
}


u64 ail_md_consumption()
{
	u32 i;
	u64 sum = 0;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
	for(i = 0; i < ail_md_alloc_line_count; i++)
		sum += ail_md_alloc_lines[i].size;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
	return sum;
}

void ail_md_reset()
{
	u32 i;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_lock(ail_md_alloc_mutex);
#ifdef AIL_MD_MEM_PRINT
	printf("Memmory reset --------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
#endif
	for(i = 0; i < ail_md_alloc_line_count; i++)
		free(ail_md_alloc_lines[i].allocs);
	ail_md_alloc_line_count = 0;
	if(ail_md_alloc_mutex != NULL)
		ail_md_alloc_mutex_unlock(ail_md_alloc_mutex);
}

void exit_crash(u32 i)
{
	(void)i;
	u32 *a = NULL;
	a[0] = 0;
}

#endif // _AIL_MD_IMPL_GUARD_
#endif // AIL_MD_IMPL

#ifdef AIL_MD_MEM_DEBUG
#define malloc(n)     ail_md_malloc(n, __FILE__, __LINE__)     // Replaces malloc
#define calloc(n, m)  ail_md_calloc(n, m, __FILE__, __LINE__)  // Replaces calloc
#define realloc(n, m) ail_md_realloc(n, m, __FILE__, __LINE__) // Replaces realloc
#define free(n)       ail_md_free(n, __FILE__, __LINE__)       // Replaces free
#define fopen(n, m)   ail_md_mem_fopen(n, m, __FILE__, __LINE__)
#define fclose(n)     ail_md_mem_fclose(n, __FILE__, __LINE__)
#endif

#ifdef AIL_MD_EXIT
#define exit(n) exit_crash(n) // overwriting exit(n) with a function guarantueed to crash
#endif