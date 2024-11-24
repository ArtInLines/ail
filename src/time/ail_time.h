/*
*** Time-related functions ***
*
*/

#ifndef _AIL_TIME_H_
#define _AIL_TIME_H_

#include "../base/ail_base.h"

#include <time.h> // For clock

// Sleep `msecs` milliseconds
internal void ail_time_sleep(u64 msecs);

// Time in seconds here
inline_func f64 ail_time_clock_start(void);
inline_func f64 ail_time_clock_elapsed(f64 start);

#endif // _AIL_TIME_H_


#if !defined(AIL_NO_TIME_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_TIME_GUARD_
#define _AIL_TIME_GUARD_

#if AIL_OS_WIN
#   include <windows.h> // For Sleep
#   define AIL_TIME_FLAG_WINSLEEP
#elif _POSIX_C_SOURCE >= 199309L
#   define AIL_TIME_FLAG_NANOSLEEP
#   include <time.h>    // for nanosleep
#else
#   define AIL_TIME_FLAG_USLEEP
#   include <unistd.h>  // for usleep
#endif

void ail_time_sleep(u64 msecs)
{
#if defined(AIL_TIME_FLAG_NANOSLEEP)
    struct timespec ts;
    ts.tv_sec = msecs / 1000;
    ts.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep(&ts, NULL);
#elif defined(AIL_TIME_FLAG_WINSLEEP)
    Sleep(msecs);
#elif defined(AIL_TIME_FLAG_USLEEP)
    if (msecs >= 1000) sleep(msecs / 1000);
    usleep((msecs % 1000) * 1000);
#else
#    error "Missing feature flag not implemented yet"
#endif
}

f64 ail_time_clock_start(void)
{
#if AIL_OS_WIN
    return (f64)timeGetTime() / 1000;
#elif AIL_OS_POSIX
    struct timespec ts = {0};
    int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    AIL_ASSERT(ret == 0);
    return (f64)ts.tv_sec + ts.tv_nsec*1e-9;
#else
    return (f64)clock() / CLOCKS_PER_SEC;
#endif
}

f64 ail_time_clock_elapsed(f64 start)
{
    return ail_time_clock_start() - start;
}


#endif // _AIL_TIME_GUARD_
#endif // AIL_NO_TIME_IMPL
