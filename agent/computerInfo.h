#ifndef _COMPUTER_INFO
#define _COMPUTER_INFO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define FILE_BUFFER_SIZE 512
#define UNAME_ERROR -1
#define MAC_ADDR_SIZE 18
#define INFO_READING_OK 0
#define INFO_READING_ERROR -1

typedef struct {
	char *osName;
	char *osRelease;
	char *osArch;
	char *cpuName;
	int memorySize;
	char macAddress[MAC_ADDR_SIZE];
	char ipv4Address[INET_ADDRSTRLEN];
	char ipv6Address[INET6_ADDRSTRLEN];
} computer;

/* Prototypes */
int collectInfo(computer *info);

#endif
