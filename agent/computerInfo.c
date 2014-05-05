#include "computerInfo.h"

static void
getOsInfo(computer *pc)
{
	struct utsname osInfo;

	int rc = uname(&osInfo);
	if (rc != UNAME_ERROR) {
		pc->osName = strndup(osInfo.sysname, sizeof(osInfo.sysname));
		pc->osRelease = strndup(osInfo.release, sizeof(osInfo.release));
		pc->osArch = strndup(osInfo.machine, sizeof(osInfo.machine));
	}
}

static void
getCpuName(computer *pc)
{
#ifdef __FreeBSD__
	pc->cpuName = malloc(BUFSIZ);
	size_t len = BUFSIZ;
	sysctlbyname("hw.model", pc->cpuName, &len, NULL, 0);
#endif
#ifdef __linux__
	FILE *cpuInfo;
	char fileBuffer[FILE_BUFFER_SIZE];
	char *cpuNameLine;
	int labelSize;

	labelSize = strlen("model name\t: ");
	cpuInfo = fopen("/proc/cpuinfo", "r");

	if (cpuInfo != NULL) {
		while(fgets(fileBuffer, FILE_BUFFER_SIZE, cpuInfo) != NULL) {
			cpuNameLine = strstr(fileBuffer, "model name\t:");
			if (cpuNameLine != NULL) {
				/* -1 is to remove '\n' */
				pc->cpuName = strndup(cpuNameLine + labelSize,
						      strlen(cpuNameLine) - labelSize - 1);
				break;
			}
		}
		fclose(cpuInfo);
	}
#endif
}

static void
getMemorySize(computer *pc)
{
#ifdef __FreeBSD__
	int *physMem = malloc(sizeof(int));
	size_t len = sizeof(physMem);

	sysctlbyname("hw.physmem", physMem, &len, NULL, 0);
	pc->memorySize = *physMem / pow(1024, 2);
#endif
#ifdef __linux__
	FILE *memInfo;
	char fileBuffer[FILE_BUFFER_SIZE];
	char *memSizeLine;
	int memInKb;

	pc->memorySize = 0;
	memInfo = fopen("/proc/meminfo", "r");

	if (memInfo != NULL) {
		while(fgets(fileBuffer, FILE_BUFFER_SIZE, memInfo) != NULL) {
			memSizeLine = strstr(fileBuffer, "MemTotal:");

			if (memSizeLine != NULL) {
				sscanf(memSizeLine, "MemTotal: %d", &memInKb);
				pc->memorySize = memInKb / 1024;
				break;
			}
		}

		fclose(memInfo);
	}
#endif
}

static void
getNetworkInfo(computer *pc)
{
	struct ifaddrs *ifa;
	struct ifaddrs *ifs;
	unsigned char *macAddr;
	struct sockaddr_in *ipAddr;
	struct sockaddr_in6 *ipAddr6;

	macAddr = malloc(sizeof(struct sockaddr));
	ipAddr = malloc(sizeof(struct sockaddr_in));
	ipAddr6 = malloc(sizeof(struct sockaddr_in6));
	getifaddrs(&ifs);

/*
 * TODO: Have a generic mechanism to deal with interfaces.
 * Keep WiFi?
 */
	for (ifa = ifs; ifa != NULL; ifa = ifa->ifa_next) {
		if ((!strcmp(ifa->ifa_name, "bge0")
		     || !strcmp(ifa->ifa_name, "wlan0"))) {
			if (ifa->ifa_addr->sa_family == ETH_IF) {
				macAddr = (unsigned char*)((struct sockaddr*)ifa->ifa_addr->sa_data);
				macAddr += 10;
				sprintf(pc->macAddress,
					"%02x:%02x:%02x:%02x:%02x:%02x",
					*macAddr,
					*(macAddr + 1),
					*(macAddr + 2),
					*(macAddr + 3),
					*(macAddr + 4),
					*(macAddr + 5));
			} else if (ifa->ifa_addr->sa_family == AF_INET) {
				ipAddr->sin_addr.s_addr =
					((struct sockaddr_in*)(ifa->ifa_addr))->sin_addr.s_addr;
				strncpy(pc->ipv4Address, inet_ntoa(ipAddr->sin_addr), INET_ADDRSTRLEN);
			} else if (ifa->ifa_addr->sa_family == AF_INET6) {
				ipAddr6->sin6_addr =
					((struct sockaddr_in6*)(ifa->ifa_addr))->sin6_addr;
				inet_ntop(AF_INET6,
					  (const void *)&ipAddr6->sin6_addr,
					  pc->ipv6Address,
					  INET6_ADDRSTRLEN);
			}
		}
	}
}

/*
 * Collects all computer information
 */
int
collectInfo(computer *info)
{
	int rc = INFO_READING_OK;

	getOsInfo(info);
	getCpuName(info);
	getMemorySize(info);
	getNetworkInfo(info);

	if ((info->osName == NULL)
	    || (info->osRelease == NULL)
	    || (info->osArch == NULL)
	    || (info->cpuName == NULL)
	    || (info->memorySize == 0)) {
		rc = INFO_READING_ERROR;
	}

	return rc;
}
