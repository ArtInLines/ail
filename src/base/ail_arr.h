/*
*** Arrays ***
* Arrays are implemented as duck-typed templates through macros
* The following kinds of Arrays are implemented:
  * SA: "Sized Array"
    * Contains a pointer and length
    * Can be used like a slice in most other languages
  * CA: "Capped Array"
    * Contains a pointer, length and capacity
    * Can be used like a fixed-size slice with an index representing the amount of filled elements
  * DA: "Dynamic Array"
    * Contains a pointer, length, capacity and allocator, so it can be dynamically resized
    * Can be used like a Vector in C++
*
*/

#ifndef _AIL_ARR_H_
#define _AIL_ARR_H_

#include "./ail_base.h"
#include "./ail_base_math.h"
#include "./ail_mem.h"

AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

#ifndef AIL_ARR_INIT_CAP
#   define AIL_ARR_INIT_CAP 256
#endif

inline_func void* ail_arr_copy(void *src, u64 size, AIL_Allocator allocator);
inline_func void ail_arr_setn(void *data, u64 el_size, u64 idx, void *elems, u64 n);
inline_func void ail_arr_resize(void **data, u64 *cap, u64 *len, u64 el_size, u64 new_cap, AIL_Allocator allocator);
inline_func void ail_arr_maybe_grow(void **data, u64 *cap, u64 *len, u64 el_size, u64 n, AIL_Allocator allocator);
inline_func void ail_arr_maybe_pushn(void **data, u64 cap, u64 *len, u64 el_size, void *elems, u64 n);
inline_func void ail_arr_pushn(void **data, u64 *cap, u64 *len, u64 el_size, void *elems, u64 n, AIL_Allocator allocator);
inline_func void ail_arr_push_right(void *data, u64 *len, u64 el_size, u64 to_idx, u64 from_idx);
inline_func void ail_arr_grow_with_gap(void **data, u64 *cap, u64 *len, u64 el_size, u64 new_cap, u64 gap_start, u64 gap_len, AIL_Allocator allocator);
inline_func void ail_arr_maybe_grow_with_gap(void **data, u64 *cap, u64 *len, u64 el_size, u64 gap_start, u64 gap_len, AIL_Allocator allocator);
inline_func void ail_arr_insertn(void **data, u64 *cap, u64 *len, u64 el_size, u64 idx, void *elems, u64 n, AIL_Allocator al);
inline_func void ail_arr_rm(void *data, u64 *len, u64 el_size, u64 idx, u64 n);

#define AIL_SA_INIT(T) typedef struct AIL_SA_##T { T *data; u64 len; } AIL_SA_##T
#define AIL_SA(T) AIL_SA_##T
#define AIL_CA_INIT(T) typedef struct AIL_CA_##T { T *data; u64 len; u64 cap; } AIL_CA_##T
#define AIL_CA(T) AIL_CA_##T
#define AIL_DA_INIT(T) typedef struct AIL_DA_##T { T *data; u64 len; u64 cap; AIL_Allocator allocator; } AIL_DA_##T
#define AIL_DA(T) AIL_DA_##T
AIL_SA_INIT(u8);    AIL_CA_INIT(u8);    AIL_DA_INIT(u8);
AIL_SA_INIT(u16);   AIL_CA_INIT(u16);   AIL_DA_INIT(u16);
AIL_SA_INIT(u32);   AIL_CA_INIT(u32);   AIL_DA_INIT(u32);
AIL_SA_INIT(u64);   AIL_CA_INIT(u64);   AIL_DA_INIT(u64);
AIL_SA_INIT(i8);    AIL_CA_INIT(i8);    AIL_DA_INIT(i8);
AIL_SA_INIT(i16);   AIL_CA_INIT(i16);   AIL_DA_INIT(i16);
AIL_SA_INIT(i32);   AIL_CA_INIT(i32);   AIL_DA_INIT(i32);
AIL_SA_INIT(i64);   AIL_CA_INIT(i64);   AIL_DA_INIT(i64);
AIL_SA_INIT(f32);   AIL_CA_INIT(f32);   AIL_DA_INIT(f32);
AIL_SA_INIT(f64);   AIL_CA_INIT(f64);   AIL_DA_INIT(f64);
AIL_SA_INIT(pchar); AIL_CA_INIT(pchar); AIL_DA_INIT(pchar);
AIL_SA_INIT(void);  AIL_CA_INIT(void);  AIL_DA_INIT(void);
AIL_SA_INIT(char);  AIL_CA_INIT(char);  AIL_DA_INIT(char);

#define ail_sa_empty(T)                 ail_sa_from_parts(NULL, 0)
#define ail_sa_from_parts(d, l)         { .data = (d), .len = (l) }
#define ail_sa_from_arr(arr)            ail_sa_from_parts(arr, ail_arrlen(arr))
#define ail_sa_from_struct(st)          ail_sa_from_parts((st).data, (st).len)
#define ail_sa_new(T)                   ail_sa_new_with_len(T, AIL_ARR_INIT_CAP)
#define ail_sa_new_arr(arr)             ail_sa_new_arr_alloc(arr, ail_default_allocator)
#define ail_sa_new_struct(st)           ail_sa_new_struct_alloc(st, (st).allocator)
#define ail_sa_new_with_len(T, l)       ail_sa_new_with_alloc(T, l, ail_default_allocator)
#define ail_sa_new_zero_init(T, l)      ail_sa_new_zero_alloc(T, l, ail_default_allocator)
#define ail_sa_new_arr_alloc(arr, al)   ail_sa_from_parts(ail_arr_copy(al, arr, ail_arrlen(arr)), ail_arrlen(arr))
#define ail_sa_new_struct_alloc(st, al) ail_sa_from_parts(ail_arr_copy(al, (st).data, sizeof((st).data[0])*(st).len), (st).len)
#define ail_sa_new_with_alloc(T, l, al) ail_sa_from_parts(ail_call_alloc ((al), sizeof(T)*(l)), l)
#define ail_sa_new_zero_alloc(T, l, al) ail_sa_from_parts(ail_call_calloc((al), sizeof(T)*(l)), l)
#define ail_sa_empty_t(T)                    (AIL_SA(T))ail_sa_empty(T)
#define ail_sa_from_parts_t(T, d, l)         (AIL_SA(T))ail_sa_from_parts(d, l)
#define ail_sa_from_arr_t(T, arr)            (AIL_SA(T))ail_sa_from_arr(arr)
#define ail_sa_from_struct_t(T, st)          (AIL_SA(T))ail_sa_from_struct(st)
#define ail_sa_new_t(T)                      (AIL_SA(T))ail_sa_new(T)
#define ail_sa_new_arr_t(T, arr)             (AIL_SA(T))ail_sa_new_arr(arr)
#define ail_sa_new_struct_t(T, st)           (AIL_SA(T))ail_sa_new_struct(st)
#define ail_sa_new_with_len_t(T, l)          (AIL_SA(T))ail_sa_new_with_len(T, l)
#define ail_sa_new_zero_init_t(T, l)         (AIL_SA(T))ail_sa_new_zero_init(T, l)
#define ail_sa_new_arr_alloc_t(T, arr, al)   (AIL_SA(T))ail_sa_new_arr_alloc(arr, al)
#define ail_sa_new_struct_alloc_t(T, st, al) (AIL_SA(T))ail_sa_new_struct_alloc(st, al)
#define ail_sa_new_with_alloc_t(T, l, al)    (AIL_SA(T))ail_sa_new_with_alloc(T, l, al)
#define ail_sa_new_zero_alloc_t(T, l, al)    (AIL_SA(T))ail_sa_new_zero_alloc(T, l, al)

#define ail_ca_empty(T)                 ail_ca_from_parts(NULL, 0, 0)
#define ail_ca_from_parts(d, l, c)      { .data = (d), .len = (l), .cap = (c) }
#define ail_ca_from_arr(arr)            ail_ca_from_parts(arr, ail_arrlen(arr), ail_arrlen(arr))
#define ail_ca_from_sa(sa, al)          ail_ca_from_parts((sa).data, (sa).len, (sa).len)
#define ail_ca_from_struct(st)          ail_ca_from_parts((st).data, (st).len, (st).cap)
#define ail_ca_new(T)                   ail_ca_new_with_cap(T, AIL_ARR_INIT_CAP)
#define ail_ca_new_arr(arr)             ail_ca_new_arr_alloc(arr, ail_default_allocator)
#define ail_ca_new_sa(sa)               ail_ca_new_sa_alloc(sa, ail_default_allocator)
#define ail_ca_new_struct(st)           ail_ca_new_struct_alloc(st, (st).allocator)
#define ail_ca_new_with_cap(T, c)       ail_ca_new_with_alloc(T, c, ail_default_allocator)
#define ail_ca_new_zero_init(T, c)      ail_ca_new_zero_alloc(T, c, ail_default_allocator)
#define ail_ca_new_arr_alloc(arr, al)   ail_ca_from_parts(ail_arr_copy(al, arr, ail_arrlen(arr)), ail_arrlen(arr), ail_arrlen(arr))
#define ail_ca_new_sa_alloc(sa, al)     ail_ca_from_parts(ail_arr_copy(al, (sa).data, sizeof((sa).data[0])*(sa).len), (sa).len, (sa).len)
#define ail_ca_new_struct_alloc(st, al) ail_ca_from_parts(ail_arr_copy(al, (st).data, sizeof((st).data[0])*(st).cap), (st).len, (st).cap)
#define ail_ca_new_with_alloc(T, c, al) ail_ca_from_parts(ail_call_alloc ((al), sizeof(T)*(c)), 0, c)
#define ail_ca_new_zero_alloc(T, c, al) ail_ca_from_parts(ail_call_calloc((al), sizeof(T)*(c)), 0, c)
#define ail_ca_empty_t(T)                    (AIL_CA(T))ail_ca_empty(T)
#define ail_ca_from_parts_t(T, d, l, c, al)  (AIL_CA(T))ail_ca_from_parts(d, l, c, al)
#define ail_ca_from_arr_t(T, arr)            (AIL_CA(T))ail_ca_from_arr(arr)
#define ail_ca_from_sa_t(T, sa, al)          (AIL_CA(T))ail_ca_from_sa(sa, al)
#define ail_ca_from_struct_t(T, st)          (AIL_CA(T))ail_ca_from_struct(st)
#define ail_ca_new_t(T)                      (AIL_CA(T))ail_ca_new(T)
#define ail_ca_new_arr_t(T, arr)             (AIL_CA(T))ail_ca_new_arr(arr)
#define ail_ca_new_sa_t(T, sa)               (AIL_CA(T))ail_ca_new_sa(sa)
#define ail_ca_new_struct_t(T, st)           (AIL_CA(T))ail_ca_new_struct(st)
#define ail_ca_new_with_cap_t(T, c)          (AIL_CA(T))ail_ca_new_with_cap(T, c)
#define ail_ca_new_zero_init_t(T, c)         (AIL_CA(T))ail_ca_new_zero_init(T, c)
#define ail_ca_new_arr_alloc_t(T, arr, al)   (AIL_CA(T))ail_ca_new_arr_alloc(arr, al)
#define ail_ca_new_sa_alloc_t(T, sa, al)     (AIL_CA(T))ail_ca_new_sa_alloc(sa, al)
#define ail_ca_new_struct_alloc_t(T, st, al) (AIL_CA(T))ail_ca_new_struct_alloc(st, al)
#define ail_ca_new_with_alloc_t(T, c, al)    (AIL_CA(T))ail_ca_new_with_alloc(T, c, al)
#define ail_ca_new_zero_alloc_t(T, c, al)    (AIL_CA(T))ail_ca_new_zero_alloc(T, c, al)

#define ail_da_empty(T)                 ail_da_from_parts(NULL, 0, 0, ail_default_allocator)
#define ail_da_empty_with_alloc(T, al)  ail_da_from_parts(NULL, 0, 0, (al))
#define ail_da_from_parts(d, l, c, al)  { .data = (d), .len = (l), .cap = (c), .allocator = (al) }
#define ail_da_from_sa(sa, al)          ail_da_from_parts((sa).data, (sa).len, (sa).len, (al))
#define ail_da_from_ca(ca, al)          ail_da_from_parts((ca).data, (ca).len, (ca).cap, (al))
#define ail_da_from_struct(st)          ail_da_from_parts((st).data, (st).len, (st).cap, (st).allocator)
#define ail_da_new(T)                   ail_da_new_with_cap(T, AIL_ARR_INIT_CAP)
#define ail_da_new_arr(arr)             ail_da_new_arr_alloc(arr, ail_default_allocator)
#define ail_da_new_sa(sa)               ail_da_new_sa_alloc(sa, ail_default_allocator)
#define ail_da_new_ca(ca)               ail_da_new_ca_alloc(ca, ail_default_allocator)
#define ail_da_new_struct(st)           ail_da_new_struct_alloc(st, (st).allocator)
#define ail_da_new_with_cap(T, c)       ail_da_new_with_alloc(T, c, ail_default_allocator)
#define ail_da_new_zero_init(T, c)      ail_da_new_zero_alloc(T, c, ail_default_allocator)
#define ail_da_new_arr_alloc(arr, al)   ail_da_from_parts(ail_arr_copy(al, arr, ail_arrlen(arr)), ail_arrlen(arr), ail_arrlen(arr), al)
#define ail_da_new_sa_alloc(sa, al)     ail_da_from_parts(ail_arr_copy(al, (sa).data, sizeof((sa).data[0])*(sa).len), (sa).len, (sa).len, al)
#define ail_da_new_ca_alloc(ca, al)     ail_da_from_parts(ail_arr_copy(al, (ca).data, sizeof((ca).data[0])*(ca).cap), (ca).len, (ca).cap, al)
#define ail_da_new_struct_alloc(st, al) ail_da_from_parts(ail_arr_copy(al, (st).data, sizeof((st).data[0])*(st).cap), (st).len, (st).cap, al)
#define ail_da_new_with_alloc(T, c, al) ail_da_from_parts(ail_call_alloc ((al), sizeof(T)*(c)), 0, c, al)
#define ail_da_new_zero_alloc(T, c, al) ail_da_from_parts(ail_call_calloc((al), sizeof(T)*(c)), 0, c, al)
#define ail_da_from_parts_t(T, d, l, c, al)  (AIL_DA(T))ail_da_from_parts(d, l, c, al)
#define ail_da_empty_t(T)                    (AIL_DA(T))ail_da_empty(T)
#define ail_da_empty_with_alloc_t(T, al)     (AIL_DA(T))ail_da_empty_with_alloc(T, al)
#define ail_da_from_sa_t(T, sa, al)          (AIL_DA(T))ail_da_from_sa(sa, al)
#define ail_da_from_ca_t(T, ca, al)          (AIL_DA(T))ail_da_from_ca(ca, al)
#define ail_da_from_struct_t(T, st)          (AIL_DA(T))ail_da_from_struct(st)
#define ail_da_new_t(T)                      (AIL_DA(T))ail_da_new(T)
#define ail_da_new_arr_t(T, arr)             (AIL_DA(T))ail_da_new_arr(arr)
#define ail_da_new_sa_t(T, sa)               (AIL_DA(T))ail_da_new_sa(sa)
#define ail_da_new_ca_t(T, ca)               (AIL_DA(T))ail_da_new_ca(ca)
#define ail_da_new_struct_t(T, st)           (AIL_DA(T))ail_da_new_struct(st)
#define ail_da_new_with_cap_t(T, c)          (AIL_DA(T))ail_da_new_with_cap(T, c)
#define ail_da_new_zero_init_t(T, c)         (AIL_DA(T))ail_da_new_zero_init(T, c)
#define ail_da_new_arr_alloc_t(T, arr, al)   (AIL_DA(T))ail_da_new_arr_alloc(arr, al)
#define ail_da_new_sa_alloc_t(T, sa, al)     (AIL_DA(T))ail_da_new_sa_alloc(sa, al)
#define ail_da_new_ca_alloc_t(T, ca, al)     (AIL_DA(T))ail_da_new_ca_alloc(ca, al)
#define ail_da_new_struct_alloc_t(T, st, al) (AIL_DA(T))ail_da_new_struct_alloc(st, al)
#define ail_da_new_with_alloc_t(T, c, al)    (AIL_DA(T))ail_da_new_with_alloc(T, c, al)
#define ail_da_new_zero_alloc_t(T, c, al)    (AIL_DA(T))ail_da_new_zero_alloc(T, c, al)

#define ail_sa_free(saPtr, al) do { ail_call_free((al), (saPtr)->data); (saPtr)->data = NULL; (saPtr)->len = 0; } while(0)
#define ail_ca_free(caPtr, al) do { ail_sa_free(caPtr, al); (caPtr)->cap = 0; } while(0)
#define ail_da_free(daPtr) ail_ca_free(daPtr, (daPtr)->allocator)
#define ail_da_free_a(daPtr, al) ail_ca_free(daPtr, al)

#define ail_sa_setn(sa, idx, elems, n) ail_arr_setn((sa).data, sizeof((sa).data[0]), idx, elems, n)
#define ail_ca_setn(ca, idx, elems, n) ail_sa_setn(ca, idx, elems, n)
#define ail_da_setn(da, idx, elems, n) ail_sa_setn(da, idx, elems, n)

#define ail_ca_resize(caPtr, new_cap, al)   ail_da_resize_a(caPtr, new_cap, al)
#define ail_da_resize(daPtr, new_cap)       ail_da_resize_a(daPtr, new_cap, (daPtr)->allocator)
#define ail_da_resize_a(daPtr, new_cap, al) ail_arr_resize(ail_field_ptr_of(daPtr, data), ail_field_ptr_of(daPtr, cap), ail_field_ptr_of(daPtr, len), sizeof((daPtr)->data[0]), new_cap, al)

#define ail_ca_maybe_grow(caPtr, n, al) ail_da_maybe_grow_a(caPtr, n, al)
#define ail_da_maybe_grow(daPtr, n)     ail_da_maybe_grow_a(daPtr, n, (daPtr)->allocator)
#define ail_da_maybe_grow_a(daPtr, n, al) ail_arr_maybe_grow(ail_field_ptr_of(daPtr, data), ail_field_ptr_of(daPtr, cap), ail_field_ptr_of(daPtr, len), sizeof((daPtr)->data[0]), n, al)


#define ail_ca_maybe_push(caPtr, elem) do {                                 \
    if ((caPtr)->len < (caPtr)->cap) (caPtr)->data[(caPtr)->len++] = (elem) \
    } while(0)
#define ail_da_maybe_push(daPtr, elem) ail_ca_maybe_push(daPtr, elem)

#define ail_ca_push(caPtr, elem, al) ail_da_push_a(caPtr, elem, al)
#define ail_da_push(daPtr, elem)     ail_da_push_a(daPtr, elem, (daPtr)->allocator)
#define ail_da_push_a(daPtr, elem, al) do {     \
        ail_da_maybe_grow_a(daPtr, 1, (al));    \
        (daPtr)->data[(daPtr)->len++] = (elem); \
    } while(0)

#define ail_ca_maybe_pushn(caPtr, elems, n) ail_arr_maybe_pushn(ail_field_ptr_of(caPtr, data), (caPtr)->cap, ail_field_ptr_of(caPtr, len), sizeof((caPtr)->data[0]), elems, n)
#define ail_da_maybe_pushn(daPtr, elems, n) ail_ca_maybe_pushn(daPtr, elems, n)

#define ail_ca_pushn(caPtr, elems, n, al) ail_da_pushn_a(caPtr, elems, n, al)
#define ail_da_pushn(daPtr, elems, n)     ail_da_pushn_a(daPtr, elems, n, (daPtr)->allocator)
#define ail_da_pushn_a(daPtr, elems, n, al) ail_arr_pushn(ail_field_ptr_of(daPtr, data), ail_field_ptr_of(daPtr, cap), ail_field_ptr_of(daPtr, len), sizeof((daPtr)->data[0]), elems, n, al)

#define ail_ca_grow_with_gap(caPtr, gap_start, gap_len, new_cap, al)   ail_arr_grow_with_gap(ail_field_ptr_of(caPtr, data), ail_field_ptr_of(caPtr, cap), ail_field_ptr_of(caPtr, len), sizeof((caPtr)->data[0]), new_cap, gap_start, gap_len, al)
#define ail_da_grow_with_gap(daPtr, gap_start, gap_len, new_cap)       ail_ca_grow_with_gap(daPtr, gap_start, gap_len, new_cap, (daPtr)->allocator)
#define ail_da_grow_with_gap_a(daPtr, gap_start, gap_len, new_cap, al) ail_da_grow_with_gap(daPtr, gap_start, gap_len, new_cap, al)

#define ail_ca_maybe_grow_with_gap(caPtr, idx, n, al)   ail_arr_maybe_grow_with_gap(ail_field_ptr_of(caPtr, data), ail_field_ptr_of(caPtr, cap), ail_field_ptr_of(caPtr, len), sizeof((caPtr)->data[0]), idx, (idx) + (n), al)
#define ail_da_maybe_grow_with_gap(daPtr, idx, n)       ail_ca_maybe_grow_with_gap(daPtr, idx, n, (daPtr)->allocator)
#define ail_da_maybe_grow_with_gap_a(daPtr, idx, n, al) ail_ca_maybe_grow_with_gap(daPtr, idx, n, al)

#define ail_ca_insert(caPtr, idx, elem, al) do {       \
        ail_ca_maybe_grow_with_gap(caPtr, idx, 1, al); \
        (caPtr)->data[(idx)] = (elem);                 \
    } while(0)
#define ail_da_insert(daPtr, idx, elem)       ail_ca_insert(daPtr, idx, elem, (daPtr)->allocator)
#define ail_da_insert_a(daPtr, idx, elem, al) ail_ca_insert(daPtr, idx, elem, al)

#define ail_ca_insertn(caPtr, idx, elems, n, al)   ail_arr_insertn(ail_field_ptr_of(caPtr, data), ail_field_ptr_of(caPtr, cap), ail_field_ptr_of(caPtr, len), sizeof((caPtr)->data[0]), idx, elems, n, al)
#define ail_da_insertn(daPtr, idx, elems, n)       ail_ca_insertn(daPtr, idx, elems, n, (daPtr)->allocator)
#define ail_da_insertn_a(daPtr, idx, elems, n, al) ail_ca_insertn(daPtr, idx, elems, n, al)

// @TODO: Add ail_da_shrink & ail_da_maybe_shrink

#define ail_ca_rm(caPtr, idx) ail_arr_rm(ail_field_ptr_of(caPtr, data), ail_field_ptr_of(caPtr, len), sizeof((caPtr)->data[0]), idx, 1)
#define ail_da_rm(daPtr, idx) ail_ca_rm(daPtr, idx)

#define ail_ca_rmn(caPtr, idx, n) ail_arr_rm(ail_field_ptr_of(caPtr, data), ail_field_ptr_of(caPtr, len), sizeof((caPtr)->data[0]), idx, n)
#define ail_da_rmn(daPtr, idx) ail_ca_rmn(daPtr, idx)

#define ail_ca_rm_swap(caPtr, idx) ((caPtr)->data[(idx)] = (caPtr)->data[--(caPtr)->len])
#define ail_da_rm_swap(daPtr, idx) ail_ca_rm_swap(daPtr, idx)


AIL_WARN_POP

#endif // _AIL_ARR_H_


#if !defined(AL_NO_ARR_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_ARR_IMPL_GUARD_
#define _AIL_ARR_IMPL_GUARD_
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

void* ail_arr_copy(void *src, u64 size, AIL_Allocator allocator)
{
    u8 *dst = ail_call_alloc(allocator, size);
    ail_mem_copy(dst, src, size);
    return dst;
}

void ail_arr_setn(void *data, u64 el_size, u64 idx, void *elems, u64 n)
{
    ail_mem_copy((u8*)data + (el_size*idx), elems, el_size*n);
}

void ail_arr_resize(void **data, u64 *cap, u64 *len, u64 el_size, u64 new_cap, AIL_Allocator allocator)
{
    *data = ail_call_realloc(allocator, *data, el_size*new_cap);
    *cap  = new_cap;
    if (*len > *cap) *len = *cap;
}

void ail_arr_maybe_grow(void **data, u64 *cap, u64 *len, u64 el_size, u64 n, AIL_Allocator allocator)
{
    if (*len + n > *cap) {
        ail_arr_resize(data, cap, len, el_size, ail_max(2*(*cap), *len + n), allocator);
    }
}

void ail_arr_maybe_pushn(void **data, u64 cap, u64 *len, u64 el_size, void *elems, u64 n)
{
    n = ail_min(n, cap - *len);
    ail_mem_copy((u8*)*data + el_size*(*len), elems, el_size*n);
    *len += n;
}

void ail_arr_pushn(void **data, u64 *cap, u64 *len, u64 el_size, void *elems, u64 n, AIL_Allocator allocator)
{
    ail_arr_maybe_grow(data, cap, len, el_size, n, allocator);
    ail_mem_copy((u8*)*data + *len, elems, el_size * n);
    *len += n;
}

void ail_arr_push_right(void *data, u64 *len, u64 el_size, u64 to_idx, u64 from_idx)
{
    u64 n = to_idx - from_idx;
    ail_mem_copy((u8*)data + el_size*to_idx, (u8*)data + el_size*from_idx, el_size*(*len - from_idx));
    *len += n;
}

void ail_arr_grow_with_gap(void **data, u64 *cap, u64 *len, u64 el_size, u64 new_cap, u64 gap_start, u64 gap_len, AIL_Allocator allocator)
{
    ail_arr_resize(data, cap, len, el_size, new_cap, allocator);
    ail_arr_push_right(*data, len, el_size, gap_start + gap_len, gap_start);
}

void ail_arr_maybe_grow_with_gap(void **data, u64 *cap, u64 *len, u64 el_size, u64 gap_start, u64 gap_len, AIL_Allocator allocator)
{
    if (*len + gap_len > *cap) {
        ail_arr_grow_with_gap(data, cap, len, el_size, ail_max(*len + gap_len, 2*(*cap)), gap_start, gap_len, allocator);
    } else {
        ail_arr_push_right(*data, len, el_size, gap_start + gap_len, gap_start);
    }
}

void ail_arr_insertn(void **data, u64 *cap, u64 *len, u64 el_size, u64 idx, void *elems, u64 n, AIL_Allocator al)
{
    ail_arr_maybe_grow_with_gap(data, cap, len, el_size, idx, n, al);
    ail_mem_copy((u8*)*data + el_size*idx, elems, el_size*n);
}

void ail_arr_rm(void *data, u64 *len, u64 el_size, u64 idx, u64 n)
{
    u64 j = idx + n;
    ail_mem_copy((u8*)data + el_size*idx, (u8*)data + el_size*j, el_size*(*len - j));
    *len -= n;
}

AIL_WARN_POP
#endif // _AIL_ARR_IMPL_GUARD_
#endif // AL_NO_ARR_IMPL
