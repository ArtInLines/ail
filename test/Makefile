ifndef $(CC)
CC=gcc
endif

CFLAGS=-Wall -Wextra -Wpedantic -g -Wno-unused-function

all: da macros sv fs hm hm_perf

da: ail_da.c
	$(CC) $(CFLAGS) -o ail_da ail_da.c

macros: ail_macros.c
	$(CC) $(CFLAGS) -o ail_macros ail_macros.c

sv: ail_sv.c
	$(CC) $(CFLAGS) -o ail_sv ail_sv.c

fs: ail_fs.c
	$(CC) $(CFLAGS) -o ail_fs ail_fs.c

hm: ail_hm.c
	$(CC) $(CFLAGS) -o ail_hm ail_hm.c

hm_perf: ail_hm_perf.c
	$(CC) $(CFLAGS) -o ail_hm_perf ail_hm_perf.c
