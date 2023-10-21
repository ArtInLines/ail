# ail

[stb](https://github.com/nothings/stb)-[style](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt) single-file MIT licensed libraries for C/C++.

Unlike the original stb-libraries, all libraries here depend on the [ail.h](./ail.h) file, so you should always include that file along with all other libraries you want from here.

All libraries are written in C99.

I'm writing these libraries primarily for myself. I will thus add features that I find useful in a design-style that I like. Alternative usecases are secondary to me.

## Libraries

| library                      | latest version | description                                                                           |
| ---------------------------- | -------------- | ------------------------------------------------------------------------------------- |
| [ail.h](./ail.h)             | 0.1            | Common definitions used by all other libraries. This file should always be included   |
| [ail_alloc.h](./ail_alloc.h) | 0.1            | Allocators                                                                            |
| [ail_fs.h](./ail_fs.h)       | 0.1            | File-System                                                                           |
| [ail_buf.h](./ail_buf.h)     | 0.1            | Simple Read-Write Buffer implementation                                               |
| [ail_gui.h](./ail_gui.h)     | 0.2            | GUI-Library for use with Raylib (can only be compiled with raylib.h already included) |

## Versioning

I may change it in the future but at the moment it is like this this:

`1.23b`

-   `1` = major version
-   `23` = minor version
-   `b` = patch
    -   1.23 => zero patches
    -   1.23a => patch 1
    -   1.23b => patch 2
    -   etc.

For versions below `1.0`, all versions are major or patch versions

## Why is it named 'ail'?

It's the shortform for my github name [ArtInLines](https://github.com/ArtInLines).
