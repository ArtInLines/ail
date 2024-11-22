/*
* Utilities for working with different Endiannesses
*
* Endianness describes the order of bytes in a word/double-word.
* The Enum AIL_Endian contains the different types of endianness recognized by ail.h
* Detecting the platform's endianness cannot be done portably & reliably at compile-time (see https://sourceforge.net/p/predef/wiki/Endianness/)
* Potential compile-time detection is inspired by https://github.com/nemequ/portable-snippets/
* The following functions are provided to work with different endiannesses:
  * ail_endian: Detect the platform's endianness
*/

#ifndef _AIL_BASE_ENDIAN_H_
#define _AIL_BASE_ENDIAN_H_

#include "./ail_base.h"

/////////////////////////
// Endianness
// Disable Implementation with `AIL_NO_ENDIAN_IMPL`
/////////////////////////
typedef enum AIL_Endian {
    AIL_ENDIAN_UNKNOWN = 0,
    AIL_ENDIAN_BIG,          // Most-Significant Byte first
    AIL_ENDIAN_LITTLE,       // Least-Significant Byte first
    AIL_ENDIAN_BIG_WORD,     // Big-Endian for Words, Little-Endian for Double-Words
    AIL_ENDIAN_LITTLE_WORD,  // Little-Endian for Words, Big-Endian for Double-Words
    AIL_ENDIAN_COUNT,
} AIL_Endian;
internal AIL_Endian ail_endian(void);

#endif // _AIL_BASE_ENDIAN_H_

#if !defined(AIL_NO_ENDIAN_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_ENDIAN_GUARD_
#define _AIL_ENDIAN_GUARD_

AIL_Endian ail_endian(void)
{
/* Some compilers like GCC or clang define  __BYTE_ORDER__. */
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return AIL_ENDIAN_LITTLE;
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return AIL_ENDIAN_BIG;
#elif defined(__BYTE_ORDER__) && defined(__ORDER_PDP_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_PDP_ENDIAN__)
    return AIL_ENDIAN_BIG_WORD;
/* We know that all x86 architectures are little endian (see https://en.wikipedia.org/wiki/X86) */
#elif AIL_ARCH_X86
    return AIL_ENDIAN_LITTLE;
/* @Note: Windows appears to always be little-endian (see https://stackoverflow.com/a/6449593), however there is no guarantuee for that to continue being the case in the future */
/* Other Operating Systems provide headers with macros for the native endianness, but I'm too lazy to add those checks right now */
#else
    persist AIL_Endian res;
    if (res) return res;
    union {
        u32 val;
        u8 data[4];
    } num;
    num.data[0] = 0x00;
    num.data[1] = 0x01;
    num.data[2] = 0x02;
    num.data[3] = 0x03;
    AIL_STATIC_ASSERT(AIL_ENDIAN_COUNT == 5);
    switch (num.val) {
        case 0x00010203: res = AIL_ENDIAN_BIG;         break;
        case 0x03020100: res = AIL_ENDIAN_LITTLE;      break;
        case 0x02030001: res = AIL_ENDIAN_BIG_WORD;    break;
        case 0x01000302: res = AIL_ENDIAN_LITTLE_WORD; break;
        default:         res = AIL_ENDIAN_UNKNOWN;
    }
    return res;
#endif
}

#endif // _AIL_ENDIAN_GUARD_
#endif // AIL_NO_ENDIAN_IMPL