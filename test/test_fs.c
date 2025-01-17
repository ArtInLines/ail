// Test ail_fs.h

#include "assert.h"
#include "../src/fs/ail_file.h"
#include <stdio.h>

int main(void)
{
    ail_default_allocator = ail_alloc_std;
    static const char *buf = "Hello World\n";
    mkdir("./tmp", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    bool succ = ail_fs_write_file("./tmp/test.txt", buf, strlen(buf));
    ASSERT(succ);
    u64 size;
    u8 *out = ail_fs_read_entire_file("./tmp/test.txt", &size, ail_default_allocator);
    ASSERT(size == strlen(buf));
    ASSERT(memcmp(out, buf, size) == 0);

    ASSERT(!remove("./tmp/test.txt"));
    rmdir("./tmp");
    ASSERT(!ail_fs_dir_exists("./tmp"));
    printf("\033[32mTest successful :)\033[0m\n");
    return 0;
}
