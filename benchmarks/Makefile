# Define OS=WIN if compiling for windows
# Define MODE=RELEASE if compiling in release mode

MODE ?= DEBUG

ifeq ($(OS),WIN)
COMP   ?= cl
CFLAGS ?= /W1 /std:c++14
ifeq ($(MODE),RELEASE)
CFLAGS += /o2
else
CFLAGS += /Zi
endif

else
COMP   ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function -Wno-unused-local-typedefs
ifeq ($(MODE), RELEASE)
CFLAGS += -O2
else
CFLAGS += -ggdb
endif
endif

all: hm

alloc: ail_alloc.c
	$(COMP) -o ail_alloc ail_alloc.c $(CFLAGS)

hm: ail_hm.c
	$(COMP) -o ail_hm ail_hm.c $(CFLAGS)