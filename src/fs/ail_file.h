/*
*** File System Utilities ***
* @TODO
*/

#ifndef _AIL_FILE_H_
#define _AIL_FILE_H_

#include "../base/ail_base.h"
#include "../base/ail_alloc.h" // For ail_mem_copy
#include <fcntl.h>    // For file access flags
#include <sys/stat.h> // For file stats

#ifndef AIL_FS_READ_TIMEOUT
#   define AIL_FS_READ_TIMEOUT 500 // in milliseconds
#endif // AIL_FS_READ_TIMEOUT
#ifndef AIL_FS_WRITE_TIMEOUT
#   define AIL_FS_WRITE_TIMEOUT 500 // in milliseconds
#endif // AIL_FS_WRITE_TIMEOUT
#ifndef AIL_FS_MAX_ATTEMPTS
#   define AIL_FS_MAX_ATTEMPTS 8
#endif // AIL_FS_MAX_ATTEMPTS

#if AIL_OS_WIN
#   include <windows.h>
#   include <direct.h>
#   define mkdir(dir, mode)      _mkdir(dir)
#   define open(name, ...)       _open(name, __VA_ARGS__)
#   define read(fd, buf, count)  _read(fd, buf, count)
#   define close(fd)             _close(fd)
#   define write(fd, buf, count) _write(fd, buf, count)
#   define dup2(fd1, fd2)        _dup2(fd1, fd2)
#   define unlink(file)          _unlink(file)
#   define rmdir(dir)            _rmdir(dir)

#   define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#   define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#else
#   include <dirent.h>
#endif

///////////////////
// Files/Directories
///////////////////

#if AIL_OS_WIN
#   define AIL_FS_DIRENT_NAME_LEN (MAX_PATH + MAX_PATH + 1)
#else
#   define AIL_FS_DIRENT_NAME_LEN (256)
#endif

typedef enum AIL_FS_Error {
    AIL_FS_ERR_SUCCESS,           // No error
    AIL_FS_ERR_ACCESS,            // No access for the desired action
    AIL_FS_ERR_BAD_HANDLE,        // the provided handle was invalid
    AIL_FS_ERR_OPEN_HANDLE_LIMIT, // Reached limit of open files/directories
    AIL_FS_ERR_NO_ENTRY,          // The desired file/directory doesn't exist
    AIL_FS_ERR_NO_MEMORY,         // Not enough memory to do the desired action
    AIL_FS_ERR_NOT_DIR,           // The provided target is not a directory
    AIL_FS_ERR_NOT_FILE,          // The provided target is not a file
} AIL_FS_Error;

typedef enum AIL_FS_Entry_Type {
    AIL_FS_ENTRY_FILE,
    AIL_FS_ENTRY_DIR,
    AIL_FS_ENTRY_OTHER,
} AIL_FS_Entry_Type;

typedef struct AIL_FS_Dirent {
    char name[AIL_FS_DIRENT_NAME_LEN];
    AIL_FS_Entry_Type type;
    AIL_FS_Error err;
} AIL_FS_Dirent;
static const AIL_FS_Dirent AIL_FS_DIRENT_NIL = {0};

typedef struct AIL_FS_Read_Dir_Res {
    void* handle;
    AIL_FS_Dirent dirent;
    AIL_FS_Error err;
} AIL_FS_Read_Dir_Res;

inline_func b32 ail_fs_dirent_is_nil(AIL_FS_Dirent a);
internal AIL_FS_Read_Dir_Res ail_fs_read_dir_init(const char *dirpath);
internal AIL_FS_Dirent ail_fs_read_dir_next(AIL_FS_Read_Dir_Res dir);
internal void ail_fs_read_dir_deinit(AIL_FS_Read_Dir_Res dir);

internal b32 ail_fs_open_file(const char *fpath, u64 *file, b32 writeable);

internal void ail_fs_close_file(u64 file);

// Reads n bytes from file 'fname' into 'buf'
// @Note: 'fd' be valid File-Handle on Windows (a void pointer cast to a u64) and a file-descriptor on Unix
// @Note: 'buf' must have space for 'maxN' bytes
// @Note: 'actualN' will contain the amount of bytes, that were actually read into 'buf'
internal b32 ail_fs_read_n_bytes(u64 fd, void *buf, u64 maxN, u64 *actualN);

// Same as ail_fs_read_n_bytes(), except that it handles opening and closing the file
internal b32 ail_fs_read_file(const char *fpath, void *buf, u64 maxN, u64 *actualN);

// Same as ail_fs_read_file(), except that it checks the file's size first and allocates an output buffer of the appropriate size for it
// Returns NULL on error
internal u8* ail_fs_read_entire_file(const char *fpath, u64 *size, AIL_Allocator allocator);

// Write `size` many bytes from `buf` into `fpath`
internal b32  ail_fs_write_file(const char *fpath, const char *buf, u64 size);

//////////////////
// Miscellanous //
//////////////////

// @Important: Not yet implemented
internal AIL_DA(pchar) ail_fs_get_files_in_dir(const char *dirpath);

inline_func b32 ail_fs_dir_exists(const char *dirpath);
internal const char *ail_fs_get_file_ext(const char *filename);
internal b32 ail_fs_is_file_ext(const char *restrict filename, const char *restrict ext);
internal b32 ail_fs_is_file(const char *path);

#endif // _AIL_FILE_H_



#if !defined(AIL_NO_FS_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_FS_IMPL_GUARD_
#define _AIL_FS_IMPL_GUARD_

///////////////////////
// Utility functions //
///////////////////////

b32 ail_fs_str_eq(const char *restrict a, const char *restrict b)
{
    while (*a && *b && *a++ == *b++) {}
    return *a == *b && !*a;
}

//////////////////////
// Read Directories //
//////////////////////

b32 ail_fs_dirent_is_nil(AIL_FS_Dirent a)
{
    return !a.name[0];
}

AIL_FS_Read_Dir_Res ail_fs_read_dir_init(const char *dirpath)
{
    ail_assert(dirpath);
    AIL_FS_Read_Dir_Res res = { 0 };
#if AIL_OS_WIN
    WCHAR buffer[MAX_PATH];
    swprintf_s(buffer, MAX_PATH, L"%hs\\*", dirpath);
    WIN32_FIND_DATAW ffd;
    res.handle = FindFirstFileW(buffer, &ffd);
    if (res.handle == INVALID_HANDLE_VALUE) res.err = AIL_FS_ERR_NO_ENTRY;
    else {
        // @TODO
        i32 len = WideCharToMultiByte(CP_UTF8, 0, buffer, lstrlenW(buffer), res.dirent.name, sizeof(res.dirent.name), NULL, NULL);
        AIL_UNUSED(len);
        res.dirent.type = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? AIL_FS_ENTRY_DIR : AIL_FS_ENTRY_FILE;
    }
#else
    AIL_TODO();
#endif
    return res;
}

AIL_FS_Dirent ail_fs_read_dir_next(AIL_FS_Read_Dir_Res dir)
{
    AIL_FS_Dirent res = dir.dirent;
    if (AIL_LIKELY(ail_fs_dirent_is_nil(res))) {
#if AIL_OS_WIN
        WIN32_FIND_DATAW ffd;
        if (!FindNextFileW(dir.handle, &ffd)) {
            DWORD err = GetLastError();
            if (err != ERROR_NO_MORE_FILES) {
                // @TODO: Figure out what error actually happened
                res.err = AIL_FS_ERR_NO_ENTRY;
            }
        } else {
            ail_assert(ail_arrlen(res.name) >= ail_arrlen(ffd.cFileName));
            ail_mem_copy(res.name, ffd.cFileName, ail_arrlen(ffd.cFileName));
            res.type = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? AIL_FS_ENTRY_DIR : AIL_FS_ENTRY_FILE;
        }
#else
        AIL_TODO();
#endif
    } else {
        dir.dirent = AIL_FS_DIRENT_NIL;
    }
    return res;
}

void ail_fs_read_dir_deinit(AIL_FS_Read_Dir_Res dir)
{
#if AIL_OS_WIN
    FindClose(dir.handle);
#else
    AIL_TODO();
#endif
}



///////////////////
// Read/Write IO //
///////////////////

b32 ail_fs_open_file(const char *fpath, u64 *file, b32 writeable)
{
#if AIL_OS_WIN
    u32 access = GENERIC_READ;
    if (writeable) access |= GENERIC_WRITE;
    void *handle = CreateFile(fpath, access, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, 0);
    if (handle == INVALID_HANDLE_VALUE) return false;
    *file = (u64)handle;
    return true;
#else
    u32 access = O_RDONLY;
    if (writeable) access = O_RDWR;
    int fd = open(fpath, access, 0777);
    if (fd == -1) return false;
    *file = fd;
    return true;
#endif // _WIN32
}

void ail_fs_close_file(u64 file)
{
#if AIL_OS_WIN
    CloseHandle((void *)file);
#else
    close(file);
#endif // _WIN32
}

b32 ail_fs_read_n_bytes(u64 fd, void *buf, u64 maxN, u64 *actualN)
{
#if AIL_OS_WIN
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
    // We wait at most for AIL_FS_MAX_ATTEMPTS*AIL_FS_READ_TIMEOUT and return -1 otherwise
    for (u8 i = 0; i < AIL_FS_MAX_ATTEMPTS && res < 0; i++) {
        DWORD dwRes = WaitForSingleObject(osReader.hEvent, AIL_FS_READ_TIMEOUT);
        switch(dwRes) {
            case WAIT_OBJECT_0: {
                // Read completed.
                b32 succ = GetOverlappedResult(file, &osReader, &nRead, FALSE);
                res = succ ? 1 : 0;
            } break;
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

b32 ail_fs_read_file(const char *fpath, void *buf, u64 maxN, u64 *actualN)
{
    u64 file;
    b32 res = ail_fs_open_file(fpath, &file, false);
    if (res) res = ail_fs_read_n_bytes((u64)file, buf, maxN, actualN);
    ail_fs_close_file(file);
    return res;
}

u8* ail_fs_read_entire_file(const char *fpath, u64 *size, AIL_Allocator allocator)
{
    // Adapted from https://stackoverflow.com/a/68156485/13764271
    u8* buf = NULL;
    *size = 0;
    struct stat sb;
    if (stat(fpath, &sb) != -1) {
        if (sb.st_size) {
            buf = ail_call_alloc(allocator, sb.st_size);
            if (buf) {
                ail_fs_read_file(fpath, buf, sb.st_size, size);
            }
        }
    }
    return buf;
}

b32 ail_fs_write_n_bytes(u64 fd, const char *buf, u64 size)
{
#if AIL_OS_WIN
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
        res = 1;
    }
    for (u32 i = 0; i < AIL_FS_MAX_ATTEMPTS && res < 0; i++) {
        DWORD dwRes = WaitForSingleObject(osWrite.hEvent, AIL_FS_WRITE_TIMEOUT);
        switch(dwRes) {
            case WAIT_OBJECT_0: {
                // Write completed
                b32 succ = GetOverlappedResult(file, &osWrite, &dwWritten, FALSE);
                res = succ ? 1 : 0;
            } break;
            case WAIT_TIMEOUT:
                // Gotta wait a little longer to finish operation
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

b32 ail_fs_write_file(const char *fpath, const char *buf, u64 size)
{
    u64 file;
    b32 res = ail_fs_open_file(fpath, &file, true);
    if (res) res = ail_fs_write_n_bytes((u64)file, buf, size);
    ail_fs_close_file(file);
    return res;
}

//////////////////
// Miscellanous //
//////////////////

AIL_DA(pchar) ail_fs_get_files_in_dir(const char *dirpath)
{
    AIL_UNUSED(dirpath);
    AIL_DA(pchar) files = ail_da_new_with_cap(pchar, 16);
    AIL_TODO();
    return files;
}

b32 ail_fs_dir_exists(const char *dirpath)
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

b32 ail_fs_is_file_ext(const char *restrict filename, const char *restrict ext)
{
    const char *file_ext = ail_fs_get_file_ext(filename);
    return ail_fs_str_eq(file_ext, ext);
}

b32 ail_fs_is_file(const char *path)
{
    struct stat result = {0};
    stat(path, &result);
    return S_ISREG(result.st_mode);
}

#endif // _AIL_FS_IMPL_GUARD_
#endif // AIL_NO_FS_IMPL
