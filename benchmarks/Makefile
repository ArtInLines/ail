all: hm_perf

hm_perf: ail_hm_perf.c
ifeq ($(COMP),cl)
	$(COMP) $(CFLAGS) ail_hm_perf.c Winmm.lib
else
	$(COMP) $(CFLAGS) -o ail_hm_perf ail_hm_perf.c
endif