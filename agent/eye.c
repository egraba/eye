#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "info.h"
#include "usage.h"

#define CLEAR_SCREEN() printf("\033[2J")
#define CURSOR_POS(row, col) printf("\033[%d;%dH", row, col)

#define PERCENT(x, y) (double) x / (double) y * 100
#define TRANSMIT_KB(x) (double) x / 1024 

/* Global variables */
int soc;

static void
set_tty_screen(struct winsize *ws)
{
	if (ioctl(0, TIOCGWINSZ, ws) < 0) {
		perror("Could not get tty size...\n");
		exit(EXIT_FAILURE);
	}
	CLEAR_SCREEN();
	CURSOR_POS(1, 1);
}

static void
display_sub_title(struct winsize *ws, char *title)
{
	int x;
	int i;

	/* Title coordinates */
	for (i = 0; i < ws->ws_col; i++) {
		printf("=");
	}
	printf("\n");

	x = (ws->ws_col - strlen(title)) / 2;
	for (i = 0; i < x; i++) {
		printf(" ");
	}
	printf("%s", title);
	for (i = x + strlen(title); i < ws->ws_col; i++) {
		printf(" ");
	}
	printf("\n");
}

static void
display_usage(int row)
{
	cpu_usage cu;
	memory_usage mu;
	network_usage nu;
	io_usage iu;

	for (;;) {
		unsigned long cu_total;
		unsigned long mu_total;

		CURSOR_POS(row, 1);
		printf("\033[J");

		get_cpu_usage(&cu);
		/*get_network_usage(&nu);
		get_io_usage(&iu);*/

		cu_total = cu.user + cu.nice + cu.sys + cu.intr + cu.idle;
		printf("CPU: %2.1f%% %2.1f%% %2.1f%% %2.1f%% %2.1f%%\n",
		       PERCENT(cu.user, cu_total),
		       PERCENT(cu.nice, cu_total),
		       PERCENT(cu.sys, cu_total),
		       PERCENT(cu.intr, cu_total),
		       PERCENT(cu.idle, cu_total));

		get_memory_usage(&mu);
		mu_total = mu.vm_active + mu.vm_total + mu.free;
		printf("Memory: %2.1f%% %2.1f%% %2.1f%%\n",
		       PERCENT(mu.vm_active, mu_total),
		       PERCENT(mu.vm_total, mu_total),
		       PERCENT(mu.free, mu_total));

		  /*printf("Active memory:    %2.1f %%\n", PERCENT(mem.active, mem.total));
		printf("Inactive memory:  %2.1f %%\n", PERCENT(mem.inactive, mem.total));
		printf("Free memory:      %2.1f %%\n", PERCENT(mem.free, mem.total));
		printf("Swap:             %2.1f %%\n", PERCENT(mem.swap_used, mem.swap_total));
		printf("Network: rec %2.1f KB | trans %2.1f KB\n",
		       TRANSMIT_KB(net.received),
		       TRANSMIT_KB(net.transmitted));
		       printf("I/O: %lu requests\n", io.progress);*/
		sleep(2);
	}
}

static void
terminate_standalone_mode()
{
	CLEAR_SCREEN();
	CURSOR_POS(1, 1);
	exit(EXIT_SUCCESS);
}

/*
 * Standalone execution
 */
static void
standalone_mode()
{
	machine info;
	struct winsize ws;

	if (collect_info(&info) < 0) {
		perror("Could not read computer information...\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, terminate_standalone_mode);
	set_tty_screen(&ws);
	printf("eye <0>, Press 'q' or ESC to quit\n");
	
	display_sub_title(&ws, "MACHINE");

	printf("Hostname        : %s\n", info.nodename);
	printf("\n");
	printf("OS  | name      : %s\n", info.sysname);
	printf("    | release   : %s\n", info.release);
	printf("    | version   : %s\n", info.version);
	printf("\n");
	printf("CPU | type      : %s\n", info.machine);
	printf("    | name      : %s\n", info.cpuname);
	printf("    | cores     : %d\n", info.ncpus);
	printf("\n");
	printf("Physical memory : %d bytes\n", info.physmem);
	
	display_sub_title(&ws, "USAGE");
	display_usage(17);
}

static void
send_message(int soc, const char *message)
{
	int msg_sent_len;

	printf("Sending:\n");
	printf("%s\n", message);
	msg_sent_len = send(soc, message, strlen(message), 0);
	printf("Message length: %d\n\n", msg_sent_len);
	if (msg_sent_len == 0) {
		perror("Message empty!\n");
	}
}

static void
terminate_connected_mode()
{
	printf("Deconnection...");
	close(soc);
	exit(EXIT_SUCCESS);
}

/*
 * Connected execution
 */
static void
connected_mode()
{
/*	struct sockaddr_in sa;

	computer info;
	cpu_usage cpu;
	memory_usage mem;
	network_usage net;
	io_usage io;

	char data[BUFSIZ];

	if ((soc = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket error...");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, terminate_connected_mode);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	inet_pton(sa.sin_family, SERVER_IP, &sa.sin_addr.s_addr);

	if (connect(soc, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("Connect error...");
		exit(EXIT_FAILURE);
	}

	collect_info(&info);
	
	sprintf(data,
		"{computerInfo:{osType:\"%s\",osRelease:\"%s\",arch:\"%s\",cpuName:\"%s\",totalMem:%d}}",
		info.os_name, info.os_release, info.os_arch, info.cpu_name, info.memory_size); 
	send_message(soc, data);

	sprintf(data,
		"{networkInfo:{mac:\"%s\",ipv4:\"%s\",ipv6:\"%s\"}}",
		info.mac_address, info.ipv4_address, info.ipv6_address); 
	send_message(soc, data);

	for (;;) {
		get_cpu_usage(&cpu);
		get_memory_usage(&mem);
		get_network_usage(&net);
		get_io_usage(&io);	

		sprintf(data,
			"{usage:{cpu:{used:%lu,total:%lu},{mem:{active:%lu,inactive:%lu,free:%lu,total:%lu},swap:{used:%lu,total:%lu}},io:%lu}}",
			cpu.used, cpu.total,
			mem.active, mem.inactive, mem.free, mem.total,
			mem.swap_used, mem.swap_total,
			io.progress);
		send_message(soc, data);
		sleep(2);
	}
*/
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
			standalone_mode();
			break;
		case 'c':
			connected_mode();
			break;
		
case '?':
		default:
			usage();
			exit(EXIT_FAILURE);
		}
	}

	exit(EXIT_SUCCESS);
}
