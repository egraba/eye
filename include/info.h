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

#define SYSNAME_LEN 10
#define NODENAME_LEN 50
#define RELEASE_LEN 5
#define VERSION_LEN 20
#define MACHINE_LEN 10
#define CPUNAME_LEN 50
#define NCPUS_LEN 2
#define PHYSMEM_LEN 16

int collect_info(machine *m);

#endif
