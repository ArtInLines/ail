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

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <shellapi.h>
typedef HANDLE AIL_Build_Proc;
#define AIL_BUILD_PROC_INVALID INVALID_HANDLE_VALUE
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
typedef pid_t AIL_Build_Proc;
#define AIL_BUILD_PROC_INVALID (-1)
#endif // _WIN32

AIL_BUILD_DEF void  ail_build_rebuild_urself(i32 argc, char **argv);
AIL_BUILD_DEF char* ail_build_get_exe_name(const char *s, u32 n, AIL_Allocator allocator);
AIL_Build_Proc ail_build_proc_start(AIL_DA(str) *argv);
b32 ail_build_proc_wait(AIL_Build_Proc proc);
AIL_BUILD_DEF void  ail_build(AIL_Build_Comp cc, AIL_Build_Flags cflags, char *out_name, char *fpath, AIL_Allocator allocator);
AIL_BUILD_DEF AIL_Build_Comp ail_build_comp_from_str(char *str);


#ifdef AIL_BUILD_IMPL
#ifndef _AIL_BUILD_IMPL_GUARD_
#define _AIL_BUILD_IMPL_GUARD_

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
char* ail_build_cmd_to_str(AIL_DA(str) cmd)
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

// @Note: Code taken almost 1-to-1 from tsoding's nob.h (https://github.com/tsoding/musializer/blob/master/nob.h)
AIL_Build_Proc ail_build_proc_start(AIL_DA(str) *argv)
{
    if (!argv->len) {
        printf("Error: Could not run empty command.\n");
        return AIL_BUILD_PROC_INVALID;
    }
    AIL_Build_Proc res;
    char *arg_str = ail_build_cmd_to_str(*argv);
    printf("CMD: '%s'\n", arg_str);
#ifdef _WIN32
    // https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output
    STARTUPINFO siStartInfo;
    ZeroMemory(&siStartInfo, sizeof(siStartInfo));
    siStartInfo.cb = sizeof(STARTUPINFO);
    // @Note: theoretically setting NULL to std handles should not be a problem
    // https://docs.microsoft.com/en-us/windows/console/getstdhandle?redirectedfrom=MSDN#attachdetach-behavior
    // @TODO: check for errors in GetStdHandle
    siStartInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
    siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    siStartInfo.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
    siStartInfo.dwFlags   |= STARTF_USESTDHANDLES;
    PROCESS_INFORMATION piProcInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    BOOL succ = CreateProcessA(NULL, arg_str, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo);
    if (!succ) {
        printf("Error: Could not create child process.\n");
        res = AIL_BUILD_PROC_INVALID;
    } else {
        CloseHandle(piProcInfo.hThread);
        res = piProcInfo.hProcess;
    }
#else
    pid_t cpid = fork();
    if (cpid < 0) {
        printf("Error: Could not create child process.\n");
        res = AIL_BUILD_PROC_INVALID;
    } else {
        ail_da_push(argv, NULL);
        if (cpid == 0) {
            if (execvp(argv.data[0], (char* const*) argv.data) < 0) {
                printf("Error: Could not execute child process.\n");
                exit(1);
            }
            AIL_UNREACHABLE();
        }
        res = cpid;
    }
#endif
    AIL_CALL_FREE(argv->allocator, arg_str);
    return res;
}

// @Note: Code taken almost 1-to-1 from tsoding's nob.h (https://github.com/tsoding/musializer/blob/master/nob.h)
b32 ail_build_proc_wait(AIL_Build_Proc proc)
{
    if (proc == AIL_BUILD_PROC_INVALID) return false;
#ifdef _WIN32
    DWORD result = WaitForSingleObject(proc, INFINITE);
    if (result == WAIT_FAILED) {
        printf("Error: Could not wait on child process: %d\n", GetLastError());
        return false;
    }
    DWORD exit_status;
    if (!GetExitCodeProcess(proc, &exit_status)) {
        printf("Error: Could not get process exit code: %d\n", GetLastError());
        return false;
    }
    if (exit_status != 0) {
        printf("Error: Command exited with exit code %d\n", exit_status);
        return false;
    }
    CloseHandle(proc);
    return true;
#else
    for (;;) {
        int wstatus = 0;
        if (waitpid(proc, &wstatus, 0) < 0) {
            printf("Error: Could not wait on command (pid %d): %s\n", proc, strerror(errno));
            return false;
        }
        if (WIFEXITED(wstatus)) {
            int exit_status = WEXITSTATUS(wstatus);
            if (exit_status != 0) {
                printf("Error: Command exited with exit code %d\n", exit_status);
                return false;
            }
            break;
        }
        if (WIFSIGNALED(wstatus)) {
            printf("Error: Command process was terminated by %s\n", strsignal(WTERMSIG(wstatus)));
            return false;
        }
    }
#endif
}

void ail_build(AIL_Build_Comp cc, AIL_Build_Flags cflags, char *out_name, char *fpath, AIL_Allocator allocator)
{
    AIL_DA(str) argv = ail_da_new_with_alloc(str, 256, allocator);
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
    ail_build_proc_start(&argv);
    ail_da_free(&argv);
}

#endif // _AIL_BUILD_IMPL_GUARD_
#endif // AIL_BUILD_IMPL

#endif // AIL_BUILD_H_
