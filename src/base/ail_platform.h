/*
* Platform Identification Macros
*
* Heavily inspired by (and adapted code of) the following:
  * https://sourceforge.net/p/predef/wiki/Home/
  * https://github.com/nemequ/hedley
  * http://web.archive.org/web/20160306052035/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
*
* Each of the following macros is defined to be either 1 or 0 (except AIL_LANG_STANDARD)
* Architecture Size (aka size of pointers):
  * AIL_32BIT
  * AIL_64BIT
* Operating System (several can be true at once):
  * AIL_OS_WIN
  * AIL_OS_LINUX
  * AIL_OS_UNIX
  * AIL_OS_POSIX
  * AIL_OS_GNU
  * AIL_OS_MAC
  * AIL_OS_IOS
  * AIL_OS_ANDROID
  * AIL_OS_WASM
  * AIL_OS_BSD
  * AIL_OS_MINGW
  * AIL_OS_CYGWIN
  * AIL_OS_SPARC
* CPU Architecture (at most one can be true):
  * AIL_ARCH_X86   - https://en.wikipedia.org/wiki/X86
  * AIL_ARCH_ARM   - https://en.wikipedia.org/wiki/ARM_architecture_family
  * AIL_ARCH_MIPS  - https://en.wikipedia.org/wiki/MIPS_architecture
  * AIL_ARCH_RISCV - https://en.wikipedia.org/wiki/RISC-V
  * AIL_ARCH_SPARC - https://en.wikipedia.org/wiki/SPARC
  * AIL_ARCH_PPC   - https://en.wikipedia.org/wiki/PowerPC
* Compiler (at most one is true):
  * AIL_COMP_CLANG        - https://clang.llvm.org/
  * AIL_COMP_MSVC         - https://learn.microsoft.com/en-us/cpp/build/reference/compiling-a-c-cpp-program
  * AIL_COMP_GCC          - https://gcc.gnu.org/
  * AIL_COMP_TCC          - https://www.bellard.org/tcc/
  * AIL_COMP_PELLES       - http://www.smorgasbordet.com/pellesc/
  * AIL_COMP_DIGITAL_MARS - https://digitalmars.com/
  * AIL_COMP_INTEL        - https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html
  * AIL_COMP_EMSCRIPTEN   - https://emscripten.org/
* Language (exactly one is true):
  * AIL_LANG_C
  * AIL_LANG_CPP
  * AIL_LANG_OBJC
* Language Standard: AIL_LANG_STANDARD
  * AIL_LANG_STANDARD is defined to be the year of the standard's release (as a number)
  * i.e. to test for C11+:
    * #if AIL_LANG_C && AIL_LANG_STANDARD >= 2011
  * to test for C++17+:
    * #if AIL_LANG_CPP && AIL_LANG_STANDARD >= 2017
*/

#ifndef _AIL_PLATFORM_H_
#define _AIL_PLATFORM_H_

#include <stdint.h> // For INTPTR_MAX, INT64_MAX, INT32_MAX
#if INTPTR_MAX == INT32_MAX
#   define AIL_32BIT 1
#   define AIL_64BIT 0
#elif INTPTR_MAX == INT64_MAX
#   define AIL_32BIT 0
#   define AIL_64BIT 1
#else
#   error "Compiling on a platform that is neither 32 nor 64 bit. Please change ail.h if you wish to compile on such a platform"
#endif

#if defined(__clang__)
#   define AIL_COMP_CLANG 1
#else
#   define AIL_COMP_CLANG 0
#endif
#if defined(_MSC_VER)
#   define AIL_COMP_MSVC 1
#else
#   define AIL_COMP_MSVC 0
#endif
#if defined(__GNUC__) && !AIL_COMP_CLANG
#   define AIL_COMP_GCC 1
#else
#   define AIL_COMP_GCC 0
#endif
#if defined(__TINYC__)
#   define AIL_COMP_TCC 1
#else
#   define AIL_COMP_TCC 0
#endif
#if defined(__POCC__)
#   define AIL_COMP_PELLES 1
#else
#   define AIL_COMP_PELLES 0
#endif
#if defined(__DMC__)
#   define AIL_COMP_DIGITAL_MARS 1
#else
#   define AIL_COMP_DIGITAL_MARS 0
#endif
#if defined(__INTEL_COMPILER)
#   define AIL_COMP_INTEL 1
#else
#   define AIL_COMP_INTEL 0
#endif
#if defined(__EMSCRIPTEN__)
#   define AIL_COMP_EMSCRIPTEN 1
#else
#   define AIL_COMP_EMSCRIPTEN 0
#endif

#if defined(__OBJC__)
#   define AIL_LANG_OBJC 1
#   define AIL_LANG_CPP  0
#   define AIL_LANG_C    0
    // @Note: I do not know of any way to get the version for Objective-C from within C
#   define AIL_LANG_STANDARD 1
#elif defined(__cplusplus)
#   define AIL_LANG_OBJC 0
#   define AIL_LANG_CPP  1
#   define AIL_LANG_C    0
#   if __cplusplus == 1
#       define AIL_LANG_STANDARD 1985
#   elif __cplusplus == 199711L
#       define AIL_LANG_STANDARD 1998
#   elif __cplusplus == 201103L
#       define AIL_LANG_STANDARD 2011
#   elif __cplusplus == 201402L
#       define AIL_LANG_STANDARD 2014
#   elif __cplusplus == 201703L
#       define AIL_LANG_STANDARD 2017
#   elif __cplusplus == 202002L
#       define AIL_LANG_STANDARD 2020
#   elif __cplusplus >= 202302L
#       define AIL_LANG_STANDARD 2023
#   else
#       define AIL_LANG_STANDARD 1989
#   endif
#else
#   define AIL_LANG_OBJC 0
#   define AIL_LANG_CPP 0
#   define AIL_LANG_C   1
#   if __STDC_VERSION__ == 199901L
#       define AIL_LANG_STANDARD 1999
#   elif __STDC_VERSION__ == 201112L
#       define AIL_LANG_STANDARD 2011
#   elif __STDC_VERSION__ == 201710L
#       define AIL_LANG_STANDARD 2017
#   elif __STDC_VERSION__ >= 202311L
#       define AIL_LANG_STANDARD 2023
#   else
#       define AIL_LANG_STANDARD 1989
#   endif
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
#	define AIL_OS_WIN 1
#else
#   define AIL_OS_WIN 0
#endif
#if defined(__linux__) || defined(__linux)
#	define AIL_OS_LINUX 1
#else
#   define AIL_OS_LINUX 0
#endif
#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
#	define AIL_OS_UNIX 1
#else
#   define AIL_OS_UNIX 0
#endif
#if defined(__posix)
#	define AIL_OS_POSIX 1
#else
#   define AIL_OS_POSIX 0
#endif
#if defined(__GNU__)
#	define AIL_OS_GNU 1
#else
#   define AIL_OS_GNU 0
#endif
#if defined(__APPLE__) && defined(__MACH__)
// @Note: There are many more target informations that could be retrieved from TargetConditionals.h
// See the following for more info:
// - https://epir.at/2019/10/30/api-availability-and-target-conditionals/
// - https://github.com/xybp888/iOS-SDKs/blob/master/iPhoneOS13.0.sdk/usr/include/TargetConditionals.h
#   include <TargetConditionals.h>
#   if TARGET_OS_IPHONE // iOS, tvOS, or watchOS device
#       define AIL_OS_MAC 0
#       define AIL_OS_IOS 1
#   elif TARGET_OS_MAC
#       define AIL_OS_MAC 1
#       define AIL_OS_IOS 0
#   else
#      define AIL_OS_MAC 0
#      define AIL_OS_IOS 0
#   endif
#else
#   define AIL_OS_MAC 0
#   define AIL_OS_IOS 0
#endif
#if defined(__ANDROID__)
#	define AIL_OS_ANDROID 1
#else
#   define AIL_OS_ANDROID 0
#endif
#if defined(__wasm__) || defined(__wasm32__) || defined(__wasm64__)
#	define AIL_OS_WASM 1
#else
#   define AIL_OS_WASM 0
#endif
#if defined(BSD)
#	define AIL_OS_BSD 1
#else
#   define AIL_OS_BSD 0
#endif
#if defined(__MINGW32__) || defined(__MINGW64__)
#	define AIL_OS_MINGW 1
#else
#   define AIL_OS_MINGW 0
#endif
#if defined(__CYGWIN__)
#	define AIL_OS_CYGWIN 1
#else
#   define AIL_OS_CYGWIN 0
#endif
#if defined(__sparc__) || defined(__sparc)
#	define AIL_OS_SPARC   1
#   define AIL_ARCH_SPARC 1
#else
#   define AIL_OS_SPARC   0
#   define AIL_ARCH_SPARC 0
#endif

#if defined(__i386) || defined(_M_IX86) || defined(_X86_) || defined(__X86__) || defined(__I86__) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || defined(_M_AMD64) || defined(_M_X64)
#	define AIL_ARCH_X86 1
#else
#   define AIL_ARCH_X86 0
#endif
#if defined(__arm__) || defined(_M_ARM) || defined(_ARM) || defined(__aarch64__)
#	define AIL_ARCH_ARM 1
#else
#   define AIL_ARCH_ARM 0
#endif
#if defined(__mips) || defined(__mips__) || defined(__MIPS__)
#	define AIL_ARCH_MIPS 1
#else
#   define AIL_ARCH_MIPS 0
#endif
#if defined(__riscv) || defined(__riscv64)
#	define AIL_ARCH_RISCV 1
#else
#   define AIL_ARCH_RISCV 0
#endif
#if defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__PPC) || defined(__PPC__) || defined(__PPC64__) || defined(__ppc__) || defined(__ppc64__) || defined(__POWERPC__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64)
#	define AIL_ARCH_PPC 1
#else
#   define AIL_ARCH_PPC 0
#endif

#endif // _AIL_PLATFORM_H_
