#ifndef _USAGE
#define _USAGE

#include <sys/param.h>
#include <sys/swap.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <machine/cpu.h>
#include <stdlib.h>
#include <string.h>

unsigned long cur[CPUSTATES];
unsigned long prev[CPUSTATES];

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

unsigned long cur_net[2];
unsigned long prev_net[2];

typedef struct {
	unsigned long received;
	unsigned long transmitted;
} network_usage;

typedef struct {
	unsigned long progress;
} io_usage;

int get_cpu_usage(cpu_usage *cu);
int get_memory_usage(memory_usage *mu);
int get_swap_usage(swap_usage *su);
int get_network_usage(network_usage *nu);
int get_io_usage(io_usage *iu);

#endif
