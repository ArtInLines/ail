#define AIL_FS_IMPL
#define AIL_HM_IMPL
#include "test_assert.h"
#include "../ail_hm.h"
#include "../ail_fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

typedef char* String;
AIL_HM_INIT(String, u32);

static AIL_HM(String, u32) hm;

double clockGetSecs(void)
{
    struct timespec ts = {0};
    int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    ASSERT(ret == 0);
    return (double)ts.tv_sec + ts.tv_nsec*1e-9;
}

bool ignoreChar(char c)
{
    return c == ' ' || c == '\t' || c == '\n'; // || c == ',' || c == '.' || c == '?' || c == '!' || c == '(' || c == ')' || c == '-' || c == '_';
}

bool strEq(char *a, char *b)
{
    while (*a && *b && *a++ == *b++) {}
    return *a == 0 && *a == *b;
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
    char *text = ail_fs_read_file(fpath, &fsize);
    hm = ail_hm_with_cap(String, u32, 64, &djb2, &strEq);

    double start = clockGetSecs();

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

    double end = clockGetSecs();

    printf("Textfile: %s (size: %llu)\n", fpath, fsize);
    printf("  Tokens: %d\n", tokenCount);
    printf("  Unique Tokens: %d\n", arrlen);
    printf("  Top 10 tokens:\n");
    for (u32 i = 0; i < 10 && i < arrlen; i++) {
        printf("    %2d: %6s (%d)\n", i, arr[i].key, arr[i].val);
    }
    printf("  Total time %.03lfs\n", end - start);
}

int main(void)
{
    // @Bug: Wrong results for shakespeare.txt ('in' should be among the top 10)
    txtFileTest("shakespeare.txt");
    // Expected result:
    // Tokens: 67506 tokens
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