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
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-function
ifeq ($(MODE), RELEASE)
CFLAGS += -O2
else
CFLAGS += -ggdb
endif
endif

all: da macros sv fs hm hm_perf

da: ail_da.c
	$(COMP) $(CFLAGS) -o ail_da ail_da.c

macros: ail_macros.c
	$(COMP) $(CFLAGS) -o ail_macros ail_macros.c

sv: ail_sv.c
	$(COMP) $(CFLAGS) -o ail_sv ail_sv.c

fs: ail_fs.c
	$(COMP) $(CFLAGS) -o ail_fs ail_fs.c

hm: ail_hm.c
	$(COMP) $(CFLAGS) -o ail_hm ail_hm.c

hm_perf: ail_hm_perf.c
ifeq ($(COMP),cl)
	$(COMP) $(CFLAGS) ail_hm_perf.c Winmm.lib
else
	$(COMP) $(CFLAGS) -o ail_hm_perf ail_hm_perf.c
endif