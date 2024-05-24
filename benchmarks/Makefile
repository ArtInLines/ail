# Define MODE=DBG if compiling in debug mode, otherwise release mode is assumed
# Define `COMP` to set the compiler to use
# Defaults to gcc
# Allowed options are:
# - gcc
# - cl
# - clang
# - tcc
# - zig
# - icx-cc

COMP ?= gcc

ifeq ($(COMP),cl)
CFLAGS ?= /W1 /std:c++14
ifeq ($(MODE),DBG)
CFLAGS += /Zi
else
CFLAGS += /O2
endif

else ifeq ($(COMP),gcc)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs
ifeq ($(MODE),DBG)
CFLAGS += -ggdb
else
CFLAGS += -O2
endif

else ifeq ($(COMP),clang)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs
ifeq ($(MODE),DBG)
CFLAGS += -ggdb
else
CFLAGS += -O2
endif

else ifeq ($(COMP),tcc)
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs
ifeq ($(MODE),DBG)
CFLAGS += -ggdb
else
CFLAGS += -O2
endif

else ifeq ($(COMP),zig)
C      := zig cc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs
ifeq ($(MODE),DBG)
CFLAGS += -ggdb
else
CFLAGS += -O2
endif

else ifeq ($(COMP),icx-cc)

CFLAS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs -Rno-debug-disables-optimization
ifeq ($(MODE),DBG)
CFLAGS += -ggdb
else
CFLAGS += -O2
endif

else
$(error Unknown option for COMP. Please use gcc, cl, clang, tcc or zig instead.)
endif

C ?= $(COMP)

all: alloc hm

alloc: ail_alloc.c
	$(C) -o ail_alloc ail_alloc.c $(CFLAGS)

hm: ail_hm.c
	$(C) -o ail_hm ail_hm.c $(CFLAGS)