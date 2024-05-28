# Predefined Macros

This is a list of some useful predefined macros.

A bigger list (that isn't strictly a superset) can be found here: https://sourceforge.net/p/predef/wiki/Home/

## To detect Compilers

| Macro-Name       | Compiler    | Comment                                                                              |
| ---------------- | ----------- | ------------------------------------------------------------------------------------ |
| `__clang__`      | Clang + Zig | Might not stay predefined in Zig if/when they switch backends                        |
| `_MSC_VER`       | MSVC        |                                                                                      |
| `__GNUC__`       | GCC         | Sometimes apparently also defined on Clang & Zig, so check for them first            |
| `__MINGW32__`    | MinGW       | Defined on 32- and 64-bit version, use __MINGW64__ for detecting 64-bit version only |
| `__TINYC__`      | TinyC       |                                                                                      |
| `__EMSCRIPTEN__` | Emscripten  |                                                                                      |

## To detect C/C++ standard

| Macro                         | Standard      |
| ----------------------------- | ------------- |
| `__STDC__`                    | C89 or higher |
| `__STDC_VERSION__ == 199901L` | C99           |
| `__STDC_VERSION__ == 201112L` | C11           |
| `__STDC_VERSION__ == 201710L` | C17           |
| `__STDC_VERSION__ == 202311L` | C23           |
| `__cplusplus`                 | C++           |
| `__cplusplus == 1`            | pre C++98     |
| `__cplusplus == 199711L`      | C++98         |
| `__cplusplus == 201103L`      | C++11         |
| `__cplusplus == 201402L`      | C++14         |
| `__cplusplus == 201703L`      | C++17         |
| `__cplusplus == 202002L`      | C++20         |
| `__cplusplus == 202302L`      | C++23         |

## To detect OS

| Macro                    | Operating System |
| ------------------------ | ---------------- |
| `_WIN32 \|\| __WIN32__`  | Win32            |
| `_WIN64`                 | Win64            |
| `__posix`                | POSIX            |
| `__unix`                 | UNIX             |
| `__linux__`              | Linux            |
| `__GNU__`                | GNU              |
| `BSD`                    | BSD              |
| `__ANDROID__`            | Android          |
| `__MINGW32__`            | MinGW32          |
| `__MINGW64__`            | MinGW64          |
| `__CYGWIN__`             | Cygwin           |
| `__sparc__ \|\| __sparc` | SPARC            |
| `__APPLE__ && __MACH__`  | MacOS            |
| See example below        | IOS              |

### Example for IOS / Apple (Source: https://stackoverflow.com/a/5920028/13764271)

```c
#if __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
         // iOS, tvOS, or watchOS Simulator
    #elif TARGET_OS_MACCATALYST
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
    #elif TARGET_OS_IPHONE
        // iOS, tvOS, or watchOS device
    #elif TARGET_OS_MAC
        // Other kinds of Apple platforms
    #else
    	#error "Unknown Apple platform"
    #endif
#endif
```

## To detect Architecture

| Macro                               | Architecture |
| ----------------------------------- | ------------ |
| `__x86_64  \|\| __x86_64__`         | x86-64       |
| `__amd64__ \|\| _M_AMD64`           | AMD64        |
| `__arm__   \|\| _M_ARM`             | ARM          |
| `__aarch64__`                       | ARM64        |
| `__i386    \|\| _M_IX86 \|\| _X86_` | Intel x86    |
| `__mips    \|\| __mips__`           | MIPS         |

## Existence of Architecture Extensions
<!-- @TODO -->
TBD