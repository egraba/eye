#include "usage.h"

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
get_cpu_usage(cpu_usage *usage)
{
#ifdef __FreeBSD__
	size_t len;
	
	/*
	 * TODO: Manage several processors.
	 */
	len = sizeof(cur_cpu);
	sysctlbyname("kern.cp_times", cur_cpu, &len, NULL, 0);
	refresh(&cur_cpu, &prev_cpu, sizeof(unsigned long), CPUSTATES);
	
	usage->total = 0;
	for (int i = 0; i < CPUSTATES; i++) {
		usage->total += cur_cpu[i];
	}
	usage->used = usage->total - *(cur_cpu + CP_IDLE);
#endif
#ifdef __linux__
	FILE *stats_file;
	char file_buffer[BUFSIZ];

	int rc = RC_CPU_READING_ERROR;
	stats_file = fopen("/proc/stat", "r");
	if (stats_file != NULL) {
		if (fgets(file_buffer, BUFSIZ, stats_file) != NULL) {
			sscanf(file_buffer,
			       "cpu %lu %lu %lu %lu %lu %lu %lu",
			       cur_cpu + USER,
			       cur_cpu + NICE,
			       cur_cpu + SYSTEM,
			       cur_cpu + IDLE,
			       cur_cpu + IOWAIT,
			       cur_cpu + IRQ,
			       cur_cpu + SOFTIRQ);

			usage->total = 0;
			refresh(&cur_cpu, &prev_cpu, sizeof(unsigned long), CPU_STATES);
			for (int i = 0; i < CPU_STATES; i++) {
				usage->total += cur_cpu[i];
			}
			usage->used = usage->total - *(cur_cpu + IDLE);
		}
		fclose(stats_file);
		rc = RC_CPU_OK;
	}
	return rc;
#endif
}

int
get_memory_usage(memory_usage *usage)
{
#ifdef __FreeBSD__
	struct vmtotal mem;
	size_t len;
	int page_size;
	
	len = sizeof(mem);
	sysctlbyname("vm.vmtotal", &mem, &len, NULL, 0);
	len = sizeof(page_size);
	sysctlbyname("vm.stats.vm.v_page_size", &page_size, &len, NULL, 0);

	usage->total = mem.t_vm / 1024 / 1024 * 2;
	usage->active = mem.t_avm / 1024 / 10;
	usage->free = mem.t_free * page_size / 1024 / 1024;
	usage->inactive = usage->total - usage->active;
#endif
#ifdef __linux__
	FILE *memory_file;
	char file_buffer[BUFSIZ];
	char *line;

	int rc = RC_MEM_READING_ERROR;
	memory_file = fopen("/proc/meminfo", "r");
	if (memory_file != NULL) {
		while (fgets(file_buffer, BUFSIZ, memory_file) != NULL) {
			line = strstr(file_buffer, "MemFree:");
			if (line != NULL) {
				sscanf(line,"MemFree: %lu", &usage->free);
			}

			line = strstr(file_buffer, "Active:");
			if (line != NULL) {
				sscanf(line,"Active: %lu", &usage->active);
			}

			line = strstr(file_buffer, "Inactive:");
			if (line != NULL) {
				sscanf(line, "Inactive: %lu", &usage->inactive);
			}

			line = strstr(file_buffer, "SwapCached:");
			if (line != NULL) {
				sscanf(line, "SwapCached: %lu", &usage->swapUsed);
			}

			line = strstr(file_buffer, "SwapTotal:");
			if (line != NULL) {
				sscanf(line, "SwapTotal: %lu", &usage->swapTotal);
			}
		}

		usage->total = usage->active + usage->inactive + usage->free;

		fclose(memory_file);
		rc = RC_MEM_OK;
	}
	return rc;
#endif
}

int
get_network_usage(network_usage *usage)
{
#ifdef __FreeBSD__

#endif
#ifdef __linux__
	FILE *network_file;
	char file_buffer[BUFSIZ];
	char *line;
	unsigned long rfu;

	int rc = RC_NET_READING_ERROR;
	network_file = fopen("/proc/net/dev", "r");
	if (network_file != NULL) {
		while (fgets(file_buffer, BUFSIZ, network_file) != NULL) {
			line = strstr(file_buffer, "eth0:");
			if (line != NULL) {
				sscanf(line,
				       "eth0: %lu %lu %lu %lu %lu %lu %lu %lu %lu",
				       cur_net,
				       &rfu, &rfu, &rfu, &rfu, &rfu, &rfu,&rfu,
				       cur_net + 1);
				usage->received = 0;
				usage->transmitted = 0;
				refresh(&cur_net, &prev_net, sizeof(unsigned long), 2);
				usage->received += *cur_net;
				usage->transmitted += *(cur_net + 1);
			}
		}
		fclose(network_file);
		rc = RC_NET_OK;
	}
	return rc;
#endif
}

int
get_io_usage(io_usage *usage)
{
#ifdef __FreeBSD__

#endif
#ifdef __linux__
	FILE *io_file;
	char file_buffer[BUFSIZ];
	char *line;
	unsigned long rfu;

	int rc = RC_IO_READING_ERROR;
	io_file = fopen("/proc/diskstats", "r");
	if (io_file != NULL) {
		while (fgets(file_buffer, BUFSIZ, io_file) != NULL) {
			line = strstr(file_buffer, "sda");
			if (line != NULL) {
				sscanf(line,
				       "sda %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
				       &rfu, &rfu, &rfu, &rfu, &rfu, &rfu, &rfu, &rfu,
				       &usage->progress,
				       &rfu, &rfu);
				break;
			}
		}
		fclose(io_file);
		rc = RC_IO_OK;
	}
	return rc;
#endif
}
