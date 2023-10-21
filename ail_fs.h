// File System Utilities
//

#ifndef AIL_FS_H_
#define AIL_FS_H_

#define AIL_TYPES_IMPL
#include "ail.h"      // For type-definitions
#include <stdlib.h>   // For memcpy (and malloc if not overwritten)
#include <fcntl.h>    // For file access flags
#include <sys/stat.h> // For file stats

#ifndef AIL_FS_MALLOC
#ifdef  AIL_MALLOC
#define AIL_FS_MALLOC(sz) AIL_MALLOC(sz)
#else
#define AIL_FS_MALLOC(sz) malloc(sz)
#endif // AIL_MALLOC
#endif // AIL_FS_MALLOC

// #if defined(_WIN32)
// 	#define mkdir(dir, mode)      _mkdir(dir)
// 	#define open(name, ...)       _open(name, __VA_ARGS__)
// 	#define read(fd, buf, count)  _read(fd, buf, count)
// 	#define close(fd)             _close(fd)
// 	#define write(fd, buf, count) _write(fd, buf, count)
// 	#define dup2(fd1, fd2)        _dup2(fd1, fd2)
// 	#define unlink(file)          _unlink(file)
// 	#define rmdir(dir)            _rmdir(dir)
// 	#define getpid()              _getpid()
// 	#define usleep(t)             Sleep((t)/1000)
// 	#define sleep(t)              Sleep((t)*1000)
// #endif

char* ail_fs_read_file(const char *fpath, u64 *size);
bool  ail_fs_write_file(const char *fpath, const char *buf, u64 size);

#endif // AIL_FS_H_

#ifdef  AIL_FS_IMPL
#ifndef _AIL_FS_IMPL_GUARD_
#define _AIL_FS_IMPL_GUARD_

char* ail_fs_read_file(const char *fpath, u64 *size)
{
    // Adapted from https://stackoverflow.com/a/68156485/13764271
    char* out = NULL;
    *size = 0;
    int fd = open(fpath, O_RDONLY, 0777);
    if (fd == -1) goto end;
    struct stat sb;
    if (stat(fpath, &sb) == -1) goto fd_end;
    if (sb.st_size == 0) goto fd_end;
    out = AIL_FS_MALLOC(sb.st_size);
    if (out == NULL) goto fd_end;
    if (read(fd, out, sb.st_size) == -1) goto fd_end;
    *size = (u64) sb.st_size;
fd_end:
    close(fd);
end:
    return out;
}

bool ail_fs_write_file(const char *fpath, const char *buf, u64 size)
{
    bool out = false;
    int fd = open(fpath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd == -1) goto end;
    u64 written = 0;
    while (written < size) {
        int res = write(fd, &buf[written], size - written);
        if (res == -1) goto fd_end;
        written += res;
    }
    out = true;
fd_end:
    close(fd);
end:
    return out;
}

#endif // _AIL_FS_IMPL_GUARD_
#endif // AIL_FS_IMPL