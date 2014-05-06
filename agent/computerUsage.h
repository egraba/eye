#ifndef _COMPUTER_USAGE
#define _COMPUTER_USAGE

#include <string.h>

#ifdef __FreeBSD__
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

#ifdef __FreeBSD__
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
} cpuUsage;

typedef struct {
	unsigned long active;
	unsigned long inactive;
	unsigned long free;
	unsigned long total;
	unsigned long swapUsed;
	unsigned long swapTotal;
} memoryUsage;

unsigned long curNet[2];
unsigned long prevNet[2];

typedef struct {
	unsigned long received;
	unsigned long transmitted;
} networkUsage;

typedef struct {
	unsigned long progress;
} ioUsage;

/* Prototypes */
int getCpuUsage(cpuUsage *usage);
int getMemoryUsage(memoryUsage *usage);
int getNetworkUsage(networkUsage *usage);
int getIoUsage(ioUsage *usage);

#endif
