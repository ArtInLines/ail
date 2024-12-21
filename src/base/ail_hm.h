/*
*** General Hashmap implementation ***
*
* @TODO: Documentation
*
* @TODO: Improve Hashmap
* see for example Jai's hashmap implementation here: https://pastebin.com/xMUQXshn
*/


#ifndef _AIL_HM_H_
#define _AIL_HM_H_

#include "ail_base.h"
#include "ail_base_math.h"
#include "ail_alloc.h"

AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

#ifndef AIL_HM_INIT_CAP
#define AIL_HM_INIT_CAP 256
#endif // AIL_HM_INIT_CAP

// @Note: Load factor should be specified per Hashmap maybe?
// @Note: Load factor is given in percent from 0 to 100
#ifndef AIL_HM_LOAD_FACTOR
#define AIL_HM_LOAD_FACTOR 80
#endif // AIL_HM_LOAD_FACTOR

typedef enum {
    AIL_HM_EMPTY         = 0, // 00
    AIL_HM_ONCE_OCCUPIED = 1, // 01
    AIL_HM_CUR_OCCUPIED  = 2, // 10
    AIL_HM_OCCUPIED      = 3, // 11
} AIL_HM_OCCUPATION;

internal u32 ail_hm_next_u32_2power(u32 x);

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
        AIL_Allocator *allocator;         \
    } AIL_HM(K, V)

#define ail_hm_from_parts(K, V, data, len, once_filled, cap, hashf, eqf, alPtr) (AIL_HM(K, V)) { (data), (len), (once_filled), (cap), (hashf), (eqf), (alPtr) }
#define ail_hm_new_with_alloc(K, V, c, hashf, eqf, alPtr) (AIL_HM(K, V)) { .data = ail_call_calloc((*alPtr), ail_hm_next_u32_2power(c), sizeof(AIL_HM(K, V))), .len = 0, .once_filled = 0, .cap = ail_hm_next_u32_2power(c), .hash = (hashf), .eq = (eqf), .allocator = (alPtr) }
#define ail_hm_new_with_cap(K, V, c, hashf, eqf) (AIL_HM(K, V)) { .data = ail_call_calloc(ail_default_allocator, ail_hm_next_u32_2power(c), sizeof(AIL_HM(K, V))), .len = 0, .cap = ail_hm_next_u32_2power(c), .hash = (hashf), .eq = (eqf), .allocator = &ail_default_allocator }
#define ail_hm_new(K, V, hashf, eqf) ail_hm_new_with_cap(K, V, AIL_HM_INIT_CAP, hashf, eqf)
#define ail_hm_new_empty(K, V, hashf, eqf) (AIL_HM(K, V)) { .data = NULL, .len = 0, .once_filled = 0, .cap = 0, .hash = (hashf), .eq = (eqf), .allocator = &ail_default_allocator }
#define ail_hm_free(hmPtr) do { ail_call_free((*(hmPtr)->allocator), (hmPtr)->data); (hmPtr)->data = NULL; (hmPtr)->len = 0; (hmPtr)->cap = 0; } while(0)

// @TODO: Current probing stategy is simple and potentially inefficient. More efficient method (as used by jblow) requires that capacity is a power of 2 though
#if 0
#define ail_hm_probe_incr(idx, hash, cap) idx = ((idx) + 1 + ((hash) % ((cap) - 1))) % (cap) // This only works when capacity is a power of 2
#else
#define ail_hm_probe_incr(idx, hash, cap) idx = ((idx) + 1) % (cap)
#endif

// @Decide: Should we round the capacity up to the next power of 2? Alternatively we might get issues with our probing-strategy...
#define ail_hm_grow(hmPtr, newCap) do {                                                                                           \
        u32 _ail_hm_grow_new_cap_    =  (newCap); /* ail_hm_next_u32_2power(newCap); */                                           \
        u32 _ail_hm_grow_occ_offset_ = ail_offset_of(&(hmPtr)->data[0], occupied);                                                 \
        void *_ail_hm_grow_new_ptr_  = ail_call_calloc((*(hmPtr)->allocator), _ail_hm_grow_new_cap_, sizeof(*((hmPtr)->data)));   \
        for (u32 _ail_hm_grow_i_ = 0; _ail_hm_grow_i_ < (hmPtr)->cap; _ail_hm_grow_i_++) {                                        \
            if ((hmPtr)->data[_ail_hm_grow_i_].occupied == AIL_HM_CUR_OCCUPIED) {                                                 \
                u32 _ail_hm_grow_hash_ = (hmPtr)->hash((hmPtr)->data[_ail_hm_grow_i_].key);                                       \
                u32 _ail_hm_grow_j_    = _ail_hm_grow_hash_ % _ail_hm_grow_new_cap_;                                              \
                char *_ail_hm_grow_tmp_ptr_ = &(((char *)_ail_hm_grow_new_ptr_)[_ail_hm_grow_j_ * sizeof(*((hmPtr)->data))]);     \
                AIL_HM_OCCUPATION _ail_hm_grow_occ_ = *((AIL_HM_OCCUPATION *)&(_ail_hm_grow_tmp_ptr_[_ail_hm_grow_occ_offset_])); \
                while (_ail_hm_grow_occ_ == AIL_HM_CUR_OCCUPIED) {                                                                \
                    ail_hm_probe_incr(_ail_hm_grow_j_, _ail_hm_grow_hash_, _ail_hm_grow_new_cap_);                                \
                    _ail_hm_grow_tmp_ptr_ = &(((char *)_ail_hm_grow_new_ptr_)[_ail_hm_grow_j_ * sizeof(*((hmPtr)->data))]);       \
                    _ail_hm_grow_occ_     = *((AIL_HM_OCCUPATION *)&(_ail_hm_grow_tmp_ptr_[_ail_hm_grow_occ_offset_]));           \
                }                                                                                                                 \
                ail_mem_copy(_ail_hm_grow_tmp_ptr_, &(hmPtr)->data[_ail_hm_grow_i_], sizeof(*((hmPtr)->data)));  \
            }                                                                                                                     \
        }                                                                                                                         \
        if ((hmPtr)->data) ail_call_free((*(hmPtr)->allocator), (hmPtr)->data);                                                   \
        (hmPtr)->cap  = _ail_hm_grow_new_cap_;                                                                                    \
        (hmPtr)->data = _ail_hm_grow_new_ptr_;                                                                                    \
    } while(0)

#define ail_hm_maybe_grow(hmPtr, toAdd) do {                                               \
        if (((hmPtr)->len + (toAdd))*100 >= (hmPtr)->cap*AIL_HM_LOAD_FACTOR) {             \
            ail_hm_grow(hmPtr, 2*((hmPtr)->cap + 1));                                      \
            /* ail_hm_grow(hmPtr, ((hmPtr)->len + (toAdd) + 1)*100/AIL_HM_LOAD_FACTOR); */ \
        }                                                                                  \
    } while(0)

#define ail_hm_get_idx(hmPtr, k, outIdx, outFound) do {                                               \
            (outFound)         = false;                                                               \
        if ((hmPtr)->cap == 0) break; /* Necessary, bc mod 0 is undefined */                          \
        u32 _ail_hm_get_hash_  = (hmPtr)->hash((k));                                                  \
        u32 _ail_hm_get_idx_   = _ail_hm_get_hash_ % (hmPtr)->cap;                                    \
        for (u32 _ail_hm_get_count_ = 0; _ail_hm_get_count_ < (hmPtr)->len; _ail_hm_get_count_++) {   \
            if (((hmPtr)->data[_ail_hm_get_idx_].occupied & AIL_HM_OCCUPIED) == 0) break;             \
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
        ail_hm_maybe_grow(hmPtr, 1);                                                                                                     \
        u32 _ail_hm_put_hash_ = (hmPtr)->hash((k));                                                                                      \
        u32 _ail_hm_put_idx_  = _ail_hm_put_hash_ % (hmPtr)->cap;                                                                        \
        u32 _ail_hm_put_once_filled_idx_;                                                                                                \
        u32 _ail_hm_put_found_once_filled_idx_ = false;                                                                                  \
        for (u32 _ail_hm_put_count_ = 0; _ail_hm_put_count_ < (hmPtr)->len &&                                                            \
            ((hmPtr)->data[_ail_hm_put_idx_].occupied & AIL_HM_OCCUPIED) > 0; _ail_hm_put_count_++) {                                    \
            if ((hmPtr)->eq((hmPtr)->data[_ail_hm_put_idx_].key, (k))) {                                                                 \
                _ail_hm_put_found_once_filled_idx_ = false;                                                                              \
                break;                                                                                                                   \
            }                                                                                                                            \
            if (AIL_UNLIKELY(!_ail_hm_put_found_once_filled_idx_ && (hmPtr)->data[_ail_hm_put_idx_].occupied == AIL_HM_ONCE_OCCUPIED)) { \
                _ail_hm_put_once_filled_idx_       = _ail_hm_put_idx_;                                                                   \
                _ail_hm_put_found_once_filled_idx_ = true;                                                                               \
            }                                                                                                                            \
            ail_hm_probe_incr(_ail_hm_put_idx_, _ail_hm_put_hash_, ((hmPtr))->cap);                                                      \
        }                                                                                                                                \
        if (_ail_hm_put_found_once_filled_idx_) _ail_hm_put_idx_ = _ail_hm_put_once_filled_idx_;                                         \
        if ((hmPtr)->data[_ail_hm_put_idx_].occupied != AIL_HM_CUR_OCCUPIED) {                                                           \
            (hmPtr)->len++;                                                                                                              \
            if ((hmPtr)->data[_ail_hm_put_idx_].occupied != AIL_HM_ONCE_OCCUPIED) (hmPtr)->once_filled++;                                \
        }                                                                                                                                \
        (hmPtr)->data[_ail_hm_put_idx_].key = (k);                                                                                       \
        (hmPtr)->data[_ail_hm_put_idx_].val = (v);                                                                                       \
        (hmPtr)->data[_ail_hm_put_idx_].occupied = AIL_HM_CUR_OCCUPIED;                                                                  \
    } while(0)

// @TODO
#define ail_hm_rm(hmPtr, k) do { \
    } while(0)


AIL_WARN_POP
#endif // _AIL_HM_H_


#if !defined(AIL_NO_HM_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef AIL_HM_IMPL_GUARD
#define AIL_HM_IMPL_GUARD
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

u32 ail_hm_next_u32_2power(u32 x)
{
    ail_next_2power_pos(x, x);
    return x;
}

AIL_WARN_POP
#endif // AIL_HM_IMPL_GUARD
#endif // AIL_NO_HM_IMPL
