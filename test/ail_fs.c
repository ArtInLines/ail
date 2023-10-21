// Test ail_fs.h

#define AIL_FS_IMPL
#include "../ail_fs.h"
#include <stdio.h>
#include "assert.h"

int main(void)
{
	static const char *buf = "Hello World\n";
	mkdir("./tmp");
	bool succ = ail_fs_writeFile("./tmp/test.txt", buf, 13);
	assert(succ);
	u64 size;
	char *out = ail_fs_readFile("./tmp/test.txt", &size);
	assert(strcmp(buf, out) == 0);
	rmdir("./tmp");
	return 0;
}