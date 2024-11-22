#include "../ail.h"
#include "../ail_sv.h"
#include "../ail_fs.h"
#include "../ail_subproc.h"
#include "./assert.h"

int main(void)
{
    u64 size;
    char *txt = ail_fs_read_entire_file("./foo1.txt", &size);
    AIL_SV foo1 = ail_sv_from_parts(txt, size);
    char *txt = ail_fs_read_entire_file("./foo2.txt", &size);
    AIL_SV foo2 = ail_sv_from_parts(txt, size);

    ail_subproc_init();
    AIL_Subproc child = ail_subproc_exec("cat foo1.txt");
    AIL_Str     out1  = ail_subproc_read(child);
    ail_subproc_close(child);
    ail_subproc_exec_sync("cat foo1.txt");
    ail_subproc_deinit();
    return 0;
}