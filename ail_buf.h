// Simple Buffer
//
// LICENSE
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

#ifndef AIL_BUF_H_
#define AIL_BUF_H_

#ifndef AIL_TYPES_IMPL
#define AIL_TYPES_IMPL
#endif // AIL_TYPES_IMPL
#include "ail.h"

#ifndef AIL_BUF_DEF
#ifdef  AIL_DEF
#define AIL_BUF_DEF AIL_DEF
#else
#define AIL_BUF_DEF
#endif // AIL_DEF
#endif // AIL_BUF_DEF
#ifndef AIL_BUF_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_BUF_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_BUF_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_BUF_DEF_INLINE

#if !defined(AIL_BUF_FREE) && !defined(AIL_BUF_MALLOC)
#if  defined(AIL_MALLOC) &&  defined(AIL_FREE)
#define AIL_BUF_MALLOC AIL_MALLOC
#define AIL_BUF_FREE   AIL_FREE
#else
#include <stdlib.h>
#define AIL_BUF_FREE(ptr)    free(ptr)
#define AIL_BUF_MALLOC(sze) malloc(sze)
#endif
#elif !defined(AIL_BUF_FREE) || !defined(AIL_BUF_MALLOC)
#error "You must define both AIL_BUF_MALLOC and AIL_BUF_FREE, or neither."
#endif

#ifndef AIL_BUF_MEMCPY
#ifdef AIL_MEMCPY
#define AIL_BUF_MEMCPY AIL_MEMCPY
#elif
#include <string.h>
#define AIL_BUF_MEMCPY(dst, src, len) memcpy(dst, src, len)
#endif
#endif

typedef struct {
	u8 *data;
	u64 idx;
	u64 len;
	u64 cap;
} AIL_Buffer;

// @TODO: Add overflow checks when reading/peeking

#ifdef AIL_FS_H_
AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_from_file(const char *filename);
AIL_BUF_DEF_INLINE bool ail_buf_copy_to_file(AIL_Buffer buf, const char *filename);
AIL_BUF_DEF_INLINE bool ail_buf_to_file(AIL_Buffer *buf, const char *filename);
#endif // AIL_FS_H_

AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_from_data(u8 *data, u64 len, u64 idx);
AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_new(u64 cap);
AIL_BUF_DEF void ail_buf_ensure_size(AIL_Buffer *buf, u64 n);
AIL_BUF_DEF_INLINE void ail_buf_free(AIL_Buffer buf);
AIL_BUF_DEF_INLINE u8  ail_buf_peek1   (AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u16 ail_buf_peek2lsb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u32 ail_buf_peek3lsb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u32 ail_buf_peek4lsb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u64 ail_buf_peek8lsb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u16 ail_buf_peek2msb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u32 ail_buf_peek3msb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u32 ail_buf_peek4msb(AIL_Buffer  buf);
AIL_BUF_DEF_INLINE u64 ail_buf_peek8msb(AIL_Buffer  buf);
AIL_BUF_DEF        char *ail_buf_peekstr (AIL_Buffer buf, u64 len);
AIL_BUF_DEF        char *ail_buf_peekcstr(AIL_Buffer buf);
AIL_BUF_DEF u8  ail_buf_read1   (AIL_Buffer *buf);
AIL_BUF_DEF u16 ail_buf_read2lsb(AIL_Buffer *buf);
AIL_BUF_DEF u32 ail_buf_read3lsb(AIL_Buffer *buf);
AIL_BUF_DEF u32 ail_buf_read4lsb(AIL_Buffer *buf);
AIL_BUF_DEF u64 ail_buf_read8lsb(AIL_Buffer *buf);
AIL_BUF_DEF u16 ail_buf_read2msb(AIL_Buffer *buf);
AIL_BUF_DEF u32 ail_buf_read3msb(AIL_Buffer *buf);
AIL_BUF_DEF u32 ail_buf_read4msb(AIL_Buffer *buf);
AIL_BUF_DEF u64 ail_buf_read8msb(AIL_Buffer *buf);
AIL_BUF_DEF char *ail_buf_readstr (AIL_Buffer *buf, u64 len);
AIL_BUF_DEF char *ail_buf_readcstr(AIL_Buffer *buf);
AIL_BUF_DEF void ail_buf_write1   (AIL_Buffer *buf, u8  val);
AIL_BUF_DEF void ail_buf_write2lsb(AIL_Buffer *buf, u16 val);
AIL_BUF_DEF void ail_buf_write3lsb(AIL_Buffer *buf, u32 val);
AIL_BUF_DEF void ail_buf_write4lsb(AIL_Buffer *buf, u32 val);
AIL_BUF_DEF void ail_buf_write8lsb(AIL_Buffer *buf, u64 val);
AIL_BUF_DEF void ail_buf_write2msb(AIL_Buffer *buf, u16 val);
AIL_BUF_DEF void ail_buf_write3msb(AIL_Buffer *buf, u32 val);
AIL_BUF_DEF void ail_buf_write4msb(AIL_Buffer *buf, u32 val);
AIL_BUF_DEF void ail_buf_write8msb(AIL_Buffer *buf, u64 val);
AIL_BUF_DEF void ail_buf_writestr (AIL_Buffer *buf, char *str, u64 len);
AIL_BUF_DEF void ail_buf_writecstr(AIL_Buffer *buf, char *str);

#endif // AIL_BUF_H_


#ifdef  AIL_BUF_IMPL
#ifndef _AIL_BUF_IMPL_GUARD_
#define _AIL_BUF_IMPL_GUARD_

#ifdef AIL_FS_H_
AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_from_file(const char *filename)
{
	u64 len;
	u8 *data = (u8 *)ail_fs_read_file(filename, &len);
	return (AIL_Buffer) {
		.data = data,
		.idx  = 0,
		.len  = len,
		.cap  = len,
	};
}

AIL_BUF_DEF_INLINE  bool ail_buf_copy_to_file(AIL_Buffer buf, const char *filename)
{
	return ail_fs_write_file(filename, (char*) buf.data, buf.len);
}

// Unlike copyToFile, this function frees the buffer after writing it to the file
AIL_BUF_DEF_INLINE bool ail_buf_to_file(AIL_Buffer *buf, const char *filename)
{
	bool out = ail_fs_write_file(filename, (char*)(buf->data), buf->len);
	AIL_BUF_FREE((void *)buf->data);
	return out;
}
#endif // AIL_FS_H_

AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_new(u64 cap)
{
	AIL_Buffer buf = {
		.data = AIL_BUF_MALLOC(cap),
		.len  = 0,
		.cap  = cap,
		.idx  = 0
	};
	return buf;
}

AIL_BUF_DEF_INLINE AIL_Buffer ail_buf_from_data(u8 *data, u64 len, u64 idx)
{
	return (AIL_Buffer) {
		.data = data,
		.idx  = idx,
		.len  = len,
		.cap  = len,
	};
}

AIL_BUF_DEF_INLINE void ail_buf_free(AIL_Buffer buf)
{
	AIL_BUF_FREE((void *)buf.data);
}

// Ensures that there's enough capacity to write `n` more bytes into the buffer
AIL_BUF_DEF void ail_buf_ensure_size(AIL_Buffer *buf, u64 n)
{
	u64 min = buf->len + n;
	if (AIL_UNLIKELY(min > buf->cap)) {
		u64 new_cap = buf->cap * 2;
		if (AIL_UNLIKELY(min > new_cap)) new_cap = min;
		u8 *new_data = (u8 *) AIL_BUF_MALLOC(new_cap);
		memcpy(new_data, buf->data, buf->len);
		AIL_BUF_FREE(buf->data);
		buf->data = new_data;
		buf->cap = new_cap;
	}
}

AIL_BUF_DEF_INLINE u8 ail_buf_peek1(AIL_Buffer buf)
{
	return buf.data[buf.idx];
}
#define ail_buf_peek1lsb(buf) ail_buf_peek1f)
#define ail_buf_peek1msb(buf) ail_buf_peek1f)

AIL_BUF_DEF_INLINE u16 ail_buf_peek2lsb(AIL_Buffer buf)
{
	return ((u16)buf.data[buf.idx + 1] << 8) | ((u16)buf.data[buf.idx + 0] << 0);
}

AIL_BUF_DEF_INLINE u32 ail_buf_peek3lsb(AIL_Buffer buf)
{
	return ((u32)buf.data[buf.idx + 2] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 0] << 0);
}

AIL_BUF_DEF_INLINE u32 ail_buf_peek4lsb(AIL_Buffer buf)
{
	return ((u32)buf.data[buf.idx + 3] << 24) | ((u32)buf.data[buf.idx + 2] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 0] << 0);
}

AIL_BUF_DEF_INLINE u64 ail_buf_peek8lsb(AIL_Buffer buf)
{
	return ((u64)buf.data[buf.idx + 7] << 56) | ((u64)buf.data[buf.idx + 6] << 48) | ((u64)buf.data[buf.idx + 5] << 40) | ((u64)buf.data[buf.idx + 4] << 32) |
		   ((u64)buf.data[buf.idx + 3] << 24) | ((u64)buf.data[buf.idx + 2] << 16) | ((u64)buf.data[buf.idx + 1] <<  8) | ((u64)buf.data[buf.idx + 0] <<  0);
}

AIL_BUF_DEF_INLINE u16 ail_buf_peek2msb(AIL_Buffer buf)
{
	return ((u16)buf.data[buf.idx + 0] << 8) | ((u16)buf.data[buf.idx + 1] << 0);
}

AIL_BUF_DEF_INLINE u32 ail_buf_peek3msb(AIL_Buffer buf)
{
	return ((u32)buf.data[buf.idx + 0] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 2] << 0);
}

AIL_BUF_DEF_INLINE u32 ail_buf_peek4msb(AIL_Buffer buf)
{
	return ((u32)buf.data[buf.idx + 0] << 24) | ((u32)buf.data[buf.idx + 1] << 16) | ((u32)buf.data[buf.idx + 2] << 8) | ((u32)buf.data[buf.idx + 3] << 0);
}

AIL_BUF_DEF_INLINE u64 ail_buf_peek8msb(AIL_Buffer buf)
{
	return ((u64)buf.data[buf.idx + 0] << 56) | ((u64)buf.data[buf.idx + 1] << 48) | ((u64)buf.data[buf.idx + 2] << 40) | ((u64)buf.data[buf.idx + 3] << 32) |
		   ((u64)buf.data[buf.idx + 4] << 24) | ((u64)buf.data[buf.idx + 5] << 16) | ((u64)buf.data[buf.idx + 6] <<  8) | ((u64)buf.data[buf.idx + 7] <<  0);
}

AIL_BUF_DEF char *ail_buf_peekstr(AIL_Buffer buf, u64 len)
{
	char *out = AIL_BUF_MALLOC(sizeof(char) * (len + 1));
	AIL_BUF_MEMCPY(out, &buf.data[buf.idx], len);
	out[len] = 0;
	return out;
}

AIL_BUF_DEF char *ail_buf_peekcstr(AIL_Buffer buf)
{
	u64 i = 0;
	while (buf.data[buf.idx + i] != 0 && buf.len > buf.idx + i) i++;
	return ail_buf_peekstr(buf, i);
}

AIL_BUF_DEF u8  ail_buf_read1(AIL_Buffer *buf)
{
	return buf->data[buf->idx++];
}
#define ail_buf_read1lsb(buf) ail_buf_read1(buf)
#define ail_buf_read1msb(buf) ail_buf_read1(buf)

AIL_BUF_DEF u16 ail_buf_read2lsb(AIL_Buffer *buf)
{
	u16 out   = ail_buf_peek2lsb(*buf);
	buf->idx += sizeof(u16);
	return out;
}

AIL_BUF_DEF u32 ail_buf_read3lsb(AIL_Buffer *buf)
{
	u32 out   = ail_buf_peek3lsb(*buf);
	buf->idx += 3;
	return out;
}

AIL_BUF_DEF u32 ail_buf_read4lsb(AIL_Buffer *buf)
{
	u32 out   = ail_buf_peek4lsb(*buf);
	buf->idx += sizeof(u32);
	return out;
}

AIL_BUF_DEF u64 ail_buf_read8lsb(AIL_Buffer *buf)
{
	u64 out   = ail_buf_peek8lsb(*buf);
	buf->idx += sizeof(u64);
	return out;
}

AIL_BUF_DEF u16 ail_buf_read2msb(AIL_Buffer *buf)
{
	u16 out   = ail_buf_peek2msb(*buf);
	buf->idx += sizeof(u16);
	return out;
}

AIL_BUF_DEF u32 ail_buf_read3msb(AIL_Buffer *buf)
{
	u32 out   = ail_buf_peek3msb(*buf);
	buf->idx += 3;
	return out;
}

AIL_BUF_DEF u32 ail_buf_read4msb(AIL_Buffer *buf)
{
	u32 out   = ail_buf_peek4msb(*buf);
	buf->idx += sizeof(u32);
	return out;
}

AIL_BUF_DEF u64 ail_buf_read8msb(AIL_Buffer *buf)
{
	u64 out   = ail_buf_peek8msb(*buf);
	buf->idx += sizeof(u64);
	return out;
}

AIL_BUF_DEF char *ail_buf_readstr(AIL_Buffer *buf, u64 len)
{
	char *out = ail_buf_peekstr(*buf, len);
	buf->idx += len;
	return out;
}

AIL_BUF_DEF char *ail_buf_readcstr(AIL_Buffer *buf)
{
	u64 len = 0;
	while (buf->data[buf->idx + len] != 0 && buf->len > buf->idx + len) len++;
	char *out = ail_buf_readstr(buf, len);
	buf->idx = len + 1;
	return out;
}


AIL_BUF_DEF void ail_buf_write1(AIL_Buffer *buf, u8  val)
{
	ail_buf_ensure_size(buf, 2);
	buf->data[buf->idx++] = val;
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}
#define ail_buf_write1lsb(buf, val) ail_buf_write1(buf, val)
#define ail_buf_write1msb(buf, val) ail_buf_write1(buf, val)

AIL_BUF_DEF void ail_buf_write2lsb(AIL_Buffer *buf, u16 val)
{
	ail_buf_ensure_size(buf, sizeof(u16));
	for (u8 i = 0; i < (u8)sizeof(u16); i++) {
		buf->data[buf->idx++] = (u8)(val & 0xff);
		val >>= 8;
	}
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write3lsb(AIL_Buffer *buf, u32 val)
{
	ail_buf_ensure_size(buf, 3);
	for (u8 i = 0; i < 3; i++) {
		buf->data[buf->idx++] = (u8)(val & 0xff);
		val >>= 8;
	}
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write4lsb(AIL_Buffer *buf, u32 val)
{
	ail_buf_ensure_size(buf, sizeof(u32));
	for (u8 i = 0; i < (u8)sizeof(u32); i++) {
		buf->data[buf->idx++] = (u8)(val & 0xff);
		val >>= 8;
	}
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write8lsb(AIL_Buffer *buf, u64 val)
{
	ail_buf_ensure_size(buf, sizeof(u64));
	for (u8 i = 0; i < (u8)sizeof(u64); i++) {
		buf->data[buf->idx++] = (u8)(val & 0xff);
		val >>= 8;
	}
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write2msb(AIL_Buffer *buf, u16 val)
{
	ail_buf_ensure_size(buf, sizeof(u16));
	for (u8 i = 0; i < (u8)sizeof(u16); i++) {
		buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u16) - i - 1)));
	}
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write3msb(AIL_Buffer *buf, u32 val)
{
	ail_buf_ensure_size(buf, 3);
	for (u8 i = 0; i < 3; i++) {
		buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(3 - i - 1)));
	}
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write4msb(AIL_Buffer *buf, u32 val)
{
	ail_buf_ensure_size(buf, sizeof(u32));
	for (u8 i = 0; i < (u8)sizeof(u32); i++) {
		buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u32) - i - 1)));
	}
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_write8msb(AIL_Buffer *buf, u64 val)
{
	ail_buf_ensure_size(buf, sizeof(u64));
	for (u8 i = 0; i < (u8)sizeof(u64); i++) {
		buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u64) - i - 1)));
	}
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_writestr(AIL_Buffer *buf, char *str, u64 len)
{
	for (u64 i = 0; i < len; i++) buf->data[buf->idx++] = str[i];
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

AIL_BUF_DEF void ail_buf_writecstr(AIL_Buffer *buf, char *str)
{
	for (u64 i = 0; str[i] != 0; i++) buf->data[buf->idx++] = str[i];
	buf->data[buf->idx++] = 0;
	if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

#endif // _AIL_BUF_IMPL_GUARD_
#endif // AIL_BUF_IMPL