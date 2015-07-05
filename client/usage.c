#include <sys/param.h>
#include <sys/swap.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <machine/cpu.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "usage.h"



unsigned long cur[CPUSTATES];
unsigned long prev[CPUSTATES];



int
get_cpu_usage(cpu_usage *cu)
{
	int rc;
	int mib[2];
	size_t len;
	unsigned long tmp[CPUSTATES];
	
	mib[0] = CTL_KERN;
	mib[1] = KERN_CPTIME;
	len = sizeof(cur);
	rc = sysctl(mib, 2, cur, &len, NULL, 0);
	if (rc != -1) {
		int i;

		for (i = 0; i < CPUSTATES; i++) {
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
	
	mib[0] = CTL_VM;
	mib[1] = VM_METER;
	len = sizeof(mem);
	rc = sysctl(mib, 2, &mem, &len, NULL, 0);

	mu->vm_active = mem.t_avm;
	mu->vm_total = mem.t_vm;
	mu->free = mem.t_free;

	return (rc);
}

int
get_swap_usage(swap_usage *su)
{
	int rc;
	int nswap;
	struct swapent swap;

	nswap = swapctl(SWAP_NSWAP, 0, 0);
	if (nswap < 1)
		rc = -1;
	else {
		rc = swapctl(SWAP_STATS, &swap, nswap);
		if (rc != -1) {
			su->used = swap.se_inuse;
			su->total = swap.se_nblks;
		}
	}

	return (rc);
}
