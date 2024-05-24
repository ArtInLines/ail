#define AIL_FS_IMPL
#define AIL_HM_IMPL
#define AIL_BENCH_IMPL
#define AIL_BENCH_PROFILE
#include "../ail_hm.h"
#include "../ail_fs.h"
#include "../ail_bench.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef char* String;
AIL_HM_INIT(String, u32);

static AIL_HM(String, u32) hm;

bool ignoreChar(char c)
{
    return c == ' ' || c == '\t' || c == '\n'; // || c == ',' || c == '.' || c == '?' || c == '!' || c == '(' || c == ')' || c == '-' || c == '_';
}

bool strEq(char *a, char *b)
{
    return strcmp(a, b) == 0;
}

u32 sum(char *a)
{
    u32 res = 0;
    while (*a) res += *a++;
    return res;
}

u32 djb2(char *a)
{
    uint32_t hash = 5381;
    while (*a)
        hash = ((hash << 5) + hash) + *a++;
    return hash;
}

i32 keyValCompRev(const void *a, const void *b)
{
    const AIL_HM_KEY_VAL(String, u32) *akv = a;
    const AIL_HM_KEY_VAL(String, u32) *bkv = b;
    return bkv->val - akv->val;
}

void txtFileTest(const char *fpath)
{
    u64 fsize;
    AIL_BENCH_PROFILE_START(ReadFile);
    char *text = ail_fs_read_entire_file(fpath, &fsize);
    AIL_BENCH_PROFILE_END(ReadFile);

    hm = ail_hm_new_with_cap(String, u32, 64, &djb2, &strEq);

    AIL_BENCH_PROFILE_START(FillHashMap);
    u32 tokenCount = 0;
    u32 i = 0;
    while (i < fsize) {
        while (i < fsize && ignoreChar(text[i])) i++;
        u32 j = i;
        while (i < fsize && !ignoreChar(text[i])) i++;
        char *s = malloc((i - j + 1) * sizeof(char));
        memcpy(s, &text[j], i - j);
        s[i - j] = 0;
        tokenCount++;
        u32 *val;
        ail_hm_get_ptr(&hm, s, val);
        if (val) (*val)++;
        else ail_hm_put(&hm, s, 1);
    }
    AIL_BENCH_PROFILE_END(FillHashMap);

    AIL_BENCH_PROFILE_START(SortTokens);
    u32 arrlen = hm.len;
    AIL_HM_KEY_VAL(String, u32) *arr = malloc(sizeof(AIL_HM_KEY_VAL(String, u32)) * arrlen);
    for (u32 i = 0, j = 0; i < hm.cap; i++) {
        if (hm.data[i].occupied == AIL_HM_CUR_OCCUPIED) {
            arr[j].key = hm.data[i].key;
            arr[j].val = hm.data[i].val;
            j++;
        }
    }
    qsort(arr, arrlen, sizeof(AIL_HM_KEY_VAL(String, u32)), keyValCompRev);
    AIL_BENCH_PROFILE_END(SortTokens);

    char *expTopTenKeys[] = { "the",  "I",  "and", "to",   "of", "a",   "my", "in", "you", "is" };
    u32   expTopTenVals[] = { 23242, 19540, 18297, 15623, 15544, 12532, 10824, 9576, 9081, 7851 };

    printf("Textfile: %s (size: %llu)\n", fpath, fsize);

    if (tokenCount == 901326) printf("\033[32m");
    else printf("\033[31m");
    printf("  Tokens: %d\033[0m\n", tokenCount);

    if (arrlen == 67506) printf("\033[32m");
    else printf("\033[31m");
    printf("  Unique Tokens: %d\033[0m\n", arrlen);

    printf("  Top 10 tokens:\n");
    for (u32 i = 0; i < 10 && i < arrlen; i++) {
        if (strcmp(arr[i].key, expTopTenKeys[i]) == 0 && arr[i].val == expTopTenVals[i]) printf("\033[32m");
        else printf("\033[31m");
        printf("    %2d: %6s (%d)\033[0m\n", i, arr[i].key, arr[i].val);
    }
}

int main(void)
{
    ail_bench_begin_profile();
    txtFileTest("shakespeare.txt");
    ail_bench_end_and_print_profile(false);
    // Expected result:
    // Tokens: 901326
    // Unique Tokens: 67506
    // Top 10 tokens:
    //    0:    the (23242)
    //    1:      I (19540)
    //    2:    and (18297)
    //    3:     to (15623)
    //    4:     of (15544)
    //    5:      a (12532)
    //    6:     my (10824)
    //    7:     in (9576)
    //    8:    you (9081)
    //    9:     is (7851)
}
