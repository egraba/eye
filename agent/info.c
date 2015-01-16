#include "info.h"

static void
get_os_info(computer *pc)
{
	struct utsname os_info;

	int rc = uname(&os_info);
	if (rc != UNAME_ERROR) {
		pc->os_name = strndup(os_info.sysname, sizeof(os_info.sysname));
		pc->os_release = strndup(os_info.release, sizeof(os_info.release));
		pc->os_arch = strndup(os_info.machine, sizeof(os_info.machine));
	}
}

static void
get_cpu_name(computer *pc)
{
#ifdef BSD
	pc->cpu_name = malloc(BUFSIZ);
	size_t len = BUFSIZ;
	sysctlbyname("hw.model", pc->cpu_name, &len, NULL, 0);
#endif
#ifdef __linux__
	FILE *cpu_info;
	char file_buffer[BUFSIZ];
	char *cpu_name_line;
	int label_size;

	label_size = strlen("model name\t: ");
	cpu_info = fopen("/proc/cpuinfo", "r");

	if (cpu_info != NULL) {
		while(fgets(file_buffer, BUFSIZ, cpu_info) != NULL) {
			cpu_name_line = strstr(file_buffer, "model name\t:");
			if (cpu_name_line != NULL) {
				/* -1 is to remove '\n' */
				pc->cpu_name = strndup(cpu_name_line + label_size,
						      strlen(cpu_name_line) - label_size - 1);
				break;
			}
		}
		fclose(cpu_info);
	}
#endif
}

static void
get_memory_size(computer *pc)
{
#ifdef BSD
	int *physMem = malloc(sizeof(int));
	size_t len = sizeof(phys_mem);

	sysctlbyname("hw.physmem", phys_mem, &len, NULL, 0);
	pc->memory_size = *phys_mem / pow(1024, 2);
#endif
#ifdef __linux__
	FILE *mem_info;
	char file_buffer[BUFSIZ];
	char *mem_size_line;
	int mem_in_kb;

	pc->memory_size = 0;
	mem_info = fopen("/proc/meminfo", "r");

	if (mem_info != NULL) {
		while(fgets(file_buffer, BUFSIZ, mem_info) != NULL) {
			mem_size_line = strstr(file_buffer, "MemTotal:");

			if (mem_size_line != NULL) {
				sscanf(mem_size_line, "MemTotal: %d", &mem_in_kb);
				pc->memory_size = mem_in_kb / 1024;
				break;
			}
		}

		fclose(mem_info);
	}
#endif
}

static void
get_network_info(computer *pc)
{
	struct ifaddrs *ifa;
	struct ifaddrs *ifs;
	unsigned char *mac_addr;
	struct sockaddr_in *ip_addr;
	struct sockaddr_in6 *ip_addr6;
	char *ip_if;

	mac_addr = malloc(sizeof(struct sockaddr));
	ip_addr = malloc(sizeof(struct sockaddr_in));
	ip_addr6 = malloc(sizeof(struct sockaddr_in6));
	getifaddrs(&ifs);

#ifdef BSD
	/*
	 * TODO: Have a generic mechanism to deal with interfaces.
	 */
	ip_if = strdup("bge0");
#endif
#ifdef __linux__
	ip_if = strdup("eth0");
#endif

	for (ifa = ifs; ifa != NULL; ifa = ifa->ifa_next) {
		if (!strcmp(ifa->ifa_name, ip_if)) {
			if (ifa->ifa_addr->sa_family == ETH_IF) {
				mac_addr = (unsigned char*)((struct sockaddr*)ifa->ifa_addr->sa_data);
				mac_addr += 10;
				sprintf(pc->mac_address,
					"%02x:%02x:%02x:%02x:%02x:%02x",
					*mac_addr,
					*(mac_addr + 1),
					*(mac_addr + 2),
					*(mac_addr + 3),
					*(mac_addr + 4),
					*(mac_addr + 5));
			} else if (ifa->ifa_addr->sa_family == AF_INET) {
				ip_addr->sin_addr.s_addr =
					((struct sockaddr_in*)(ifa->ifa_addr))->sin_addr.s_addr;
				strncpy(pc->ipv4_address, inet_ntoa(ip_addr->sin_addr), INET_ADDRSTRLEN);
			} else if (ifa->ifa_addr->sa_family == AF_INET6) {
				ip_addr6->sin6_addr =
					((struct sockaddr_in6*)(ifa->ifa_addr))->sin6_addr;
				inet_ntop(AF_INET6,
					  (const void *)&ip_addr6->sin6_addr,
					  pc->ipv6_address,
					  INET6_ADDRSTRLEN);
			}
		}
	}
}

/*
 * Collects all computer information
 */
int
collect_info(computer *info)
{
	int rc = INFO_READING_OK;

	get_os_info(info);
	get_cpu_name(info);
	get_memory_size(info);
	get_network_info(info);

	if ((info->os_name == NULL)
	    || (info->os_release == NULL)
	    || (info->os_arch == NULL)
	    || (info->cpu_name == NULL)
	    || (info->memory_size == 0)) {
		rc = INFO_READING_ERROR;
	}

	return rc;
}
