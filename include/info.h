#ifndef _INFO
#define _INFO

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
