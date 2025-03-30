/*
*** A simple instrumenting Profiler ***
*
* Define `AIL_NO_BENCH_IMPL` to not include any implementation
* Define `AIL_BENCH_PROFILE` to profile code blocks
* Define `AIL_BENCH_PROFILE_ANCHOR_COUNT` to set the maximum amount of Profile Anchors
*
* The profiler was heavily inspired by Casey Muratori's Computer Enhance series
* See https://github.com/cmuratori/computer_enhance and https://www.computerenhance.com
* for more information on Computer Enhance
*
*/

#ifndef _AIL_BENCH_H_
#define _AIL_BENCH_H_

#include "../base/ail_base.h"
#include "../base/ail_base_math.h"
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

#if AIL_OS_WIN
#   include <windows.h>
    typedef struct AIL_Bench_Global_Info {
        u64 cpu_timer_freq;
        HANDLE proc;
    } AIL_Bench_Global_Info;
#else
    typedef struct AIL_Bench_Global_Info {
        u64 cpu_timer_freq;
        void* proc;
    } AIL_Bench_Global_Info;
#endif

inline_func void ail_bench_init(void);
inline_func u64 ail_bench_count_page_faults(void);
inline_func u64 ail_bench_os_timer_freq(void);
inline_func u64 ail_bench_os_timer(void);
inline_func u64 ail_bench_calc_cpu_timer_freq(void);
inline_func u64 ail_bench_cpu_timer(void);
inline_func f64 ail_bench_cpu_elapsed_to_ms(u64 cpu_elapsed);


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
    u64 avg_with_children;
    u64 avg_wo_children;
    u64 max_with_children;
    u64 max_wo_children;
    u64 mem_size;
    u32 hit_count;
    u32 child_depth; // @TODO: Does this really work all that well? There could be different depths depending on the codepath after all...
} AIL_Bench_Profile_Anchor;

typedef struct AIL_Bench_Profile_Block {
    const char *label;
    u64 old_elapsed; // old elapsed_with_children value
    u64 start_time;
    u32 parent_idx;
    u32 anchor_idx;
} AIL_Bench_Profile_Block;

internal void ail_bench_begin_profile(void);
internal void ail_bench_end_profile(void);
internal void ail_bench_print_profile(u32 depth, b32 clear_anchors);
internal void ail_bench_end_and_print_profile(u32 depth, b32 clear_anchors);
internal void ail_bench_clear_anchors(void);
internal AIL_Bench_Profile_Block ail_bench_profile_block_start(const char *label, u32 anchor_idx, u64 mem_size);
internal void ail_bench_profile_block_end(AIL_Bench_Profile_Block *block);
internal void ail_bench_profile_print_anchors(u64 total_tsc_elapsed, u32 depth, b32 clear_anchors);

#   define AIL_BENCH_PROFILE_BLOCK_NAME(name) AIL_CONCAT(ail_bench_profile_block_, name)
#   define AIL_BENCH_PROFILE_MEM_START(name, mem_size) AIL_Bench_Profile_Block AIL_BENCH_PROFILE_BLOCK_NAME(name) = ail_bench_profile_block_start(AIL_STRINGIFY(name), __COUNTER__ + 1, (mem_size))
#   define AIL_BENCH_PROFILE_START(name) AIL_BENCH_PROFILE_MEM_START(name, 0)
#   define AIL_BENCH_PROFILE_END(name) ail_bench_profile_block_end(&AIL_BENCH_PROFILE_BLOCK_NAME(name))
#   define AIL_BENCH_END_OF_COMPILATION_UNIT() ail_static_assert(__COUNTER__ < AIL_BENCH_PROFILE_ANCHOR_COUNT, "Number of profile points exceeds size of profiler::Anchors array")
#else
#   define AIL_BENCH_PROFILE_BLOCK_NAME(...)
#   define AIL_BENCH_PROFILE_MEM_START(...)
#   define AIL_BENCH_PROFILE_START(...)
#   define AIL_BENCH_PROFILE_END(...)
#   define AIL_BENCH_END_OF_COMPILATION_UNIT(...)
#endif // AIL_BENCH_PROFILE


AIL_WARN_POP
#endif // _AIL_BENCH_H_



#if !defined(AIL_NO_BENCH_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_BENCH_IMPL_GUARD_
#define _AIL_BENCH_IMPL_GUARD_
AIL_WARN_PUSH
AIL_WARN_DISABLE(AIL_WARN_UNUSED_FUNCTION)

global AIL_Bench_Global_Info ail_bench_global_info = {0};

#ifdef AIL_BENCH_PROFILE
global AIL_Bench_Profile_Anchor ail_bench_global_anchors[AIL_BENCH_PROFILE_ANCHOR_COUNT] = {0};
global u32 ail_bench_gloabl_parent_idx = 0;

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
    u64 total_tsc_elapsed = ail_bench_global_profiler.end_tsc - ail_bench_global_profiler.start_tsc;
    if (ail_bench_global_info.cpu_timer_freq) {
        printf("Total time: %0.4fms (CPU frequency %0.3fMHz)\n", ail_bench_cpu_elapsed_to_ms(total_tsc_elapsed), (f64)ail_bench_global_info.cpu_timer_freq / 1e6);
    }
    ail_bench_profile_print_anchors(total_tsc_elapsed, depth, clear_anchors);
}

void ail_bench_end_and_print_profile(u32 depth, b32 clear_anchors)
{
    ail_bench_end_profile();
    ail_bench_print_profile(depth, clear_anchors);
}

AIL_Bench_Profile_Block ail_bench_profile_block_start(const char *label, u32 anchor_idx, u64 mem_size)
{
    AIL_Bench_Profile_Block block;
    AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[anchor_idx];
    anchor->mem_size           += mem_size;
    block.label                 = label,
    block.anchor_idx            = anchor_idx;
    block.old_elapsed           = anchor->elapsed_with_children,
    block.parent_idx            = ail_bench_gloabl_parent_idx,
    ail_bench_gloabl_parent_idx = anchor_idx;
    block.start_time            = ail_bench_cpu_timer();
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


    u64 elapsed_wo = anchor->elapsed_wo_children - anchor->old_elapsed_wo_children;
    anchor->old_elapsed_wo_children = anchor->elapsed_wo_children;
    if (!anchor->min_wo_children   || anchor->min_wo_children   > elapsed_wo) anchor->min_wo_children   = elapsed_wo;
    if (!anchor->min_with_children || anchor->min_with_children > elapsed)    anchor->min_with_children = elapsed;
    if (!anchor->max_wo_children   || anchor->max_wo_children   < elapsed_wo) anchor->max_wo_children   = elapsed_wo;
    if (!anchor->max_with_children || anchor->max_with_children < elapsed)    anchor->max_with_children = elapsed;

    ++anchor->hit_count;
    u32 n = anchor->hit_count;
    anchor->avg_wo_children   = (anchor->avg_wo_children  *(n-1)/n) + (elapsed_wo/n);
    anchor->avg_with_children = (anchor->avg_with_children*(n-1)/n) + (elapsed/n);
    anchor->label = block->label;
}

void _ail_bench_clear_anchor(AIL_Bench_Profile_Anchor *anchor)
{
    anchor->hit_count               = 0;
    anchor->elapsed_wo_children     = 0;
    anchor->elapsed_with_children   = 0;
    anchor->min_wo_children         = 0;
    anchor->min_with_children       = 0;
    anchor->max_wo_children         = 0;
    anchor->max_with_children       = 0;
    anchor->avg_wo_children         = 0;
    anchor->avg_with_children       = 0;
    anchor->old_elapsed_wo_children = 0;
    anchor->mem_size                = 0;
}

void ail_bench_clear_anchors(void)
{
    for (u64 i = 0; i < AIL_BENCH_PROFILE_ANCHOR_COUNT; i++) {
        _ail_bench_clear_anchor(ail_bench_global_anchors + i);
    }
}

// Calculates amount of characters required to write x as a decimal number
inline_func u32 ail_bench_u64_len(u64 x) {
    u32 len = 1;
    while (x > 9) {
        x /= 10;
        len++;
    }
    return len;
}

// Calculates amount of characters required to write x as a decimal number
inline_func u32 ail_bench_f64_len(f64 x, u32 precision) {
    return ail_bench_u64_len((u64)x) + 1 + precision;
}

inline_func u32 ail_bench_str_len(const char *str) {
    u32 len = 0;
    while (str[len]) len++;
    return len;
}

// @TODO: Support colored output
// This could maybe be done by adding an enum to anchors which indicates whether the anchor should
// a) always be highlighted
// b) compared with other "compare" anchors (i.e. green for fastest, red for slowest)
// Alternative approach would be to let the user provide this information when calling print()
// Mapping coloring information to a specific anchor could be done via the anchor's label then
void ail_bench_profile_print_anchors(u64 total_tsc_elapsed, u32 depth, b32 clear_anchors)
{
    // Calculate max lengths for proper alignment
    static const u32 float_print_precision = 4;
    u32 with_children_str_len  = sizeof("(ms w/children)");
    u32 max_name_hit_count_len = 0;
    u32 max_elapsed_ms_len     = 0;
    u32 max_min_ms_len         = 0;
    u32 max_min_children_len   = 0;
    u32 max_avg_ms_len         = 0;
    u32 max_avg_children_len   = 0;
    u32 max_max_ms_len         = 0;
    u32 max_max_children_len   = 0;
    u32 max_bandwidth_len      = 0;
    b32 any_with_children      = 0;
    for (u32 anchor_idx = 0; anchor_idx < AIL_BENCH_PROFILE_ANCHOR_COUNT; anchor_idx++) {
        AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[anchor_idx];
        if (anchor->elapsed_with_children && anchor->child_depth <= depth) {
            u32 name_len           = ail_bench_str_len(anchor->label);
            u32 hit_count_len      = ail_bench_u64_len(anchor->hit_count);
            u64 elapsed            = (anchor->child_depth == depth) ? anchor->elapsed_with_children : anchor->elapsed_wo_children;
            u32 elapsed_ms_len     = ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(elapsed), float_print_precision);
            u32 bandwidth_len      = ail_bench_f64_len((f64)anchor->mem_size / (f64)(AIL_GB(1) * (ail_bench_cpu_elapsed_to_ms(elapsed)/1000)), float_print_precision);
            max_bandwidth_len      = ail_max(max_bandwidth_len, bandwidth_len);
            max_name_hit_count_len = ail_max(max_name_hit_count_len, name_len + hit_count_len);
            max_elapsed_ms_len     = ail_max(max_elapsed_ms_len, elapsed_ms_len);
            if (anchor->hit_count > 1) {
                u64 min              = (anchor->child_depth == depth) ? anchor->min_with_children : anchor->min_wo_children;
                u64 avg              = (anchor->child_depth == depth) ? anchor->avg_with_children : anchor->avg_wo_children;
                u64 max              = (anchor->child_depth == depth) ? anchor->max_with_children : anchor->max_wo_children;
                u32 min_ms_len       = ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(min), float_print_precision);
                u32 max_ms_len       = ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(max), float_print_precision);
                u32 avg_ms_len       = ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(avg), float_print_precision);
                u32 min_children_len = (anchor->min_with_children != min) ? ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(anchor->min_with_children), float_print_precision) : 0;
                u32 max_children_len = (anchor->max_with_children != max) ? ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(anchor->max_with_children), float_print_precision) : 0;
                u32 avg_children_len = (anchor->avg_with_children != avg) ? ail_bench_f64_len(ail_bench_cpu_elapsed_to_ms(anchor->avg_with_children), float_print_precision) : 0;
                max_min_ms_len       = ail_max(max_min_ms_len, min_ms_len);
                max_avg_ms_len       = ail_max(max_avg_ms_len, avg_ms_len);
                max_max_ms_len       = ail_max(max_max_ms_len, max_ms_len);
                max_min_children_len = ail_max(max_min_children_len, min_children_len);
                max_avg_children_len = ail_max(max_avg_children_len, avg_children_len);
                max_max_children_len = ail_max(max_max_children_len, max_children_len);
            }
            any_with_children     |= (anchor->child_depth < depth) && (anchor->elapsed_with_children != anchor->elapsed_wo_children);
        }
    }
    char tmp_str[16] = {0};
    for (u32 anchor_idx = 0; anchor_idx < AIL_BENCH_PROFILE_ANCHOR_COUNT; anchor_idx++) {
        AIL_Bench_Profile_Anchor *anchor = &ail_bench_global_anchors[anchor_idx];
        if (anchor->elapsed_with_children) {
            if (anchor->child_depth <= depth) {
                printf("  %s[%d]: ", anchor->label, anchor->hit_count);
                i32 len_diff = max_name_hit_count_len - (ail_bench_str_len(anchor->label) + ail_bench_u64_len(anchor->hit_count));
                while (len_diff-- > 0) printf(" ");

                u64 elapsed_wo_children = (anchor->child_depth == depth) ? anchor->elapsed_with_children : anchor->elapsed_wo_children;
                f64 perc_wo_children    = 100.0 * ((f64)elapsed_wo_children / (f64)total_tsc_elapsed);
                f64 elapsed_in_ms       = ail_bench_cpu_elapsed_to_ms(elapsed_wo_children);
                len_diff = max_elapsed_ms_len - ail_bench_f64_len(elapsed_in_ms, float_print_precision);
                while (len_diff-- > 0) printf(" ");
                printf("%0.*fms ", float_print_precision, elapsed_in_ms);

                b32 has_children = (anchor->elapsed_with_children != elapsed_wo_children);
                if (!has_children) {
                    printf("(%5.2f%%) ", perc_wo_children);
                } else {
                    f64 perc_with_children = 100.0 * ((f64)anchor->elapsed_with_children / (f64)total_tsc_elapsed);
                    sprintf(tmp_str, "%5.2f%%, ", perc_wo_children);
                    printf("(%-8s%5.2f%% w/children) ", tmp_str, perc_with_children);
                }
                printf(" ");
                if (!has_children && any_with_children) printf("                   ");

                if (anchor->hit_count > 1) {
                    f64 min_elapsed_in_ms = ail_bench_cpu_elapsed_to_ms((anchor->child_depth == depth) ? anchor->min_with_children : anchor->min_wo_children);
                    printf("Min: ");
                    len_diff = max_min_ms_len - ail_bench_f64_len(min_elapsed_in_ms, float_print_precision);
                    while (len_diff-- > 0) printf(" ");
                    printf("%0.*fms ", float_print_precision, min_elapsed_in_ms);
                    len_diff = max_min_children_len + with_children_str_len;
                    if (has_children && anchor->min_with_children != anchor->min_wo_children) {
                        f64 ms = ail_bench_cpu_elapsed_to_ms(anchor->min_with_children);
                        printf("(%0.*fms w/children) ", float_print_precision, ms);
                        len_diff = max_min_children_len - ail_bench_f64_len(ms, float_print_precision);
                    }
                    while (len_diff-- > 0) printf(" ");
                    printf(" ");

                    f64 max_elapsed_in_ms = ail_bench_cpu_elapsed_to_ms((anchor->child_depth == depth) ? anchor->max_with_children : anchor->max_wo_children);
                    printf("Max: ");
                    len_diff = max_max_ms_len - ail_bench_f64_len(max_elapsed_in_ms, float_print_precision);
                    while (len_diff-- > 0) printf(" ");
                    printf("%0.*fms ", float_print_precision, max_elapsed_in_ms);
                    len_diff = max_max_children_len + with_children_str_len;
                    if (has_children && anchor->max_with_children != anchor->max_wo_children) {
                        f64 ms = ail_bench_cpu_elapsed_to_ms(anchor->max_with_children);
                        printf("(%0.*fms w/children) ", float_print_precision, ms);
                        len_diff = max_max_children_len - ail_bench_f64_len(ms, float_print_precision);
                    }
                    while (len_diff-- > 0) printf(" ");
                    printf(" ");

                    f64 avg_elapsed_in_ms = ail_bench_cpu_elapsed_to_ms((anchor->child_depth == depth) ? anchor->avg_with_children : anchor->avg_wo_children);
                    printf("Avg: ");
                    len_diff = max_avg_ms_len - ail_bench_f64_len(avg_elapsed_in_ms, float_print_precision);
                    while (len_diff-- > 0) printf(" ");
                    printf("%0.*fms ", float_print_precision, avg_elapsed_in_ms);
                    len_diff = max_avg_children_len + with_children_str_len;
                    if (has_children && anchor->avg_with_children != anchor->avg_wo_children) {
                        f64 ms = ail_bench_cpu_elapsed_to_ms(anchor->avg_with_children);
                        printf("(%0.*fms w/children) ", float_print_precision, ms);
                        len_diff = max_avg_children_len - ail_bench_f64_len(ms, float_print_precision);
                    }
                    while (len_diff-- > 0) printf(" ");
                    printf(" ");
                }

                if (anchor->mem_size) {
                    printf("Bandwidth: ");
                    f64 gb_per_sec = (f64)anchor->mem_size / (f64)(AIL_GB(1) * (elapsed_in_ms/1000));
                    i32 len_diff = max_bandwidth_len - ail_bench_f64_len(gb_per_sec, float_print_precision);
                    while (len_diff-- > 0) printf(" ");
                    if (gb_per_sec > 0.1f) printf("%2.*fGB/s ", float_print_precision, gb_per_sec);
                    else printf("%2.*fMB/s ", float_print_precision, gb_per_sec*1000.0f);
                }
                printf("\n");
            }

            if (clear_anchors) _ail_bench_clear_anchor(anchor);
        }
    }
}
#endif // AIL_BENCH_PROFILE


#if AIL_OS_WIN
#include <intrin.h>  // For __rdtsc
#include <windows.h> // For QueryPerformanceCounter, OpenProcess, LARGE_INTEGER
#include <psapi.h>   // For PROCESS_MEMORY_COUNTERS_EX, GetProcessMemoryInfo

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

u64 ail_bench_count_page_faults(void)
{
    PROCESS_MEMORY_COUNTERS_EX counters = {0};
    counters.cb = sizeof(counters);
    GetProcessMemoryInfo(ail_bench_global_info.proc, (PROCESS_MEMORY_COUNTERS*)&counters, sizeof(counters));
    return counters.PageFaultCount;
}

#else

#include <x86intrin.h> // For __rdtsc
#include <sys/time.h>  // For gettimeofday, struct timeval
#include <sys/resource.h>  // For getrusage, struct rusage

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

u64 ail_bench_count_page_faults(void)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    // ru_minflt  the number of page faults serviced without any I/O activity.
    // ru_majflt  the number of page faults serviced that required I/O activity.
    return usage.ru_minflt + usage.ru_majflt;
}

#endif


void ail_bench_init(void)
{
    ail_bench_global_info.cpu_timer_freq = ail_bench_calc_cpu_timer_freq();
#if AIL_OS_WIN
    ail_bench_global_info.proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, GetCurrentProcessId());
#endif
}

f64 ail_bench_cpu_elapsed_to_ms(u64 cpu_elapsed)
{
    return 1000.0 * (f64)cpu_elapsed / (f64)ail_bench_global_info.cpu_timer_freq;
}

u64 ail_bench_calc_cpu_timer_freq(void)
{
    u64 millisToWait = 100;
    u64 osFreq       = ail_bench_os_timer_freq();
    u64 cpuStart     = ail_bench_cpu_timer();
    u64 osStart      = ail_bench_os_timer();
    u64 osEnd        = 0;
    u64 osElapsed    = 0;
    u64 osWaitTime   = osFreq * millisToWait / 1000;
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

u64 ail_bench_cpu_timer(void)
{
#if defined(AIL_ARCH_X86) || defined(AIL_ARCH_X64)
    return __rdtsc();
#else
#   error "Benchmarking CPU Timers on non-x86_64 Architectures is not supported currently"
#endif
}

AIL_WARN_POP
#endif // _AIL_BENCH_IMPL_GUARD_
#endif // AIL_NO_BENCH_IMPL
