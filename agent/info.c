#include "info.h"

static int
get_os_info(machine *m)
{
	int rc;
	struct utsname sys;

	rc = uname(&sys);
	if (rc != -1) {
		m->sysname = strndup(sys.sysname, sizeof(sys.sysname));
		m->nodename = strndup(sys.nodename, sizeof(sys.nodename));
		m->release = strndup(sys.release, sizeof(sys.release));
		m->version = strndup(sys.version, sizeof(sys.version));
		m->machine = strndup(sys.machine, sizeof(sys.machine));
	}

	return (rc);
}

static int
get_cpu_name(machine *m)
{
	int rc;
	int mib[2];
	size_t len;

	mib[0] = CTL_HW;
	mib[1] = HW_MODEL;
	m->cpuname = malloc(BUFSIZ);
	len = BUFSIZ;
	rc = sysctl(mib, 2, m->cpuname, &len, NULL, 0);

	return (rc);
}

static int
get_cpu_number(machine *m)
{
	int rc;
	int mib[2];
	size_t len;

	mib[0] = CTL_HW;
	mib[1] = HW_NCPU;
	len = sizeof(mib);	

	rc = sysctl(mib, 2, &(m->ncpus), &len, NULL, 0);

	return (rc);
}

static int
get_memory_size(machine *m)
{
	int rc;
	int mib[2];
	size_t len;

	mib[0] = CTL_HW;
	mib[1] = HW_PHYSMEM64;
	len = sizeof(mib);

	rc = sysctl(mib, 2, &(m->physmem), &len, NULL, 0);

	return (rc);
}

int
collect_info(machine *m)
{
	int rc;
	int nerrors;

	nerrors = 0;

	rc = get_os_info(m);
	if (rc < 0)
		nerrors += 1;

	rc = get_cpu_name(m);
	if (rc < 0)
		nerrors += 1;

	rc = get_cpu_number(m);
	if (rc < 0)
		nerrors += 1;

	rc = get_memory_size(m);
	if (rc < 0)
		nerrors += 1;

	return (nerrors);
}
