// General Hashmap implementation
//
// You do not need to define `AIL_HM_IMPL`, as all implementation is via macros, that are defined in the header section

#ifndef AIL_HM_H_
#define AIL_HM_H_

#ifndef AIL_TYPES_IMPL
#define AIL_TYPES_IMPL
#endif // AIL_TYPES_IMPL
#include "ail.h"

#if !defined(AIL_HM_CALLOC) && !defined(AIL_HM_FREE)
#if  defined(AIL_CALLOC)    && defined(AIL_MALLOC)     &&  defined(AIL_FREE)
#define AIL_HM_CALLOC AIL_CALLOC
#define AIL_HM_FREE   AIL_FREE
#else
#include <stdlib.h>
#define AIL_HM_CALLOC(len, elSize) calloc(len, elSize)
#define AIL_HM_FREE(ptr)           free(ptr)
#endif
#elif !defined(AIL_HM_CALLOC) || !defined(AIL_HM_FREE)
#error "You must define AIL_HM_CALLOC and AIL_HM_FREE, or neither."
#endif

#ifndef AIL_HM_MEMCPY
#ifdef  AIL_MEMCMPY
#define AIL_HM_MEMCPY AIL_MEMCMPY
#else
#include <string.h>
#define AIL_HM_MEMCPY(dst, src, n) memcpy(dst, src, n)
#endif
#endif

#ifndef AIL_HM_INIT_CAP
#define AIL_HM_INIT_CAP 256
#endif // AIL_HM_INIT_CAP

// @Note: Load factor should be specified per Hashmap maybe?
// @Note: Load factor is given in percent from 0 to 100
#ifndef AIL_HM_LOAD_FACTOR
#define AIL_HM_LOAD_FACTOR 70
#endif // AIL_HM_LOAD_FACTOR

typedef enum {
    AIL_HM_EMPTY         = 0, // 00
    AIL_HM_ONCE_OCCUPIED = 1, // 01
    AIL_HM_CUR_OCCUPIED  = 2, // 10
    AIL_HM_OCCUPIED      = 3, // 11
} AIL_HM_OCCUPATION;

// @Note on Terminology: Box refers to an individual element in the list of elements in the hashmap
#define AIL_HM_KEY_VAL(K, V) AIL_HM_KEY_VAL_##K##_##V
#define AIL_HM_BOX(K, V) AIL_HM_BOX_##K##_##V
#define AIL_HM(K, V)     AIL_HM_##K##_##V
#define AIL_HM_INIT(K, V)                 \
    typedef struct AIL_HM_KEY_VAL(K, V) { \
        K key;                            \
        V val;                            \
    } AIL_HM_KEY_VAL(K, V);               \
    typedef struct AIL_HM_BOX(K, V) {     \
        K key;                            \
        V val;                            \
        AIL_HM_OCCUPATION occupied;       \
    } AIL_HM_BOX(K, V);                   \
    typedef struct AIL_HM(K, V) {         \
        AIL_HM_BOX(K, V) *data;           \
        u32 len;                          \
        u32 once_filled;                  \
        u32 cap;                          \
        u32(*hash)(K);                    \
        bool(*eq)(K, K);                  \
    } AIL_HM(K, V)

#define ail_hm_new(K, V, hash, eq)          ail_hm_with_cap(K, V, AIL_HM_INIT_CAP, hash, eq)
#define ail_hm_with_cap(K, V, c, hash, eq)  (AIL_HM(K, V)) { AIL_HM_CALLOC(ail_hm_next_u32_2power(c), sizeof(AIL_HM(K, V))), 0, 0, ail_hm_next_u32_2power(c), (hash), (eq) }
#define ail_hm_da_new_empty(K, V, hash, eq) (AIL_HM(K, V)) { 0, 0, 0, 0, (hash), (eq) }
#define ail_hm_free(hm)                     AIL_HM_FREE(hm.data)
// Bug: Should be able to just change the probing strategy, but test fails
#define ail_hm_probe_incr(idx, hash, cap)   idx = ((idx) + 1 + ((hash) % ((cap) - 1))) % (cap) // This only works when capacity is a power of 2

// @Decide: Should we round the capacity up to the next power of 2? Alternatively we might get issues with our probing-strategy...
// @Bug: Rehashing apparently fails...
#define ail_hm_grow(hmPtr, newCap) do {                                                                                          \
        u32 _ail_hm_grow_new_cap_    = ail_hm_next_u32_2power(newCap);                                                                  \
        u32 _ail_hm_grow_occ_offset_ = ((char *)&((hmPtr)->data[0].occupied)) - ((char *)&((hmPtr)->data[0]));                   \
        void *_ail_hm_grow_new_ptr_  = AIL_HM_CALLOC(_ail_hm_grow_new_cap_, sizeof(*((hmPtr)->data)));                           \
        for (u32 _ail_hm_grow_i_ = 0; _ail_hm_grow_i_ < (hmPtr)->cap; _ail_hm_grow_i_++) {                                       \
            if ((hmPtr)->data[_ail_hm_grow_i_].occupied == AIL_HM_CUR_OCCUPIED) {                                                \
                u32 _ail_hm_grow_hash_ = (hmPtr)->hash((hmPtr)->data[_ail_hm_grow_i_].key);                                      \
                u32 _ail_hm_grow_j_    = _ail_hm_grow_hash_ % _ail_hm_grow_new_cap_;                                             \
                char *_ail_hm_grow_tmp_ptr_;                                                                                     \
                AIL_HM_OCCUPATION _ail_hm_grow_occ_;                                                                             \
                do {                                                                                                             \
                    _ail_hm_grow_tmp_ptr_ = &(((char *)_ail_hm_grow_new_ptr_)[_ail_hm_grow_j_ * sizeof(*((hmPtr)->data))]);      \
                    _ail_hm_grow_occ_     = *((AIL_HM_OCCUPATION *)&(_ail_hm_grow_tmp_ptr_[_ail_hm_grow_occ_offset_]));          \
                    ail_hm_probe_incr(_ail_hm_grow_j_, _ail_hm_grow_hash_, _ail_hm_grow_new_cap_);                               \
                } while(_ail_hm_grow_occ_ == AIL_HM_CUR_OCCUPIED);                                                               \
                AIL_HM_MEMCPY((void *)_ail_hm_grow_tmp_ptr_, (void *)&(hmPtr)->data[_ail_hm_grow_i_], sizeof(*((hmPtr)->data))); \
            }                                                                                                                    \
        }                                                                                                                        \
        AIL_HM_FREE((hmPtr)->data);                                                                                              \
        (hmPtr)->cap  = _ail_hm_grow_new_cap_;                                                                                   \
        (hmPtr)->data = _ail_hm_grow_new_ptr_;                                                                                   \
    } while(0)

#define ail_hm_maybe_grow(hmPtr, toAdd) do {                                         \
        if (((hmPtr)->len + (toAdd))*100 >= (hmPtr)->cap*AIL_HM_LOAD_FACTOR)         \
            ail_hm_grow(hmPtr, ((hmPtr)->len + (toAdd) + 1)*100/AIL_HM_LOAD_FACTOR); \
    } while(0)

#define ail_hm_get_idx(hmPtr, k, outIdx, outFound) do {                                               \
        outFound               = false;                                                               \
        u32 _ail_hm_get_hash_  = (hmPtr)->hash((k));                                                  \
        u32 _ail_hm_get_idx_   = _ail_hm_get_hash_ % (hmPtr)->cap;                                    \
        for (u32 _ail_hm_get_count_ = 0; _ail_hm_get_count_ < (hmPtr)->len &&                         \
            ((hmPtr)->data[_ail_hm_get_idx_].occupied & AIL_HM_OCCUPIED) > 0; _ail_hm_get_count_++) { \
            if (((hmPtr))->eq(((hmPtr))->data[_ail_hm_get_idx_].key, (k))) {                          \
                (outIdx)   = _ail_hm_get_idx_;                                                        \
                (outFound) = true;                                                                    \
                break;                                                                                \
            }                                                                                         \
            ail_hm_probe_incr(_ail_hm_get_idx_, _ail_hm_get_hash_, (hmPtr)->cap);                     \
        }                                                                                             \
    } while(0)

#define ail_hm_get_ptr(hmPtr, k, outPtr) do {                                            \
        bool _ail_hm_get_ptr_found_;                                                     \
        u32  _ail_hm_get_ptr_idx_;                                                       \
        ail_hm_get_idx(hmPtr, k, _ail_hm_get_ptr_idx_, _ail_hm_get_ptr_found_);          \
        if (_ail_hm_get_ptr_found_) outPtr = &((hmPtr)->data[_ail_hm_get_ptr_idx_].val); \
        else outPtr = 0;                                                                 \
    } while(0)

#define ail_hm_get_val(hmPtr, k, outVal, outFound) do {                   \
        u32  _ail_hm_get_val_idx_;                                        \
        ail_hm_get_idx(hmPtr, k, _ail_hm_get_val_idx_, outFound);         \
        if ((outFound)) outVal = (hmPtr)->data[_ail_hm_get_val_idx_].val; \
    } while(0)

#define ail_hm_put(hmPtr, k, v) do {                                                                                                     \
        if ((hmPtr)->cap == 0) ail_hm_grow(hmPtr, AIL_HM_INIT_CAP);                                                                      \
        u32 _ail_hm_put_hash_ = (hmPtr)->hash((k));                                                                                      \
        u32 _ail_hm_put_idx_  = _ail_hm_put_hash_ % (hmPtr)->cap;                                                                        \
        u32 _ail_hm_put_once_filled_idx_;                                                                                                \
        u32 _ail_hm_put_found_once_filled_idx_ = false;                                                                                  \
        for (u32 _ail_hm_put_count_ = 0; _ail_hm_put_count_ < (hmPtr)->len &&                                                            \
            ((hmPtr)->data[_ail_hm_put_idx_].occupied & AIL_HM_OCCUPIED) > 0; _ail_hm_put_count_++) {                                    \
            if ((hmPtr)->eq((hmPtr)->data[_ail_hm_put_idx_].key, (k))) goto _ail_hm_put_set_val_;                                        \
            if (AIL_UNLIKELY(!_ail_hm_put_found_once_filled_idx_ && (hmPtr)->data[_ail_hm_put_idx_].occupied == AIL_HM_ONCE_OCCUPIED)) { \
                _ail_hm_put_once_filled_idx_       = _ail_hm_put_idx_;                                                                   \
                _ail_hm_put_found_once_filled_idx_ = true;                                                                               \
            }                                                                                                                            \
            ail_hm_probe_incr(_ail_hm_put_idx_, _ail_hm_put_hash_, ((hmPtr))->cap);                                                      \
        }                                                                                                                                \
        if (_ail_hm_put_found_once_filled_idx_) _ail_hm_put_idx_ = _ail_hm_put_once_filled_idx_;                                         \
    _ail_hm_put_set_val_:                                                                                                                \
        if ((hmPtr)->data[_ail_hm_put_idx_].occupied != AIL_HM_CUR_OCCUPIED) {                                                           \
            (hmPtr)->len++;                                                                                                              \
            if ((hmPtr)->data[_ail_hm_put_idx_].occupied != AIL_HM_ONCE_OCCUPIED) (hmPtr)->once_filled++;                                \
        }                                                                                                                                \
        (hmPtr)->data[_ail_hm_put_idx_].key = (k);                                                                                       \
        (hmPtr)->data[_ail_hm_put_idx_].val = (v);                                                                                       \
        (hmPtr)->data[_ail_hm_put_idx_].occupied = AIL_HM_CUR_OCCUPIED;                                                                  \
        ail_hm_maybe_grow(hmPtr, 1);                                                                                                     \
    } while(0)

// @TODO
#define ail_hm_rm(hmPtr, k) do { \
    } while(0)

u32 ail_hm_next_u32_2power(u32 x)
{
    AIL_NEXT_2POWER(x, x);
    return x;
}

#endif // AIL_HM_H_