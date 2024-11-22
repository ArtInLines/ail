# Code-Structure

`ail` consists of several modules. A module is a directory containing a list of related files together.

You can include specific files from a module directly or you can instead also include the `ail_<module_name>_all.h` file to include all files from a specific module.

Similarly, if you want to include all modules, you can simply include `ail_all.h`.

## Dependencies

Unlike [stb-style header-only-libraries](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt) there a dependencies betweeen files in `ail`, making it sadly not possible to just copy-paste a single file into your project and including it.

This library should be very easy to include regardless, as you can simply copy-paste directories of files into your project and simply including those.
Since all files in `ail` are header files (including implementations that can be disabled when including the files (see [below](#usage))), using this library is as easy as taking the source directories and adding `#include`s into your project.

Dependencies between files in `ail` follow a clear structure, allowing you to easily remove unrequired files:
- Files within a module may depend on each other
- Files within a module may depend on the `base` module
- Files within a module may depend on headers provided by the target platform (e.g. `<windows.h>`, `<sys/mman.h>`, etc.)
- Files withing a module may not depend on anything else

## Usage

All files within modules are `.h` files containing both declarations and implementations. By default the implementations are included as well, however you can chose not to include any implementation by specifying specific macros:
- To not include any implementations at all, simply define `AIL_NO_IMPL` before including the headers
- To not include any implementations from a specific module, define `AIL_NO_<MODULE>_IMPL` before including the module headers
- To not include the implementations from a specific file, define `AIL_NO_<FILE>_IMPL` before including the file

## Modules

| Module | Description                                                                                                                                                      |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| base   | Base-Module included by all other modules; contains useful macros, typedefs, print-formatting, strings, allocators, simple container types, and some more things |
| math   | Mathematics                                                                                                                                                      |
| fs     | File-System functions for working with files, directories, terminal and paths                                                                                    |
| net    | Networking                                                                                                                                                       |
| proc   | Multi-Processing, Threading and Atomics                                                                                                                          |
| time   | Time & Dates                                                                                                                                                     |
| bench  | Benchmarking utilities                                                                                                                                           |
