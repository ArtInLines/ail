// Test the AIL_DA macros in ail.h

// #define AIL_ALL_IMPL
// #include "../ail.h"
#define AIL_HM_IMPL
#include "../ail_hm.h"
#include "../ail_fs.h"
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
    bool found;
    u32  x;
    ail_hm_get_val(&hm, "test", x, found);
    AIL_ASSERT(found);
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
    AIL_ASSERT(found);
    // printf("%s == %s?\n", x, val);
    return strEq(x, val);
}

int main(void)
{
    if (strTest())    printf("\033[32mTest with strings succesful :)\033[0m\n");
    else              printf("\033[31mTest with strings failed    :(\033[0m\n");
    if (structTest()) printf("\033[32mTest with Vec3 succesful    :)\033[0m\n");
    else              printf("\033[31mTest with Vec3 failed       :(\033[0m\n");
    return 0;
}