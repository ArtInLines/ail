// File System Utilities
//

#ifndef AIL_FS_H_
#define AIL_FS_H_

#define AIL_TYPES_IMPL
#include "ail.h"      // For type-definitions
#include <stdlib.h>   // For memcpy (and malloc if not overwritten)
#include <fcntl.h>    // For file access flags
#include <sys/stat.h> // For file stats

#ifndef AIL_FS_DEF
#ifdef  AIL_DEF
#define AIL_FS_DEF AIL_DEF
#else
#define AIL_FS_DEF
#endif // AIL_DEF
#endif // AIL_FS_DEF
#ifndef AIL_FS_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_FS_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_FS_DEF_INLINE inline
#endif // AIL_DEF_INLINE
#endif // AIL_FS_DEF_INLINE

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

AIL_FS_DEF char* ail_fs_read_file(const char *fpath, u64 *size);
AIL_FS_DEF bool  ail_fs_write_file(const char *fpath, const char *buf, u64 size);
AIL_FS_DEF const char *ail_fs_get_file_ext(const char *filename);
AIL_FS_DEF bool ail_fs_is_file_ext(const char *restrict filename, const char *restrict ext);
AIL_FS_DEF bool ail_fs_is_file(const char *path);

#endif // AIL_FS_H_

#ifdef  AIL_FS_IMPL
#ifndef _AIL_FS_IMPL_GUARD_
#define _AIL_FS_IMPL_GUARD_

AIL_FS_DEF char* ail_fs_read_file(const char *fpath, u64 *size)
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

AIL_FS_DEF bool ail_fs_write_file(const char *fpath, const char *buf, u64 size)
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

AIL_FS_DEF bool ail_fs_str_eq(const char *restrict a, const char *restrict b)
{
	while (*a && *b && *a++ == *b++) {}
    return *a == *b && !*a;
}

AIL_FS_DEF const char *ail_fs_get_file_ext(const char *filename)
{
    u32 idx = 0;
    for (u32 i = 0; filename[i]; i++) {
        if (filename[i] == '.') idx = i + 1;
    }
    return &filename[idx];
}

AIL_FS_DEF bool ail_fs_is_file_ext(const char *restrict filename, const char *restrict ext)
{
    const char *file_ext = ail_fs_get_file_ext(filename);
    return ail_fs_str_eq(file_ext, ext);
}

AIL_FS_DEF bool ail_fs_is_file(const char *path)
{
    struct stat result = {0};
    stat(path, &result);
    return S_ISREG(result.st_mode);
}

#endif // _AIL_FS_IMPL_GUARD_
#endif // AIL_FS_IMPL