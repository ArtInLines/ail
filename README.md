# ail

[stb](https://github.com/nothings/stb)-[style](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt) single-file MIT licensed libraries for C/C++.

## Disclaimer

These libraries are still heavily under development as I experiment with using them in my own projects. There is not any documentation yet and breaking changes might be introduced at any point.

Unlike the original stb-libraries, all libraries here depend on the [ail.h](./ail.h) file, so you should always include that file along with all other libraries you want from here.

All libraries are written in C99.

I'm writing these libraries primarily for myself. I will thus add features that I find useful in a design-style that I like. Alternative usecases are secondary to me.

## Libraries

| library                      | description                                                                                          |
| ---------------------------- | ---------------------------------------------------------------------------------------------------- |
| [ail.h](./ail.h)             | Common definitions used by all other libraries. This file should always be included                  |
| [ail_hm.h](./ail_hm.h)       | Generic Hashmap implementation, where keys and values can be of any type                             |
| [ail_bench.h](./ail_bench.h) | Benchmarking                                                                                         |
| [ail_alloc.h](./ail_alloc.h) | Custom Allocators                                                                                    |
| [ail_sv.h](./ail_sv.h)       | Simple String-View                                                                                   |
| [ail_md.h](./ail_md.h)       | Memory Debugging Utility (slightly modified version of Eskil Steenberg's Forge)                      |
| [ail_buf.h](./ail_buf.h)     | Simple Read-Write Buffer implementation                                                              |
| [ail_ring.h](./ail_ring.h)   | Simple Ring-Buffer implementation                                                                    |
| [ail_fs.h](./ail_fs.h)       | Cross-Platform File-System Functions                                                                 |
| [ail_time.h](./ail_time.h)   | Cross-Platform Time functions                                                                        |
| [ail_gui.h](./ail_gui.h)     | DEPRECATED: GUI-Library for use with Raylib (can only be compiled when raylib.h is already included) |

## Conventions

All libraries here follow the following conventions. Knowing these can make usage more intuitive (especially while documentation is still lacking):

-   Everything (except the type macros in `ail.h`) is prefixed with the name of the library (e.g. `ail_hm_new`). This is done because C99 has no feature for namespacing
-   All functions use snake case (this is not consisten across all libraries yet)
-   Macros are uppe-case, unless they are meant to be used like normal functions
-   Upper-case parameters in a macro definition are Types
-   Parameters to a macro are never meant to be pointers unless they are called `...Ptr` (eg. `daPtr`)

## Notes on Generic Data Structures

There are currently implementation for generic dynamic arrays (prefixed with `ail_da` in [ail.h](./ail.h)) and generic hashmaps ([ail_hm.h](./ail_hm.h)). Both are implemented in very similar ways.

To be generic, they use duck-typing via macros. Thus, any structure that contains the same fields as my dynamic arrays or my hashmaps, can be treated like one.

Instead of having to create a new structure each time, you can simply use `AIL_DA(T)`, where `T` is the type of the elements in your array (same for hashmaps with `AIL_HM(K, V)`, where `K`, `V` are the types for the keys and values). This macro translates to a specific name for the necessary structure.

Some common structures (like `AIL_DA(char)`) are already predefined and can just be used. Other structures still need to be defined of course, which can be done with the macro `AIL_DA_INIT(T)`. Since the initialization macro translates to a type-definition, it neds to be put in your file's global scope. You can see [test/ail_da.c](./test/ail_da.c) for an example usage.

The main benefit of this approach is that you can use any structure that names its fields in the right way as a dynamic array or hashmap. Further, the types of dynamic arrays / hashmaps become very explicit in the code, when variables are defined with `AIL_DA(char) x;`.

## Why is it named 'ail'?

It's the shortform for my github name [ArtInLines](https://github.com/ArtInLines).
