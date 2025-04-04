// Test ail_idxbuf.h

// #define AIL_NO_IMPL

#include "assert.h"
#include "../src/base/ail_base_math.h"
#include "../src/base/ail_alloc.h"
#include "../src/base/ail_idxbuf.h"
#include <stdlib.h>
#include <stdio.h>

// @TODO: Test reading

int main(void)
{
    u8 expected[] = { 1, 2, 0, 0, 2, 3, 3, 0, 0, 3, 3, 4, 0, 3, 0, 0, 3, 0, 4, 1, 2, 3, 4, 5, 6, 7, 8, 8, 7, 6, 5, 4, 3, 2, 1 };
    // u8 data[ail_arrlen(expected)];
    AIL_IdxBuffer buf = ail_idxbuf_new(ail_arrlen(expected), ail_alloc_pager);

    ail_idxbuf_write1   (&buf, 1);
    ail_idxbuf_write2lsb(&buf, 2);
    ail_idxbuf_write2msb(&buf, 2);
    ail_idxbuf_write3lsb(&buf, 0x303);
    ail_idxbuf_write3msb(&buf, 0x303);
    ail_idxbuf_write4lsb(&buf, 0x30004);
    ail_idxbuf_write4msb(&buf, 0x30004);
    ail_idxbuf_write8lsb(&buf, 0x807060504030201);
    ail_idxbuf_write8msb(&buf, 0x807060504030201);

    for (u32 i = 0; i < ail_arrlen(expected); i++) {
        if (expected[i] != buf.data[i]) {
            printf("\033[31mFailed Test:\033[0m\n");
            printf("  expected[%d] = %d, buf.data[%d] = %d\n", i, expected[i], i, buf.data[i]);
            return 1;
        }
    }
    printf("\033[32mTest successful :)\033[0m\n");
    return 0;
}
