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

#define INFO_MSG_LEN (\
SYSNAME_LEN + \
NODENAME_LEN + \
RELEASE_LEN + \
VERSION_LEN + \
MACHINE_LEN + \
CPUNAME_LEN + \
NCPUS_LEN + \
PHYSMEM_LEN \
)

int collect_info(machine *m);

#endif
