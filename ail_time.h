// Time-related functions
//
// LICENSE
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

#ifndef AIL_TIME_H_
#define AIL_TIME_H_

#define AIL_TYPES_IMPL
#include "ail.h"

#include <time.h> // For clock

#ifndef AIL_TIME_DEF
#ifdef  AIL_DEF
#define AIL_TIME_DEF AIL_DEF
#else
#define AIL_TIME_DEF
#endif // AIL_DEF
#endif // AIL_TIME_DEF
#ifndef AIL_TIME_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_TIME_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_TIME_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_TIME_DEF_INLINE

// Sleep `msecs` milliseconds
AIL_TIME_DEF void ail_time_sleep(u64 msecs);

// Time in seconds here
AIL_TIME_DEF_INLINE f64 ail_time_clock_start(void);
AIL_TIME_DEF_INLINE f64 ail_time_clock_elapsed(f64 start);

#endif // AIL_TIME_H_


#ifdef AIL_TIME_IMPL
#ifndef _AIL_TIME_GUARD_
#define _AIL_TIME_GUARD_

#ifdef WIN32
#include <windows.h> // For Sleep
#define AIL_TIME_FLAG_WINSLEEP
#elif _POSIX_C_SOURCE >= 199309L
#define AIL_TIME_FLAG_NANOSLEEP
#include <time.h>    // for nanosleep
#else
#define AIL_TIME_FLAG_USLEEP
#include <unistd.h>  // for usleep
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
#ifdef _WIN32
    return (f64)timeGetTime() / 1000;
#elif __posix
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
#endif // AIL_TIME_IMPL
