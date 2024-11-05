// A simple instrumenting Profiler
//
// Define `AIL_BENCH_IMPL` to enable the implementation
// Define `AIL_BENCH_PROFILE` to profile code blocks
// Define `AIL_BENCH_PROFILE_ANCHOR_COUNT` to set the maximum amount of Profile Anchors
//
// The profiler was heavily inspired by Casey Muratori's Computer Enhance series
// See https://github.com/cmuratori/computer_enhance and https://www.computerenhance.com
// for more information on Computer Enhance
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

#ifndef AIL_BENCH_H_
#define AIL_BENCH_H_

#define AIL_TYPES_IMPL
#include "ail.h"

#ifndef AIL_BENCH_DEF
#define AIL_BENCH_DEF AIL_DEF
#endif // AIL_BENCH_DEF
#ifndef AIL_BENCH_DEF_INLINE
#define AIL_BENCH_DEF_INLINE AIL_DEF_INLINE
#endif // AIL_BENCH_DEF_INLINE

AIL_BENCH_DEF_INLINE u64 ail_bench_os_timer_freq(void);
AIL_BENCH_DEF_INLINE u64 ail_bench_os_timer(void);
AIL_BENCH_DEF_INLINE u64 ail_bench_cpu_timer_freq(void);
AIL_BENCH_DEF_INLINE u64 ail_bench_cpu_timer(void);
f64 ail_bench_cpu_elapsed_to_ms_fast(u64 cpu_elapsed, u64 cpu_freq);
#define ail_bench_cpu_elapsed_to_ms(cpu_elapsed) ail_bench_cpu_elapsed_to_ms_fast(cpu_elapsed, ail_bench_cpu_timer_freq())


#ifdef AIL_BENCH_PROFILE
#ifndef AIL_BENCH_PROFILE_ANCHOR_COUNT
#define AIL_BENCH_PROFILE_ANCHOR_COUNT 4096
#endif // AIL_BENCH_PROFILE_ANCHOR_COUNT
#include <stdio.h> // For printf

typedef struct AIL_Bench_Profiler {
    u64 start_tsc;
    u64 end_tsc;
} AIL_Bench_Profiler;
static AIL_Bench_Profiler ail_bench_global_profiler;

typedef struct AIL_Bench_Profile_Anchor {
    const char *label;
    u64 elapsed_with_children;
    u64 elapsed_wo_children;
    u64 old_elapsed_wo_children;
    u64 min_with_children;
    u64 min_wo_children;
    u64 hit_count;
    u32 child_depth;
} AIL_Bench_Profile_Anchor;

typedef struct AIL_Bench_Profile_Block {
    const char *label;
    u64 old_elapsed; // old elapsed_with_children value
    u64 start_time;
    u32 parent_idx;
    u32 anchor_idx;
} AIL_Bench_Profile_Block;

AIL_BENCH_DEF void ail_bench_begin_profile(void);
AIL_BENCH_DEF void ail_bench_end_profile(void);
AIL_BENCH_DEF void ail_bench_print_profile(u32 depth, b32 clear_anchors);
AIL_BENCH_DEF void ail_bench_end_and_print_profile(u32 depth, b32 clear_anchors);
AIL_BENCH_DEF void ail_bench_clear_anchors(void);
AIL_BENCH_DEF AIL_Bench_Profile_Block ail_bench_profile_block_start(const char *label, u32 anchor_idx);
AIL_BENCH_DEF void ail_bench_profile_block_end(AIL_Bench_Profile_Block *block);
AIL_BENCH_DEF void ail_bench_profile_print_anchors(u64 total_tsc_elapsed, u32 depth, b32 clear_anchors);

#define AIL_BENCH_PROFILE_BLOCK_NAME(name) AIL_CONCAT(ail_bench_profile_block_, name)
#define AIL_BENCH_PROFILE_START(name) AIL_Bench_Profile_Block AIL_BENCH_PROFILE_BLOCK_NAME(name) = ail_bench_profile_block_start(AIL_STRINGIFY(name), __COUNTER__ + 1)
#define AIL_BENCH_PROFILE_END(name) ail_bench_profile_block_end(&AIL_BENCH_PROFILE_BLOCK_NAME(name))
#define AIL_BENCH_END_OF_COMPILATION_UNIT() AIL_STATIC_ASSERT(__COUNTER__ < AIL_BENCH_PROFILE_ANCHOR_COUNT, "Number of profile points exceeds size of profiler::Anchors array")
#endif // AIL_BENCH_PROFILE


#endif // AIL_BENCH_H_



#ifdef AIL_BENCH_IMPL
#ifndef _AIL_BENCH_IMPL_GUARD_
#define _AIL_BENCH_IMPL_GUARD_


#ifdef AIL_BENCH_PROFILE
static AIL_Bench_Profile_Anchor ail_bench_global_anchors[AIL_BENCH_PROFILE_ANCHOR_COUNT] = { 0 };
static u32 ail_bench_gloabl_parent_idx = 0;

void ail_bench_begin_profile(void)
{
    ail_bench_global_profiler.start_tsc = ail_bench_cpu_timer();
}

void ail_bench_end_profile(void)
{
    ail_bench_global_profiler.end_tsc = ail_bench_cpu_timer();
}

void ail_bench_print_profile(u32 depth, b32 clear_anchors)
{
    u64 cpu_freq = ail_bench_cpu_timer_freq();
    u64 total_tsc_elapsed = ail_bench_global_profiler.end_tsc - ail_bench_global_profiler.start_tsc;
    if (cpu_freq) {
        printf("Total time: %0.4fms (CPU frequency %0.3fMHz)\n", ail_bench_cpu_elapsed_to_ms_fast(total_tsc_elapsed, cpu_freq), (f64)cpu_freq / 1e6);
    }
    ail_bench_profile_print_anchors(total_tsc_elapsed, depth, clear_anchors);
}

void ail_bench_end_and_print_profile(u32 depth, b32 clear_anchors)
{
    ail_bench_end_profile();
    ail_bench_print_profile(depth, clear_anchors);
}

AIL_Bench_Profile_Block ail_bench_profile_block_start(const char *label, u32 anchor_idx)
{
    AIL_Bench_Profile_Block block;
    block.label       = label,
    block.anchor_idx  = anchor_idx;
    block.old_elapsed = ail_bench_global_anchors[anchor_idx].elapsed_with_children,
    block.parent_idx  = ail_bench_gloabl_parent_idx,
    ail_bench_gloabl_parent_idx = anchor_idx;
    block.start_time  = ail_bench_cpu_timer();
    return block;
}

void ail_bench_profile_block_end(AIL_Bench_Profile_Block *block)
{
    u64 elapsed = ail_bench_cpu_timer() - block->start_time;
    ail_bench_gloabl_parent_idx = block->parent_idx;

    AIL_Bench_Profile_Anchor *parent = &ail_bench_global_anchors[block->parent_idx];
    AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[block->anchor_idx];
    anchor->child_depth              = parent->child_depth + 1;
    parent->elapsed_wo_children     -= elapsed;
    anchor->elapsed_wo_children     += elapsed;
    anchor->elapsed_with_children    = block->old_elapsed + elapsed;

    u64 cur_elapsed_wo_children      = anchor->elapsed_wo_children - anchor->old_elapsed_wo_children;
    anchor->old_elapsed_wo_children  = anchor->elapsed_wo_children;
    if (!anchor->min_wo_children   || anchor->min_wo_children   < cur_elapsed_wo_children) anchor->min_wo_children   = cur_elapsed_wo_children;
    if (!anchor->min_with_children || anchor->min_with_children < elapsed)                 anchor->min_with_children = elapsed;

    ++anchor->hit_count;
    anchor->label = block->label;
}

void _ail_bench_clear_anchor(AIL_Bench_Profile_Anchor *anchor)
{
    anchor->hit_count               = 0;
    anchor->elapsed_wo_children     = 0;
    anchor->elapsed_with_children   = 0;
    anchor->min_wo_children         = 0;
    anchor->min_with_children       = 0;
    anchor->old_elapsed_wo_children = 0;
}

void ail_bench_clear_anchors(void)
{
    for (u64 i = 0; i < AIL_BENCH_PROFILE_ANCHOR_COUNT; i++) {
        _ail_bench_clear_anchor(ail_bench_global_anchors + i);
    }
}

// Calculates amount of characters required to write x as a decimal number
static inline u32 ail_bench_u64_len(u64 x) {
    u32 len = 1;
    while (x > 9) {
        x /= 10;
        len++;
    }
    return len;
}

// Calculates amount of characters required to write x as a decimal number
static inline u32 ail_bench_f64_len(f64 x, u32 precision) {
    return ail_bench_u64_len((u64)x) + 1 + precision;
}

static inline u32 ail_bench_str_len(const char *str) {
    u32 len = 0;
    while (str[len]) len++;
    return len;
}

void ail_bench_profile_print_anchors(u64 total_tsc_elapsed, u32 depth, b32 clear_anchors)
{
    // Calculate max lengths for proper alignment
    static const u32 float_print_precision = 4;
    u64 cpu_freq = ail_bench_cpu_timer_freq();
    u32 max_name_hit_count_len = 0;
    u32 max_elapsed_cycles_len = 0;
    u32 max_elapsed_ms_len     = 0;
    u32 max_min_elapsed_ms_len = 0;
    b32 any_with_children      = 0;
    for (u32 anchor_idx = 0; anchor_idx < AIL_BENCH_PROFILE_ANCHOR_COUNT; anchor_idx++) {
        AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[anchor_idx];
        if (anchor->elapsed_with_children && anchor->child_depth <= depth) {
            u32 name_len           = ail_bench_str_len(anchor->label);
            u32 hit_count_len      = ail_bench_u64_len(anchor->hit_count);
            u32 elapsed_cycles_len = ail_bench_u64_len(anchor->elapsed_wo_children);
            u32 elapsed_ms_len     = ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms_fast(anchor->elapsed_wo_children, cpu_freq), float_print_precision);
            u32 min_elapsed_ms_len = ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms_fast(anchor->min_wo_children, cpu_freq), float_print_precision);
            max_name_hit_count_len = AIL_MAX(max_name_hit_count_len, name_len + hit_count_len);
            max_elapsed_cycles_len = AIL_MAX(max_elapsed_cycles_len, elapsed_cycles_len);
            max_elapsed_ms_len     = AIL_MAX(max_elapsed_ms_len,     elapsed_ms_len);
            max_min_elapsed_ms_len = AIL_MAX(max_min_elapsed_ms_len, min_elapsed_ms_len);
            any_with_children     |= (anchor->child_depth < depth) && (anchor->elapsed_with_children != anchor->elapsed_wo_children);
        }
    }
    char tmp_str[16] = {0};
    for (u32 anchor_idx = 0; anchor_idx < AIL_BENCH_PROFILE_ANCHOR_COUNT; anchor_idx++) {
        AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[anchor_idx];
        if (anchor->elapsed_with_children) {
            if (anchor->child_depth <= depth) {
                u64 elapsed_wo_children = (anchor->child_depth == depth) ? anchor->elapsed_with_children : anchor->elapsed_wo_children;
                f64 perc_wo_children = 100.0 * ((f64)elapsed_wo_children / (f64)total_tsc_elapsed);
                printf("  %s[%zu]: ", anchor->label, anchor->hit_count);
                u32 len_diff = max_name_hit_count_len - (ail_bench_str_len(anchor->label) + ail_bench_u64_len(anchor->hit_count));
                while (len_diff--) printf(" ");
                len_diff = max_elapsed_cycles_len - ail_bench_u64_len(elapsed_wo_children);
                while (len_diff--) printf(" ");
                printf("%zu cycles, ", elapsed_wo_children);
                f64 elapsed_in_ms = ail_bench_cpu_elapsed_to_ms_fast(elapsed_wo_children, cpu_freq);
                len_diff = max_elapsed_ms_len - ail_bench_f64_len(elapsed_in_ms, float_print_precision);
                while (len_diff--) printf(" ");
                printf("%0.*fms ", float_print_precision, elapsed_in_ms);

                b32 has_children = (anchor->elapsed_with_children != elapsed_wo_children);
                if (!has_children) {
                    printf("(%5.2f%%) ", perc_wo_children);
                } else {
                    f64 perc_with_children = 100.0 * ((f64)anchor->elapsed_with_children / (f64)total_tsc_elapsed);
                    sprintf(tmp_str, "%5.2f%%, ", perc_wo_children);
                    printf("(%-8s%5.2f%% w/children) ", tmp_str, perc_with_children);
                }

                if (!has_children && any_with_children) printf("                   ");
                f64 min_elapsed_in_ms = ail_bench_cpu_elapsed_to_ms_fast((anchor->child_depth == depth) ? anchor->min_with_children : anchor->min_wo_children, cpu_freq);
                len_diff = max_min_elapsed_ms_len - ail_bench_f64_len(min_elapsed_in_ms, float_print_precision);
                printf("Min: ");
                while (len_diff--) printf(" ");
                printf("%0.*fms ", float_print_precision, min_elapsed_in_ms);
                if (has_children && anchor->min_with_children != anchor->min_wo_children) {
                    printf("(%0.*fms w/children)", float_print_precision, ail_bench_cpu_elapsed_to_ms_fast(anchor->min_with_children, cpu_freq));
                }
                printf("\n");
            }

            if (clear_anchors) _ail_bench_clear_anchor(anchor);
        }
    }
}
#endif // AIL_BENCH_PROFILE



f64 ail_bench_cpu_elapsed_to_ms_fast(u64 cpu_elapsed, u64 cpu_freq)
{
    return 1000.0 * (f64)cpu_elapsed / (f64)cpu_freq;
}

u64 ail_bench_cpu_timer(void)
{
    // @Note: This only works on x86-64 architectures
    return __rdtsc();
}

u64 ail_bench_cpu_timer_freq(void)
{
    u64 millisToWait = 100;
    u64 osFreq       = ail_bench_os_timer_freq();

    u64 cpuStart  = ail_bench_cpu_timer();
    u64 osStart   = ail_bench_os_timer();
    u64 osEnd     = 0;
    u64 osElapsed = 0;
    u64 osWaitTime = osFreq * millisToWait / 1000;
    while(osElapsed < osWaitTime) {
        osEnd     = ail_bench_os_timer();
        osElapsed = osEnd - osStart;
    }

    u64 cpuEnd     = ail_bench_cpu_timer();
    u64 cpuElapsed = cpuEnd - cpuStart;

    u64 cpu_freq = 0;
    if (osElapsed) cpu_freq = osFreq * cpuElapsed / osElapsed;

    return cpu_freq;
}


#if defined(_WIN32) || defined(__WIN32__)
#include <intrin.h>
#include <windows.h>

u64 ail_bench_os_timer_freq(void)
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return freq.QuadPart;
}

u64 ail_bench_os_timer(void)
{
    LARGE_INTEGER freq;
    QueryPerformanceCounter(&freq);
    return freq.QuadPart;
}

#else

#include <x86intrin.h>
#include <sys/time.h>

u64 ail_bench_os_timer_freq(void)
{
    return 1000000;
}

u64 ail_bench_os_timer(void)
{
    struct timeval time;
    gettimeofday(&time, 0);
    return ail_bench_os_timer_freq()*(u64)time.tv_sec + (u64)time.tv_usec;
}

#endif

#endif // _AIL_BENCH_IMPL_GUARD_
#endif // AIL_BENCH_IMPL
