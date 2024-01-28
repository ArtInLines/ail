// File System Utilities
//
// LICENSE
/*
Copyright (c) 2024 Val Richter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
#define AIL_FS_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_FS_DEF_INLINE

#ifndef AIL_FS_MALLOC
#ifdef  AIL_MALLOC
#define AIL_FS_MALLOC(sz) AIL_MALLOC(sz)
#else
#define AIL_FS_MALLOC(sz) malloc(sz)
#endif // AIL_MALLOC
#endif // AIL_FS_MALLOC

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
	#define mkdir(dir, mode)      _mkdir(dir)
	#define open(name, ...)       _open(name, __VA_ARGS__)
	#define read(fd, buf, count)  _read(fd, buf, count)
	#define close(fd)             _close(fd)
	#define write(fd, buf, count) _write(fd, buf, count)
	#define dup2(fd1, fd2)        _dup2(fd1, fd2)
	#define unlink(file)          _unlink(file)
	#define rmdir(dir)            _rmdir(dir)

    #define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
    #define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)

    #ifndef READ_TIMEOUT
    #define READ_TIMEOUT 500 // in milliseconds
    #endif // READ_TIMEOUT
    #ifndef WRITE_TIMEOUT
    #define WRITE_TIMEOUT INFINITE // in milliseconds
    #endif // WRITE_TIMEOUT
#endif

///////////////////////////
// Reading/Writing Files //
///////////////////////////

// Reads n bytes from file 'fname' into 'buf'
// @Note: 'fd' be valid File-Handle on Windows (a void pointer cast to a u64) and a file-descriptor on Unix
// @Note: 'buf' must have space for 'maxN' bytes
// @Note: 'actualN' will contain the amount of bytes, that were actually read into 'buf'
AIL_FS_DEF bool ail_fs_read_n_bytes(u64 fd, void *buf, u64 maxN, u64 *actualN);

// Same as ail_fs_read_n_bytes(), except that it handles opening and closing the file
AIL_FS_DEF bool ail_fs_read_file(const char *fpath, void *buf, u64 maxN, u64 *actualN);

// Same as ail_fs_read_file(), except that it checks the file's size first and allocates an output buffer of the appropriate size for it
// Returns NULL on error
AIL_FS_DEF char* ail_fs_read_entire_file(const char *fpath, u64 *size);

// Write `size` many bytes from `buf` into `fpath`
AIL_FS_DEF bool  ail_fs_write_file(const char *fpath, const char *buf, u64 size);

//////////////////
// Miscellanous //
//////////////////

#ifdef AIL_DA_IMPL
// @Important: Not yet implemented
AIL_FS_DEF AIL_DA(str) ail_fs_get_files_in_dir(const char *dirpath);
#endif

AIL_FS_DEF_INLINE bool ail_fs_dir_exists(const char *dirpath);
AIL_FS_DEF const char *ail_fs_get_file_ext(const char *filename);
AIL_FS_DEF bool ail_fs_is_file_ext(const char *restrict filename, const char *restrict ext);
AIL_FS_DEF bool ail_fs_is_file(const char *path);

#endif // AIL_FS_H_



#ifdef  AIL_FS_IMPL
#ifndef _AIL_FS_IMPL_GUARD_
#define _AIL_FS_IMPL_GUARD_

///////////////////////
// Utility functions //
///////////////////////

bool ail_fs_str_eq(const char *restrict a, const char *restrict b)
{
	while (*a && *b && *a++ == *b++) {}
    return *a == *b && !*a;
}

///////////////////
// Read/Write IO //
///////////////////

bool ail_fs_read_n_bytes(u64 fd, void *buf, u64 maxN, u64 *actualN)
{
#ifdef _WIN32
    *actualN = 0;
    void *file = (void *)fd;
    OVERLAPPED osReader = {0};
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osReader.hEvent == NULL) return false;

    // Issue Read operation
    DWORD nToRead = (DWORD) maxN;
    DWORD nRead = 0;
    i8 res = -1;
    if (!ReadFile(file, buf, nToRead, &nRead, &osReader)) {
        if(GetLastError() != ERROR_IO_PENDING) res = 0;
    } else {
        // Read completed immediately
        res = 1;
    }
    while (res < 0) {
        DWORD dwRes = WaitForSingleObject(osReader.hEvent, READ_TIMEOUT);
        switch(dwRes) {
            case WAIT_OBJECT_0:
                // Read completed.
                bool succ = GetOverlappedResult(file, &osReader, &nRead, FALSE);
                res = succ ? 1 : 0;
                break;
            case WAIT_TIMEOUT:
                // Gotta wait a little longer to finish operation
                break;
            default:
                // Error in the WaitForSingleObject
                // This indicates a problem with the OVERLAPPED structure's event handle.
                res = 0;
                break;
        }
    }
    CloseHandle(osReader.hEvent);
    *actualN = (u64) nRead;
    return res > 0;
#else
    *actualN = maxN;
    return read((int)fd, buf, maxN) != -1;
#endif // _WIN32
}

bool ail_fs_read_file(const char *fpath, void *buf, u64 maxN, u64 *actualN)
{
#ifdef _WIN32
    void *file = CreateFile(fpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, 0);
    if (file == INVALID_HANDLE_VALUE) return false;
    bool res = ail_fs_read_n_bytes((u64)file, buf, maxN, actualN);
    CloseHandle(file);
    return res;
#else
    int fd = open(fpath, O_RDONLY, 0777);
    if (fd == -1) return false;
    bool res = ail_fs_read_n_bytes((u64)fd, buf, maxN, actualN);
    close(fd);
    return out;
#endif // _WIN32
}

char* ail_fs_read_entire_file(const char *fpath, u64 *size)
{
    // Adapted from https://stackoverflow.com/a/68156485/13764271
    char* buf = NULL;
    *size = 0;
    struct stat sb;
    if (stat(fpath, &sb) != -1) {
        if (sb.st_size) {
            buf = AIL_FS_MALLOC(sb.st_size);
            if (buf) {
                ail_fs_read_file(fpath, buf, sb.st_size, size);
            }
        }
    }
    return buf;
}

bool ail_fs_write_n_bytes(u64 fd, const char *buf, u64 size)
{
#ifdef _WIN32
    void *file = (void *)fd;
    OVERLAPPED osWrite = {0};
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osWrite.hEvent == NULL) return false;

    // Issue Write operation
    i8 res = -1;
    DWORD dwWritten;
    if (!WriteFile(file, buf, (DWORD)size, &dwWritten, &osWrite)) {
        if (GetLastError() != ERROR_IO_PENDING) res = 1;
    } else {
        // Write completed immediately.
        res = 0;
    }
    while (res < 0) {
        DWORD dwRes = WaitForSingleObject(osWrite.hEvent, WRITE_TIMEOUT);
        switch(dwRes) {
            case WAIT_OBJECT_0:
                // Write completed
                bool succ = GetOverlappedResult(file, &osWrite, &dwWritten, FALSE);
                res = succ ? 1 : 0;
                break;
            default:
                // Error in the WaitForSingleObject
                // This indicates a problem with the OVERLAPPED structure's event handle.
                res = 1;
                break;
        }
    }
    CloseHandle(osWrite.hEvent);
    return res > 0 && dwWritten == size;
#else
    u64 written = 0;
    while (written < size) {
        int res = write(fd, &buf[written], size - written);
        if (res == -1) return false;
        written += res;
    }
    return true;
#endif
}

bool ail_fs_write_file(const char *fpath, const char *buf, u64 size)
{
#ifdef _WIN32
    void *file = CreateFile(fpath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);
    if (file == INVALID_HANDLE_VALUE) return false;
    bool res = ail_fs_write_n_bytes((u64)file, buf, size);
    CloseHandle(file);
    return res;
#else
    int fd = open(fpath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd == -1) return false;
    bool res = ail_fs_write_n_bytes((u64)fd, buf, size);
    close(fd);
    return res;
#endif // _WIN32
}

//////////////////
// Miscellanous //
//////////////////

#ifdef AIL_DA_IMPL
AIL_DA(str) ail_fs_get_files_in_dir(const char *dirpath)
{
    AIL_UNUSED(dirpath);
    AIL_DA(str) files = ail_da_new_with_cap(str, 16);
    AIL_TODO();
    return files;
}
#endif

bool ail_fs_dir_exists(const char *dirpath)
{
    struct stat sb;
    return stat(dirpath, &sb) == 0 && S_ISDIR(sb.st_mode);
}

const char *ail_fs_get_file_ext(const char *filename)
{
    u32 idx = 0;
    for (u32 i = 0; filename[i]; i++) {
        if (filename[i] == '.') idx = i + 1;
    }
    return &filename[idx];
}

bool ail_fs_is_file_ext(const char *restrict filename, const char *restrict ext)
{
    const char *file_ext = ail_fs_get_file_ext(filename);
    return ail_fs_str_eq(file_ext, ext);
}

bool ail_fs_is_file(const char *path)
{
    struct stat result = {0};
    stat(path, &result);
    return S_ISREG(result.st_mode);
}

#endif // _AIL_FS_IMPL_GUARD_
#endif // AIL_FS_IMPL