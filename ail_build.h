// Build System for writing build-scripts in C
//
// Strongly inspired by Tsoding's nob.h (https://github.com/tsoding/musializer/blob/master/nob.h)
// @Note: Works but barely useable yet
//
// LICENSE regarding Tsoding's code
/*
Copyright 2023 Alexey Kutepov <reximkut@gmail.com> and Musializer Contributors

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//
// LICENSE regarding modifications & original code
/*
Copyright (c) 2024 Lily Val Richter

Permission is hereby granted, free_one of charge, to any person obtaining a copy
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

#ifndef AIL_BUILD_H_
#define AIL_BUILD_H_

#define AIL_ALL_IMPL
#include "ail.h"

#ifndef AIL_BUILD_DEF
#ifdef  AIL_DEF
#define AIL_BUILD_DEF AIL_DEF
#else
#define AIL_BUILD_DEF
#endif // AIL_DEF
#endif // AIL_BUILD_DEF
#ifndef AIL_BUILD_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_BUILD_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_BUILD_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_BUILD_DEF_INLINE

typedef enum AIL_Build_Comp_Type {
    AIL_BUILD_COMP_DEFAULT,
    AIL_BUILD_COMP_MSVC,
    AIL_BUILD_COMP_GCC,
    AIL_BUILD_COMP_CLANG,
    AIL_BUILD_COMP_ZIG,
    AIL_BUILD_COMP_TCC,
    AIL_BUILD_COMP_INTEL,
    AIL_BUILD_COMP_INTEL_CL,
    AIL_BUILD_COMP_DMC,
    AIL_BUILD_COMP_PELLESC,
    AIL_BUILD_COMP_OTHER,
    AIL_BUILD_COMP_COUNT,
} AIL_Build_Comp_Type;

typedef struct AIL_Build_Comp {
    AIL_Build_Comp_Type type;
    char *name;
} AIL_Build_Comp;

#define AIL_BUILD_COMP_IS_SPECIAL(cc) (cc == AIL_BUILD_COMP_DEFAULT || cc == AIL_BUILD_COMP_OTHER || cc == AIL_BUILD_COMP_COUNT)

typedef enum AIL_Build_Flags {
    AIL_BUILD_FLAG_DEBUG   = 1 << 0,
    AIL_BUILD_FLAG_O1      = 1 << 1,
    AIL_BUILD_FLAG_O2      = 1 << 2,
    AIL_BUILD_FLAG_O3      = 1 << 3,
    AIL_BUILD_FLAG_OSIZE   = 1 << 4,
    AIL_BUILD_FLAG_WALL    = 1 << 5,
    AIL_BUILD_FLAG_STD_C99 = 1 << 6,
    // @TODO: Add more options
} AIL_Build_Flags;

#include <stdio.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <shellapi.h>
typedef HANDLE AIL_Build_OS_Pipe;
typedef HANDLE AIL_Build_OS_Proc;
#define _AIL_BUILD_INVALID_OS_PROC_ INVALID_HANDLE_VALUE
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
typedef int AIL_Build_OS_Pipe;
typedef int AIL_Build_OS_Proc;
#define _AIL_BUILD_INVALID_OS_PROC_ (-1)
#endif // _WIN32

typedef struct {
    char *out;
    b32 succ;
} AIL_Build_Proc_Res;

AIL_BUILD_DEF void  ail_build_rebuild_urself(i32 argc, char **argv);
AIL_BUILD_DEF char* ail_build_get_exe_name(const char *s, u32 n, AIL_Allocator allocator);
AIL_BUILD_DEF AIL_Build_Proc_Res ail_build_proc_exec(AIL_DA(pchar) *argv, AIL_Allocator allocator);
AIL_BUILD_DEF void  ail_build(AIL_Build_Comp cc, AIL_Build_Flags cflags, char *out_name, char *fpath, AIL_Allocator allocator);
AIL_BUILD_DEF AIL_Build_Comp ail_build_comp_from_str(char *str);

#endif // AIL_BUILD_H_



#if !defined(AIL_NO_BUILD_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_BUILD_IMPL_GUARD_
#define _AIL_BUILD_IMPL_GUARD_

#ifndef AIL_BUILD_PIPE_SIZE
#define AIL_BUILD_PIPE_SIZE 2048
#endif

// Case-insensitive str-compare - expects `b` to be lowercase
#define _AIL_BUILD_STREQ_I(a_str, a_len, b_literal) ail_build_streq_i(a_str, a_len, b_literal, (sizeof(b_literal) - 1))
b32 ail_build_streq_i(const char *restrict a, u64 alen, const char *restrict b, u64 blen)
{
    if (alen != blen) return false;
    for (u32 i = 0; i < alen; i++) {
        if (tolower(a[i]) != b[i]) return false;
    }
    return true;
}

void  ail_build_rebuild_urself(i32 argc, char **argv)
{
    AIL_TODO();
}

AIL_Build_Comp ail_build_comp_from_str(char *str)
{
    AIL_Build_Comp comp = { 0 };
    u64 len = strlen(str);
    if (_AIL_BUILD_STREQ_I(str, len, "cl") || _AIL_BUILD_STREQ_I(str, len, "msvc")) {
        comp.type = AIL_BUILD_COMP_MSVC;;
    } else if (_AIL_BUILD_STREQ_I(str, len, "gcc") || _AIL_BUILD_STREQ_I(str, len, "gnu")) {
        comp.type = AIL_BUILD_COMP_GCC;
    } else if (_AIL_BUILD_STREQ_I(str, len, "clang") || _AIL_BUILD_STREQ_I(str, len, "llvm")) {
        comp.type = AIL_BUILD_COMP_CLANG;
    } else if (_AIL_BUILD_STREQ_I(str, len, "zig") || _AIL_BUILD_STREQ_I(str, len, "zig cc")) {
        comp.type = AIL_BUILD_COMP_ZIG;
    } else if (_AIL_BUILD_STREQ_I(str, len, "tcc") || _AIL_BUILD_STREQ_I(str, len, "tiny cc")) {
        comp.type = AIL_BUILD_COMP_TCC;
    } else if (_AIL_BUILD_STREQ_I(str, len, "intel") || _AIL_BUILD_STREQ_I(str, len, "icx-cc")) {
        comp.type = AIL_BUILD_COMP_INTEL;
    } else if (_AIL_BUILD_STREQ_I(str, len, "intel cl") || _AIL_BUILD_STREQ_I(str, len, "icx-cl")) {
        comp.type = AIL_BUILD_COMP_INTEL_CL;
    } else if (_AIL_BUILD_STREQ_I(str, len, "dmc")) {
        comp.type = AIL_BUILD_COMP_DMC;
    } else if (_AIL_BUILD_STREQ_I(str, len, "pocc") || _AIL_BUILD_STREQ_I(str, len, "pelles") || _AIL_BUILD_STREQ_I(str, len, "pelles c")) {
        comp.type = AIL_BUILD_COMP_PELLESC;
    } else {
        comp.type  = AIL_BUILD_COMP_OTHER;
        comp.name = str;
    }
    return comp;
}

char* ail_build_get_exe_name(const char *s, u32 n, AIL_Allocator allocator)
{
    // @TODO: Don't assume that we necessarily compile a file named '*.c'
    // @TODO: Add support for more platforms, like MacOS
#ifdef _WIN32
#   define _AIL_BUILD_EXE_EXT "exe"
#else
#   define _AIL_BUILD_EXE_EXT ""
#endif
    u32 ext_len = 1; // extension assumed to be '.c'
    char *buf = AIL_CALL_ALLOC(allocator, n - ext_len + sizeof(_AIL_BUILD_EXE_EXT));
    memcpy(buf, s, n - ext_len);
    memcpy(&buf[n - ext_len], _AIL_BUILD_EXE_EXT, sizeof(_AIL_BUILD_EXE_EXT));
    return buf;
}

// @Note: Code taken almost 1-to-1 from tsoding's nob.h (https://github.com/tsoding/musializer/blob/master/nob.h)
char* ail_build_cmd_to_str(AIL_DA(pchar) cmd)
{
    AIL_ASSERT(cmd.len);
    AIL_DA(char) str = ail_da_new_with_alloc(char, 256, cmd.allocator);
    ail_da_pushn(&str, cmd.data[0], strlen(cmd.data[0]));
    for (u32 i = 1; i < cmd.len; i++) {
        char *arg = cmd.data[i];
        AIL_ASSERT(arg);
        ail_da_push(&str, ' ');
        if (!strchr(arg, ' ')) {
            ail_da_pushn(&str, arg, strlen(arg));
        } else {
            ail_da_push(&str, '"');
            ail_da_pushn(&str, arg, strlen(arg));
            ail_da_push(&str, '"');
        }
    }
    ail_da_push(&str, 0);
    return str.data;
}

AIL_Build_Proc_Res ail_build_proc_exec(AIL_DA(pchar) *argv, AIL_Allocator allocator)
{
    AIL_Build_Proc_Res res = {
        .out  = "",
        .succ = false,
    };
    if (!argv->len) {
        printf("Error: Cannot run empty command.\n");
        return res;
    }
    char *arg_str = ail_build_cmd_to_str(*argv);
    printf("CMD: '%s'\n", arg_str);
#ifdef _WIN32
    AIL_Build_OS_Pipe pipe_read, pipe_write;
    SECURITY_ATTRIBUTES saAttr = {0};
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    if (!CreatePipe(&pipe_read, &pipe_write, &saAttr, 0)) {
        printf("Error: Could not establish pipe to child process\n");
        return res;
    }

    STARTUPINFO siStartInfo;
    ZeroMemory(&siStartInfo, sizeof(siStartInfo));
    siStartInfo.cb = sizeof(STARTUPINFO);

    siStartInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
    siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (siStartInfo.hStdOutput == INVALID_HANDLE_VALUE || siStartInfo.hStdError == INVALID_HANDLE_VALUE) {
        printf("Error: Could not get the handles to child process stdout/stderr.");
        return res;
    }
    siStartInfo.hStdOutput = pipe_write;
    siStartInfo.hStdError  = pipe_write;
    siStartInfo.dwFlags   |= STARTF_USESTDHANDLES;
    PROCESS_INFORMATION piProcInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    if (!CreateProcessA(NULL, arg_str, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)) {
        printf("Error: Could not create child process.\n");
        // @Note: Intentional Memory Leak - this library is only used for short-running build processes, leaking memory is fine here
        // CloseHandle(pipe_write);
        // CloseHandle(pipe_read);
        return res;
    }

    if (WaitForSingleObject(piProcInfo.hProcess, INFINITE) == WAIT_FAILED) {
        printf("Error: Failed to wait for child process to exit.\n");
        // @Note: Intentional Memory Leak - this library is only used for short-running build processes, leaking memory is fine here
        // CloseHandle(pipe_write);
        // CloseHandle(pipe_read);
        // CloseHandle(piProcInfo.hProcess);
        return res;
    }

    char *buf = AIL_CALL_ALLOC(allocator, AIL_BUILD_PIPE_SIZE);
    if (!buf) {
        printf("Error: Could not allocate enough memory to read child process' output.\n");
        return res;
    }
    DWORD nBytesRead;
    ReadFile(pipe_read, buf, AIL_BUILD_PIPE_SIZE, &nBytesRead, 0);
    res.out = buf;
#else
    AIL_Build_OS_Pipe pipefd[2];
    if (pipe(pipefd) != -1) {
        printf("Error: Could not establish pipe to child process.\n");
        return res;
    }

    AIL_DA(char) da = ail_da_new_with_alloc(char, AIL_BUILD_PIPE_SIZE, allocator);
    char buf[AIL_BUILD_PIPE_SIZE] = {0};
    pid_t cpid = fork();
    if (cpid < 0) {
        printf("Error: Could not create child process.\n");
        return res;
    } else if (cpid == 0) { // Run by child
        ail_da_push(argv, NULL);
        close(pipefd[0]);
        execvp(argv->data[0], (char* const*) argv->data);
        while (read(stdout, buf, AIL_BUILD_PIPE_SIZE) != EOF) {
            u64 len = strlen(buf);
            ail_da_pushn(&da, buf, len);
            memset(buf, 0, len);
        }
        write(pipefd[1], da.data, da.len);
        close(pipefd[1]); // Required for reader to see EOF
    } else { // Run by parent
        close(pipefd[1]);
        while (read(pipefd[0], buf, AIL_BUILD_PIPE_SIZE) != EOF) {
            u64 len = strlen(buf);
            ail_da_pushn(&da, buf, len);
            memset(buf, 0, len);
        }
        int wstatus = 0;
        if (waitpid(cpid, &wstatus, 0) < 0) {
            printf("Error: Failed to wait for child process to exit.\n");
            return res;
        }
        ail_da_push(&da, 0);
        res.out = da.data;
    }
#endif
    // @Note: Intentional Memory Leak - this library is only used for short-running build processes, leaking memory is fine here
    // AIL_CALL_FREE(argv->allocator, arg_str);
    res.succ = true;
    return res;
}

void ail_build(AIL_Build_Comp cc, AIL_Build_Flags cflags, char *out_name, char *fpath, AIL_Allocator allocator)
{
    AIL_DA(pchar) argv = ail_da_new_with_alloc(pchar, 256, allocator);
    switch (cc.type) {
        case AIL_BUILD_COMP_DEFAULT: {
            AIL_TODO(); // Pick compiler based on current platform and availability of compilers (how do I check that?)
        } break;
        case AIL_BUILD_COMP_MSVC: {
            ail_da_push(&argv, "cl");
            u64 out_name_len = strlen(out_name);
            char *buf = AIL_CALL_ALLOC(allocator, out_name_len + 4);
            memcpy(buf, "/Fe", 3);
            memcpy(&buf[3], out_name, out_name_len + 1);
            ail_da_push(&argv, buf);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "/Zi");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "/O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "/Ox");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "/O2");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "/Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "/Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "/std:c11"); // MSVC only supports C11, C17 & C-latest (https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)
        } break;
        case AIL_BUILD_COMP_GCC: {
            ail_da_push(&argv, "gcc");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_CLANG: {
            ail_da_push(&argv, "clang");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_ZIG: {
            ail_da_push(&argv, "zig cc");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_TCC: {
            ail_da_push(&argv, "tcc");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_INTEL: {
            ail_da_push(&argv, "icx-cc");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_INTEL_CL: {
            ail_da_push(&argv, "icx-cl");
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_DMC: {
            ail_da_push(&argv, "dmc");
            u64 out_name_len = strlen(out_name);
            char *buf = AIL_CALL_ALLOC(allocator, out_name_len + 3);
            memcpy(buf, "-o", 2);
            memcpy(&buf[2], out_name, out_name_len + 1);
            ail_da_push(&argv, buf);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-o1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-o2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-o3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-os");
            if (cflags & AIL_BUILD_FLAG_WALL)    {}
            if (cflags & AIL_BUILD_FLAG_STD_C99) {}
        } break;
        case AIL_BUILD_COMP_PELLESC: {
            ail_da_push(&argv, "cc");
            u64 out_name_len = strlen(out_name);
            char *buf = AIL_CALL_ALLOC(allocator, out_name_len + 4);
            memcpy(buf, "/Fe", 3);
            memcpy(&buf[3], out_name, out_name_len + 1);
            ail_da_push(&argv, buf);
            ail_da_push(&argv, fpath);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "/Zi");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "/Ob1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "/Ot");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "/Ox");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "/Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "/W2");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "/std:c99"); // MSVC only supports C11, C17 & C-latest (https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170)
        } break;
        case AIL_BUILD_COMP_OTHER: {
            ail_da_push(&argv, cc.name);
            ail_da_push(&argv, fpath);
            ail_da_push(&argv, "-o");
            ail_da_push(&argv, out_name);
            if (cflags & AIL_BUILD_FLAG_DEBUG)   ail_da_push(&argv, "-g");
            if (cflags & AIL_BUILD_FLAG_O1)      ail_da_push(&argv, "-O1");
            if (cflags & AIL_BUILD_FLAG_O2)      ail_da_push(&argv, "-O2");
            if (cflags & AIL_BUILD_FLAG_O3)      ail_da_push(&argv, "-O3");
            if (cflags & AIL_BUILD_FLAG_OSIZE)   ail_da_push(&argv, "-Os");
            if (cflags & AIL_BUILD_FLAG_WALL)    ail_da_push(&argv, "-Wall");
            if (cflags & AIL_BUILD_FLAG_STD_C99) ail_da_push(&argv, "-std=c99");
        } break;
        case AIL_BUILD_COMP_COUNT: {
            AIL_UNREACHABLE();
        } break;


    }
    AIL_Build_Proc_Res res = ail_build_proc_exec(&argv, allocator);
    if (res.succ) puts(res.out);
}

#endif // _AIL_BUILD_IMPL_GUARD_
#endif // AIL_NO_BUILD_IMPL
