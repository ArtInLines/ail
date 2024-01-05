// Test the AIL_DA macros in ail.h

#define AIL_ALL_IMPL
#include "../ail.h"
#include <stdio.h>
#include <stdbool.h>

#define LEN 10
#define EL_TO_INSERT 19

typedef struct Vec2 {
    u32 x;
    u32 y;
} Vec2;
AIL_DA_INIT(Vec2);

bool intTest(void)
{
	i32 buf[LEN] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	AIL_DA(i32) da = ail_da_new_with_cap(i32, LEN);
	for (u32 i = 0; i < LEN; i++) ail_da_push(&da, 1);
	ail_da_insert(&da, 1, EL_TO_INSERT);
	ail_da_pushn(&da, buf, LEN);
	ail_da_insertn(&da, 12, buf, 5);

	i32 expected = LEN + 55 + 15 + EL_TO_INSERT;
	i32 sum = 0;
	for (u32 i = 0; i < da.len; i++) sum += da.data[i];
	printf("sum: %d == %d?\n", sum, expected);
	return sum == expected;
}

bool structTest(void)
{
    Vec2 buf[2] = { (Vec2){2, 2}, (Vec2){3, 3} };
    AIL_DA(Vec2) da = ail_da_new_empty(Vec2);
    for (u32 i = 0; i < LEN; i++) ail_da_push(&da, ((Vec2){1, 2}));
    ail_da_insertn(&da, 3, buf, 2);
    ail_da_pushn(&da, buf, 2);
    ail_da_rm_swap(&da, 0);
    ail_da_rm(&da, 0);

    Vec2 expected = { (LEN-1)+5+2, (LEN-1)*2+5+2 };
    Vec2 sum = {0};
    for (u32 i = 0; i < da.len; i++) {
        sum.x += da.data[i].x;
        sum.y += da.data[i].y;
    }
    printf("sum: (%d, %d) == (%d, %d)?\n", sum.x, sum.y, expected.x, expected.y);
    return sum.x == expected.x && sum.y == expected.y;
}

int main(void)
{
    if (intTest())    printf("Test with ints succesfull :)\n");
    else              printf("Test with ints failed     :(\n");
    if (structTest()) printf("Test with vec2 succesfull :)\n");
    else              printf("Test with vec2 failed     ;(\n");
    return 0;
}