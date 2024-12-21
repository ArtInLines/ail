# ail

A modern Standard Library for C with a focus on usability, portability and performance.

Since this library does not rely on C's standard library, it should work on all platforms including WASM and embedded environments.

## Disclaimer

This library is still heavily under development as I experiment with using it in my own projects. Many features are not implemented yet, there is very little documentation and breaking changes might be introduced at any point.

It is not recommended to use this library for production yet.

All libraries are written in C11 (often even C99). C++ portability is not a high priority right now.

## Structure

The structure of the library is explained in [src/README.md](./src/README.md)

## Conventions

The general naming scheme for function is as follows: `<ail>_<type>_<action>_<modifier>+`.

Anything named with leading underscores is not meant as part of the module's public interface. Even once this library is at a point where it doesn't change much anymore, these functions/variables/types/macros may be changed or removed at any point.

### Naming Conventions

- `new`: in a function name indicates an allocation for an object
- `from`: in a function name indicates creation of an object without any allocation
- `init`: Initializes a data-structure or module, potentially setting some hidden state - might allocate stuff
- `deinit`: Resets whatever the corresponding `init` function set and free what it allocated

### Upper-/Lowercase?

- Functions: **lower-snake-case**
- Function-like macros: (usually) **lower-snake-case**
- Other Macros: **upper-snake-case**
- Variables: **lower-snake-case**
- Typenames: **upper-snake-case**
- Primitive Type Aliases: **lower-case**

### Generic Data-Structures

There are several implementations for generic data-structures in this library (e.g. dynamic arrays in [ail_arr.h](./src/base/ail_arr.h)).
These are implemented as duck-typed templates using C macro system.
For explanation, the dynamic array implementation from [ail_arr.h](./src/base/ail_arr.h) shall be taken as an example:

To create a dynamic array of `T`s (where `T` is any type), you need to create a new struct for this dynamic array of `T`s.
This struct must have at least the following attributes:
- `T *data`
- `u64 len`
- `u64 cap`
- `AIL_Allocator al`
Any function for dynamic arrays, works with any struct that has these attributes.

To streamline this use-case, the `AIL_DA_INIT(T)` macro creates a struct with only exactly those attributes.
The name of this templated type is `AIL_DA_T` and can be used with `AIL_DA(T)` as well.

## Why is it named 'ail'?

It's the acronym for my github name [ArtInLines](https://github.com/ArtInLines).
