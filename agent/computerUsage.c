#include "computerUsage.h"

static void
refresh(void *elemA, void *elemB, int elemSize, int nbElems)
{
	void *temp[nbElems * elemSize];

	memcpy(temp, elemA, nbElems * elemSize);
	for (int i = 0; i < nbElems; i++) {
		*((int *) elemA + i * elemSize / sizeof(int)) -= *((int *) elemB + i * elemSize / sizeof(int));
	}
	memcpy(elemB, temp, nbElems * elemSize);
}

int
getCpuUsage(cpuUsage *usage)
{
	FILE *statsFile;
	char fileBuffer[FILE_BUFFER_SIZE];

	int rc = RC_CPU_READING_ERROR;
	statsFile = fopen("/proc/stat", "r");
	if (statsFile != NULL) {
		if (fgets(fileBuffer, FILE_BUFFER_SIZE, statsFile) != NULL) {
			sscanf(fileBuffer,
			       "cpu %lu %lu %lu %lu %lu %lu %lu",
			       curCpu + USER,
			       curCpu + NICE,
			       curCpu + SYSTEM,
			       curCpu + IDLE,
			       curCpu + IOWAIT,
			       curCpu + IRQ,
			       curCpu + SOFTIRQ);

			usage->total = 0;
			refresh(&curCpu, &prevCpu, sizeof(unsigned long), CPU_STATES);
			for (int i = 0; i < CPU_STATES; i++) {
				usage->total += curCpu[i];
			}
			usage->used = usage->total - *(curCpu + IDLE);
		}
		fclose(statsFile);
		rc = RC_CPU_OK;
	}
	return rc;
}

int
getMemoryUsage(memoryUsage *usage)
{
	FILE *memoryFile;
	char fileBuffer[FILE_BUFFER_SIZE];
	char *line;

	int rc = RC_MEM_READING_ERROR;
	memoryFile = fopen("/proc/meminfo", "r");
	if (memoryFile != NULL) {
		while (fgets(fileBuffer, FILE_BUFFER_SIZE, memoryFile) != NULL) {
			line = strstr(fileBuffer, "MemFree:");
			if (line != NULL) {
				sscanf(line,"MemFree: %lu", &usage->free);
			}

			line = strstr(fileBuffer, "Active:");
			if (line != NULL) {
				sscanf(line,"Active: %lu", &usage->active);
			}

			line = strstr(fileBuffer, "Inactive:");
			if (line != NULL) {
				sscanf(line, "Inactive: %lu", &usage->inactive);
			}

			line = strstr(fileBuffer, "SwapCached:");
			if (line != NULL) {
				sscanf(line, "SwapCached: %lu", &usage->swapUsed);
			}

			line = strstr(fileBuffer, "SwapTotal:");
			if (line != NULL) {
				sscanf(line, "SwapTotal: %lu", &usage->swapTotal);
			}
		}

		usage->total = usage->active + usage->inactive + usage->free;

		fclose(memoryFile);
		rc = RC_MEM_OK;
	}
	return rc;
}

int
getNetworkUsage(networkUsage *usage)
{
	FILE *networkFile;
	char fileBuffer[FILE_BUFFER_SIZE];
	char *line;
	unsigned long rfu;

	int rc = RC_NET_READING_ERROR;
	networkFile = fopen("/proc/net/dev", "r");
	if (networkFile != NULL) {
		while (fgets(fileBuffer, FILE_BUFFER_SIZE, networkFile) != NULL) {
			line = strstr(fileBuffer, "eth0:");
			if (line != NULL) {
				sscanf(line,
				       "eth0: %lu %lu %lu %lu %lu %lu %lu %lu %lu",
				       curNet,
				       &rfu, &rfu, &rfu, &rfu, &rfu, &rfu,&rfu,
				       curNet + 1);
				usage->received = 0;
				usage->transmitted = 0;
				refresh(&curNet, &prevNet, sizeof(unsigned long), 2);
				usage->received += *curNet;
				usage->transmitted += *(curNet + 1);
			}
		}
		fclose(networkFile);
		rc = RC_NET_OK;
	}
	return rc;
}

int
getIoUsage(ioUsage *usage)
{
	FILE *ioFile;
	char fileBuffer[FILE_BUFFER_SIZE];
	char *line;
	unsigned long rfu;

	int rc = RC_IO_READING_ERROR;
	ioFile = fopen("/proc/diskstats", "r");
	if (ioFile != NULL) {
		while (fgets(fileBuffer, FILE_BUFFER_SIZE, ioFile) != NULL) {
			line = strstr(fileBuffer, "sda");
			if (line != NULL) {
				sscanf(line,
				       "sda %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
				       &rfu, &rfu, &rfu, &rfu, &rfu, &rfu, &rfu, &rfu,
				       &usage->progress,
				       &rfu, &rfu);
				break;
			}
		}
		fclose(ioFile);
		rc = RC_IO_OK;
	}
	return rc;
}

int
getDiskUsage(diskUsage *usage)
{
	return 0;
}
