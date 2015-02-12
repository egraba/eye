#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 9999
#define MAX_CLIENTS 50

int
main(void)
{
	int s;
	int c;
	struct sockaddr_in sa;
	unsigned int len;
	char data[BUFSIZ];

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket error...");
		exit(EXIT_FAILURE);
	}

	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(DEFAULT_PORT);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(s, (struct sockaddr*) &sa, sizeof(sa)) < 0) {
		perror("Bind error...");
		exit(EXIT_FAILURE);
	}

	listen(s, MAX_CLIENTS);

	for (;;) {
		len = sizeof(sa);

		if ((c = accept(s, (struct sockaddr*) &sa, &len)) < 0) {
			perror("Accept error...");
			exit(EXIT_FAILURE);
		}

		switch(fork()) {
		case 0:
			break;
		case -1:
			perror("Fork error...");
			close(s);
			exit(EXIT_FAILURE);
		default:
			close(s);
			exit(EXIT_SUCCESS);
			break;
		}

		memset(data, 0, BUFSIZ);
		if (recv(c, data, BUFSIZ, 0) < 0)
			perror("Couldn't read data from client...");
		else
			printf("Received:\n%s\n\n", data);
	}

	exit(EXIT_SUCCESS);
}
