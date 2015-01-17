#include "usage.h"

int
get_cpu_usage(cpu_usage *cu)
{
	int rc;
	int mib[2];
	size_t len;
	unsigned long tmp[5];
	
	mib[0] = CTL_KERN;
	mib[1] = KERN_CPTIME;
	len = sizeof(cur);
	rc = sysctl(mib, 2, cur, &len, NULL, 0);
	if (rc != -1) {
		int i;

		for (i = 0; i < 5; i++) {
			tmp[i] = cur[i];
			cur[i] -= prev[i];
			prev[i] = tmp[i];
		}
	}

	cu->user = cur[0];
	cu->nice = cur[1];
	cu->sys = cur[2];
	cu->intr = cur[3];
	cu->idle = cur[4];

	return (rc);
}

int
get_memory_usage(memory_usage *mu)
{
	int rc;
	int mib[2];
	struct vmtotal mem;
	size_t len;
	int page_size;
	
	mib[0] = CTL_VM;
	mib[1] = VM_METER;
	len = sizeof(mem);
	rc = sysctl(mib, 2, &mem, &len, NULL, 0);
/*	len = sizeof(page_size);
	rc = sysctlbyname("vm.stats.vm.v_page_size", &page_size, &len, NULL, 0);

	usage->total = mem.t_vm / 1024 / 1024 * 2;
	usage->active = mem.t_avm / 1024 / 10;
	usage->free = mem.t_free * page_size / 1024 / 1024;
	usage->inactive = usage->total - usage->active;*/

	mu->vm_active = mem.t_avm;
	mu->vm_total = mem.t_vm;
	mu->free = mem.t_free;

	return (rc);
}

int
get_network_usage(network_usage *usage)
{

}

int
get_io_usage(io_usage *usage)
{

}
