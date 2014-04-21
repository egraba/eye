#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "computerInfo.h"
#include "computerUsage.h"

#define CLEAR_SCREEN() printf("\033[2J")
#define CURSOR_POS(row, col) printf("\033[%d;%dH", row, col)

#define PERCENT(x, y) (double) x / (double) y * 100

static void
setTtyScreen(struct winsize *ws)
{
	if (ioctl(0, TIOCGWINSZ, ws) < 0) {
		perror("Could not get tty size...\n");
		exit(EXIT_FAILURE);
	}
	CLEAR_SCREEN();
	CURSOR_POS(1, 1);
}

static void
displaySubTitle(struct winsize *ws, char *title)
{
	int titleX;

	/* Title coordinates */
	for (int i = 0; i < ws->ws_col; i++) {
		printf("=");
	}
	printf("\n");

	titleX = (ws->ws_col - strlen(title)) / 2;
	for (int i = 0; i < titleX; i++) {
		printf(" ");
	}
	printf("%s", title);
	for (int i = titleX + strlen(title); i < ws->ws_col; i++) {
		printf(" ");
	}
	printf("\n");
}

static void
displayUsage(int row)
{
	cpuUsage cpu;
	memoryUsage mem;
	networkUsage net;
	ioUsage io;

	for (;;) {
		CURSOR_POS(row, 1);
		printf("\033[J");
		getCpuUsage(&cpu);
		getMemoryUsage(&mem);
		getNetworkUsage(&net);
		getIoUsage(&io);
		printf("CPU:              %2.1f %%\n", PERCENT(cpu.used, cpu.total));
		printf("Active memory:    %2.1f %%\n", PERCENT(mem.active, mem.total));
		printf("Inactive memory:  %2.1f %%\n", PERCENT(mem.inactive, mem.total));
		printf("Free memory:      %2.1f %%\n", PERCENT(mem.free,mem.total));
		printf("Swap:             %2.1f %%\n", PERCENT(mem.swapUsed, mem.swapTotal));
		printf("Network: rec %2.1f KB | trans %2.1f KB\n",
		       (double) net.received / 1024,
		       (double) net.transmitted / 1024);
		printf("I/O: %lu requests\n", io.progress);
		printf("Disk:\n");
		sleep(2);
	}
}

static void
terminate()
{
	CLEAR_SCREEN();
	CURSOR_POS(1, 1);
	exit(EXIT_SUCCESS);
}

/*
 * Standalone execution
 */
static void
standaloneMode()
{
	computer info;
	struct winsize ws;

	if (collectInfo(&info) < 0) {
		perror("Could not read computer information...\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, terminate);
	setTtyScreen(&ws);
	printf("eye <0>, Press 'q' or ESC to quit\n");
	
	displaySubTitle(&ws, "COMPUTER");
	printf("OS type:      %s\n", info.osName);
	printf("OS release:   %s\n", info.osRelease);
	printf("Architecture: %s\n", info.osArch);
	printf("CPU:          %s\n", info.cpuName);
	printf("Total memory: %d MB\n", info.memorySize);
	
	displaySubTitle(&ws, "NETWORK");
	printf("Mac address:  %s\n", info.macAddress);
	printf("IPv4 address: %s\n", info.ipv4Address);
	printf("IPv6 address: %s\n", info.ipv6Address);
	
	displaySubTitle(&ws, "USAGE");
	displayUsage(16);
}

/*
 * Connected execution
 */
static void
connectedMode()
{

}

static void
usage()
{
	printf("usage: eye [-s] [-c]\n");
	printf("\t-s\tStandalone mode\n");
	printf("\t-c\tConnected mode\n");
}

/*
 * The main
 */
int
main(int argc, char **argv)
{
	int c;

	if (argc < 2) {
		usage();
		exit(EXIT_SUCCESS);
	}

	while ((c = getopt (argc, argv, "sc")) != -1) {
		switch (c) {
		case 's':
			standaloneMode();
			break;
		case 'c':
			connectedMode();
			break;
		case '?':
		default:
			usage();
			exit(EXIT_FAILURE);
		}
	}

	exit(EXIT_SUCCESS);
}
