# Define `COMP` to set the compiler to use
# Defaults to gcc
# Allowed options are:
# - "gcc"       GNU Compiler Collection: https://gcc.gnu.org/ or on Windows using https://www.mingw-w64.org/
# - "cl"        Microsoft Visual C (MSVC): https://learn.microsoft.com/en-us/cpp/build/reference/compiler-command-line-syntax?view=msvc-170
# - "msvc"      Microsoft Visual C (MSVC): https://learn.microsoft.com/en-us/cpp/build/reference/compiler-command-line-syntax?view=msvc-170
# - "clang"     LLVM frontend for C: https://clang.llvm.org/
# - "tcc"       Tiny C Compiler: https://www.bellard.org/tcc/
# - "zig"       Zig Language's C cross-compiler: https://ziglang.org/ (see also: https://andrewkelley.me/post/zig-cc-powerful-drop-in-replacement-gcc-clang.html)
# - "icx-cc"    Intel's Optimizing C compiler: https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit.html
# - "icx-cl"    Intel's Optimizing C compiler: https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit.html
# - "dmc"       Digital Mars C compiler: https://www.digitalmars.com/download/freecompiler.html
# - "pelles"    Pelles C compiler: http://www.smorgasbordet.com/pellesc/

# Interesting C Compilers that should maybe be tested some day but are not yet supported (usually because of lacking Windows support):
# - cproc    (https://git.sr.ht/~mcf/cproc)
# - scc      (https://git.simple-cc.org/)
# - CompCert (https://compcert.org/compcert-C.html)
# - Open64   (https://sourceforge.net/projects/open64/)
# - chibicc  (https://github.com/rui314/chibicc)


COMP ?= gcc

ifeq ($(COMP),msvc)
C := cl
endif

ifeq ($(COMP),cl)
CFLAGS ?= /W1 /std:c++14 /Zi
else ifeq ($(COMP),gcc)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -Wno-unused-function -Wno-unused-variable -Wno-unused-local-typedefs -Wno-zero-length-array -Wno-sign-compare -ggdb
else ifeq ($(COMP),clang)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -Wno-unused-function -Wno-unused-variable -Wno-unused-local-typedefs -Wno-zero-length-array -Wno-sign-compare -ggdb
else ifeq ($(COMP),tcc)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -Wno-unused-function -Wno-unused-variable -Wno-unused-local-typedefs -Wno-zero-length-array -Wno-sign-compare -ggdb
else ifeq ($(COMP),zig)
C      := zig cc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -Wno-unused-function -Wno-unused-variable -Wno-unused-local-typedefs -Wno-zero-length-array -Wno-sign-compare -ggdb
else ifeq ($(COMP),icx-cc)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -Wno-unused-function -Wno-unused-variable -Wno-unused-local-typedefs -Wno-zero-length-array -Wno-sign-compare -ggdb -Rno-debug-disables-optimization
else ifeq ($(COMP),dmc)

else ifeq ($(COMP),pelles)
C      := cc

else
$(error Unknown option for COMP. Please use gcc, cl, clang, tcc or zig instead.)
endif

C ?= $(COMP)

all: da macros math sv fs hm alloc buf ring pm

da: ail_da.c
	$(C) $(CFLAGS) -o ail_da ail_da.c

macros: ail_macros.c
	$(C) $(CFLAGS) -o ail_macros ail_macros.c

math: ail_math.c
	$(C) $(CFLAGS) -o ail_math ail_math.c

sv: ail_sv.c
	$(C) $(CFLAGS) -o ail_sv ail_sv.c

fs: ail_fs.c
	$(C) $(CFLAGS) -o ail_fs ail_fs.c

hm: ail_hm.c
	$(C) $(CFLAGS) -o ail_hm ail_hm.c

alloc: ail_alloc.c
	$(C) $(CFLAGS) -o ail_alloc ail_alloc.c

buf: ail_buf.c
	$(C) $(CFLAGS) -o ail_buf ail_buf.c

ring: ail_ring.c
	$(C) $(CFLAGS) -o ail_ring ail_ring.c

pm: ail_pm.c
	$(C) $(CFLAGS) -o ail_pm ail_pm.c
