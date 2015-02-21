#ifndef _INFO
#define _INFO

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *sysname;
	char *nodename;
	char *release;
	char *version;
	char *machine;
	char *cpuname;
	int ncpus;
	int physmem;
} machine;

int collect_info(machine *m);

#endif
