/*
*** Cross-Platform Terminal I/O Wrapper ***
*
* Define AIL_NO_TERM_IMPL to not include any implementations from this file
*
*/

#ifndef AIL_TERM_H_
#define AIL_TERM_H_

#include "../base/ail_base.h"

#if defined(_WIN32) || defined(__WIN32__)
    AIL_WARN_PUSH
    AIL_WARN_DISABLE(AIL_WARN_ALL)
#	include <windows.h>
    AIL_WARN_POP
	typedef HANDLE AIL_TermHandle;
	typedef struct AIL_TermState { DWORD in, out, err; } AIL_TermState;
#else
#   include <termios.h>
	typedef int AIL_TermHandle;
	typedef struct termios AIL_TermState;
#endif

typedef struct AIL_TermHandles {
    AIL_TermHandle in, out, err;
} AIL_TermHandles;

typedef struct AIL_TermContext {
    AIL_TermHandles handles;
    AIL_TermState   initial, current;
} AIL_TermContext;

typedef enum AIL_FLAG_ENUM AIL_TermMode {
	AIL_TERM_MODE_ECHO       = 1 << 0, // Echo input stdin to stdout automatically
	AIL_TERM_MODE_LINE_INPUT = 1 << 1, // Input is only processed when enter is pressed
	AIL_TERM_MODE_INSERT     = 1 << 2, // Insert text instead of overriding text at specified position
	AIL_TERM_MODE_MOUSE      = 1 << 3, // Mouse input events enabled @Note: Only has an effect on Windows
	AIL_TERM_MODE_CTRL_PROC  = 1 << 4, // Certain control sequences (like Ctrl+c) are handled by the console
	AIL_TERM_MODE_VPROC      = 1 << 5, // Process virtual ansi codes
	AIL_TERM_MODE_FLAG_COUNT = 1 << 6, // Count of flags in this enum
} AIL_TermMode;


global AIL_TermHandles ail_term_global_handles;
global AIL_TermState   ail_term_global_initial_state;
global AIL_TermState   ail_term_global_current_state;


internal void ail_term_init(void);
internal void ail_term_deinit(void);
internal AIL_TermHandles ail_term_get_handles(void);
internal AIL_TermMode  ail_term_get_mode(void);
internal void ail_term_set_mode(AIL_TermMode mode);
internal void ail_term_add_mode(AIL_TermMode mode);
internal void ail_term_sub_mode(AIL_TermMode mode);

internal c32 ail_term_get_char(void);

internal void          ail_term_set_state(AIL_TermState state);
internal AIL_TermMode  ail_term_state_get_mode(AIL_TermState state);
internal AIL_TermState ail_term_state_set_mode(AIL_TermState state, AIL_TermMode mode);
internal AIL_TermState _ail_term_get_state(void);
internal AIL_TermState ail_term_state_add_mode(AIL_TermState state, AIL_TermMode mode);
internal AIL_TermState ail_term_state_sub_mode(AIL_TermState state, AIL_TermMode mode);

#endif // AIL_TERM_H_


#if !defined(AIL_NO_TERM_IMPL) && !defined(AIL_NO_IMPL)
#ifndef AIL_TERM_IMPL_GUARD
#define AIL_TERM_IMPL_GUARD

internal void term_init(void)
{
	ail_term_global_handles       = term_get_handles();
	ail_term_global_initial_state = _term_get_state();
	ail_term_global_current_state = ail_term_global_initial_state;
}

internal void term_deinit(void)
{
	term_set_state(ail_term_global_initial_state);
}

internal AIL_TermMode term_get_mode(void)
{
	return term_state_get_mode(ail_term_global_current_state);
}

internal AIL_TermState term_state_add_mode(AIL_TermState state, AIL_TermMode mode)
{
	AIL_TermMode cur = term_state_get_mode(ail_term_global_current_state);
	return term_state_set_mode(state, cur | mode);
}

internal AIL_TermState term_state_sub_mode(AIL_TermState state, AIL_TermMode mode)
{
	AIL_TermMode cur = term_state_get_mode(ail_term_global_current_state);
	return term_state_set_mode(state, cur & ~mode);
}

internal void term_set_mode(AIL_TermMode mode)
{
	term_set_state(term_state_set_mode(ail_term_global_current_state, mode));
}

internal void term_add_mode(AIL_TermMode mode)
{
	term_set_state(term_state_add_mode(ail_term_global_current_state, mode));
}

internal void term_sub_mode(AIL_TermMode mode)
{
	term_set_state(term_state_sub_mode(ail_term_global_current_state, mode));
}

internal int term_get_char(void)
{
	return getchar(); // @TODO: Rewrite without libc
}


#if defined(_WIN32) || defined(__WIN32__)
//////////////////////////
// Windows Implementation
//////////////////////////

internal AIL_TermHandles term_get_handles(void)
{
	AIL_TermHandles res = {
		.in  = GetStdHandle(STD_INPUT_HANDLE),
		.out = GetStdHandle(STD_OUTPUT_HANDLE),
		.err = GetStdHandle(STD_ERROR_HANDLE),
	};
	AIL_ASSERT(res.in  != INVALID_HANDLE_VALUE);
	AIL_ASSERT(res.out != INVALID_HANDLE_VALUE);
	AIL_ASSERT(res.err != INVALID_HANDLE_VALUE);
	return res;
}

internal AIL_TermState _term_get_state(void)
{
	AIL_TermState state;
	AIL_ASSERT(GetConsoleMode(ail_term_global_handles.in,  &state.in));
	AIL_ASSERT(GetConsoleMode(ail_term_global_handles.out, &state.out));
	AIL_ASSERT(GetConsoleMode(ail_term_global_handles.err, &state.err));
	return state;
}

internal AIL_TermMode term_state_get_mode(AIL_TermState state)
{
	AIL_STATIC_ASSERT(AIL_TERM_MODE_FLAG_COUNT == (1 << 6));
	AIL_TermMode mode = 0;
	if (state.in  & ENABLE_ECHO_INPUT)      mode |= AIL_TERM_MODE_ECHO;
	if (state.in  & ENABLE_LINE_INPUT)      mode |= AIL_TERM_MODE_LINE_INPUT;
	if (state.in  & ENABLE_INSERT_MODE)     mode |= AIL_TERM_MODE_INSERT;
	if (state.in  & ENABLE_MOUSE_INPUT)     mode |= AIL_TERM_MODE_MOUSE;
	if (state.in  & ENABLE_PROCESSED_INPUT) mode |= AIL_TERM_MODE_CTRL_PROC;
	if (state.out & ENABLE_PROCESSED_OUTPUT &&
		state.out & ENABLE_VIRTUAL_TERMINAL_PROCESSING &&
		state.err & ENABLE_PROCESSED_OUTPUT &&
		state.err & ENABLE_VIRTUAL_TERMINAL_PROCESSING) mode |= AIL_TERM_MODE_VPROC;
	return mode;
}

internal AIL_TermState term_state_set_mode(AIL_TermState state, AIL_TermMode mode)
{
	AIL_STATIC_ASSERT(AIL_TERM_MODE_FLAG_COUNT == (1 << 6));
	if (mode & AIL_TERM_MODE_ECHO)       state.in |=  ENABLE_ECHO_INPUT;
	else                             state.in &= ~ENABLE_ECHO_INPUT;
	if (mode & AIL_TERM_MODE_LINE_INPUT) state.in |=  ENABLE_LINE_INPUT;
	else                             state.in &= ~ENABLE_LINE_INPUT;
	if (mode & AIL_TERM_MODE_INSERT)     state.in |=  ENABLE_INSERT_MODE;
	else                             state.in &= ~ENABLE_INSERT_MODE;
	if (mode & AIL_TERM_MODE_MOUSE)      state.in |=  ENABLE_MOUSE_INPUT;
	else                             state.in &= ~ENABLE_MOUSE_INPUT;
	if (mode & AIL_TERM_MODE_CTRL_PROC)  state.in |=  ENABLE_PROCESSED_INPUT;
	else                             state.in &= ~ENABLE_PROCESSED_INPUT;
	if (mode & AIL_TERM_MODE_VPROC) {
		state.out |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		state.err |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	} else {
		state.out &= ~(ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		state.err &= ~(ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	}
	return state;
}

internal void term_set_state(AIL_TermState state)
{
	if (!SetConsoleMode(ail_term_global_handles.in,  state.in)) {
		printf("Error in setting console mode for STDIN (handle: %p, state: %lu): %lu\n", ail_term_global_handles.in, state.in, GetLastError());
	}
	if (!SetConsoleMode(ail_term_global_handles.out, state.out)) {
		printf("Error in setting console mode for STDOUT (handle: %p, state: %lu): %lu\n", ail_term_global_handles.out, state.out, GetLastError());
	}
	if (!SetConsoleMode(ail_term_global_handles.err, state.err)) {
		printf("Error in setting console mode for STDERR (handle: %p, state: %lu): %lu\n", ail_term_global_handles.err, state.err, GetLastError());
	}
	ail_term_global_current_state = state;
}


#else
////////////////////////
// POSIX Implementation
////////////////////////

internal AIL_TermHandles term_get_handles(void)
{
	return (AIL_TermHandles) {
		.in  = STDIN_FILENO,
		.out = STDOUT_FILENO,
		.err = STDERR_FILENO,
	};
}

internal AIL_TermState _term_get_state(void)
{
	AIL_TermState state;
	AIL_ASSERT(!tcgetattr(ail_term_global_handles.in, &state));
	return state;
}

internal AIL_TermMode term_state_get_mode(AIL_TermState state)
{
	AIL_STATIC_ASSERT(AIL_TERM_MODE_FLAG_COUNT == (1 << 6));
	AIL_TermMode mode = 0;
	if (state.c_lflag & ECHO)   mode |= AIL_TERM_MODE_ECHO;
	if (state.c_lflag & ICANON) mode |= AIL_TERM_MODE_LINE_INPUT | AIL_TERM_MODE_INSERT;
	if (state.c_lflag & ISIG)   mode |= AIL_TERM_MODE_CTRL_PROC;
	// @TODO: Which flags effect processing of ansi escape codes? Can they be disabled at all?
	mode |= AIL_TERM_MODE_VPROC;
	return mode;
}

internal AIL_TermState term_state_set_mode(AIL_TermState state, AIL_TermMode mode)
{
	AIL_STATIC_ASSERT(AIL_TERM_MODE_FLAG_COUNT == (1 << 6));
	if (mode & AIL_TERM_MODE_ECHO)       state.c_lflag |=   ECHO | ECHONL;
	else                             state.c_lflag &= ~(ECHO | ECHONL);
	// @TODO: One of the following modes might turn ICANON on and the other turn it off again
	if (mode & AIL_TERM_MODE_LINE_INPUT) state.c_lflag |=  ICANON;
	else                             state.c_lflag &= ~ICANON;
	if (mode & AIL_TERM_MODE_INSERT)     state.c_lflag |=  ICANON;
	else                             state.c_lflag &= ~ICANON;
	if (mode & AIL_TERM_MODE_CTRL_PROC)  state.c_lflag |=  ISIG;
	else                             state.c_lflag &= ~ISIG;
	return state;
}

internal void term_set_state(AIL_TermState state)
{
	// @TODO: Maybe use TCSADRAIN instead of TCSANOW?
	AIL_ASSERT(!tcsetattr(ail_term_global_handles.in, TCSANOW, &state));
	ail_term_global_current_state = state;
}

#endif


#endif // AIL_TERM_IMPL_GUARD
#endif // AIL_NO_TERM_IMPL
