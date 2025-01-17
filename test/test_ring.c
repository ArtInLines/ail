// Test ail_ring.h

#include "assert.h"
#include "../src/base/ail_ring.h"
#include <stdio.h>

// @TODO: Test the actual ring-buffer-ness of it

int main(void)
{
    u8 expected[] = { 1, 2, 0, 0, 2, 4, 0, 3, 0, 0, 3, 0, 4, 1, 2, 3, 4, 5, 6, 7, 8, 8, 7, 6, 5, 4, 3, 2, 1 };
    u8 data[ail_arrlen(expected)];
    AIL_RingBuffer rb = ail_ring_from_data(data, ail_arrlen(data));

    ail_ring_write1   (&rb, 1);
    ail_ring_write2lsb(&rb, 2);
    ail_ring_write2msb(&rb, 2);
    ail_ring_write4lsb(&rb, 0x30004);
    ail_ring_write4msb(&rb, 0x30004);
    ail_ring_write8lsb(&rb, 0x807060504030201);
    ail_ring_write8msb(&rb, 0x807060504030201);

    for (u32 i = 0; i < ail_arrlen(expected); i++) {
        if (expected[i] != rb.data[i]) {
            printf("\033[31mFailed Test:\033[0m\n");
            printf("  expected[%d] = %d, rb.data[%d] = %d\n", i, expected[i], i, rb.data[i]);
            return 1;
        }
    }
    printf("\033[32mTest successful :)\033[0m\n");
    return 0;
}
