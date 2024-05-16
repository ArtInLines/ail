# Define `COMP` to set the compiler to use
# Defaults to gcc
# Allowed options are:
# - gcc
# - cl
# - clang
# - tcc
# - zig
# - icx-cc

# Interesting C Compilers that should maybe be tested some day but are not yet supported:
# - cproc    (https://git.sr.ht/~mcf/cproc)
# - scc      (https://git.simple-cc.org/)
# - CompCert (https://compcert.org/compcert-C.html)
# - Open64   (https://sourceforge.net/projects/open64/)


COMP ?= gcc

ifeq ($(COMP),cl)
CFLAGS ?= /W1 /std:c++14 /Zi
else ifeq ($(COMP),gcc)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs -ggdb
else ifeq ($(COMP),clang)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs -ggdb
else ifeq ($(COMP),tcc)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs -ggdb
else ifeq ($(COMP),zig)
C      := zig cc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs -ggdb
else ifeq ($(COMP),icx-cc)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs -ggdb -Rno-debug-disables-optimization
else
$(error Unknown option for COMP. Please use gcc, cl, clang, tcc or zig instead.)
endif

C ?= $(COMP)

all: da macros sv fs hm alloc buf ring

da: ail_da.c
	$(C) $(CFLAGS) -o ail_da ail_da.c

macros: ail_macros.c
	$(C) $(CFLAGS) -o ail_macros ail_macros.c

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