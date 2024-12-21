/*
*** Arrays ***
* @TODO: Documentation
*
*/

#ifndef _AIL_ARR_H_
#define _AIL_ARR_H_

#include "./ail_base.h"
// #include "./ail_alloc.h" is included at the end of the file, so that type templates are defined first, as ail_mem might want to make use of them

#ifndef AIL_DA_INIT_CAP
#   define AIL_DA_INIT_CAP 256
#endif

#define AIL_SA_INIT(T) typedef struct AIL_SA_##T { T *data; u64 len; } AIL_SA_##T
#define AIL_SA(T) AIL_SA_##T
#define AIL_CA_INIT(T) typedef struct AIL_CA_##T { T *data; u64 len; u64 cap; } AIL_CA_##T
#define AIL_CA(T) AIL_CA_##T
#define AIL_DA_INIT(T) typedef struct AIL_DA_##T { T *data; u64 len; u64 cap; AIL_Allocator allocator; } AIL_DA_##T
#define AIL_DA(T) AIL_DA_##T
AIL_DA_INIT(u8);    AIL_SA_INIT(u8);    AIL_CA_INIT(u8);
AIL_DA_INIT(u16);   AIL_SA_INIT(u16);   AIL_CA_INIT(u16);
AIL_DA_INIT(u32);   AIL_SA_INIT(u32);   AIL_CA_INIT(u32);
AIL_DA_INIT(u64);   AIL_SA_INIT(u64);   AIL_CA_INIT(u64);
AIL_DA_INIT(i8);    AIL_SA_INIT(i8);    AIL_CA_INIT(i8);
AIL_DA_INIT(i16);   AIL_SA_INIT(i16);   AIL_CA_INIT(i16);
AIL_DA_INIT(i32);   AIL_SA_INIT(i32);   AIL_CA_INIT(i32);
AIL_DA_INIT(i64);   AIL_SA_INIT(i64);   AIL_CA_INIT(i64);
AIL_DA_INIT(f32);   AIL_SA_INIT(f32);   AIL_CA_INIT(f32);
AIL_DA_INIT(f64);   AIL_SA_INIT(f64);   AIL_CA_INIT(f64);
AIL_DA_INIT(pchar); AIL_SA_INIT(pchar); AIL_CA_INIT(pchar);
AIL_DA_INIT(void);  AIL_SA_INIT(void);  AIL_CA_INIT(void);
AIL_DA_INIT(char);  AIL_SA_INIT(char);  AIL_CA_INIT(char);

#define ail_sa_from_parts(d, l) { .data = (d),       .len = (l) }
#define ail_sa_from_arr(arr)    { .data = (arr),     .len = ail_arrlen(arr) }
#define ail_sa_from_struct(st)  { .data = (st).data, .len = (st).len }
#define ail_sa_from_parts_t(T, d, l) (AIL_SA(T))ail_sa_from_parts(d, l)
#define ail_sa_from_arr_t(T, arr)    (AIL_SA(T))ail_sa_from_arr(arr)
#define ail_sa_from_struct_t(T, st)  (AIL_SA(T))ail_sa_from_struct(st)

#define ail_ca_from_parts(d, l) { .data = (d),       .len = (l) }
#define ail_ca_from_arr(arr)    { .data = (arr),     .len = ail_arrlen(arr) }
#define ail_ca_from_struct(st)  { .data = (st).data, .len = (st).len }
#define ail_ca_from_parts_t(T, d, l) (AIL_CA(T))ail_ca_from_parts(d, l)
#define ail_ca_from_arr_t(T, arr)    (AIL_CA(T))ail_ca_from_arr(arr)
#define ail_ca_from_struct_t(T, st)  (AIL_CA(T))ail_ca_from_struct(st)

#define ail_da_from_parts(d, l, c, al)  { .data = (d), .len = (l), .cap = (c), .allocator = (al) }
#define ail_da_from_struct(st)          { .data = (st).data, .len = (st).len, .cap = (st).cap, .allocator = (st).allocator }
#define ail_da_new_with_alloc(T, c, al) { .data = ail_call_alloc ((al), sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = (al) }
#define ail_da_new_zero_alloc(T, c, al) { .data = ail_call_calloc((al), sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = (al) }
#define ail_da_new_with_cap(T, c)       { .data = ail_call_alloc(ail_default_allocator, sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = ail_default_allocator }
#define ail_da_new_zero_init(T, c)      { .data = ail_call_calloc(ail_default_allocator, sizeof(T)*(c)), .len = 0, .cap = (c), .allocator = ail_default_allocator }
#define ail_da_new(T)                   { .data = ail_call_alloc(ail_default_allocator, sizeof(T) * AIL_DA_INIT_CAP), .len = 0, .cap = AIL_DA_INIT_CAP, .allocator = ail_default_allocator }
#define ail_da_new_empty(T)             { .data = NULL, .len = 0, .cap = 0, .allocator = ail_default_allocator }
#define ail_da_new_zero_init_t(T, c)        (AIL_DA(T))ail_da_new_zero_init()
#define ail_da_new_empty_t(T)               (AIL_DA(T))ail_da_new_empty(T)
#define ail_da_new_t(T)                     (AIL_DA(T))ail_da_new(T)
#define ail_da_new_with_cap_t(T, c)         (AIL_DA(T))ail_da_new_with_cap(T, c)
#define ail_da_new_zero_alloc_t(T, c, al)   (AIL_DA(T))ail_da_new_zero_alloc(T, c, al)
#define ail_da_new_with_alloc_t(T, c, al)   (AIL_DA(T))ail_da_new_with_alloc(T, c, al)
#define ail_da_from_struct_t(T, st)         (AIL_DA(T))ail_da_from_struct(st)
#define ail_da_from_parts_t(T, d, l, c, al) (AIL_DA(T))ail_da_from_parts(d, l, c, al)

#define ail_sa_free(saPtr, al) do { ail_call_free((al), (saPtr)->data); (saPtr)->data = NULL; (saPtr)->len = 0; } while(0)
#define ail_ca_free(caPtr, al) do { ail_sa_free(caPtr, al); (caPtr)->cap = 0; } while(0)
#define ail_da_free(daPtr) ail_ca_free(daPtr, (daPtr)->allocator)
#define ail_da_free_a(daPtr, al) ail_ca_free(daPtr, al)

#define ail_sa_setn(sa, idx, elems, n) do {                                                \
        for (unsigned int _ail_da_setn_i_ = 0; _ail_da_setn_i_ < (n); _ail_da_setn_i_++) { \
            (sa).data[(idx) + _ail_da_setn_i_] = (elems)[_ail_da_setn_i_];                 \
        }                                                                                  \
    } while(0)
#define ail_ca_setn(ca, idx, elems, n) ail_sa_setn(ca, idx, elems, n)
#define ail_da_setn(da, idx, elems, n) ail_sa_setn(da, idx, elems, n)

#define ail_ca_resize(caPtr, newCap, al) ail_da_resize_a(caPtr, newCap, al)
#define ail_da_resize(daPtr, newCap)     ail_da_resize_a(daPtr, newCap, (daPtr)->allocator)
#define ail_da_resize_a(daPtr, newCap, al) do {                                                   \
        (daPtr)->data = ail_call_realloc((al), (daPtr)->data, sizeof((daPtr)->data[0])*(newCap)); \
        (daPtr)->cap  = (newCap);                                                                 \
        if ((daPtr)->len > (daPtr)->cap) (daPtr)->len = (daPtr)->cap;                             \
    } while(0)

#define ail_ca_maybe_grow(caPtr, n, al) ail_da_maybe_grow_a(caPtr, n, al)
#define ail_da_maybe_grow(daPtr, n)     ail_da_maybe_grow_a(daPtr, n, (daPtr)->allocator)
#define ail_da_maybe_grow_a(daPtr, n, al) do {                                         \
        if ((daPtr)->len + (n) > (daPtr)->cap)                                         \
            ail_da_resize_a(daPtr, ail_max(2*(daPtr)->cap, (daPtr)->cap + (n)), (al)); \
    } while(0)


#define ail_ca_maybe_push(caPtr, elem) do {                                 \
    if ((caPtr)->len < (caPtr)->cap) (caPtr)->data[(caPtr)->len++] = (elem) \
    } while(0)
#define ail_da_maybe_push(daPtr, elem) ail_ca_maybe_push(daPtr, elem)

#define ail_ca_push(caPtr, elem, al) ail_da_push_a(daPtr, elem, al)
#define ail_da_push(daPtr, elem)     ail_da_push_a(daPtr, elem, (daPtr)->allocator)
#define ail_da_push_a(daPtr, elem, al) do {     \
        ail_da_maybe_grow_a(daPtr, 1, (al));    \
        (daPtr)->data[(daPtr)->len++] = (elem); \
    } while(0)

#define ail_ca_maybe_pushn(caPtr, elems, n) do {                                                                                \
        ail_mem_copy((caPtr)->data + (caPtr)->len, (elems), sizeof((caPtr)->data[0]) * AIL_MIN(n, (caPtr)->cap - (caPtr)->len)) \
        (caPtr)->len += AIL_MIN(n, (caPtr)->cap - (caPtr)->len);                                                                \
    } while(0)
#define ail_da_maybe_pushn(daPtr, elems, n) ail_ca_maybe_pushn(daPtr, elems, n)

#define ail_ca_pushn(caPtr, elems, n, al) ail_da_pushn_a(caPtr, elems, n, al)
#define ail_da_pushn(daPtr, elems, n)     ail_da_pushn_a(daPtr, elems, n, (daPtr)->allocator)
#define ail_da_pushn_a(daPtr, elems, n, al) do {                                           \
        ail_da_maybe_grow_a(daPtr, n, (al));                                               \
        ail_mem_copy((daPtr)->data + (daPtr)->len, (elems), sizeof((daPtr)->data[0])*(n)); \
        (daPtr)->len += (n);                                                               \
    } while(0)

#define ail_ca_grow_with_gap(caPtr, gapStart, gapLen, newCap, al) do {                                                                           \
        (caPtr)->data = ail_call_realloc((al), (caPtr)->data, sizeof((caPtr)->data[0])*(newCap));                                                               \
        (caPtr)->cap  = (newCap);                                                                                                                \
        ail_assert((caPtr)->data != NULL);                                                                                                       \
        ail_mem_copy(&(caPtr)->data[((gapStart) + (gapLen))], &(caPtr)->data[(gapStart)], sizeof((caPtr)->data[0])*((caPtr)->len - (gapStart))); \
        (caPtr)->len += (gapLen);                                                                                                                \
    } while(0)
#define ail_da_grow_with_gap(daPtr, gapStart, gapLen, newCap)       ail_ca_grow_with_gap(daPtr, gapStart, gapLen, newCap, (daPtr)->allocator)
#define ail_da_grow_with_gap_a(daPtr, gapStart, gapLen, newCap, al) ail_da_grow_with_gap(daPtr, gapStart, gapLen, newCap, al)

#define ail_ca_maybe_grow_with_gap(caPtr, idx, n, al) do {                                                    \
        if ((caPtr)->len + (n) > (caPtr)->cap) {                                                              \
            ail_ca_grow_with_gap((caPtr), (idx), (n), ail_max(2*(caPtr)->cap, (caPtr)->cap + (n)), (al));     \
        } else {                                                                                              \
            for (unsigned int _ail_da_i_ = 1; _ail_da_i_ <= ((caPtr)->len - (idx)); _ail_da_i_++) {           \
                (caPtr)->data[((caPtr)->len + (n)) - _ail_da_i_] =  (caPtr)->data[(caPtr)->len - _ail_da_i_]; \
            }                                                                                                 \
            (caPtr)->len += (n);                                                                              \
        }                                                                                                     \
    } while(0)
#define ail_da_maybe_grow_with_gap(daPtr, idx, n)       ail_ca_maybe_grow_with_gap(daPtr, idx, n, (daPtr)->allocator)
#define ail_da_maybe_grow_with_gap_a(daPtr, idx, n, al) ail_ca_maybe_grow_with_gap(daPtr, idx, n, al)

#define ail_ca_insert(caPtr, idx, elem, al) do {       \
        ail_ca_maybe_grow_with_gap(caPtr, idx, 1, al); \
        (caPtr)->data[(idx)] = (elem);                 \
    } while(0)
#define ail_da_insert(daPtr, idx, elem)       ail_ca_insert(daPtr, idx, elem, (daPtr)->allocator)
#define ail_da_insert_a(daPtr, idx, elem, al) ail_ca_insert(daPtr, idx, elem, al)

#define ail_ca_insertn(caPtr, idx, elems, n, al) do {  \
        ail_ca_maybe_grow_with_gap(caPtr, idx, n, al); \
        ail_ca_setn((*(caPtr)), idx, elems, n);             \
    } while(0)
#define ail_da_insertn(daPtr, idx, elems, n)       ail_ca_insertn(daPtr, idx, elems, n, (daPtr)->allocator)
#define ail_da_insertn_a(daPtr, idx, elems, n, al) ail_ca_insertn(daPtr, idx, elems, n, al)

// @TODO: Add ail_da_shrink & ail_da_maybe_shrink

#define ail_ca_rm(caPtr, idx) do {                                                            \
        (caPtr)->len--;                                                                       \
        ail_mem_copy(&(caPtr)->data[(idx)], &(caPtr)->data[(idx) + 1], sizeof((caPtr)->data[0])*((caPtr)->len - (idx))); \
    } while(0)
#define ail_da_rm(daPtr, idx) ail_ca_rm(daPtr, idx)

#define ail_ca_rmn(caPtr, idx, n) do {                                                        \
        (caPtr)->len -= (n);                                                                  \
        ail_mem_copy(&(caPtr)->data[(idx)], &(caPtr)->data[(idx) + 1], sizeof((caPtr)->data[0])*((caPtr)->len - (idx))); \
    } while(0)
#define ail_da_rmn(daPtr, idx) ail_ca_rmn(daPtr, idx)

#define ail_ca_rm_swap(caPtr, idx) (caPtr)->data[(idx)] = (caPtr)->data[--(caPtr)->len]
#define ail_da_rm_swap(daPtr, idx) ail_ca_rm_swap(daPtr, idx)


#include "ail_alloc.h" // @Note: Must be included at the end, to include definitions of type templates first

#endif // _AIL_ARR_H_
