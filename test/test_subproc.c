#include "../src/base/ail_base.h"
#include "../src/base/ail_str.h"
#include "../src/fs/ail_file.h"
#include "../src/proc/ail_subproc.h"
#include "./assert.h"

int main(void)
{
    ail_default_allocator = ail_alloc_std;
    u64 size;
    u8 *txt1 = ail_fs_read_entire_file("./foo1.txt", &size, ail_default_allocator);
    AIL_Str foo1 = ail_str_from_parts(txt1, size);
    u8 *txt2 = ail_fs_read_entire_file("./foo2.txt", &size, ail_default_allocator);
    AIL_Str foo2 = ail_str_from_parts(txt2, size);

    ail_subproc_init();
    AIL_Subproc child = ail_subproc_exec("cat foo1.txt");
    AIL_Str     out1  = ail_subproc_read(child);
    ail_subproc_close(child);
    ail_subproc_exec_sync("cat foo1.txt");
    ail_subproc_deinit();
    return 0;
}