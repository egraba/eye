#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static void
print_raw_data(char *data, int len)
{
	int i;
	int j;

	for (i = 0; i < len; i += 16) {
		for (j = 0; j < 16; j++) {
			if (i + j < len)
				printf("%02X ", data[i + j]);
			else
				printf("   ");
		}

		for (j = 0; j < 16; j++) {
			if (i + j < len) {
				if (data[i + j] == '\0')
					printf(".");
				else
					printf("%c", data[i + j]);
			}
		}
		printf("\n");
	}
}

int
parse_info(machine *info, char *data, int data_len)
{
	printf("--- Info message BEGIN ---\n");
	print_raw_data(data, data_len);
	printf("--- Info message END ---\n\n");

	printf("--- Info message data ---\n");
	info->sysname = strndup(data, SYSNAME_LEN);
	printf("sysname:  %s\n", info->sysname);
	data += SYSNAME_LEN;
	
	info->nodename = strndup(data, NODENAME_LEN);
	printf("nodename: %s\n", info->nodename);
	data += NODENAME_LEN;

	info->release = strndup(data, RELEASE_LEN);
	printf("release:  %s\n", info->release);
	data += RELEASE_LEN;

	info->version = strndup(data, VERSION_LEN);
	printf("version:  %s\n", info->version);
	data += VERSION_LEN;

	info->machine = strndup(data, MACHINE_LEN);
	printf("machine:  %s\n", info->machine);
	data += MACHINE_LEN;

	info->cpuname = strndup(data, CPUNAME_LEN);
	printf("cpuname:  %s\n", info->cpuname);
	data += CPUNAME_LEN;

	info->ncpus = atoi(strndup(data, NCPUS_LEN));
	printf("ncpus:    %d\n", info->ncpus);
	data += NCPUS_LEN;

	info->physmem = atoi(strndup(data, PHYSMEM_LEN));
	printf("physmem:  %d\n", info->physmem);
	printf("\n");

	return (0);
}

int
parse_usage(cpu_usage *cpu,
	    memory_usage *mem,
	    swap_usage *swap,
	    char *data,
	    int data_len)
{
	printf("--- Usage message BEGIN ---\n");
	print_raw_data(data, data_len);
	printf("--- Usage message END ---\n\n");

	printf("--- Usage message data ---\n");
	cpu->user = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("cpu_user:      %lu\n", cpu->user);
	data += USAGE_DATA_LEN;

	cpu->nice = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("cpu_nice:      %lu\n", cpu->nice);
	data += USAGE_DATA_LEN;

	cpu->sys = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("cpu_sys:       %lu\n", cpu->sys);
	data += USAGE_DATA_LEN;

	cpu->intr = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("cpu_intr:      %lu\n", cpu->intr);
	data += USAGE_DATA_LEN;

	cpu->idle = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("cpu_idle:      %lu\n", cpu->idle);
	data += USAGE_DATA_LEN;

	mem->vm_active = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("mem_vm_active: %lu\n", mem->vm_active);
	data += USAGE_DATA_LEN;

	mem->vm_total = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("mem_vm_total:  %lu\n", mem->vm_total);
	data += USAGE_DATA_LEN;

	mem->free = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("mem_free:      %lu\n", mem->free);
	data += USAGE_DATA_LEN;

	swap->used = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("swap_used:     %lu\n", swap->used);
	data += USAGE_DATA_LEN;

	swap->total = strtoul(strndup(data, USAGE_DATA_LEN), NULL, 0);
	printf("swap_total:    %lu\n", swap->total);
	printf("\n");

	return (0);
}
