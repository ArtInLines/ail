/*
*** Ring-Buffer ***
*/

#ifndef _AIL_RING_H_
#define _AIL_RING_H_

#include "ail_base.h"

typedef struct AIL_RingBuffer {
    u64 start;
    u64 end;
    u64 buffer_size;
    u8 *data;
} AIL_RingBuffer;

inline AIL_RingBuffer ail_ring_from_data(u8 *buffer, u64 buffer_size);
inline AIL_RingBuffer ail_ring_new(u64 buffer_size, AIL_Allocator allocator);
inline void ail_buf_free(AIL_RingBuffer rb, AIL_Allocator allocator);
internal u64  ail_ring_len      (AIL_RingBuffer  rb);
internal void ail_ring_pop      (AIL_RingBuffer *rb);
internal void ail_ring_popn     (AIL_RingBuffer *rb, u64 n);
internal u8   ail_ring_peek     (AIL_RingBuffer  rb);
internal u8   ail_ring_peek_at  (AIL_RingBuffer  rb, u64 offset);
internal u16  ail_ring_peek2msb (AIL_RingBuffer  rb);
internal u16  ail_ring_peek2lsb (AIL_RingBuffer  rb);
internal u32  ail_ring_peek4msb (AIL_RingBuffer  rb);
internal u32  ail_ring_peek4lsb (AIL_RingBuffer  rb);
internal u64  ail_ring_peek8msb (AIL_RingBuffer  rb);
internal u64  ail_ring_peek8lsb (AIL_RingBuffer  rb);
internal void ail_ring_peekn    (AIL_RingBuffer  rb, u64 n, u8 *buf);
internal u8    ail_ring_read    (AIL_RingBuffer *rb);
internal u16  ail_ring_read2msb (AIL_RingBuffer *rb);
internal u16  ail_ring_read2lsb (AIL_RingBuffer *rb);
internal u32  ail_ring_read4msb (AIL_RingBuffer *rb);
internal u32  ail_ring_read4lsb (AIL_RingBuffer *rb);
internal u64  ail_ring_read8msb (AIL_RingBuffer *rb);
internal u64  ail_ring_read8lsb (AIL_RingBuffer *rb);
internal void ail_ring_readn    (AIL_RingBuffer *rb, u64 n, u8 *buf);
internal void ail_ring_write_at (AIL_RingBuffer *rb, u64 offset, u8 x); // @Note: Does not modify rb->end
internal void ail_ring_write1   (AIL_RingBuffer *rb, u8 x);
internal void ail_ring_write2msb(AIL_RingBuffer *rb, u16 x);
internal void ail_ring_write2lsb(AIL_RingBuffer *rb, u16 x);
internal void ail_ring_write4msb(AIL_RingBuffer *rb, u32 x);
internal void ail_ring_write4lsb(AIL_RingBuffer *rb, u32 x);
internal void ail_ring_write8msb(AIL_RingBuffer *rb, u64 x);
internal void ail_ring_write8lsb(AIL_RingBuffer *rb, u64 x);
internal void ail_ring_writen   (AIL_RingBuffer *rb, u64 n, u8 *buf);

#endif // _AIL_RING_H_


#if !defined(AIL_NO_RING_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_RING_IMPL_GUARD_
#define _AIL_RING_IMPL_GUARD_

AIL_RingBuffer ail_ring_from_data(u8 *buffer, u64 buffer_size)
{
    return (AIL_RingBuffer) {
        .data = buffer,
        .buffer_size = buffer_size,
    };
}

AIL_RingBuffer ail_ring_new(u64 buffer_size, AIL_Allocator allocator)
{
    return (AIL_RingBuffer) {
        .data = AIL_CALL_ALLOC(allocator, buffer_size),
        .buffer_size = buffer_size,
    };
}

void ail_buf_free(AIL_RingBuffer rb, AIL_Allocator allocator)
{
    AIL_CALL_FREE(allocator, rb.data);
}

u64 ail_ring_len(AIL_RingBuffer rb)
{
    bool wrapped = rb.end < rb.start;
    return (!wrapped)*(rb.end - rb.start) + (wrapped)*(rb.end + rb.buffer_size - rb.start);
}

// Pops first element or does nothing if buffer is empty
void ail_ring_pop(AIL_RingBuffer *rb)
{
    if (rb->end != rb->start) rb->start = (rb->start + 1) % rb->buffer_size;
}

void ail_ring_popn(AIL_RingBuffer *rb, u64 n)
{
    if (ail_ring_len(*rb) < n) rb->start = rb->end = 0;
    else rb->start = (rb->start + n) % rb->buffer_size;
}

// Returns the next byte or 0 if the buffer's length is 0
u8 ail_ring_peek(AIL_RingBuffer rb)
{
    return (rb.end != rb.start)*rb.data[rb.start];
}

u8 ail_ring_peek_at(AIL_RingBuffer rb, u64 offset)
{
    return (ail_ring_len(rb) > offset)*rb.data[(rb.start + offset) % rb.buffer_size];
}

u16 ail_ring_peek2msb(AIL_RingBuffer rb)
{
    return ((u16)ail_ring_peek_at(rb, 0) << 8) | ((u16)ail_ring_peek_at(rb, 1));
}

u16 ail_ring_peek2lsb(AIL_RingBuffer rb)
{
    return ((u16)ail_ring_peek_at(rb, 1) << 8) | ((u16)ail_ring_peek_at(rb, 0));
}

u32 ail_ring_peek4msb(AIL_RingBuffer rb)
{
    return ((u32)ail_ring_peek_at(rb, 0) << 24) | ((u32)ail_ring_peek_at(rb, 1) << 16) | ((u32)ail_ring_peek_at(rb, 2) << 8) | ((u32)ail_ring_peek_at(rb, 3));
}

u32 ail_ring_peek4lsb(AIL_RingBuffer rb)
{
    return ((u32)ail_ring_peek_at(rb, 3) << 24) | ((u32)ail_ring_peek_at(rb, 2) << 16) | ((u32)ail_ring_peek_at(rb, 1) << 8) | ((u32)ail_ring_peek_at(rb, 0));
}

u64 ail_ring_peek8msb(AIL_RingBuffer rb)
{
    return ((u64)ail_ring_peek_at(rb, 0) << 7*8) | ((u64)ail_ring_peek_at(rb, 1) << 6*8) | ((u64)ail_ring_peek_at(rb, 2) << 5*8) | ((u64)ail_ring_peek_at(rb, 3) << 4*8) |
           ((u64)ail_ring_peek_at(rb, 4) << 3*8) | ((u64)ail_ring_peek_at(rb, 5) << 2*8) | ((u64)ail_ring_peek_at(rb, 6) << 1*8) | ((u64)ail_ring_peek_at(rb, 7) << 0*8);
}

u64 ail_ring_peek8lsb(AIL_RingBuffer rb)
{
    return ((u64)ail_ring_peek_at(rb, 7) << 7*8) | ((u64)ail_ring_peek_at(rb, 6) << 6*8) | ((u64)ail_ring_peek_at(rb, 5) << 5*8) | ((u64)ail_ring_peek_at(rb, 4) << 4*8) |
           ((u64)ail_ring_peek_at(rb, 3) << 3*8) | ((u64)ail_ring_peek_at(rb, 2) << 2*8) | ((u64)ail_ring_peek_at(rb, 1) << 1*8) | ((u64)ail_ring_peek_at(rb, 0) << 0*8);
}

void ail_ring_peekn(AIL_RingBuffer rb, u64 n, u8 *buf)
{
    // @TODO: Use ail_mem_copy
    for (u64 i = 0, j = rb.start; i < n; i++, j = (j+1)%rb.buffer_size) {
        buf[i] = rb.data[j];
        // @Safety @Performance
        // @Note: The more safe version would set the value to 0 if it's out of bounds
        // buf[i] = rb.data[j]*(j < rb.end || (rb.end < rb.start && rb.start <= j));
    }
}

u8 ail_ring_read(AIL_RingBuffer *rb)
{
    u8 res = ail_ring_peek(*rb);
    ail_ring_popn(rb, 1);
    return res;
}

u16 ail_ring_read2msb(AIL_RingBuffer *rb)
{
    u16 res = ail_ring_peek2msb(*rb);
    ail_ring_popn(rb, 2);
    return res;
}

u16 ail_ring_read2lsb(AIL_RingBuffer *rb)
{
    u16 res = ail_ring_peek2lsb(*rb);
    ail_ring_popn(rb, 2);
    return res;
}

u32 ail_ring_read4msb(AIL_RingBuffer *rb)
{
    u32 res = ail_ring_peek4msb(*rb);
    ail_ring_popn(rb, 4);
    return res;
}

u32 ail_ring_read4lsb(AIL_RingBuffer *rb)
{
    u32 res = ail_ring_peek4lsb(*rb);
    ail_ring_popn(rb, 4);
    return res;
}

u64 ail_ring_read8msb(AIL_RingBuffer *rb)
{
    u64 res = ail_ring_peek8msb(*rb);
    ail_ring_popn(rb, 8);
    return res;
}

u64 ail_ring_read8lsb(AIL_RingBuffer *rb)
{
    u64 res = ail_ring_peek8lsb(*rb);
    ail_ring_popn(rb, 8);
    return res;
}

void ail_ring_readn(AIL_RingBuffer *rb, u64 n, u8 *buf)
{
    ail_ring_peekn(*rb, n, buf);
    ail_ring_popn(rb, n);
}

void ail_ring_write_at(AIL_RingBuffer *rb, u64 offset, u8 x)
{
    rb->data[(rb->end + offset)%rb->buffer_size] = x;
    AIL_RING_ASSERT((rb->end < rb->start) == (rb->end + offset < rb->start));
}

void ail_ring_write1(AIL_RingBuffer *rb, u8 x)
{
    ail_ring_write_at(rb, 0, x);
    rb->end = (rb->end + 1)%rb->buffer_size;
}

void ail_ring_write2msb(AIL_RingBuffer *rb, u16 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 0*8));
    rb->end = (rb->end + 2)%rb->buffer_size;
}

void ail_ring_write2lsb(AIL_RingBuffer *rb, u16 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 0*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 1*8));
    rb->end = (rb->end + 2)%rb->buffer_size;
}

void ail_ring_write4msb(AIL_RingBuffer *rb, u32 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 3*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 2*8));
    ail_ring_write_at(rb, 2, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 3, (u8)(x >> 0*8));
    rb->end = (rb->end + 4)%rb->buffer_size;
}

void ail_ring_write4lsb(AIL_RingBuffer *rb, u32 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 0*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 2, (u8)(x >> 2*8));
    ail_ring_write_at(rb, 3, (u8)(x >> 3*8));
    rb->end = (rb->end + 4)%rb->buffer_size;
}

void ail_ring_write8msb(AIL_RingBuffer *rb, u64 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 7*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 6*8));
    ail_ring_write_at(rb, 2, (u8)(x >> 5*8));
    ail_ring_write_at(rb, 3, (u8)(x >> 4*8));
    ail_ring_write_at(rb, 4, (u8)(x >> 3*8));
    ail_ring_write_at(rb, 5, (u8)(x >> 2*8));
    ail_ring_write_at(rb, 6, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 7, (u8)(x >> 0*8));
    rb->end = (rb->end + 8)%rb->buffer_size;
}

void ail_ring_write8lsb(AIL_RingBuffer *rb, u64 x)
{
    ail_ring_write_at(rb, 0, (u8)(x >> 0*8));
    ail_ring_write_at(rb, 1, (u8)(x >> 1*8));
    ail_ring_write_at(rb, 2, (u8)(x >> 2*8));
    ail_ring_write_at(rb, 3, (u8)(x >> 3*8));
    ail_ring_write_at(rb, 4, (u8)(x >> 4*8));
    ail_ring_write_at(rb, 5, (u8)(x >> 5*8));
    ail_ring_write_at(rb, 6, (u8)(x >> 6*8));
    ail_ring_write_at(rb, 7, (u8)(x >> 7*8));
    rb->end = (rb->end + 8)%rb->buffer_size;
}

void ail_ring_writen(AIL_RingBuffer *rb, u64 n, u8 *buf)
{
    // @TODO: Use ail_mem_copy
    for (u64 i = 0; i < n; i++, rb->end = (rb->end+1)%rb->buffer_size) {
        rb->data[rb->end] = buf[i];

        u8 tmp = (rb->end+1)%rb->buffer_size;
        AIL_RING_ASSERT(tmp != rb->start);
    }
}

#endif // _AIL_RING_IMPL_GUARD_
#endif // AIL_NO_RING_IMPL
