// Test ail_fs.h

#include "test_assert.h"
#include "../ail_fs.h"
#include <stdio.h>

int main(void)
{
    static const char *buf = "Hello World\n";
    mkdir("./tmp", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    bool succ = ail_fs_write_file("./tmp/test.txt", buf, strlen(buf));
    ASSERT(succ);
    u64 size;
    char *out = ail_fs_read_entire_file("./tmp/test.txt", &size);
    ASSERT(size == strlen(buf));
    ASSERT(memcmp(out, buf, size) == 0);

    ASSERT(!remove("./tmp/test.txt"));
    rmdir("./tmp");
    ASSERT(!ail_fs_dir_exists("./tmp"));
    printf("\033[32mTest successful :)\033[0m\n");
    return 0;
}
