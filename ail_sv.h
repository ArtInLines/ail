#ifndef AIL_SV_H_
#define AIL_SV_H_

#define AIL_ALL_IMPL
#include "ail.h"

#if !defined(AIL_SV_MALLOC)
#if  defined(AIL_MALLOC)
#define AIL_SV_MALLOC AIL_MALLOC
#else
#include <stdlib.h>
#define AIL_SV_MALLOC(size) malloc(size)
#endif
#elif !defined(AIL_SV_MALLOC) || !defined(AIL_HM_FREE)
#error "You must define AIL_SV_MALLOC and AIL_HM_FREE, or neither."
#endif

#ifndef AIL_SV_MEMCPY
#ifdef  AIL_MEMCMPY
#define AIL_SV_MEMCPY AIL_MEMCMPY
#else
#include <string.h>
#define AIL_SV_MEMCPY(dst, src, n) memcpy(dst, src, n)
#endif
#endif

typedef struct AIL_SV {
    const char *str;
    u64 len;
} AIL_SV;
AIL_DA_INIT(AIL_SV);

AIL_SV ail_sv_from_parts(const char *str, u64 len);
AIL_SV ail_sv_from_da(AIL_DA(char) str);
AIL_SV ail_sv_from_cstr(const char *str);
char* ail_sv_copy_to_cstr(AIL_SV sv);
bool ail_sv_eq(AIL_SV a, AIL_SV b);
bool ail_sv_starts_with(AIL_SV str, AIL_SV prefix);
bool ail_sv_starts_with_char(AIL_SV str, char prefix);
bool ail_sv_ends_with(AIL_SV str, AIL_SV suffix);
bool ail_sv_ends_with_char(AIL_SV str, char suffix);
AIL_DA(AIL_SV) ail_sv_split(AIL_SV sv, AIL_SV split_by, bool ignore_empty);
AIL_DA(AIL_SV) ail_sv_split_lines(AIL_SV sv, bool ignore_empty);
AIL_SV ail_sv_offset(AIL_SV sv, u64 offset);
AIL_SV ail_sv_trim(AIL_SV sv);
AIL_SV ail_sv_join(AIL_SV *list, u64 n, AIL_SV joiner);
AIL_SV ail_sv_join_da(AIL_DA(AIL_SV) list, AIL_SV joiner);
AIL_SV ail_sv_replace(AIL_SV sv, AIL_SV to_replace, AIL_SV replace_with);

#endif // AIL_SV_H_

#ifdef AIL_SV_IMPL
#ifndef _AIL_SV_IMPL_GUARD_
#define _AIL_SV_IMPL_GUARD_

AIL_SV ail_sv_from_parts(const char *s, u64 len)
{
    return (AIL_SV) {
        .str = s,
        .len = len,
    };
}

AIL_SV ail_sv_from_da(AIL_DA(char) str)
{
    return ail_sv_from_parts(str.data, str.len);
}

AIL_SV ail_sv_from_cstr(const char *str)
{
    u64 len = 0;
    while (str[len]) len++;
    return ail_sv_from_parts(str, len);
}

char* ail_sv_copy_to_cstr(AIL_SV sv)
{
    char *out = AIL_SV_MALLOC(sv.len + 1);
    AIL_SV_MEMCPY(out, sv.str, sv.len);
    out[sv.len] = 0;
    return out;
}

bool ail_sv_eq(AIL_SV a, AIL_SV b)
{
    if (a.len != b.len) return false;
    for (u32 i = 0; i < a.len; i++) {
        if (a.str[i] != b.str[i]) return false;
    }
    return true;
}

AIL_DA(AIL_SV) ail_sv_split(AIL_SV sv, AIL_SV split_by, bool ignore_empty)
{
    AIL_DA(AIL_SV) res = ail_da_new(AIL_SV);
    u64 lstart = 0;
    u64 llen   = 0;
    if (sv.len >= split_by.len) {
        for (u64 i = 0; i <= sv.len - split_by.len; i++) {
            if (ail_sv_starts_with(ail_sv_offset(sv, i), split_by)) {
                if (!ignore_empty || llen > 0) {
                    ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
                }
                i += split_by.len - 1;
                lstart = i + 1;
                llen   = 0;
            } else {
                llen++;
            }
        }
        if (!ail_sv_starts_with(ail_sv_offset(sv, sv.len - split_by.len - 1), split_by)) llen += split_by.len;
    } else {
        llen = sv.len;
    }
    if (lstart < sv.len) {
        ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
    }
    return res;
}

AIL_DA(AIL_SV) ail_sv_split_lines(AIL_SV sv, bool ignore_empty)
{
    // @Cleanup: Almost identical to ail_sv_split - maybe we can unite them somehow
    AIL_DA(AIL_SV) res = ail_da_new(AIL_SV);
    u64 lstart = 0;
    u64 llen   = 0;
    if (sv.len > 0) {
        for (u64 i = 0; i < sv.len; i++) {
            if (sv.str[i] == '\n') {
                if (!ignore_empty || llen > 0) {
                    ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
                }
                lstart = i + 1;
                llen   = 0;
            } else if (sv.str[i] == '\r') {
                if (AIL_LIKELY(i + 1 < sv.len && sv.str[i + 1] == '\n')) continue;
                else AIL_UNREACHABLE();
            } else {
                llen++;
            }
        }
    }
    if (lstart < sv.len && (!ignore_empty || llen > 0)) {
        ail_da_push(&res, ail_sv_from_parts(&sv.str[lstart], llen));
    }
    return res;
}

AIL_SV ail_sv_offset(AIL_SV sv, u64 offset)
{
    return ail_sv_from_parts(&sv.str[offset], sv.len - offset);
}

bool ail_sv_is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

AIL_SV ail_sv_trim(AIL_SV sv)
{
    if (sv.len == 0) return sv;
    u64 start = 0;
    u64 end   = sv.len - 1;
    while (start < sv.len && ail_sv_is_space(sv.str[start])) start++;
    while (end > start && ail_sv_is_space(sv.str[end])) end--;
    if (start < sv.len) return ail_sv_from_parts(&sv.str[start], end - start + 1);
    else return ail_sv_from_parts("", 0);
}

AIL_SV ail_sv_join(AIL_SV *list, u64 n, AIL_SV joiner)
{
    if (n == 0) return ail_sv_from_parts("", 0);
    u64 res_len = joiner.len*(n - 1);
    for (u64 i = 0; i < n; i++) res_len += list[i].len;
    char *res = AIL_SV_MALLOC(res_len + 1);
    for (u64 i = 0, j = 0; i < n; i++) {
        memcpy(&res[j], list[i].str, list[i].len);
        j += list[i].len;
        if (i + 1 < n) {
            memcpy(&res[j], joiner.str, joiner.len);
            j += joiner.len;
        }
    }
    return ail_sv_from_parts(res, res_len);
}

AIL_SV ail_sv_join_da(AIL_DA(AIL_SV) list, AIL_SV joiner)
{
    return ail_sv_join(list.data, list.len, joiner);
}

AIL_SV ail_sv_replace(AIL_SV sv, AIL_SV to_replace, AIL_SV replace_with)
{
    AIL_DA(AIL_SV) list = ail_sv_split(sv, to_replace, true);
    // ail_da_printf(list, "'%s'", ail_sv_copy_to_cstr(list.data[i]));
    AIL_SV out = ail_sv_join(list.data, list.len, replace_with);
    // printf("replaced result: %s\n", ail_sv_copy_to_cstr(out));
    ail_da_free(&list);
    return out;
}

bool ail_sv_starts_with(AIL_SV str, AIL_SV prefix)
{
    if (prefix.len > str.len) return false;
    for (u32 i = 0; i < prefix.len; i++) {
        if (str.str[i] != prefix.str[i]) return false;
    }
    return true;
}

bool ail_sv_starts_with_char(AIL_SV str, char prefix)
{
    return str.len > 0 && str.str[0] == prefix;
}

bool ail_sv_ends_with(AIL_SV str, AIL_SV suffix)
{
    if (suffix.len > str.len) return false;
    for (u32 i = suffix.len; i > 0; i--) {
        if (str.str[str.len - i] != suffix.str[suffix.len - i]) return false;
    }
    return true;
}

bool ail_sv_ends_with_char(AIL_SV str, char suffix)
{
    return str.len > 0 && str.str[str.len - 1] == suffix;
}

#endif // _AIL_SV_IMPL_GUARD_
#endif // AIL_SV_IMPL