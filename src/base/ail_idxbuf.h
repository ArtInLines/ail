/*
*** Simple Indexed Buffer ***
*/

#ifndef _AIL_IDXBUF_H_
#define _AIL_IDXBUF_H_

#include "ail_base.h"
#include "ail_alloc.h"

typedef struct AIL_IdxBuffer {
    u8 *data;
    u64 idx;
    u64 len;
    u64 cap;
    AIL_Allocator allocator;
} AIL_IdxBuffer;

// inline_func AIL_IdxBuffer ail_idxbuf_from_file(const char *filename);
// inline_func bool ail_idxbuf_copy_to_file(AIL_IdxBuffer buf, const char *filename);
// inline_func bool ail_idxbuf_to_file(AIL_IdxBuffer *buf, const char *filename);
inline_func AIL_IdxBuffer ail_idxbuf_from_data(u8 *data, u64 len, u64 idx, AIL_Allocator allocator);
inline_func AIL_IdxBuffer ail_idxbuf_new(u64 cap, AIL_Allocator allocator);
internal void ail_idxbuf_ensure_size(AIL_IdxBuffer *buf, u64 n);
inline_func void ail_idxbuf_free(AIL_IdxBuffer buf);
inline_func u8  ail_idxbuf_peek1   (AIL_IdxBuffer  buf);
inline_func u16 ail_idxbuf_peek2lsb(AIL_IdxBuffer  buf);
inline_func u32 ail_idxbuf_peek3lsb(AIL_IdxBuffer  buf);
inline_func u32 ail_idxbuf_peek4lsb(AIL_IdxBuffer  buf);
inline_func u64 ail_idxbuf_peek8lsb(AIL_IdxBuffer  buf);
inline_func u16 ail_idxbuf_peek2msb(AIL_IdxBuffer  buf);
inline_func u32 ail_idxbuf_peek3msb(AIL_IdxBuffer  buf);
inline_func u32 ail_idxbuf_peek4msb(AIL_IdxBuffer  buf);
inline_func u64 ail_idxbuf_peek8msb(AIL_IdxBuffer  buf);
internal        char *ail_idxbuf_peekstr (AIL_IdxBuffer buf, u64 len);
internal        char *ail_idxbuf_peekcstr(AIL_IdxBuffer buf);
internal u8  ail_idxbuf_read1   (AIL_IdxBuffer *buf);
internal u16 ail_idxbuf_read2lsb(AIL_IdxBuffer *buf);
internal u32 ail_idxbuf_read3lsb(AIL_IdxBuffer *buf);
internal u32 ail_idxbuf_read4lsb(AIL_IdxBuffer *buf);
internal u64 ail_idxbuf_read8lsb(AIL_IdxBuffer *buf);
internal u16 ail_idxbuf_read2msb(AIL_IdxBuffer *buf);
internal u32 ail_idxbuf_read3msb(AIL_IdxBuffer *buf);
internal u32 ail_idxbuf_read4msb(AIL_IdxBuffer *buf);
internal u64 ail_idxbuf_read8msb(AIL_IdxBuffer *buf);
internal char *ail_idxbuf_readstr (AIL_IdxBuffer *buf, u64 len);
internal char *ail_idxbuf_readcstr(AIL_IdxBuffer *buf);
internal void ail_idxbuf_write1   (AIL_IdxBuffer *buf, u8  val);
internal void ail_idxbuf_write2lsb(AIL_IdxBuffer *buf, u16 val);
internal void ail_idxbuf_write3lsb(AIL_IdxBuffer *buf, u32 val);
internal void ail_idxbuf_write4lsb(AIL_IdxBuffer *buf, u32 val);
internal void ail_idxbuf_write8lsb(AIL_IdxBuffer *buf, u64 val);
internal void ail_idxbuf_write2msb(AIL_IdxBuffer *buf, u16 val);
internal void ail_idxbuf_write3msb(AIL_IdxBuffer *buf, u32 val);
internal void ail_idxbuf_write4msb(AIL_IdxBuffer *buf, u32 val);
internal void ail_idxbuf_write8msb(AIL_IdxBuffer *buf, u64 val);
internal void ail_idxbuf_writestr (AIL_IdxBuffer *buf, char *str, u64 len);
internal void ail_idxbuf_writecstr(AIL_IdxBuffer *buf, char *str);

#endif // _AIL_IDXBUF_H_


#if !defined(AIL_NO_IDXBUF_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_IDXBUF_IMPL_GUARD_
#define _AIL_IDXBUF_IMPL_GUARD_

// @TODO: Copy to fs module
// AIL_IdxBuffer ail_idxbuf_from_file(const char *filename)
// {
//     u64 len;
//     u8 *data = (u8 *)ail_fs_read_entire_file(filename, &len);
//     return (AIL_IdxBuffer) {
//         .data = data,
//         .idx  = 0,
//         .len  = len,
//         .cap  = len,
//     };
// }

// bool ail_idxbuf_copy_to_file(AIL_IdxBuffer buf, const char *filename)
// {
//     return ail_fs_write_file(filename, (char*) buf.data, buf.len);
// }

// // Unlike copyToFile, this function frees the buffer after writing it to the file
// bool ail_idxbuf_to_file(AIL_IdxBuffer *buf, const char *filename)
// {
//     bool out = ail_fs_write_file(filename, (char*)(buf->data), buf->len);
//     ail_call_free(buf->allocator, buf->data);
//     return out;
// }

AIL_IdxBuffer ail_idxbuf_new(u64 cap, AIL_Allocator allocator)
{
    AIL_IdxBuffer buf;
    buf.data = ail_call_alloc(allocator, cap);
    buf.len  = 0;
    buf.cap  = cap;
    buf.idx  = 0;
    buf.allocator = allocator;
    return buf;
}

AIL_IdxBuffer ail_idxbuf_from_data(u8 *data, u64 len, u64 idx, AIL_Allocator allocator)
{
    AIL_IdxBuffer buf;
    buf.data = data;
    buf.idx  = idx;
    buf.len  = len;
    buf.cap  = len;
    buf.allocator = allocator;
    return buf;
}

void ail_idxbuf_free(AIL_IdxBuffer buf)
{
    ail_call_free(buf.allocator, buf.data);
}

// Ensures that there's enough capacity to write `n` more bytes into the buffer
void ail_idxbuf_ensure_size(AIL_IdxBuffer *buf, u64 n)
{
    u64 min = buf->len + n;
    if (AIL_UNLIKELY(min > buf->cap)) {
        u64 new_cap = buf->cap * 2;
        if (AIL_UNLIKELY(min > new_cap)) new_cap = min;
        buf->data = ail_call_realloc(buf->allocator, buf->data, new_cap);
        buf->cap = new_cap;
    }
}

u8 ail_idxbuf_peek1(AIL_IdxBuffer buf)
{
    return buf.data[buf.idx];
}
#define ail_idxbuf_peek1lsb(buf) ail_idxbuf_peek1(buf)
#define ail_idxbuf_peek1msb(buf) ail_idxbuf_peek1(buf)

u16 ail_idxbuf_peek2lsb(AIL_IdxBuffer buf)
{
    return ((u16)buf.data[buf.idx + 1] << 8) | ((u16)buf.data[buf.idx + 0] << 0);
}

u32 ail_idxbuf_peek3lsb(AIL_IdxBuffer buf)
{
    return ((u32)buf.data[buf.idx + 2] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 0] << 0);
}

u32 ail_idxbuf_peek4lsb(AIL_IdxBuffer buf)
{
    return ((u32)buf.data[buf.idx + 3] << 24) | ((u32)buf.data[buf.idx + 2] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 0] << 0);
}

u64 ail_idxbuf_peek8lsb(AIL_IdxBuffer buf)
{
    return ((u64)buf.data[buf.idx + 7] << 56) | ((u64)buf.data[buf.idx + 6] << 48) | ((u64)buf.data[buf.idx + 5] << 40) | ((u64)buf.data[buf.idx + 4] << 32) |
           ((u64)buf.data[buf.idx + 3] << 24) | ((u64)buf.data[buf.idx + 2] << 16) | ((u64)buf.data[buf.idx + 1] <<  8) | ((u64)buf.data[buf.idx + 0] <<  0);
}

u16 ail_idxbuf_peek2msb(AIL_IdxBuffer buf)
{
    return ((u16)buf.data[buf.idx + 0] << 8) | ((u16)buf.data[buf.idx + 1] << 0);
}

u32 ail_idxbuf_peek3msb(AIL_IdxBuffer buf)
{
    return ((u32)buf.data[buf.idx + 0] << 16) | ((u32)buf.data[buf.idx + 1] << 8) | ((u32)buf.data[buf.idx + 2] << 0);
}

u32 ail_idxbuf_peek4msb(AIL_IdxBuffer buf)
{
    return ((u32)buf.data[buf.idx + 0] << 24) | ((u32)buf.data[buf.idx + 1] << 16) | ((u32)buf.data[buf.idx + 2] << 8) | ((u32)buf.data[buf.idx + 3] << 0);
}

u64 ail_idxbuf_peek8msb(AIL_IdxBuffer buf)
{
    return ((u64)buf.data[buf.idx + 0] << 56) | ((u64)buf.data[buf.idx + 1] << 48) | ((u64)buf.data[buf.idx + 2] << 40) | ((u64)buf.data[buf.idx + 3] << 32) |
           ((u64)buf.data[buf.idx + 4] << 24) | ((u64)buf.data[buf.idx + 5] << 16) | ((u64)buf.data[buf.idx + 6] <<  8) | ((u64)buf.data[buf.idx + 7] <<  0);
}

char *ail_idxbuf_peekstr(AIL_IdxBuffer buf, u64 len)
{
    char *out = ail_call_alloc(buf.allocator, sizeof(char) * (len + 1));
    ail_mem_copy(out, &buf.data[buf.idx], len);
    out[len] = 0;
    return out;
}

char *ail_idxbuf_peekcstr(AIL_IdxBuffer buf)
{
    u64 i = 0;
    while (buf.data[buf.idx + i] != 0 && buf.len > buf.idx + i) i++;
    return ail_idxbuf_peekstr(buf, i);
}

u8  ail_idxbuf_read1(AIL_IdxBuffer *buf)
{
    return buf->data[buf->idx++];
}
#define ail_idxbuf_read1lsb(buf) ail_idxbuf_read1(buf)
#define ail_idxbuf_read1msb(buf) ail_idxbuf_read1(buf)

u16 ail_idxbuf_read2lsb(AIL_IdxBuffer *buf)
{
    u16 out   = ail_idxbuf_peek2lsb(*buf);
    buf->idx += sizeof(u16);
    return out;
}

u32 ail_idxbuf_read3lsb(AIL_IdxBuffer *buf)
{
    u32 out   = ail_idxbuf_peek3lsb(*buf);
    buf->idx += 3;
    return out;
}

u32 ail_idxbuf_read4lsb(AIL_IdxBuffer *buf)
{
    u32 out   = ail_idxbuf_peek4lsb(*buf);
    buf->idx += sizeof(u32);
    return out;
}

u64 ail_idxbuf_read8lsb(AIL_IdxBuffer *buf)
{
    u64 out   = ail_idxbuf_peek8lsb(*buf);
    buf->idx += sizeof(u64);
    return out;
}

u16 ail_idxbuf_read2msb(AIL_IdxBuffer *buf)
{
    u16 out   = ail_idxbuf_peek2msb(*buf);
    buf->idx += sizeof(u16);
    return out;
}

u32 ail_idxbuf_read3msb(AIL_IdxBuffer *buf)
{
    u32 out   = ail_idxbuf_peek3msb(*buf);
    buf->idx += 3;
    return out;
}

u32 ail_idxbuf_read4msb(AIL_IdxBuffer *buf)
{
    u32 out   = ail_idxbuf_peek4msb(*buf);
    buf->idx += sizeof(u32);
    return out;
}

u64 ail_idxbuf_read8msb(AIL_IdxBuffer *buf)
{
    u64 out   = ail_idxbuf_peek8msb(*buf);
    buf->idx += sizeof(u64);
    return out;
}

char *ail_idxbuf_readstr(AIL_IdxBuffer *buf, u64 len)
{
    char *out = ail_idxbuf_peekstr(*buf, len);
    buf->idx += len;
    return out;
}

char *ail_idxbuf_readcstr(AIL_IdxBuffer *buf)
{
    u64 len = 0;
    while (buf->data[buf->idx + len] != 0 && buf->len > buf->idx + len) len++;
    char *out = ail_idxbuf_readstr(buf, len);
    buf->idx = len + 1;
    return out;
}


void ail_idxbuf_write1(AIL_IdxBuffer *buf, u8  val)
{
    ail_idxbuf_ensure_size(buf, 1);
    buf->data[buf->idx++] = val;
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}
#define ail_idxbuf_write1lsb(buf, val) ail_idxbuf_write1(buf, val)
#define ail_idxbuf_write1msb(buf, val) ail_idxbuf_write1(buf, val)

void ail_idxbuf_write2lsb(AIL_IdxBuffer *buf, u16 val)
{
    ail_idxbuf_ensure_size(buf, sizeof(u16));
    for (u8 i = 0; i < (u8)sizeof(u16); i++) {
        buf->data[buf->idx++] = (u8)(val & 0xff);
        val >>= 8;
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_write3lsb(AIL_IdxBuffer *buf, u32 val)
{
    ail_idxbuf_ensure_size(buf, 3);
    for (u8 i = 0; i < 3; i++) {
        buf->data[buf->idx++] = (u8)(val & 0xff);
        val >>= 8;
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_write4lsb(AIL_IdxBuffer *buf, u32 val)
{
    ail_idxbuf_ensure_size(buf, sizeof(u32));
    for (u8 i = 0; i < (u8)sizeof(u32); i++) {
        buf->data[buf->idx++] = (u8)(val & 0xff);
        val >>= 8;
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_write8lsb(AIL_IdxBuffer *buf, u64 val)
{
    ail_idxbuf_ensure_size(buf, sizeof(u64));
    for (u8 i = 0; i < (u8)sizeof(u64); i++) {
        buf->data[buf->idx++] = (u8)(val & 0xff);
        val >>= 8;
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_write2msb(AIL_IdxBuffer *buf, u16 val)
{
    ail_idxbuf_ensure_size(buf, sizeof(u16));
    for (u8 i = 0; i < (u8)sizeof(u16); i++) {
        buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u16) - i - 1)));
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_write3msb(AIL_IdxBuffer *buf, u32 val)
{
    ail_idxbuf_ensure_size(buf, 3);
    for (u8 i = 0; i < 3; i++) {
        buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(3 - i - 1)));
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_write4msb(AIL_IdxBuffer *buf, u32 val)
{
    ail_idxbuf_ensure_size(buf, sizeof(u32));
    for (u8 i = 0; i < (u8)sizeof(u32); i++) {
        buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u32) - i - 1)));
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_write8msb(AIL_IdxBuffer *buf, u64 val)
{
    ail_idxbuf_ensure_size(buf, sizeof(u64));
    for (u8 i = 0; i < (u8)sizeof(u64); i++) {
        buf->data[buf->idx++] = (u8)(0xff & (val >> 8*(sizeof(u64) - i - 1)));
    }
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_writestr(AIL_IdxBuffer *buf, char *str, u64 len)
{
    for (u64 i = 0; i < len; i++) buf->data[buf->idx++] = str[i];
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

void ail_idxbuf_writecstr(AIL_IdxBuffer *buf, char *str)
{
    for (u64 i = 0; str[i] != 0; i++) buf->data[buf->idx++] = str[i];
    buf->data[buf->idx++] = 0;
    if (AIL_LIKELY(buf->idx > buf->len)) buf->len = buf->idx;
}

#endif // _AIL_IDXBUF_IMPL_GUARD_
#endif // AIL_NO_IDXBUF_IMPL
