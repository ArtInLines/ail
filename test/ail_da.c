// Test the AIL_DA macros in ail.h

#define AIL_ALL_IMPL
#include "../ail.h"
#include <stdio.h>

#define BUF_LEN 10
#define LEN BUF_LEN
#define EL_TO_INSERT 19

int main(void)
{
	i32 buf[BUF_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	AIL_DA da = ail_da_with_cap(i32, BUF_LEN);
	for (u32 i = 0; i < LEN; i++) ail_da_push(&da, 1);
	ail_da_insert(&da, 1, EL_TO_INSERT);
	ail_da_pushn(&da, buf, BUF_LEN);
	ail_da_insertn(&da, 12, buf, 5);
	i32 sum = 0;
	for (u32 i = 0; i < da.len; i++) sum += ((i32 *) da.data)[i];
	printf("sum: %d == %d?\n", sum, LEN + 55 + 15 + EL_TO_INSERT);
	return 0;
}