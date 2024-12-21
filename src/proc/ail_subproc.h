/*
*** Cross-Platform Subprocess Wrapper ***
*
* Define AIL_NO_SUBPROC_IMPL to not include any implementations from this file
* Define AIL_SUBPROC_PIPE_SIZE to set the size of the pipe used for reading the subprocess' stdout/stderr
*
*/

#ifndef _AIL_SUBPROC_H_
#define _AIL_SUBPROC_H_

#include "../base/ail_base.h"
#include "../base/ail_str.h"
#include "../fs/ail_term.h" // @Cleanup

#ifndef AIL_SUBPROC_PIPE_SIZE
#   define AIL_SUBPROC_PIPE_SIZE AIL_KB(2)
#endif

#if defined(_WIN32) || defined(__WIN32__)
    AIL_WARN_PUSH
    AIL_WARN_DISABLE(AIL_WARN_ALL)
#   include <windows.h>
#   include <direct.h>
#   include <shellapi.h>
    AIL_WARN_POP
#else
#   include <sys/types.h>
#   include <sys/wait.h>
#   include <sys/stat.h>
#   include <termios.h>
#   include <unistd.h>
#	include <stdio.h>
#endif // _WIN32

typedef struct AIL_Subproc_Res {
    i32 exitCode;
    b32 finished;
} AIL_Subproc_Res;


// Forward declarations of functions, that all platforms need to implement
internal AIL_Subproc_Res ail_subproc_exec_internal(AIL_DA(pchar) *argv, char *arg_str, AIL_Allocator allocator);
internal void ail_subproc_init(void);


global AIL_TermState subproc_term_state;

#endif // _AIL_SUBPROC_H_


#if !defined(AIL_NO_SUBPROC_IMPL) && !defined(AIL_NO_PROC_IMPL) && !defined(AIL_NO_IMPL)
#ifndef AIL_SUBPROC_IMPL_GUARD
#define AIL_SUBPROC_IMPL_GUARD
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

internal void ail_subproc_init(void)
{
	subproc_term_state = ail_term_global_current_state;
    ail_term_add_mode(AIL_TERM_MODE_VPROC);
}

internal void ail_subproc_deinit(void)
{
	ail_term_set_state(subproc_term_state);
}

internal AIL_Subproc_Res ail_subproc_exec(AIL_DA(pchar) *argv, char *arg_str, AIL_Allocator allocator)
{
    if (!argv->len) {
        // log_err("Cannot run empty command");
        return (AIL_Subproc_Res){0};
    }
    // log_info("Running '%s'...", arg_str);
    return ail_subproc_exec_internal(argv, arg_str, allocator);
}

internal void ail_subproc_print_output(AIL_Str out)
{
    if (out.len == 1 && out.data[0] == '\n') out.len = 0;

    // @Note: Certain ANSI escape codes should not be forwarded to the console to prevent weird artifacts
    // Currently these codes are specifically some erase functions
    // For a full list of existing ansi escape codes, see this handy cheatsheet: https://gist.github.com/ConnerWill/d4b6c776b509add763e17f9f113fd25b
    AIL_Str forbidden_seqs[] = {
        ail_str8_from_lit("\x1b[H"),  // Moves cursor to position 0,0
        ail_str8_from_lit("\x1b[1J"), // Eares from cursor to beginning of screen
        ail_str8_from_lit("\x1b[2J"), // Eares entire screen
    };
    // @Note: Certain ANSI escape codes may change the console state (i.e. changing color mode)
    // thus we need to save the previous state and restore it after printing is done
    AIL_TermState state = ail_term_global_current_state;
    while (out.len) {
        AIL_Str_Find_Of_Res res = ail_str_find_of(out, forbidden_seqs, ail_arrlen(forbidden_seqs));
        if (res.str_idx < 0) {
            printf("%s", out.data);
            break;
        } else {
            char c = out.data[res.str_idx];
            *((char *)(out.data + res.str_idx)) = 0;
            printf("%s", out.data);
            *((char *)(out.data + res.str_idx)) = c;
            out = ail_str_offset(out, res.str_idx + forbidden_seqs[res.needle_idx].len);
        }
    }
    ail_term_set_state(state);
}


#if AIL_OS_WIN
//////////////////////////
// Windows Implementation
//////////////////////////

// Code mostly adapted from the following documentation (with lots of experimentation until it worked properly):
// - https://learn.microsoft.com/en-us/windows/console/creating-a-pseudoconsole-session
// - https://learn.microsoft.com/en-us/windows/win32/ProcThread/creating-a-child-process-with-redirected-input-and-output
internal AIL_Subproc_Res ail_subproc_exec_internal(AIL_DA(pchar) *argv, char *arg_str, AIL_Allocator allocator)
{
    AIL_UNUSED(argv);
    AIL_Subproc_Res res = {
        .exitCode = 0,
        .finished = false,
    };
    HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE pipe_in_read = 0, pipe_in_write = 0;
    HANDLE pipe_out_read = 0, pipe_out_write = 0;
    HPCON hpc = NULL;
    AIL_DA(u8) buf = ail_da_new_with_alloc(u8, AIL_SUBPROC_PIPE_SIZE, allocator);
    SECURITY_ATTRIBUTES saAttr = {0};
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    AIL_UNUSED(saAttr);
    if (!CreatePipe(&pipe_in_read, &pipe_in_write, NULL, 0)) {
        // log_err("Could not establish pipe to child process");
        goto done;
    }
    if (!CreatePipe(&pipe_out_read, &pipe_out_write, NULL, 0)) {
        // log_err("Could not establish secondary pipes to child process");
        CloseHandle(pipe_in_read);
        goto done;
    }

    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if (!GetConsoleScreenBufferInfo(stdout_handle, &console_info)) {
        // log_err("Could not retrieve information about parent console");
        goto done;
    }
    if (FAILED(CreatePseudoConsole(console_info.dwSize, pipe_in_read, pipe_out_write, 0, &hpc))) {
        // log_err("Could not establish pseudo console");
        goto done;
    }

    STARTUPINFOEXW si;
    ZeroMemory(&si, sizeof(si));
    si.StartupInfo.cb = sizeof(STARTUPINFOEXW);
    size_t listSize;
    InitializeProcThreadAttributeList(NULL, 1, 0, &listSize);
    si.lpAttributeList = ail_call_alloc(allocator, listSize);
    if (!si.lpAttributeList) {
        // log_err("Could not allocate enough memory to provide complex startup info to child process");
        goto done;
    }
    if (!InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &listSize)) {
        // log_err("Could not initialize startup infor for child process");
        goto done;
    }
    if (!UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE, hpc, sizeof(hpc), NULL, NULL)) {
        printf("Error: %lu\n", GetLastError());
        // log_err("Could not set the child process as a pseudo console");
        goto done;
    }

    si.StartupInfo.hStdError  = stderr_handle;
    si.StartupInfo.hStdOutput = stdout_handle;
    if (si.StartupInfo.hStdOutput == INVALID_HANDLE_VALUE || si.StartupInfo.hStdError == INVALID_HANDLE_VALUE) {
        // log_err("Could not get the handles to child process stdout/stderr");
        goto done;
    }
    si.StartupInfo.hStdInput  = pipe_in_read;
    si.StartupInfo.hStdOutput = pipe_in_write;
    si.StartupInfo.hStdError  = pipe_in_write;
    si.StartupInfo.dwFlags   |= STARTF_USESTDHANDLES;
    PROCESS_INFORMATION piProcInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    size_t nConverted;
    wchar_t *w_arg_str = ail_call_alloc(allocator, sizeof(wchar_t)*(strlen(arg_str) + 1));
    mbstowcs_s(&nConverted, w_arg_str, strlen(arg_str) + 1, arg_str, strlen(arg_str) + 1);
    if (!CreateProcessW(NULL, w_arg_str, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &si.StartupInfo, &piProcInfo)) {
        // log_err("Could not create child process");
        goto done;
    }
    ail_call_free(allocator, w_arg_str);

    if (WaitForSingleObject(piProcInfo.hProcess, INFINITE) == WAIT_FAILED) {
        // log_err("Failed to wait for child process to exit");
        goto done;
    }
    DWORD nExitCode;
    if (!GetExitCodeProcess(piProcInfo.hProcess, &nExitCode)) {
        // log_err("Failed to retrieve exit code of child process");
        goto done;
    }
    res.exitCode = nExitCode;

    DWORD nBytesRead, nBytesWritten;
    // @Note: We need to write at least one byte into the pipe, or else the ReadFile will block forever, waiting for something to read.
    WriteFile(pipe_out_write, "\n", 1, &nBytesWritten, NULL);
    for (;;) {
        u32 n = buf.cap - buf.len;
        ail_assert(n >= AIL_SUBPROC_PIPE_SIZE);
        if (!ReadFile(pipe_out_read, &buf.data[buf.len], n, &nBytesRead, 0)) {
            // log_err("Failed to read stdout from child process");
            goto done;
        }
        buf.len += nBytesRead;
        if (nBytesRead < n) break;
        ail_da_resize(&buf, buf.len + AIL_SUBPROC_PIPE_SIZE);
    }
    ail_da_push(&buf, 0);
    buf.len--;
    res.finished = true;
    ail_subproc_print_output(ail_str_from_arr(buf));

done:
    if (pipe_in_read)        CloseHandle(pipe_in_read);
    if (pipe_in_write)       CloseHandle(pipe_in_write);
    if (pipe_out_read)       CloseHandle(pipe_out_read);
    if (pipe_out_write)      CloseHandle(pipe_out_write);
    if (piProcInfo.hProcess) CloseHandle(piProcInfo.hProcess);
    if (hpc)                 ClosePseudoConsole(hpc);
    if (buf.data)            ail_da_free(&buf);
    if (si.lpAttributeList)  ail_call_free(allocator, si.lpAttributeList);
    return res;
}


#else
////////////////////////
// POSIX Implementation
////////////////////////

AIL_Subproc_Res ail_subproc_exec_internal(AIL_DA(pchar) *argv, char *arg_str, AIL_Allocator allocator)
{
    AIL_UNUSED(arg_str);
    AIL_Subproc_Res res = { 0 };
    int pipefd[2] = {0};
    if (pipe(pipefd) < 0) {
    	memset(pipefd, 0, sizeof(pipefd));
        log_err("Could not establish pipe to child process: %s", strerror(errno));
        goto done;
    }

    AIL_DA(char) da = ail_da_new_with_alloc(char, AIL_SUBPROC_PIPE_SIZE, allocator);
    char buf[AIL_SUBPROC_PIPE_SIZE] = {0};
    pid_t cpid = fork();
    if (cpid < 0) {
        log_err("Could not create child process");
        goto done;
    } else if (cpid == 0) { // Run by child
        ail_da_push(argv, NULL);
        close(pipefd[0]);
        execvp(argv->data[0], (char* const*) argv->data);
        while (read(STDOUT_FILENO, buf, AIL_SUBPROC_PIPE_SIZE) != EOF) {
            u64 len = strlen(buf);
            ail_da_pushn(&da, buf, len);
            memset(buf, 0, len);
        }
        write(pipefd[1], da.data, da.len);
        close(pipefd[1]); // Required for reader to see EOF
    } else { // Run by parent
        close(pipefd[1]);
        while (read(pipefd[0], buf, AIL_SUBPROC_PIPE_SIZE) != EOF) {
            u64 len = strlen(buf);
            if (!len) break;
            ail_da_pushn(&da, buf, len);
            memset(buf, 0, len);
        }
        int wstatus = 0;
        if (waitpid(cpid, &wstatus, 0) < 0) {
            log_err("Failed to wait for child process to exit");
            goto done;
        }
        if (WIFEXITED(wstatus)) res.exitCode = WEXITSTATUS(wstatus);

        ail_da_push(&da, 0);
        da.len--;
        res.finished = true;
        ail_subproc_print_output(ail_str_from_da_nil_term(da));

done:
        if (pipefd[0]) close(pipefd[0]);
        if (da.data)   ail_da_free(&da);
    }
    return res;
}
#endif

AIL_WARN_POP
#endif // AIL_SUBPROC_IMPL_GUARD
#endif // AIL_NO_SUBPROC_IMPL
