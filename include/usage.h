#ifndef _USAGE
#define _USAGE

typedef struct {
	unsigned long user;
	unsigned long nice;
	unsigned long sys;
	unsigned long intr;
	unsigned long idle;
} cpu_usage;

typedef struct {
	unsigned long vm_active;
	unsigned long vm_total;
	unsigned long free;
} memory_usage;

typedef struct {
	unsigned long used;
	unsigned long total;
} swap_usage;

int get_cpu_usage(cpu_usage *cu);
int get_memory_usage(memory_usage *mu);
int get_swap_usage(swap_usage *su);

#endif
