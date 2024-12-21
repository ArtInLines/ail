#define AIL_HM_LOAD_FACTOR 80
#include "../src/base/ail_alloc.h"
#include "../src/base/ail_hm.h"
#include "assert.h"
#include <stdio.h>
#include <stdbool.h>

typedef char* String;
AIL_HM_INIT(String, u32);

bool strEq(char *a, char *b)
{
    while (*a && *b && *a++ == *b++) {}
    return *a == *b;
}

u32 sum(char *a)
{
    u32 res = 0;
    while (*a) res += *a++;
    return res;
}

bool strTest(void)
{
    AIL_HM(String, u32) hm = ail_hm_new(String, u32, &sum, &strEq);
    u32 expected = 4;
    ail_hm_put(&hm, "test", expected);
    ail_hm_put(&hm, "t2", 2*expected);
    bool found;
    u32  x;
    ail_hm_get_val(&hm, "test", x, found);
    ASSERT(found);
    // printf("%d == %d?\n", x, expected);
    return x == expected;
}

typedef struct {
    float x;
    float y;
    float z;
} Vec3;
AIL_HM_INIT(Vec3, String);

bool vec3Eq(Vec3 a, Vec3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

u32 vec3Hash(Vec3 v)
{
    return (u32) (v.x * (v.y * 2) + (v.z / 5));
}

bool structTest(void)
{
    AIL_HM(Vec3, String) hm = ail_hm_new(Vec3, String, &vec3Hash, &vec3Eq);
    Vec3   key = { 1.5, 2.7, 3.14 };
    String val = "test";
    ail_hm_put(&hm, key, val);
    bool   found;
    String x;
    ail_hm_get_val(&hm, key, x, found);
    ASSERT(found);
    // printf("%s == %s?\n", x, val);
    return strEq(x, val);
}

AIL_HM_INIT(pchar, u32);
bool miniTestEq(pchar a, pchar b)
{
    return strcmp(a, b) == 0;
}

u32 miniTestHash(pchar k)
{
    uint32_t hash = 5381;
    while (*k)
        hash = ((hash << 5) + hash) + *k++;
    return hash;
}

bool miniTest(void)
{
#define MINI_MAGIC 8
    AIL_HM(pchar, u32) hm = ail_hm_new_with_cap(pchar, u32, 0, &miniTestHash, &miniTestEq);
    for (u32 i = 0; i < MINI_MAGIC*16; i++) {
        pchar k = ail_call_alloc(ail_default_allocator, 8);
        sprintf(k, "hi-%d", i%16);
        u32 *val;
        ail_hm_get_ptr(&hm, k, val);
        if (val) (*val)++;
        else ail_hm_put(&hm, k, 1);
    }
    ASSERT(hm.len == 16);
    for (u32 i = 0; i < hm.cap; i++) {
        if (hm.data[i].occupied == AIL_HM_CUR_OCCUPIED) {
            ASSERT(hm.data[i].val == MINI_MAGIC);
        }
    }
    return true;
}

int main(void)
{
    ail_default_allocator = ail_alloc_std;
    if (miniTest())   printf("\033[32mMini-Test succesful         :)\033[0m\n");
    else              printf("\033[31mMini-Test failed            :(\033[0m\n");
    if (strTest())    printf("\033[32mTest with strings succesful :)\033[0m\n");
    else              printf("\033[31mTest with strings failed    :(\033[0m\n");
    if (structTest()) printf("\033[32mTest with Vec3 succesful    :)\033[0m\n");
    else              printf("\033[31mTest with Vec3 failed       :(\033[0m\n");
    return 0;
}
