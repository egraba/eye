#ifndef _USAGE
#define _USAGE

#include <string.h>

#ifdef BSD
#include <stdlib.h>
#include <devstat.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#endif

#ifdef __linux__
#include <stdio.h>
#endif

#ifdef __linux__
#define CPU_STATES 7

#define RC_CPU_OK 0
#define RC_CPU_READING_ERROR -1

#define RC_MEM_OK 0
#define RC_MEM_READING_ERROR -1

#define RC_NET_OK 0
#define RC_NET_READING_ERROR -1

#define RC_IO_OK 0
#define RC_IO_READING_ERROR -1
#endif

#ifdef BSD
unsigned long curCpu[CPUSTATES];
unsigned long prevCpu[CPUSTATES];
#endif

#ifdef __linux__
unsigned long curCpu[CPU_STATES];
unsigned long prevCpu[CPU_STATES];

enum cpuState {
	USER,
	NICE,
	SYSTEM,
	IDLE,
	IOWAIT,
	IRQ,
	SOFTIRQ
};
#endif

typedef struct {
	unsigned long used;
	unsigned long total;
} cpu_usage;

typedef struct {
	unsigned long active;
	unsigned long inactive;
	unsigned long free;
	unsigned long total;
	unsigned long swap_used;
	unsigned long swap_total;
} memory_usage;

unsigned long cur_net[2];
unsigned long prev_net[2];

typedef struct {
	unsigned long received;
	unsigned long transmitted;
} network_usage;

typedef struct {
	unsigned long progress;
} io_usage;

/* Prototypes */
int get_cpu_usage(cpu_usage *usage);
int get_memory_usage(memory_usage *usage);
int get_network_usage(network_usage *usage);
int get_io_usage(io_usage *usage);

#endif
