#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_AGENTS 50
#define PORT 1111

/* Global variables */
pid_t pid;
int s, c;

static int
receiveMessage(int clientSocket, char *message)
{
	int r;
	char data[BUFSIZ];
	memset(data, 0, BUFSIZ);
	r = recv(clientSocket, data, BUFSIZ, 0);
	if (r < 0) {
		perror("Couldn't read data from agent...");

	} else {
		printf("Received:\n");
		printf("%s\n\n", data);
		message = strdup(data);
	}

	return r;
}

static void
terminate()
{
	printf("Closing existing connections...\n");
	close(s);
	close(c);
	exit(EXIT_SUCCESS);
}

int
main(void)
{
	socklen_t len;
	struct sockaddr_in sa;

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket error...");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, terminate);

	bzero(&sa, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port   = htons(PORT);

	if (INADDR_ANY)
		sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr *) &sa, sizeof sa) < 0) {
		perror("Bind error...");
		exit(EXIT_FAILURE);
	}

	listen(s, MAX_AGENTS);

	for (;;) {
		len = sizeof(sa);

		if ((c = accept(s, (struct sockaddr *)&sa, &len)) < 0) {
			perror("Accept error...");
			close(c);
			sleep(5);
		
		} else {
			switch (pid = fork()) {
			case 0:
				break;
			case -1:
				perror("Fork error...");
				exit(EXIT_FAILURE);
				break;
			default:
				close(s);
				exit(EXIT_SUCCESS);
				break;
			}

			char * message = NULL;
			receiveMessage(c, message);
			receiveMessage(c, message);
			for (;;) {
				int r;
				r = receiveMessage(c, message);
				if (r <= 0) {
					break;
				}
			}
		}
	}
}
