#ifndef _COMPUTER_USAGE
#define _COMPUTER_USAGE

#include <stdio.h>
#include <string.h>

#define FILE_BUFFER_SIZE 512
#define CPU_STATES 7

#define RC_CPU_OK 0
#define RC_CPU_READING_ERROR -1

#define RC_MEM_OK 0
#define RC_MEM_READING_ERROR -1

#define RC_NET_OK 0
#define RC_NET_READING_ERROR -1

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
} ioUsage;

typedef struct {
} diskUsage;

/* Prototypes */
int getCpuUsage(cpuUsage *usage);
int getMemoryUsage(memoryUsage *usage);
int getNetworkUsage(networkUsage *usage);
int getIoUsage(ioUsage *usage);
int getDiskUsage(diskUsage *usage);

#endif
